#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Function {
    public:
        Function(std::vector<std::string> function_lines);
    private:
        std::vector<std::string> lines;
};

#endif