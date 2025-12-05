#ifndef RCL_PNG
#define RCL_PNG

#include <vector>
#include <cstring>
#include <algorithm>
#include <iostream>

namespace rcl
{
    // Huffman tree node for DEFLATE decompression
    struct HuffmanNode 
    {
        int symbol;
        int left, right;
        
        HuffmanNode() : symbol(-1), left(-1), right(-1) {}
    };

    // Bit reader for DEFLATE decompression
    class BitReader 
    {
    private:
        const std::vector<unsigned char>& data;
        size_t byte_pos;
        int bit_pos;
        
    public:
        BitReader(const std::vector<unsigned char>& d) : data(d), byte_pos(0), bit_pos(0) {}
        
        bool has_bits(int count) const 
        {
            size_t bits_available = (data.size() - byte_pos) * 8 - bit_pos;
            return bits_available >= static_cast<size_t>(count);
        }
        
        uint32_t read_bits(int count) 
        {
            if (!has_bits(count)) return 0;
            
            uint32_t result = 0;
            for (int i = 0; i < count; i++) 
            {
                if (bit_pos >= 8) 
                {
                    byte_pos++;
                    bit_pos = 0;
                }
                
                if (byte_pos >= data.size()) break;
                
                int bit = (data[byte_pos] >> bit_pos) & 1;
                result |= (bit << i);
                bit_pos++;
            }
            return result;
        }
        
        void align_to_byte() 
        {
            if (bit_pos > 0) 
            {
                byte_pos++;
                bit_pos = 0;
            }
        }
        
        size_t get_position() const { return byte_pos; }
    };

    // Build canonical Huffman tree from code lengths
    std::vector<HuffmanNode> build_huffman_tree(const std::vector<int>& code_lengths) 
    {
        std::vector<HuffmanNode> tree;
        
        // Create symbol-length pairs and sort by length, then by symbol
        std::vector<std::pair<int, int>> symbol_lengths;
        for (size_t i = 0; i < code_lengths.size(); i++) 
        {
            if (code_lengths[i] > 0) 
            {
                symbol_lengths.push_back({code_lengths[i], i});
            }
        }
        
        if (symbol_lengths.empty()) return tree;
        
        std::sort(symbol_lengths.begin(), symbol_lengths.end());
        
        // Calculate canonical codes
        std::vector<std::pair<uint32_t, int>> codes; // code, symbol
        uint32_t code = 0;
        int prev_length = 0;
        
        for (auto& sl : symbol_lengths) 
        {
            int length = sl.first;
            int symbol = sl.second;
            
            code <<= (length - prev_length);
            codes.push_back({code, symbol});
            code++;
            prev_length = length;
        }
        
        // Build tree structure
        tree.resize(1); // Root
        int next_node = 1;
        
        for (size_t i = 0; i < codes.size(); i++) 
        {
            uint32_t huffman_code = codes[i].first;
            int symbol = codes[i].second;
            int length = symbol_lengths[i].first;
            
            int current = 0;
            
            for (int bit = length - 1; bit >= 0; bit--) 
            {
                int direction = (huffman_code >> bit) & 1;
                
                if (bit == 0) 
                {
                    // Leaf node
                    if (direction == 0) 
                    {
                        if (tree[current].left == -1) 
                        {
                            tree.resize(std::max(static_cast<size_t>(next_node + 1), tree.size()));
                            tree[current].left = next_node;
                            tree[next_node].symbol = symbol;
                            next_node++;
                        }
                    } 
                    else 
                    {
                        if (tree[current].right == -1) 
                        {
                            tree.resize(std::max(static_cast<size_t>(next_node + 1), tree.size()));
                            tree[current].right = next_node;
                            tree[next_node].symbol = symbol;
                            next_node++;
                        }
                    }
                } 
                else 
                {
                    // Internal node
                    if (direction == 0) 
                    {
                        if (tree[current].left == -1) 
                        {
                            tree.resize(std::max(static_cast<size_t>(next_node + 1), tree.size()));
                            tree[current].left = next_node++;
                        }
                        current = tree[current].left;
                    } 
                    else 
                    {
                        if (tree[current].right == -1) 
                        {
                            tree.resize(std::max(static_cast<size_t>(next_node + 1), tree.size()));
                            tree[current].right = next_node++;
                        }
                        current = tree[current].right;
                    }
                }
            }
        }
        
        return tree;
    }

