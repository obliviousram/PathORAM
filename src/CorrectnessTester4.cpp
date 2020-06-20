

#include "CorrectnessTester4.h"
#include "Bucket.h"
#include "ServerStorage.h"
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "RandomForOram.h"
#include "UntrustedStorageInterface.h"
#include "OramReadPathEviction.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

CorrectnessTester4::CorrectnessTester4() {
}

int* CorrectnessTester4::sampleData(int i) {
    int* newArray = new int[Block::BLOCK_SIZE];
    for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
        newArray[j] = i;
    }
    return newArray;
}

void CorrectnessTester4::runCorrectnessTest() {

    int bucketSize = 10;
    int numBlocks = pow(2, 12);
    Bucket::setMaxSize(bucketSize);

    UntrustedStorageInterface* storage = new ServerStorage();

    RandForOramInterface* random = new RandomForOram();

    OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);

    cout << "Warming up the stash by writing blocks" << endl;

    for(int i = 0; i < 30000; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, this->sampleData(i));
    }

    for(int i = 0; i < 5*numBlocks; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, this->sampleData(i));
    }


    for(int i = 0; i < numBlocks; i++){
        int* accessed = oram->access(OramInterface::Operation::READ, i, NULL);
        string holder = "";
        int* correct_data = sampleData(4*numBlocks + i);
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            if (correct_data[j] != temp) {
                cout << "Expected and found values don't match!" << endl;
            }
            holder += to_string(temp);
            holder += " ";
        }
        if (i % 1000 == 0) {
            cout << "Reading Block " << i << " from ORAM. Value is : " << holder << std::endl;
            cout << "Stash size is " << to_string(oram->getStashSize()) << std::endl;
        }

    }


}

