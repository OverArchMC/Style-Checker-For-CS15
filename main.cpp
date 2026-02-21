#include "StyleChecker.h"

using namespace std;

int main (int argc, char *argv[]) {
    // Check for proper command line arguments
    if (argc < 2) {
        cerr << "Usage: ./style_checker filename" << endl;
        exit(EXIT_FAILURE);
    }
    // Attempt to open the file
    string filename = argv[1];
    ifstream infile(filename);
    if (infile.fail()) {
        cerr << "ERROR: Cannot open file " << filename << endl;
    }

    // Check style
    StyleChecker checker(infile);
    checker.parseFunctions();
    checker.checkFuncLength(30);
    checker.printLines();
    // checker.printFunctions();

    infile.close();

    // Return todo list

    return 0;
}









