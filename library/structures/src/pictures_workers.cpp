#include "pictures_workers.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cstring>

#include "interval.hpp"
#include "ppm_workers.hpp"
#include "png_workers.hpp"

namespace rcl
{

void ImportPPM(const char* path, int& width, int& height, rcl::vec3*& data)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) 
    {
        std::cerr << "Error: Cannot open file " << path << std::endl;
        return;
    }
        
    try 
    {
        // Read magic number
        std::string magicNumber;
         file >> magicNumber;
            
        if (magicNumber != "P3") 
        {
            std::cerr << "Error: Unsupported PPM format. Only P3 are supported." << std::endl;
            return;
        }
            
        // Skip comments
        skipComments(file);
            
        // Read dimensions
        file >> width >> height;
        if (width <= 0 || height <= 0) 
        {
            std::cerr << "Error: Invalid image dimensions" << std::endl;
            return;
        }
            
        // Skip comments after dimensions
        skipComments(file);
            
        // Read max value
        int maxVal;
        file >> maxVal;
        if (maxVal <= 0 || maxVal > 65535) 
        {
            std::cerr << "Error: Invalid max value" << std::endl;
            return;
        }

        // Resize pixel array
        data = new vec3[width * height];
            
        // Read pixel data based on format
        readP3Format(file, data, width * height, maxVal);
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error reading PPM file: " << e.what() << std::endl;
    }
}
void ExportPPM(const char* path, int width, int height, const rcl::vec3* const & data)
{
    std::fstream stream(path, std::fstream::out);
    stream << "P3\n" << width << ' ' << height << "\n255\n";
    
    int imageSize = width * height;

    for (int i = 0; i < imageSize; i++) 
    {
        static const rcl::Interval<double> intensity(0.000, 0.999);
        rcl::ivec3 finalColor;
        finalColor.r = int(256 * intensity.Clamp(data[i].r));
        finalColor.g = int(256 * intensity.Clamp(data[i].g));
        finalColor.b = int(256 * intensity.Clamp(data[i].b));
        stream << finalColor << '\n';
    }
            
    stream.close();
}

void ImportPNG(const char* path, int& width, int& height, rcl::vec3*& data)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open PNG file " << path << std::endl;
        return;
    }

    // Check PNG signature
    unsigned char signature[8];
    file.read(reinterpret_cast<char*>(signature), 8);
    unsigned char png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    
    if (memcmp(signature, png_signature, 8) != 0)
    {
        std::cerr << "Error: Not a valid PNG file " << path << std::endl;
        file.close();
        return;
    }

    // Read IHDR chunk
    uint32_t chunk_length = read_uint32_be(file);
    char chunk_type[5] = {0};
    file.read(chunk_type, 4);

    if (strcmp(chunk_type, "IHDR") != 0 || chunk_length != 13)
    {
        std::cerr << "Error: Invalid PNG IHDR chunk" << std::endl;
        file.close();
        return;
    }

    width = read_uint32_be(file);
    height = read_uint32_be(file);
    
    unsigned char bit_depth, color_type, compression, filter_method, interlace;
    file.read(reinterpret_cast<char*>(&bit_depth), 1);
    file.read(reinterpret_cast<char*>(&color_type), 1);
    file.read(reinterpret_cast<char*>(&compression), 1);
    file.read(reinterpret_cast<char*>(&filter_method), 1);
    file.read(reinterpret_cast<char*>(&interlace), 1);

    // Skip IHDR CRC
    file.seekg(4, std::ios::cur);

    // Validate format support
    if (compression != 0 || filter_method != 0 || interlace != 0)
    {
        std::cerr << "Error: Unsupported PNG compression, filter method, or interlace" << std::endl;
        file.close();
        return;
    }

    if (bit_depth != 8 && bit_depth != 16)
    {
        std::cerr << "Error: Unsupported bit depth. Only 8 and 16-bit are supported." << std::endl;
        file.close();
        return;
    }

    if (color_type > 6 || color_type == 1 || color_type == 5)
    {
        std::cerr << "Error: Unsupported color type" << std::endl;
        file.close();
        return;
    }

    int bytes_per_pixel = get_bytes_per_pixel(color_type, bit_depth);
    std::vector<unsigned char> compressed_data;
    std::vector<unsigned char> palette;

    // Read chunks until IEND
    while (file.good())
    {
        if (file.peek() == EOF) break;
        
        chunk_length = read_uint32_be(file);
        if (file.fail()) break;
        
        file.read(chunk_type, 4);
        if (file.fail()) break;
        
        chunk_type[4] = '\0'; // Ensure null termination
        
        if (strcmp(chunk_type, "PLTE") == 0 && color_type == 3)
        {
            // Read palette
            palette.resize(chunk_length);
            file.read(reinterpret_cast<char*>(palette.data()), chunk_length);
        }
        else if (strcmp(chunk_type, "IDAT") == 0)
        {
            std::vector<unsigned char> chunk_data(chunk_length);
            file.read(reinterpret_cast<char*>(chunk_data.data()), chunk_length);
            compressed_data.insert(compressed_data.end(), chunk_data.begin(), chunk_data.end());
        }
        else if (strcmp(chunk_type, "IEND") == 0)
        {
            break;
        }
        else
        {
            // Skip unknown chunks
            file.seekg(chunk_length, std::ios::cur);
        }
        
        // Skip CRC
        file.seekg(4, std::ios::cur);
    }

    file.close();

    if (compressed_data.empty()) {
        std::cerr << "Error: No image data found in PNG" << std::endl;
        return;
    }

    // Decompress image data
    std::vector<unsigned char> uncompressed_data;
    if (!full_inflate(compressed_data, uncompressed_data))
    {
        // Fallback to simple inflate for uncompressed blocks
        if (!simple_inflate(compressed_data, uncompressed_data))
        {
            std::cerr << "Error: Failed to decompress PNG data" << std::endl;
            return;
        }
    }

    // Verify we have enough data
    size_t expected_size = height * (width * bytes_per_pixel + 1);
    if (uncompressed_data.size() < expected_size)
    {
        std::cerr << "Error: Insufficient PNG data after decompression. Expected: " 
                  << expected_size << ", Got: " << uncompressed_data.size() << std::endl;
        return;
    }

    // Allocate output data
    data = new rcl::vec3[width * height];

    // Unfilter and convert to vec3
    std::vector<unsigned char> current_row(width * bytes_per_pixel);
    std::vector<unsigned char> previous_row(width * bytes_per_pixel, 0);
    
    for (int y = 0; y < height; y++)
    {
        int row_start = y * (width * bytes_per_pixel + 1);
        if (row_start >= static_cast<int>(uncompressed_data.size())) {
            std::cerr << "Error: Row start beyond data bounds" << std::endl;
            delete[] data;
            data = nullptr;
            return;
        }
        
        unsigned char filter_type = uncompressed_data[row_start];
        
        // Copy current row data
        if (row_start + 1 + width * bytes_per_pixel > static_cast<int>(uncompressed_data.size())) {
            std::cerr << "Error: Row data beyond bounds" << std::endl;
            delete[] data;
            data = nullptr;
            return;
        }
        
        std::copy(uncompressed_data.begin() + row_start + 1,
                  uncompressed_data.begin() + row_start + 1 + width * bytes_per_pixel,
                  current_row.begin());
        
        // Unfilter
        unfilter_scanline(current_row.data(), 
                         (y > 0) ? previous_row.data() : nullptr, 
                         filter_type, bytes_per_pixel, width);
        
        // Convert to vec3
        for (int x = 0; x < width; x++)
        {
            int pixel_index = y * width + x;
            int byte_index = x * bytes_per_pixel;
            
            unsigned char rgba_pixel[4];
            convert_png_pixel(rgba_pixel, &current_row[byte_index], color_type, bit_depth, 
                            palette.empty() ? nullptr : palette.data());
            
            data[pixel_index].r = static_cast<float>(rgba_pixel[0]) / 255.0f;
            data[pixel_index].g = static_cast<float>(rgba_pixel[1]) / 255.0f;
            data[pixel_index].b = static_cast<float>(rgba_pixel[2]) / 255.0f;
        }
        
        // Save current row as previous for next iteration
        previous_row = current_row;
    }
}

