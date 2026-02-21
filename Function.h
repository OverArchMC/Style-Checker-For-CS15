#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Function {
    public:
        Function(std::vector<std::string> function_lines);
    private:
        std::vector<std::string> lines;
}