#include "StyleChecker.h"


StyleChecker::StyleChecker(std::ifstream &input) {
   readLines(input);
}

void StyleChecker::readLines(std::ifstream &input) {
    // Get every line
    std::string line;
    while (std::getline(input, line)) {
        // Add it to lines
        lines.push_back(line);
    }
}

void StyleChecker::printLines() {
    int num_lines = lines.size();
    for (int i = 0; i < num_lines; i++) {
        std::cout << lines[i] << std::endl;
    }
}

void StyleChecker::printFunctions() {
    std::ofstream output("functions.txt");
    int num_functions = functions.size();
    for (int i = 0; i < num_functions; i++) {
        Function func = functions[i];
        output << std::endl << std::endl << "Function #" << i << " starts on line " << func.start
        << " and ends on " << func.end << std::endl << std::endl;
        for (int l = func.start; l < func.end; l++) {
            output << lines[l] << std::endl;
        }
    }
}

void StyleChecker::parseFunctions() {
    int func_start;
    int func_end;
    int num_lines = lines.size();
    for (int i = 0; i < num_lines; i++) {
        std::string curr_line = lines[i];
        if (isFunctionStart(curr_line)) {
            func_start = i;
            func_end = findFunctionEnd(func_start);
            if (func_end != -1) {
                StyleChecker::Function func;
                func.start = func_start;
                func.end = func_end;
                functions.push_back(func);
            }
        }
    }
}
    
bool StyleChecker::isFunctionStart(const std::string& line) {
    // Matches patterns like:
    // int foo(...)
    // std::string MyClass::myMethod(int a, double b)
    // template functions, const, static, virtual, etc.
    std::regex funcPattern(
        "^\\s*" // disregards leading whitespace
        "(?:(?:inline|static|virtual|explicit|constexpr|const|friend|extern)\\s+)*" // matches these expressions
        "(?:[\\w\\s*&:<>,]+?\\s+)?"   // return type is optional for constructors
        "([\\w:~]+)\\s*" // Matches scope resolution operator and destructor
        "\\([^)]*\\)" // Matches parameter list
        "(?:\\s*(?:const|noexcept|override|final))*" // Matches trailing quantifiers
        "\\s*\\{?\\s*$" // Optional opening curly brace
    );

    return std::regex_match(line, funcPattern);
}

int StyleChecker::findFunctionEnd(int startingLine) {
    int balance = 0;
    bool started = false;

    int num_lines = lines.size();
    
    for (int i = startingLine; i < num_lines; i++) {
        // std::cout << "Current balance: " << balance << std::endl;
        for (char c : lines[i]) {
            if (c == '{') {
                started = true;
                balance++;
            }
            else if (c == '}') {
                balance--;
            }
            if (balance == 0 and started) {
                // std::cout << "Found end!" << std::endl;
                return i;
            }
        }
    }
    return -1;
}

void StyleChecker::checkFuncLength(int max_len) {
    for (StyleChecker::Function func : functions) {
        if ((func.end - func.start) > max_len) {
            func.too_long = true;
            int length = func.end - func.start;
            std::stringstream ss;
            ss << "// Exceeds " << max_len << " line limit (" << length << " lines)";
            lines.insert(lines.begin() + func.start, ss.str());
        }
    }
}

// void check_every_line(){
//     for (int i = 0; i < lines.size(); i++) {
//         // call every single testing function, which could MODIFY LINE LENGTH, 
//         // so need to adjust i accordingly 
//         line_length();
//     }
// }


// bool StyleChecker::function_length() {
//     return false;
// }

// bool StyleChecker::line_length(const std::string &lines) {
//     bool pass = true;
//     for(int i = 0; i < lines.size(); i++){
//         if(lines[i].length() > 80){ 
//             std::cout << "Line " << i << "exceeds 80 character limit. The 
//             length is currently " << lines[i].length() << " characters long";
//             pass = false;
//         }

//         if (lines[i] == '|' and lines[i+1] == '|') 
            
//     }
//     return pass;
// }

// bool StyleChecker::and_or_not(const std::string &lines){
//     for (int i = 0; i < lines.size(); i++) {
//         char temp = lines[i];
//         if (lines[i] == '|' || )
//     }
// }