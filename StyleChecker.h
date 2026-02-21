
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

    struct Function {
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
<<<<<<< HEAD
    
    void oncePerFile();
    
=======
    void oncePerFile();
>>>>>>> 1c4ea3de260f5870367caf671e00874029979c69
    void lineLength(int i);
    void whileBoolean(int i);
    void operatorSpacing(int i);
    void singleLineLoop(int i);
    void breakStatements(int i);
    void argumentSpacing(int i);
    void indentation(int i, int *level);

    // CHECK ONCE FOR EVERY LINE 
    // 80 characters (done)
    // no || ! && (done)
    // if while for statement ) {           (done)
    // no if, else if, else on the same line (done)
    // there should be spaces in between +, = *, etc use (need to test) (done)
    // indenting                            (done)
    // can't write loops dependent on true/false (done)
    // no break (done)

    // CHECK ONCE PER FILE 
    // headers (joey)
    // not global variable (neeed to redo)
    // data members in classes should be declared private (sylvia)!!!!!
    // no std namespace in .h file (done)

    // CHECK ONCE PER FUNCTION (kyra)
    // 30 lines 
    // function contracts 

    // no repetitive code...
};

#endif