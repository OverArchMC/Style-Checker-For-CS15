#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Function.h"

class StyleChecker{
public:
    StyleChecker(std::ifstream &input, std::ofstream &output);
    void printLines(std::ofstream &output);
    void run();
    void parseFunctions();
    
private:
    std::vector<std::string> lines;
    //std::vector<Function> functions;

    void readLines(std::ifstream &input, std::ofstream &output);
    void lineLength();
    

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
    // function contracts 
    // headers 
    // no global variables
    // data members in classes should be delcared private
    // no std namespace in .h file 

    // CHECK ONCE PER FUNCTION 
    // 30 lines (kyra)

    // no repetitive code...
};