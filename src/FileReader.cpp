//
//
//

#include "FileReader.h"
#include <iostream>
#include <fstream>
#include "CorrectnessTester1.h"
#include "CorrectnessTester2.h"
#include "CorrectnessTester3.h"
#include "CorrectnessTester4.h"
using namespace std;

//fileName should be .txt
FileReader::FileReader(string fileName, bool out) {
    //out means the file is meant to be written into, otherwise read it
    if(out) {
        output(fileName);
    } else input(fileName);
}

//reads the file line by line and prints it out
void FileReader::input(string fileName) {
    string line;
    ifstream myfile (fileName);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            cout << line << '\n';
        }
        myfile.close();
    }
    else cout << "Unable to open file";
}

void FileReader::output(string fileName) {
    ofstream myfile (fileName);
    if (myfile.is_open())
    {
        //write in the tests to fileName
        myfile << "This is a line.\n";
        myfile.close();
    }
    else cout << "Unable to open file";
}
