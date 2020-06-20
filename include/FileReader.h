//
//
//

#ifndef PORAM_FILEREADER_H
#define PORAM_FILEREADER_H
#include <iostream>
#include <fstream>
using namespace std;


class FileReader {
    FileReader(string fileName, bool out);
    void input(string fileName);
    void output(string fileName);
};


#endif //PORAM_FILEREADER_H
