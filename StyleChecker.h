#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Function.h"

class StyleChecker{
public:
    StyleChecker(std::ifstream &input);
    void printLines();
    void parseFunctions();
    
private:
    std::vector<std::string> lines;
    std::vector<Function> functions;

    void readLines(std::ifstream &input);
    void check_every_line();
    bool function_length();
    bool line_length();

    // CHECK ONCE FOR EVERY LINE 
    // 80 characters 
    // no || ! && 
    // if while for statement ) {
    // no if, else if, else on the same line 
    // there should be spaces in between +, = *, etc use
    // indenting 
    // can't write loops dependent on true/false
    // no break

    // CHECK ONCE PER FILE 
    // function contracts 
    // headers 
    // no global variables
    // data members in classes should be delcared private
    // no std namespace in .h file 

    // CHECK ONCE PER FUNCTION 
    // 30 lines 

    // no repetitive code...
};