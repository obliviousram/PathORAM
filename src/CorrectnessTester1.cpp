
#include "CorrectnessTester1.h"
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

CorrectnessTester1::CorrectnessTester1() {
}

int* CorrectnessTester1::sampleData(int i) {
	int* newArray = new int[Block::BLOCK_SIZE];
	for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
		newArray[j] = i;
	}
	return newArray;
}

void CorrectnessTester1::runCorrectnessTest() {

	int bucketSize = 4;
	int numBlocks = pow(2, 12);
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();

	OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);

	cout << "Warming up the stash by writing blocks" << endl;


	for(int i = 0; i < 3000; i++){
			int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, this->sampleData(i));
			if (oram->getStashSize() != 0 && i%10 == 0) {
				cout << "Writing Block " << i % numBlocks << " to ORAM. The stash size is: " << to_string(oram->getStashSize()) << std::endl;
			}
	}


	for(int i = 0; i < numBlocks; i++){
		int* accessed = oram->access(OramInterface::Operation::READ, i, NULL);
		string holder = "";
		if (accessed == NULL) {
			cout << "Reading Block " << i << " from ORAM. Block not found!" << std::endl;
		}
		else {
			for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
				int temp = accessed[j];
				holder += to_string(temp);
				holder += " ";
			}
			if (i % 1000 == 0) {
				cout << "Reading Block " << i << " from ORAM. Value is : " << holder << std::endl;
				cout << "Stash size is " << to_string(oram->getStashSize()) << std::endl;
			}
		}
		
		
	}

	// for(int i = 0; i < numBlocks; i++){
	// 		int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, this->sampleData(i));
	// 		cout << "Writing Block " << i % numBlocks << " to ORAM. The stash size is: " << to_string(oram->getStashSize()) << std::endl;
	// }

}
