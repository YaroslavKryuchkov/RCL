#ifndef RCL_PPM
#define RCL_PPM

namespace rcl
{

namespace
{
    void skipComments(std::ifstream& file) 
    {
        char ch;
        while (file.peek() == ' ' || file.peek() == '\t' || file.peek() == '\n' || file.peek() == '\r') 
        {
            file.get();
        }
        
        while (file.peek() == '#') 
        {
            std::string commentLine;
            std::getline(file, commentLine);
            
            while (file.peek() == ' ' || file.peek() == '\t' || file.peek() == '\n' || file.peek() == '\r') 
            {
                file.get();
            }
        }
    }

    void readP3Format(std::ifstream& file, rcl::vec3*& data, int size, int maxVal) 
    {
        for (int i = 0; i < size; ++i) 
        {
            int r, g, b;
            file >> r >> g >> b;
            
            if (file.fail()) 
            {
                std::cerr << "Error: Failed to read pixel data" << std::endl;
                return;
            }
            
            data[i].r = ((float)r) / maxVal;
            data[i].g = ((float)g) / maxVal;
            data[i].b = ((float)b) / maxVal;
        }
    }
}

}
#endif