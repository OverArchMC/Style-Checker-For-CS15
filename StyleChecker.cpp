#include "StyleChecker.h"

StyleChecker::StyleChecker(std::ifstream &input, std::ofstream &output) {
   readLines(input, output);
   oncePerFile();
   parseFunctions();
   checkFuncLength(30);
}

void StyleChecker::readLines(std::ifstream &input, std::ofstream &output) {
    // Get every line
    std::string line;
    while (std::getline(input, line)) {
        // Add it to lines
        lines.push_back(line);
    }
}

void StyleChecker::printLines(std::ofstream &output) {
    int num_lines = lines.size();
    for (int i = 0; i < num_lines; i++) {
        output << lines[i] << std::endl;
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
>>>>>>> e667fc2e5201190b80a81ed12a45b902b2e8af4c
    }
}
<<<<<<< HEAD

void StyleChecker::oncePerFile(){
    // these variables are for standard namespace 
    bool have_namespace = false; 
    bool have_main = false; 
    int namespaceLineNum; 

    // these variables are for members declared private 
    bool headerFile = false;
    bool reachedPrivate = false;

    int braces = 0;
    for (size_t i = 0; i < lines.size(); i++) {

        // check for members declared private 
        if (lines.at(i).find("class ") != std::string::npos) headerFile = true;
        //if (lines.at(i).find("private:") != std::string::npos) reachedPrivate = true;
        // I DON'T KNOW HOW TO CHECK IF SOMETHING IS A FUNCTION OR A VARIABLE - like what if it's a struct type 
        
        // check for standard namespace pt 1
        if (lines.at(i).find("using namespace std;") != std::string::npos) {
            have_namespace = true;
            namespaceLineNum = i + 1;
        }
        if (lines.at(i).find("int main") != std::string::npos) have_main = true;

        //check for globals
        for(int j = 0; j < lines.at(i).length(); j++){
            if(lines.at(i)[j] == '{') braces++;
            if(lines.at(i)[j] == '}') braces--;
            //checking to see if we're in a function or not...
        }
        if(braces == 0){
            headerFile = false;
            reachedPrivate = false;
            if(lines.at(i).find(";") != std::string::npos && lines.at(i).find("const") == std::string::npos){
                std::string error = " Not allowed a global variable.";
                lines[i] += error;
                //if not a global const nothing can be put outside the functions
            }
        }
        if(headerFile){
            if (lines.at(i).find("private:") != std::string::npos) reachedPrivate = true;
            else if(lines.at(i).find("public:")!= std::string::npos){
                reachedPrivate = false;
            }
        }
        // check for file header
        if(i == 0){
            if(lines.at(i).find("/*") == std::string::npos || lines.at(i).find("*/") == std::string::npos) {
                std::string comment = std::string("/*\n * File: ") +
                    "\n * Author: \n * Date: \n * File Description: \n */\n";
                lines[i] = comment + "\n" + lines[i];
            }
        }
    }
    // check for standard namespace pt 2
    if (have_namespace) {
        if (!have_main) lines[namespaceLineNum] += " // Shouldn't have std namespace";
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

void StyleChecker::breakStatements(int i) {
    if (lines[i].find("break;" ) != std::string::npos){
        std::string info = " // No break statements allowed";
        lines[i] += info;
    }
}

void StyleChecker::argumentSpacing(int i) {
    if (lines[i].find("for(") != std::string::npos) {
        std::string comment = " // Add a space between the for loop arguments and the brackets";
        lines[i] += comment;
    }
    if (lines.at(i).find("if(") != std::string::npos) {
        std::string comment = " // Add a space between the if statement arguments and the brackets";
        lines[i] += comment; 
    }
    if (lines.at(i).find("while(") != std::string::npos) {
        std::string comment = " // Add a space between the while loop arguments and the brackets";
        lines[i] += comment; 
    }

    if (lines.at(i).find("for") != std::string::npos && lines.at(i).find(") {") != std::string::npos){
        std::string comment = " // Add a space between the for loop arguments and the brackets";
        lines[i] += comment; 
    }
    if (lines.at(i).find("if") != std::string::npos) {
        std::string comment = " // Add a space between the if statement arguments and the brackets";
        lines[i] += comment; 
    }
    if (lines.at(i).find("while") != std::string::npos) {
        std::string comment = " // Add a space between the while loop arguments and the brackets";
        lines[i] += comment; 
    }
}

void StyleChecker::operatorSpacing(int i) {
    // check for || && ! and check for tab pt 1 and check for + - = spacing
    bool indentDone = false; // for checking tab
    bool skip = false; // for checking !
    for (size_t j = 0; j < lines.at(i).length(); j++){
        if (lines.at(i)[j] == '|' and lines.at(i)[j] == '|') {
            std::string comment = " // Use \"or\" instead of \"||\" ";
            lines[i] += comment;
            break;
        }
        if (lines.at(i)[j] == '&' and lines.at(i)[j] == '&') {
            std::string comment = " // Use \"and\" instead of \"&&\" ";
            lines[i] += comment;
            break;
        }
        if (lines.at(i)[j] == '!' and lines.at(i)[j+1] == '=') skip == true;
        else if (lines.at(i)[j] == '!' and lines.at(i)[j] != '=' and !(skip)) {
            std::string comment = " // Use \"not\" instead of \"!\" ";
            lines[i] += comment;
            skip == false;
            break;
        }
        // checking for + - * == spacing 
        //     if (original.at(j) == '+' || original.at(j) == '*' || original.at(j) == '/' || original.at(j) == '%') {
        //         if (original.at(j-1) != ' ' || original.at(j+1) != ' ') {
        //             lines[i] += " // Add space before and after binary operator";
        //             break;
        //         }
        //     }
        //     if (original.at(j) == '-' && original.at(j+1) != '>') {
        //         if (original.at(j-1) != ' ' || original.at(j+1) != ' ') {
        //             lines[i] += " // Add space before and after binary operator"; 
        //             break;
        //         }
        //     }
        //     if (original.at(j) == '=' && original.at(j+1) != '=' && original.at(j-1) != '=') {
        //         if (original.at(j-1) != ' ' || original.at(j+1) != ' ') {
        //             lines[i] += " // Add space before and after binary operator"; 
        //             break;
        //         }
        //     }
        // }




        
        // checking for + - * == spacing 
    //     if (lines.at(i)[j] == '+' || lines.at(i)[j] == '*' || lines.at(i)[j] == '/' || lines.at(i)[j] == '%') {
    //         if (lines.at(i)[j-1] != ' ' || lines.at(i)[j+1] != ' ') {
    //             lines[i] += " // Add space between and after binary operator";
    //             break;
    //         }
    //     }
    //     if (lines.at(i)[j] == '-' && lines.at(i)[j+1] != '>') {
    //         if (lines.at(i)[j-1] != ' ' || lines.at(i)[j+1] != ' ') {
    //             lines[i] += " // Add space between and after binary operator"; 
    //             break;
    //         }
    //     }
    //     if (lines.at(i)[j] == '=' && lines.at(i)[j+1] != '=' && lines.at(i)[j-1] != '=') {
    //         if (lines.at(i)[j-1] != ' ' || lines.at(i)[j+1] != ' ') {
    //             lines[i] += " // Add space between and after binary operator"; 
    //             break;
    //         }
    //     }
    }
}

void StyleChecker::indentation(int i, int *level) {
    // indentation 
    int leading_spaces = 0;
    for(size_t j = 0; j < lines.at(i).length(); j++){
        if(lines.at(i)[j] == ' '){
            leading_spaces++;
        } else {
            break;
        }
    }
    
    if(lines.at(i).find('}') != std::string::npos){
        (*level)--;
    }
    
    int expected_spaces = (*level) * 4;
    if(leading_spaces != expected_spaces && leading_spaces != 0){
        std::string comment = " // Incorrect indentation. Expected " + std::to_string(expected_spaces) + " spaces.";
        lines[i] += comment;
    }
    
    if(lines.at(i).find('{') != std::string::npos){
        (*level)++;
    }
}

void StyleChecker::singleLineLoop(int i) {
// no single line loops
    if(lines.at(i).find("for") != std::string::npos || lines.at(i).find("while") != std::string::npos || lines.at(i).find("if") != std::string::npos){
        if(lines.at(i).find(";") != std::string::npos){
            std::string comment = " // No single line loops or if statements.";
            lines[i] += comment;
        }
    }
}

void StyleChecker::lineLength(int i) {
    if(lines[i].length() > 80) {
        std::string comment = " // Exceeds 80-char line limit; length: " + std::to_string(lines[i].length());
        lines[i] += comment;
    }
}

void StyleChecker::whileBoolean(int i) {
    // can't write loops dependent on true/false
    if (lines[i].find("while (true)") != std::string::npos or lines.at(i).find("while (false)") != std::string::npos){
        std::string error = " // Loops must be dependent on a condition";
        lines[i] += error;
    }
}

void StyleChecker::run() {
    
    int indent_level = 0; // for indentation check 

    for (size_t i = 0; i < lines.size(); i++) {
        // Skip comment lines
        if(lines.at(i).find("//") != std::string::npos && lines.at(i).find("//") == lines.at(i).find_first_not_of(" \t")){
            continue;
        }       
        lineLength(i);
        whileBoolean(i);
        operatorSpacing(i);
        singleLineLoop(i);
        breakStatements(i);
        argumentSpacing(i);
        indentation(i, &indent_level);
>>>>>>> e667fc2e5201190b80a81ed12a45b902b2e8af4c
    }
}