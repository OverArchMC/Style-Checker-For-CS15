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

    // This is the file that contains comments 
    std::ofstream outputFile(argv[2]);

    // Check style
    StyleChecker checker(infile, outputFile);
    checker.run();
    checker.printLines(outputFile);

    // Close file stream
    infile.close();

    // output todo 

    return 0;
}









