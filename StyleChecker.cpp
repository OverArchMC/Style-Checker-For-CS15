#include "StyleChecker.h"

//#include <ofstream>
#include <string>
#include <vector>
#include <algorithm>

static bool hasFileHeader(const std::vector<std::string>& lines);

StyleChecker::StyleChecker(std::ifstream &input, std::ofstream &output) {
   readLines(input, output);
   lineLength();
   oncePerFile();
   printLines(output);
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


// std::vector<std::string> splitStringIntoWords(const std::string& input) {
//     std::stringstream ss(input);
//     std::vector<std::string> words;
//     std::string word;
    
//     // Read words one by one using the >> operator, which delimits by whitespace
//     while (ss >> word) {
//         words.push_back(word);
//     }

//     return words;
// }


// void StyleChecker::parseFunctions() {
//     int num_lines = lines.size();
//     int func_start;
//     int func_end;
//     for (int i =  0; i < num_lines; i++) {
//         int line_length = lines[i].size();
//         if (lines[i].at(line_length - 1))
//     }
// }

void StyleChecker::lineLength() {
    int indent_level = 0;

    for (size_t i = 0; i < lines.size(); i++){
        const std::string original = lines[i]; // keep unmodified line for checks

        // Skip comment lines (use original)
        if (original.find("//") != std::string::npos &&
            original.find("//") == original.find_first_not_of(" \t")) {
                continue; 
        }
        if (original.length() > 80){ 
            std::string comment = " // Exceeds 80-char line limit; length: " + std::to_string(original.length());
            lines[i] += comment;
        }

        // can't write loops dependent on true/false
        if(original.find("while (true)") != std::string::npos || original.find("while (false)") != std::string::npos){
            std::string error = " // Loops must be dependent on a condition";
            lines[i] += error;
        }

        // check for || && ! and check for tab pt 1 and check for + - = spacing
        bool indentDone = false; // for checking tab
        bool skip = false; // for checking !
        for (size_t j = 0; j < original.length(); j++){
            if (original.at(j) == '|' and original.at(j) == '|') {
                std::string comment = " // Use \"or\" instead of \"||\" ";
                lines[i] += comment;
                break;
            }
            if (original.at(j) == '&' and original.at(j) == '&') {
                std::string comment = " // Use \"and\" instead of \"&&\" ";
                lines[i] += comment;
                break;
            }
            if (original.at(j) == '!' and original.at(j+1) == '=') skip == true;
            else if (original.at(j) == '!' and original.at(j) != '=' and !(skip)) {
                std::string comment = " // Use \"not\" instead of \"!\" ";
                lines[i] += comment;
                skip == false;
                break;
            }

            // checking for + - * == spacing 
            if (original.at(j) == '+' || original.at(j) == '*' || original.at(j) == '/' || original.at(j) == '%') {
                if (original.at(j-1) != ' ' || original.at(j+1) != ' ') {
                    lines[i] += " // Add space before and after binary operator";
                    break;
                }
            }
            if (original.at(j) == '-' && original.at(j+1) != '>') {
                if (original.at(j-1) != ' ' || original.at(j+1) != ' ') {
                    lines[i] += " // Add space before and after binary operator"; 
                    break;
                }
            }
            if (original.at(j) == '=' && original.at(j+1) != '=' && original.at(j-1) != '=') {
                if (original.at(j-1) != ' ' || original.at(j+1) != ' ') {
                    lines[i] += " // Add space before and after binary operator"; 
                    break;
                }
            }
        }
       
        // check for break;
        if(original.find("break;" ) != std::string::npos){
            std::string info = " // No break statements allowed";
            lines[i] += info;
        }

        // space_between_arguments_and_brackets
        if(original.find("for(") != std::string::npos) {
            std::string comment = " // Add a space between the for loop arguments and the brackets";
            lines[i] += comment; 
        }
        if(original.find("if(") != std::string::npos) {
            std::string comment = " // Add a space between the if statement arguments and the brackets";
            lines[i] += comment; 
        }
        if(original.find("while(") != std::string::npos) {
            std::string comment = " // Add a space between the while loop arguments and the brackets";
            lines[i] += comment; 
        }

        if(original.find("for") != std::string::npos && original.find(") {") != std::string::npos){
            std::string comment = " // Add a space between the for loop arguments and the brackets";
            lines[i] += comment; 
        }
        if(original.find("if") != std::string::npos && original.find(") {") != std::string::npos) {
            std::string comment = " // Add a space between the if statement arguments and the brackets";
            lines[i] += comment; 
        }
        if(original.find("while") != std::string::npos && original.find(") {") != std::string::npos) {
            std::string comment = " // Add a space between the while loop arguments and the brackets";
            lines[i] += comment; 
        }


        // indentation 
        int leading_spaces = 0;
        for(size_t j = 0; j < original.length(); j++){
            if(original.at(j) == ' '){
                leading_spaces++;
            } else {
                break;
            }
        }
        
        if(original.find('}') != std::string::npos){
            indent_level--;
        }
        
        int expected_spaces = indent_level * 4;
        if(leading_spaces != expected_spaces && leading_spaces != 0){
            std::string comment = " // Incorrect indentation. Expected " + std::to_string(expected_spaces) + " spaces.";
            lines[i] += comment;
        }
        
        if(original.find('{') != std::string::npos){
            indent_level++;
        }

        // no single line loops 
        if(original.find("for") != std::string::npos || original.find("while") != std::string::npos || original.find("if") != std::string::npos){
            if(original.find(";") != std::string::npos){
                std::string comment = " // No single line loops or if statements.";
                lines[i] += comment;
            }
        }

        // check operator spacing (this is repeat code)
        // for (size_t j = 0; j < original.length(); j++) {
        //     if (original.at(j) == '+' || original.at(j) == '-' || original.at(j) == '*' ||
        //         original.at(j) == '/' || original.at(j) == '>' || original.at(j) == '<') {
        //         if (j > 0 && j < original.length() - 1) {
        //             if (original.at(j-1) != ' ' || original.at(j+1) != ' ') {
        //                 std::string comment = " // Add spaces around operators";
        //                 lines[i] += comment;
        //                 break;
        //             }
        //         }
        //     }
        // }
    }
    
}

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
        if (i == 0) {
            if (!hasFileHeader(lines)) {
                std::string comment = std::string("/*\n * File: \n * Author: \n * Date: \n * File Description: \n */\n");
                lines[i] = comment + "\n" + lines[i];
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
        if (inBlock && line.find("File:") != std::string::npos) sawFileTag = true;
        if (line.find("*/") != std::string::npos && inBlock) {
            return sawFileTag;
        }
    }
    return false;
}