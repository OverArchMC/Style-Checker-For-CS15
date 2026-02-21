#include "StyleChecker.h"

//#include <ofstream>
#include <string>

StyleChecker::StyleChecker(std::ifstream &input, std::ofstream &output) {
   readLines(input, output);
   lineLength();
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
    
    int indent_level = 0; // for indentation check 

    for (size_t i = 0; i < lines.size(); i++){
        // Skip comment lines
        if(lines.at(i).find("//") != std::string::npos && lines.at(i).find("//") == lines.at(i).find_first_not_of(" \t")){
            continue;
        }
        
        // check for 80 characters 
        if(lines.at(i).length() > 80){ 
            std::string comment = " // Exceeds 80-char line limit; length: " + std::to_string(lines[i].length());
            lines[i] += comment;
        }

        // can't write loops dependent on true/false
        if(lines.at(i).find("while (true)") != std::string::npos or lines.at(i).find("while (false)") != std::string::npos){
            std::string error = " // Loops must be dependent on a condition";
            lines[i] += error;
        }

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
            if (lines.at(i)[j] == '+' || lines.at(i)[j] == '*' || lines.at(i)[j] == '/' || lines.at(i)[j] == '%') {
                if (lines.at(i)[j-1] != ' ' || lines.at(i)[j+1] != ' ') {
                    lines[i] += " // Add space between and after binary operator";
                    break;
                }
            }
            if (lines.at(i)[j] == '-' && lines.at(i)[j+1] != '>') {
                if (lines.at(i)[j-1] != ' ' || lines.at(i)[j+1] != ' ') {
                    lines[i] += " // Add space between and after binary operator"; 
                    break;
                }
            }
            if (lines.at(i)[j] == '=' && lines.at(i)[j+1] != '=' && lines.at(i)[j-1] != '=') {
                if (lines.at(i)[j-1] != ' ' || lines.at(i)[j+1] != ' ') {
                    lines[i] += " // Add space between and after binary operator"; 
                    break;
                }
            }
        }

        // check for break;
        if(lines.at(i).find("break;" ) != std::string::npos){
            std::string info = " // No break statements allowed";
            lines[i] += info;
        }

        // space_between_arguments_and_brackets
        if(lines.at(i).find("for(") != std::string::npos) {
            std::string comment = " // Add a space between the for loop arguments and the brackets";
            lines[i] += comment; 
        }
        if(lines.at(i).find("if(") != std::string::npos) {
            std::string comment = " // Add a space between the if statement arguments and the brackets";
            lines[i] += comment; 
        }
        if(lines.at(i).find("while(") != std::string::npos) {
            std::string comment = " // Add a space between the while loop arguments and the brackets";
            lines[i] += comment; 
        }

        if(lines.at(i).find("for") != std::string::npos && lines.at(i).find(") {") != std::string::npos){
            std::string comment = " // Add a space between the for loop arguments and the brackets";
            lines[i] += comment; 
        }
        if(lines.at(i).find("if") != std::string::npos) {
            std::string comment = " // Add a space between the if statement arguments and the brackets";
            lines[i] += comment; 
        }
        if(lines.at(i).find("while") != std::string::npos) {
            std::string comment = " // Add a space between the while loop arguments and the brackets";
            lines[i] += comment; 
        }


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
            indent_level--;
        }
        
        int expected_spaces = indent_level * 4;
        if(leading_spaces != expected_spaces && leading_spaces != 0){
            std::string comment = " // Incorrect indentation. Expected " + std::to_string(expected_spaces) + " spaces.";
            lines[i] += comment;
        }
        
        if(lines.at(i).find('{') != std::string::npos){
            indent_level++;
        }

        // no single line loops 
        if(lines.at(i).find("for") != std::string::npos || lines.at(i).find("while") != std::string::npos || lines.at(i).find("if") != std::string::npos){
            if(lines.at(i).find(";") != std::string::npos){
                std::string comment = " // No single line loops or if statements.";
                lines[i] += comment;
            }
        }

        // check operator spacing
        for(size_t j = 0; j < lines.at(i).length(); j++) {
            if (lines.at(i).at(j) == '+' || lines.at(i).at(j) == '-' || lines.at(i).at(j) == '*' || lines.at(i).at(j) == '/' || lines.at(i).at(j) == '>' || lines.at(i).at(j) == '<'){
                if (j > 0 && j < lines.at(i).length() - 1) {
                    if (lines.at(i).at(j-1) != ' ' || lines.at(i).at(j+1) != ' ') {
                        std::string comment = " // Add spaces around operators.";
                        lines[i] += comment;
                        break;
                    }
                }
            }
        }
    }
    
}