    // Decode symbol using Huffman tree
    int decode_symbol(BitReader& reader, const std::vector<HuffmanNode>& tree) 
    {
        if (tree.empty()) return -1;
        
        int current = 0;
        while (current < static_cast<int>(tree.size()) && tree[current].symbol == -1) 
        {
            if (!reader.has_bits(1)) return -1;
            
            int bit = reader.read_bits(1);
            if (bit == 0) 
            {
                if (tree[current].left == -1) return -1;
                current = tree[current].left;
            } 
            else 
            {
                if (tree[current].right == -1) return -1;
                current = tree[current].right;
            }
        }
        
        if (current >= static_cast<int>(tree.size())) return -1;
        return tree[current].symbol;
    }

    // Simple inflate for uncompressed blocks only
    bool simple_inflate(const std::vector<unsigned char>& compressed, std::vector<unsigned char>& uncompressed) 
    {
        if (compressed.size() < 6) return false; // Need at least zlib header + some data
        
        // Skip zlib header (2 bytes)
        size_t pos = 2;
        
        while (pos < compressed.size()) 
        {
            if (pos + 5 > compressed.size()) break;
            
            // Read block header
            unsigned char bfinal = compressed[pos] & 0x01;
            unsigned char btype = (compressed[pos] >> 1) & 0x03;
            pos++;
            
            if (btype == 0) 
            {
                // Uncompressed block
                if (pos + 4 > compressed.size()) break;
                
                uint16_t len = compressed[pos] | (compressed[pos + 1] << 8);
                uint16_t nlen = compressed[pos + 2] | (compressed[pos + 3] << 8);
                pos += 4;
                
                if (len != (nlen ^ 0xFFFF)) return false;
                if (pos + len > compressed.size()) break;
                
                for (int i = 0; i < len; i++) 
                {
                    uncompressed.push_back(compressed[pos + i]);
                }
                pos += len;
            } 
            else 
            {
                // Compressed block - not supported in simple inflate
                return false;
            }
            
            if (bfinal) break;
        }
        
        return !uncompressed.empty();
    }

