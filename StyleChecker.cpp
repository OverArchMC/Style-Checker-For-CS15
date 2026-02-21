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


std::vector<std::string> StyleChecker::splitStringIntoWords(const std::string& input) {
    std::stringstream ss(input);
    std::vector<std::string> words;
    std::string word;
    // Read words one by one using the >> operator, which delimits by whitespace
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}


void StyleChecker::parseFunctions() {
    int num_lines = lines.size();
    int func_start;
    int func_end;
    for (int i =  0; i < num_lines; i++) {
        if (isFunctionStart(lines[i])) {
            func_start = i;
        }
    }
}
    
bool StyleChecker::isFunctionStart(const std::string& line) {
    // Matches patterns like:
    // int foo(...)
    // std::string MyClass::myMethod(int a, double b)
    // template functions, const, static, virtual, etc.
    std::regex funcPattern(
        R"(^\s*)"                          // optional leading whitespace
        R"((?:(?:inline|static|virtual|explicit|constexpr|const|friend|extern)\s+)*)"  // optional specifiers
        R"((?:[\w\s*&:<>,]+?)\s+)"         // return type
        R"(([\w:~]+)\s*)"                  // function name (including destructors ~, scope ::)
        R"(\([^)]*\))"                     // parameter list
        R"((?:\s*(?:const|noexcept|override|final))*)"  // optional trailing qualifiers
        R"(\s*\{?\s*$)"                    // optional opening brace at end of line
    );

    return std::regex_match(line, funcPattern);
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