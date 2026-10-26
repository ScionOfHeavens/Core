#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class FileReader{
public:
    static std::string read(std::string filePath) {
        std::string content;
        std::ifstream fstream;
        
        fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            fstream.open(filePath);
            std::stringstream sstream;
            sstream << fstream.rdbuf();
            fstream.close();
            content = sstream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        return content;
    }
};