    const int code_length_order[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
    
    // Decode code lengths for dynamic Huffman
    std::vector<int> decode_code_lengths(BitReader& reader, const std::vector<HuffmanNode>& code_length_tree, int count) 
    {
        std::vector<int> lengths;
        lengths.reserve(count);
        
        while (static_cast<int>(lengths.size()) < count) 
        {
            int symbol = decode_symbol(reader, code_length_tree);
            if (symbol == -1) break;
            
            if (symbol <= 15) 
            {
                // Literal code length
                lengths.push_back(symbol);
            } 
            else if (symbol == 16) 
            {
                // Repeat previous code length 3-6 times
                if (lengths.empty()) break;
                if (!reader.has_bits(2)) break;
                
                int repeat = 3 + reader.read_bits(2);
                int prev_length = lengths.back();
                
                for (int i = 0; i < repeat && static_cast<int>(lengths.size()) < count; i++) 
                {
                    lengths.push_back(prev_length);
                }
            } 
            else if (symbol == 17) 
            {
                // Repeat 0 length 3-10 times
                if (!reader.has_bits(3)) break;
                
                int repeat = 3 + reader.read_bits(3);
                
                for (int i = 0; i < repeat && static_cast<int>(lengths.size()) < count; i++) 
                {
                    lengths.push_back(0);
                }
            } 
            else if (symbol == 18) 
            {
                // Repeat 0 length 11-138 times  
                if (!reader.has_bits(7)) break;
                
                int repeat = 11 + reader.read_bits(7);
                
                for (int i = 0; i < repeat && static_cast<int>(lengths.size()) < count; i++) 
                {
                    lengths.push_back(0);
                }
            }
        }
        
        // Pad with zeros if needed
        while (static_cast<int>(lengths.size()) < count) 
        {
            lengths.push_back(0);
        }
        
        return lengths;
    }
    
    // Process dynamic Huffman block
    bool process_dynamic_huffman_block(BitReader& reader, std::vector<unsigned char>& uncompressed) 
    {
        // Read header
        if (!reader.has_bits(14)) return false;
        
        int hlit = reader.read_bits(5) + 257;  // Number of literal/length codes
        int hdist = reader.read_bits(5) + 1;   // Number of distance codes  
        int hclen = reader.read_bits(4) + 4;   // Number of code length codes
        
        // Validate ranges
        if (hlit > 286 || hdist > 30 || hclen > 19) return false;
        
        // Read code lengths for the code length alphabet
        std::vector<int> code_length_lengths(19, 0);
        for (int i = 0; i < hclen; i++) 
        {
            if (!reader.has_bits(3)) return false;
            code_length_lengths[code_length_order[i]] = reader.read_bits(3);
        }
        
        // Build code length Huffman tree
        auto code_length_tree = build_huffman_tree(code_length_lengths);
        if (code_length_tree.empty()) return false;
        
        // Decode literal/length code lengths
        auto literal_lengths = decode_code_lengths(reader, code_length_tree, hlit);
        if (static_cast<int>(literal_lengths.size()) != hlit) return false;
        
        // Decode distance code lengths
        auto distance_lengths = decode_code_lengths(reader, code_length_tree, hdist);
        if (static_cast<int>(distance_lengths.size()) != hdist) return false;
        
        // Build literal/length and distance Huffman trees
        auto literal_tree = build_huffman_tree(literal_lengths);
        auto distance_tree = build_huffman_tree(distance_lengths);
        
        if (literal_tree.empty()) return false;
        
        // Length and distance tables (same as fixed Huffman)
        int length_extra_bits[] = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
        int length_base[] = {3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
        
        int distance_extra_bits[] = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};
        int distance_base[] = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
        
        // Decode compressed data
        while (true) 
        {
            int symbol = decode_symbol(reader, literal_tree);
            if (symbol == -1) return false;
            
            if (symbol < 256) 
            {
                // Literal byte
                uncompressed.push_back(symbol);
            } 
            else if (symbol == 256) 
            {
                // End of block
                break;
            } 
            else if (symbol >= 257 && symbol <= 285) 
            {
                // Length/distance pair
                int length_code = symbol - 257;
                if (length_code >= 29) return false;
                
                int length = length_base[length_code];
                if (length_extra_bits[length_code] > 0) 
                {
                    if (!reader.has_bits(length_extra_bits[length_code])) return false;
                    length += reader.read_bits(length_extra_bits[length_code]);
                }
                
                // Get distance
                int distance_symbol = decode_symbol(reader, distance_tree);
                if (distance_symbol == -1 || distance_symbol >= hdist) return false;
                
                int distance = distance_base[distance_symbol];
                if (distance_extra_bits[distance_symbol] > 0) 
                {
                    if (!reader.has_bits(distance_extra_bits[distance_symbol])) return false;
                    distance += reader.read_bits(distance_extra_bits[distance_symbol]);
                }
                
                // Validate distance
                if (distance > static_cast<int>(uncompressed.size()) || distance <= 0) return false;
                
                // Copy previous data
                size_t start_pos = uncompressed.size() - distance;
                for (int i = 0; i < length; i++) 
                {
                    uncompressed.push_back(uncompressed[start_pos + (i % distance)]);
                }
            } 
            else 
            {
                // Invalid symbol
                return false;
            }
        }
        
        return true;
    }

    // Full DEFLATE decompression (improved version)
    bool full_inflate(const std::vector<unsigned char>& compressed, std::vector<unsigned char>& uncompressed) 
    {
        if (compressed.size() < 2) return false;
        
        BitReader reader(compressed);
        
        // Skip zlib header
        reader.read_bits(16);
        
        // Fixed Huffman tables for DEFLATE (for block type 1)
        std::vector<int> literal_lengths(288);
        for (int i = 0; i <= 143; i++) literal_lengths[i] = 8;
        for (int i = 144; i <= 255; i++) literal_lengths[i] = 9;
        for (int i = 256; i <= 279; i++) literal_lengths[i] = 7;
        for (int i = 280; i <= 287; i++) literal_lengths[i] = 8;
        
        std::vector<int> distance_lengths(32, 5);
        
        auto literal_tree = build_huffman_tree(literal_lengths);
        auto distance_tree = build_huffman_tree(distance_lengths);
        
        // Length and distance tables
        int length_extra_bits[] = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
        int length_base[] = {3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
        
        int distance_extra_bits[] = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};
        int distance_base[] = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
        
        bool is_final = false;
        while (!is_final && reader.has_bits(3)) 
        {
            is_final = reader.read_bits(1);
            int block_type = reader.read_bits(2);
            
            if (block_type == 0) 
            {
                // Uncompressed block
                reader.align_to_byte();
                
                if (!reader.has_bits(32)) return false;
                
                uint16_t len = reader.read_bits(16);
                uint16_t nlen = reader.read_bits(16);
                
                if (len != (nlen ^ 0xFFFF)) return false;
                
                for (int i = 0; i < len; i++) 
                {
                    if (!reader.has_bits(8)) return false;
                    uncompressed.push_back(reader.read_bits(8));
                }
            } 
            else if (block_type == 1) 
            {
                // Fixed Huffman block
                while (true) 
                {
                    int symbol = decode_symbol(reader, literal_tree);
                    if (symbol == -1) break;
                    
                    if (symbol < 256) 
                    {
                        uncompressed.push_back(symbol);
                    } 
                    else if (symbol == 256) 
                    {
                        break; // End of block
                    } 
                    else if (symbol >= 257 && symbol <= 285) 
                    {
                        // Length/distance pair
                        int length_code = symbol - 257;
                        if (length_code >= 29) break;
                        
                        int length = length_base[length_code];
                        if (length_extra_bits[length_code] > 0) 
                        {
                            if (!reader.has_bits(length_extra_bits[length_code])) break;
                            length += reader.read_bits(length_extra_bits[length_code]);
                        }
                        
                        int distance_symbol = decode_symbol(reader, distance_tree);
                        if (distance_symbol == -1 || distance_symbol >= 30) break;
                        
                        int distance = distance_base[distance_symbol];
                        if (distance_extra_bits[distance_symbol] > 0) 
                        {
                            if (!reader.has_bits(distance_extra_bits[distance_symbol])) break;
                            distance += reader.read_bits(distance_extra_bits[distance_symbol]);
                        }
                        
                        // Copy previous data
                        if (distance > static_cast<int>(uncompressed.size())) break;
                        
                        size_t start_pos = uncompressed.size() - distance;
                        for (int i = 0; i < length; i++) 
                        {
                            uncompressed.push_back(uncompressed[start_pos + (i % distance)]);
                        }
                    }
                }
            } 
            else if (block_type == 2) 
            {
                // Dynamic Huffman block - NEW SUPPORT!
                if (!process_dynamic_huffman_block(reader, uncompressed)) {
                    return false;
                }
            } 
            else 
            {
                // Reserved block type
                return false;
            }
        }
        
        return true;
    }

    // Convert different PNG color types to RGB
    void convert_png_pixel(unsigned char* rgba_pixel, const unsigned char* src_pixel, 
                          int color_type, int bit_depth, const unsigned char* palette = nullptr) 
    {
        switch (color_type) 
        {
            case 0: // Grayscale
                if (bit_depth == 8) 
                {
                    rgba_pixel[0] = rgba_pixel[1] = rgba_pixel[2] = src_pixel[0];
                    rgba_pixel[3] = 255;
                } 
                else if (bit_depth == 16) 
                {
                    rgba_pixel[0] = rgba_pixel[1] = rgba_pixel[2] = src_pixel[0];
                    rgba_pixel[3] = 255;
                }
                break;
                
            case 2: // RGB
                if (bit_depth == 8) 
                {
                    rgba_pixel[0] = src_pixel[0];
                    rgba_pixel[1] = src_pixel[1];
                    rgba_pixel[2] = src_pixel[2];
                    rgba_pixel[3] = 255;
                } 
                else if (bit_depth == 16) 
                {
                    rgba_pixel[0] = src_pixel[0];
                    rgba_pixel[1] = src_pixel[2];
                    rgba_pixel[2] = src_pixel[4];
                    rgba_pixel[3] = 255;
                }
                break;
                
            case 3: // Palette
                if (palette && src_pixel[0] * 3 + 2 < 768) // 256 * 3 = 768
                {
                    int index = src_pixel[0] * 3;
                    rgba_pixel[0] = palette[index];
                    rgba_pixel[1] = palette[index + 1];
                    rgba_pixel[2] = palette[index + 2];
                    rgba_pixel[3] = 255;
                } 
                else 
                {
                    rgba_pixel[0] = rgba_pixel[1] = rgba_pixel[2] = rgba_pixel[3] = 0;
                }
                break;
                
            case 4: // Grayscale + Alpha
                if (bit_depth == 8) 
                {
                    rgba_pixel[0] = rgba_pixel[1] = rgba_pixel[2] = src_pixel[0];
                    rgba_pixel[3] = src_pixel[1];
                } 
                else if (bit_depth == 16) 
                {
                    rgba_pixel[0] = rgba_pixel[1] = rgba_pixel[2] = src_pixel[0];
                    rgba_pixel[3] = src_pixel[2];
                }
                break;
                
            case 6: // RGBA
                if (bit_depth == 8) 
                {
                    rgba_pixel[0] = src_pixel[0];
                    rgba_pixel[1] = src_pixel[1];
                    rgba_pixel[2] = src_pixel[2];
                    rgba_pixel[3] = src_pixel[3];
                } 
                else if (bit_depth == 16) 
                {
                    rgba_pixel[0] = src_pixel[0];
                    rgba_pixel[1] = src_pixel[2];
                    rgba_pixel[2] = src_pixel[4];
                    rgba_pixel[3] = src_pixel[6];
                }
                break;
                
            default:
                rgba_pixel[0] = rgba_pixel[1] = rgba_pixel[2] = rgba_pixel[3] = 0;
                break;
        }
    }

    // Get bytes per pixel for different PNG formats
    int get_bytes_per_pixel(int color_type, int bit_depth) 
    {
        int channels = 1;
        switch (color_type) 
        {
            case 0: channels = 1; break; // Grayscale
            case 2: channels = 3; break; // RGB
            case 3: channels = 1; break; // Palette
            case 4: channels = 2; break; // Grayscale + Alpha
            case 6: channels = 4; break; // RGBA
        }
        
        return (channels * bit_depth + 7) / 8;
    }

    // PNG row filtering functions
    void unfilter_scanline(unsigned char* current, const unsigned char* previous, 
                          unsigned char filter_type, int bytes_per_pixel, int width) 
    {
        int row_bytes = width * bytes_per_pixel;
        
        switch (filter_type) 
        {
            case 0: // None
                break;
                
            case 1: // Sub
                for (int i = bytes_per_pixel; i < row_bytes; i++) 
                {
                    current[i] = (current[i] + current[i - bytes_per_pixel]) & 0xFF;
                }
                break;
                
            case 2: // Up
                if (previous) 
                {
                    for (int i = 0; i < row_bytes; i++) 
                    {
                        current[i] = (current[i] + previous[i]) & 0xFF;
                    }
                }
                break;
                
            case 3: // Average
                for (int i = 0; i < row_bytes; i++) 
                {
                    int left = (i >= bytes_per_pixel) ? current[i - bytes_per_pixel] : 0;
                    int up = previous ? previous[i] : 0;
                    current[i] = (current[i] + ((left + up) / 2)) & 0xFF;
                }
                break;
                
            case 4: // Paeth
                for (int i = 0; i < row_bytes; i++) 
                {
                    int left = (i >= bytes_per_pixel) ? current[i - bytes_per_pixel] : 0;
                    int up = previous ? previous[i] : 0;
                    int up_left = (previous && i >= bytes_per_pixel) ? previous[i - bytes_per_pixel] : 0;
                    
                    int p = left + up - up_left;
                    int pa = abs(p - left);
                    int pb = abs(p - up);
                    int pc = abs(p - up_left);
                    
                    int paeth;
                    if (pa <= pb && pa <= pc) paeth = left;
                    else if (pb <= pc) paeth = up;
                    else paeth = up_left;
                    
                    current[i] = (current[i] + paeth) & 0xFF;
                }
                break;
        }
    }

    // Utility functions for reading PNG data
    uint32_t read_uint32_be(std::istream& stream) 
    {
        uint32_t value = 0;
        for (int i = 0; i < 4; i++) 
        {
            value = (value << 8) | static_cast<unsigned char>(stream.get());
        }
        return value;
    }

    // CRC-32 calculation for PNG chunks
uint32_t crc32(const unsigned char* data, size_t length) 
{
    static uint32_t crc_table[256];
    static bool table_computed = false;
    
    if (!table_computed) {
        for (uint32_t n = 0; n < 256; n++) {
            uint32_t c = n;
            for (int k = 0; k < 8; k++) {
                if (c & 1)
                    c = 0xedb88320L ^ (c >> 1);
                else
                    c = c >> 1;
            }
            crc_table[n] = c;
        }
        table_computed = true;
    }
    
    uint32_t c = 0xffffffff;
    for (size_t n = 0; n < length; n++) {
        c = crc_table[(c ^ data[n]) & 0xff] ^ (c >> 8);
    }
    return c ^ 0xffffffff;
}

// Write a PNG chunk with proper CRC
void write_chunk(std::ofstream& file, const char* type, const unsigned char* data, uint32_t length) 
{
    // Write length (big-endian)
    file.put((length >> 24) & 0xFF);
    file.put((length >> 16) & 0xFF);
    file.put((length >> 8) & 0xFF);
    file.put(length & 0xFF);
    
    // Write type
    file.write(type, 4);
    
    // Write data
    if (data && length > 0) {
        file.write(reinterpret_cast<const char*>(data), length);
    }
    
    // Calculate and write CRC
    std::vector<unsigned char> crc_data;
    crc_data.insert(crc_data.end(), type, type + 4);
    if (data && length > 0) {
        crc_data.insert(crc_data.end(), data, data + length);
    }
    
    uint32_t crc = crc32(crc_data.data(), crc_data.size());
    file.put((crc >> 24) & 0xFF);
    file.put((crc >> 16) & 0xFF);
    file.put((crc >> 8) & 0xFF);
    file.put(crc & 0xFF);
}

// Simple deflate compression (creates uncompressed blocks)
std::vector<unsigned char> simple_deflate(const std::vector<unsigned char>& input) 
{
    std::vector<unsigned char> output;
    
    // Zlib header (CMF=0x78, FLG=0x01 for default compression)
    output.push_back(0x78);
    output.push_back(0x01);
    
    size_t remaining = input.size();
    size_t pos = 0;
    
    while (remaining > 0) {
        size_t block_size = std::min(remaining, static_cast<size_t>(65535));
        bool is_final = (remaining == block_size);
        
        // Block header: BFINAL(1) + BTYPE(2) = 000 for uncompressed, non-final
        // or 001 for uncompressed, final
        output.push_back(is_final ? 0x01 : 0x00);
        
        // Length (little-endian)
        uint16_t len = static_cast<uint16_t>(block_size);
        uint16_t nlen = ~len;
        
        output.push_back(len & 0xFF);
        output.push_back((len >> 8) & 0xFF);
        output.push_back(nlen & 0xFF);
        output.push_back((nlen >> 8) & 0xFF);
        
        // Copy data
        output.insert(output.end(), input.begin() + pos, input.begin() + pos + block_size);
        
        pos += block_size;
        remaining -= block_size;
    }
    
    // Adler-32 checksum (simplified - just use 1 for now)
    output.push_back(0x00);
    output.push_back(0x00);
    output.push_back(0x00);
    output.push_back(0x01);
    
    return output;
}

}
#endif