#include "StyleChecker.h"

#include <string>
#include <vector>
#include <algorithm>

static bool hasFileHeader(const std::vector<std::string>& lines);

StyleChecker::StyleChecker(std::ifstream &input, std::ofstream &output) {
   readLines(input, output);
   parseFunctions();
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

void StyleChecker::oncePerFile() {
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
        if (lines.at(i).find("private:") != std::string::npos) reachedPrivate = true;
        if (lines.at(i).find("(") == std::string::npos) 
            if (headerFile && !reachedPrivate) lines[i] += " // Member variables should be declared private";
        
        // check for standard namespace pt 1
        if (lines.at(i).find("using namespace std;") != std::string::npos) {
            have_namespace = true;
            namespaceLineNum = i + 1;
        }
        if (lines.at(i).find("int main") != std::string::npos) have_main = true;

        //check for globals..we cant really test this yet
        for(int j = 0; j < lines.at(i).length(); j++){
            if(lines.at(i)[j] == '{') braces++;
            if(lines.at(i)[j] == '}') braces--;
            //checking to see if we're in a function or not...
        }
        if(braces == 0){
            headerFile = false;
            reachedPrivate = false;
            if(lines.at(i).find(";") != std::string::npos && lines.at(i).find("const") == std::string::npos && lines.at(i).find("using") == std::string::npos && lines.at(i).find("#") == std::string::npos && lines.at(i).find("//") == std::string::npos){
                std::string error = " // Global variable not allowed";
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
        if (i == 0) {
            if (!hasFileHeader(lines)) {
                lines[0] += " // Where is your file header? (filename, author, date, file purpose)";
                // Temporarily commenting this section out for now, because this changes the total line length of the file and that might affect everything else (like this loop)
                // std::string comment = std::string("/*\n * File: \n * Author: \n * Date: \n * File Description: \n */\n"); 
                // lines[i] = comment + "\n" + lines[i]; 
            }
        }
    }
    // check for standard namespace pt 2
    if (have_namespace) {
        if (!have_main) lines[namespaceLineNum] += " // Shouldn't have std namespace";
    }
}

// Detect existing file header near the top
static bool hasFileHeader(const std::vector<std::string>& lines) {
    const size_t maxScan = std::min<size_t>(10, lines.size());
    bool inBlock = false;
    bool sawFileTag = false;

    for (size_t i = 0; i < maxScan; ++i) {
        const std::string& line = lines[i];
        if (line.find("/*") != std::string::npos) inBlock = true;
        if (inBlock && (line.find(".cpp") != std::string::npos || inBlock && line.find(".h") != std::string::npos)) sawFileTag = true;
        if (line.find("*/") != std::string::npos && inBlock) {
            return sawFileTag;
        }
        if (line.find("//") != std::string::npos &&
           (line.find(".cpp") != std::string::npos || 
            line.find(".h") != std::string::npos)) return true;
    
    }
    return false;
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
        for (char c : lines[i]) {
            if (c == '{') {
                started = true;
                balance++;
            }
            else if (c == '}') {
                balance--;
            }
            if (balance == 0 and started) {
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
            ss << " // Exceeds " << max_len << "-line limit (" << length << " lines)";
            lines[func.start] += ss.str();
        }
    }
}

void StyleChecker::oncePerFunction(){
    // if this is not .cpp file, SKIP WHOLE FUNCTIO
    for (size_t i = 0; i < lines.size(); i++) {
        // check for braces 
        if (lines[i].find("{") != std::string::npos) {
            if (lines[i].find("for") != std::string::npos || lines[i].find("if") != std::string::npos || lines[i].find("while") != std::string::npos || lines[i].find("else if") != std::string::npos || lines[i].find("else") != std::string::npos ||
                lines[i-1].find("for") != std::string::npos || lines[i-1].find("if") != std::string::npos || lines[i-1].find("while") != std::string::npos || lines[i-1].find("else if") != std::string::npos || lines[i-1].find("else") != std::string::npos) {
                    continue;
                }
            // now we have confirmed that it's not an if/ elseif statement or for/while loop

            // check if function contract exists
            int currLine = i - 1;
            bool purposeExists = false;
            while (currLine >= 0 && (lines[currLine].find("//") != std::string::npos || lines[currLine].find("*") != std::string::npos)) {
                if (lines[currLine].find("purpose:") != std::string::npos || lines[currLine].find("Purpose:") != std::string::npos){
                    purposeExists = true;
                    break;
                }
                currLine--;
            }
            if (!purposeExists) lines[i-1] += " // Add function contract";
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
    const std::string original = lines[i]; // use unmodified line

    if (original.find("for(") != std::string::npos) {
        lines[i] += " // Add a space between the for loop keyword and '('";
        return;
    }
    if (original.find("if(") != std::string::npos) {
        lines[i] += " // Add a space between the if keyword and '('";
        return;
    }
    if (original.find("while(") != std::string::npos) {
        lines[i] += " // Add a space between the while keyword and '('";
        return;
    }
}

void StyleChecker::operatorSpacing(int i) {
    // check for || && ! and check for tab pt 1 and check for + - = spacing
    std::string code = lines.at(i);
    size_t commentPos = code.find("//");
    if (commentPos != std::string::npos) {
        code = code.substr(0, commentPos);
    }
    size_t lastCode = code.find_last_not_of(" \t");
    if (lastCode != std::string::npos) {
        code = code.substr(0, lastCode + 1);
    }

    size_t firstCode = code.find_first_not_of(" \t");
    if (firstCode != std::string::npos && code[firstCode] == '#') {
        return;
    }

    for (size_t j = 0; j < code.length(); j++){
        if (j + 1 < code.length() && code[j] == '|' and code[j+1] == '|') {
            std::string comment = " // Use \"or\" instead of \"||\" ";
            lines[i] += comment;
            break;
        }
        if (j + 1 < code.length() && code[j] == '&' and code[j+1] == '&') {
            std::string comment = " // Use \"and\" instead of \"&&\" ";
            lines[i] += comment;
            break;
        }
        if (code[j] == '!' && (j + 1 >= code.length() || code[j+1] != '=')) {
            std::string comment = " // Use \"not\" instead of \"!\" ";
            lines[i] += comment;
            break;
        }

        if (j + 1 < code.length() && code[j] == '-' && code[j+1] == '>') {
            j++;
            continue;
        }

        if (j + 1 < code.length() && ((code[j] == '=' && code[j+1] == '=') || (code[j] == '!' && code[j+1] == '=') || (code[j] == '<' && code[j+1] == '=') || (code[j] == '>' && code[j+1] == '='))) {
            if (j == 0 || j + 2 >= code.length() || code[j-1] != ' ' || code[j+2] != ' ') {
                lines[i] += " // Add spaces around operator '='";
                break;
            }
            j++;
            continue;
        }

        if (j + 1 < code.length() && (code[j] == '+' || code[j] == '-' || code[j] == '*' || code[j] == '/' || code[j] == '%') && code[j+1] == '=') {
            if (j == 0 || j + 2 >= code.length() || code[j-1] != ' ' || code[j+2] != ' ') {
                lines[i] += " // Add spaces around operator '" + std::string(1, code[j]) + "'";
                break;
            }
            j++;
            continue;
        }

        if (j + 1 < code.length() && ((code[j] == '+' && code[j+1] == '+') || (code[j] == '-' && code[j+1] == '-'))) {
            j++;
            continue;
        }

        if (code[j] == '+' || code[j] == '-' || code[j] == '=' || code[j] == '/' || code[j] == '%' || code[j] == '*' || code[j] == '<' || code[j] == '>') {
            if (code[j] == '-' && ((j + 1 < code.length() && code[j+1] == '>') || (j > 0 && code[j-1] == '<'))) continue;
            if ((code[j] == '<' || code[j] == '>') && ((j + 1 < code.length() && code[j+1] == code[j]) || (j > 0 && code[j-1] == code[j]))) continue;
            if (code[j] == '=' && ((j + 1 < code.length() && code[j+1] == '=') || (j > 0 && (code[j-1] == '=' || code[j-1] == '!' || code[j-1] == '<' || code[j-1] == '>' || code[j-1] == '+' || code[j-1] == '-' || code[j-1] == '*' || code[j-1] == '/' || code[j-1] == '%')))) continue;
            if ((code[j] == '+' || code[j] == '-' || code[j] == '*' || code[j] == '/' || code[j] == '%') && (j + 1 < code.length() && code[j+1] == '=')) continue;
            if (j == code.length() - 1) continue;
            if (j == 0 || code[j-1] != ' ' || code[j+1] != ' ') {
                std::string comment = " // Add spaces around operator '" + std::string(1, code[j]) + "'";
                lines[i] += comment;
                break;
            }
        }
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
    int expected_spaces2 = (*level) * 8;
    bool skipIndentCheck = false;
    if (i > 0) {
        std::string prev = lines.at(i - 1);
        size_t prevComment = prev.find("//");
        if (prevComment != std::string::npos) {
            prev = prev.substr(0, prevComment);
        }
        size_t prevEnd = prev.find_last_not_of(" \t");
        if (prevEnd != std::string::npos) {
            char lastChar = prev[prevEnd];
            if (lastChar != '{' && lastChar != '}' && lastChar != ';') {
                skipIndentCheck = true;
            }
        }
    }

    if(!skipIndentCheck && leading_spaces != expected_spaces && leading_spaces != expected_spaces2 && leading_spaces != 0){
        lines[i] += " // Incorrect indentation";
    }
    
    if(lines.at(i).find('{') != std::string::npos){
        (*level)++;
    }
    // edge case: 
    // if (condition) 
    //     no braces 
}

void StyleChecker::singleLineLoop(int i) {
// no single line loops
    if(lines.at(i).find("for") != std::string::npos || lines.at(i).find("while") != std::string::npos || lines.at(i).find("if (") != std::string::npos){
        if(lines.at(i).find(";") != std::string::npos){
            std::string comment = " // No single line loops or if statements.";
            lines[i] += comment;
        }
    }
}

void StyleChecker::lineLength(int i, int original_length) {
    // std::cout << "LINE LENGTH CALLED FOR LINE " << i << std::endl;
    if(original_length > 80) {
        std::string comment = " // Exceeds 80-char line limit (length: " + std::to_string(original_length) + " )";
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
    bool blockComment = false;
    

    for (size_t i = 0; i < lines.size(); i++) {
        bool singleComment = false;
        int original_length = lines[i].length();
        

        if (lines.at(i).find("//") != std::string::npos && lines.at(i).find("//") == lines.at(i).find_first_not_of(" \t")){
            singleComment = true;
        }

        if (lines[i].find("/*") != std::string::npos) {
            blockComment = true;
        }

        if (lines[i].find("*/") != std::string::npos) {
            blockComment = false;
            singleComment = true;
        }

        // Skip comment lines
        if (blockComment or singleComment) {
            continue;
        }
        
        lineLength(i, original_length);
        whileBoolean(i);
        operatorSpacing(i);
        singleLineLoop(i);
        breakStatements(i);
        argumentSpacing(i);
        indentation(i, &indent_level);
    }
    oncePerFile();
    checkFuncLength(30);
    oncePerFunction();
}