void ExportPNG(const char* path, int width, int height, const rcl::vec3* const & data)
{
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot create PNG file " << path << std::endl;
        return;
    }

    // Write PNG signature
    unsigned char signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    file.write(reinterpret_cast<char*>(signature), 8);

    // Write IHDR chunk
    unsigned char ihdr_data[13];
    
    // Width and height in big-endian
    ihdr_data[0] = (width >> 24) & 0xFF;
    ihdr_data[1] = (width >> 16) & 0xFF;
    ihdr_data[2] = (width >> 8) & 0xFF;
    ihdr_data[3] = width & 0xFF;
    
    ihdr_data[4] = (height >> 24) & 0xFF;
    ihdr_data[5] = (height >> 16) & 0xFF;
    ihdr_data[6] = (height >> 8) & 0xFF;
    ihdr_data[7] = height & 0xFF;
    
    ihdr_data[8] = 8;   // bit depth
    ihdr_data[9] = 2;   // color type (RGB)
    ihdr_data[10] = 0;  // compression
    ihdr_data[11] = 0;  // filter method
    ihdr_data[12] = 0;  // interlace

    write_chunk(file, "IHDR", ihdr_data, 13);

    // Prepare image data
    static const rcl::Interval<double> intensity(0.000, 0.999);
    std::vector<unsigned char> image_data;
    
    for (int y = 0; y < height; y++)
    {
        // Add filter type byte (0 = None filter)
        image_data.push_back(0);
        
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;
            
            // Clamp values to [0, 255] range
            int r = static_cast<int>(256 * intensity.Clamp(data[index].r));
            int g = static_cast<int>(256 * intensity.Clamp(data[index].g));
            int b = static_cast<int>(256 * intensity.Clamp(data[index].b));
            
            // Ensure values don't exceed 255
            r = std::min(r, 255);
            g = std::min(g, 255);
            b = std::min(b, 255);
            
            image_data.push_back(static_cast<unsigned char>(r));
            image_data.push_back(static_cast<unsigned char>(g));
            image_data.push_back(static_cast<unsigned char>(b));
        }
    }

    // Compress image data using simple deflate (uncompressed blocks)
    std::vector<unsigned char> compressed_data = simple_deflate(image_data);

    // Write IDAT chunk
    write_chunk(file, "IDAT", compressed_data.data(), compressed_data.size());

    // Write IEND chunk
    write_chunk(file, "IEND", nullptr, 0);

    file.close();
}

}