
#ifndef __STYLE_CHECKER_H_
#define __STYLE_CHECKER_H_

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <regex>

class StyleChecker{

public:
    StyleChecker(std::ifstream &input, std::ofstream &output);
    void printLines(std::ofstream &output);
    void printFunctions();
    void run();
    void parseFunctions();
    void checkFuncLength(int max_len);

    struct Comment {
        int start;
        int end;
    };

    struct Function {
        Comment contract;
        bool too_long = false;
        int start;
        int end;
    };
    
private:
    std::vector<std::string> lines;
    std::vector<Function> functions;

    bool isFunctionStart(const std::string& line);
    int findFunctionEnd(int startingLine);
    void readLines(std::ifstream &input, std::ofstream &output);
    void oncePerFile();
    void lineLength(int i, int original_length);
    void whileBoolean(int i);
    void operatorSpacing(int i);
    void singleLineLoop(int i);
    void breakStatements(int i);
    void argumentSpacing(int i);
    void indentation(int i, int *level);

    // LAST THING if we want to do this: 
    // function contracts 
};

#endif