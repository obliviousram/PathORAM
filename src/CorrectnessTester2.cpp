
#include "CorrectnessTester2.h"
#include "Bucket.h"
#include "ServerStorage.h"
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "RandomForOram.h"
#include "UntrustedStorageInterface.h"
#include "OramReadPathEviction.h"
#include "OramDeterministic.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

CorrectnessTester2::CorrectnessTester2() {
}

int* CorrectnessTester2::sampleData(int i) {
	int* newArray = new int[Block::BLOCK_SIZE];
	for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
		newArray[j] = i;
	}
	return newArray;
}

void CorrectnessTester2::runCorrectnessTest() {

	int bucketSize = 2;
	int numBlocks = pow(2, 4);
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();

	//OramInterface* oram = new OramDeterministic(storage, random, bucketSize, numBlocks);
	OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);

	cout << "Warming up the stash by writing blocks" << endl;

	// for(int i = 0; i < 10; i++){
	// 		cout << "Writing Block " << i << " to ORAM. The stash size is: " << to_string(oram->getStashSize()) << std::endl;
	// 		int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, this->sampleData(i));
	// }

	for(int i = 0; i < numBlocks; i++){
			if (i%1000 == 0) {
				cout << "Writing Block " << i << " to ORAM. The stash size is: " << to_string(oram->getStashSize()) << std::endl;
			}
			int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, this->sampleData(i));
	}

	for(int i = 0; i < numBlocks; i++){
			if (i%1000 == 0) {
				cout << "Writing Block " << i << " to ORAM. The stash size is: " << to_string(oram->getStashSize()) << std::endl;
			}
			int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, this->sampleData((i+1)%numBlocks));
	}

	for(int i = 0; i < numBlocks; i++){
			if (i%1000 == 0) {
				cout << "Writing Block " << i << " to ORAM. The stash size is: " << to_string(oram->getStashSize()) << std::endl;
			}
			int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, this->sampleData((i+2)%numBlocks));
	}


	for(int i = 0; i < numBlocks; i++){
		int* accessed = oram->access(OramInterface::Operation::READ, i, NULL);
		string holder = "";
		int* correct_data = sampleData((i+2)%numBlocks);
		if (accessed == NULL) {
			cout << "Reading Block " << i << " from ORAM. Block not found!" << std::endl;
		}
		for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
			int temp = accessed[j];
			if (correct_data[j] != temp) {
				cout << "Expected and found values don't match!" << " Accessed = " << temp << " Correct = " << correct_data[j] << endl;
			}
			holder += to_string(temp);
			holder += " ";
		}
		//if (i % 1000 == 0) {
			cout << "Reading Block " << i << " from ORAM. Value is : " << holder << std::endl;
		//}
		//if (oram->getStashSize() > 0) {
			cout << "Stash size is " << to_string(oram->getStashSize()) << std::endl;
		//}
		
	}


}
