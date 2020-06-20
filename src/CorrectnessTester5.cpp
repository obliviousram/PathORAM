
#include "CorrectnessTester5.h"
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

CorrectnessTester5::CorrectnessTester5() {
}

int* CorrectnessTester5::sampleData(int i) {
	int* newArray = new int[Block::BLOCK_SIZE];
	for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
		newArray[j] = i;
	}
	return newArray;
}

void CorrectnessTester5::runCorrectnessTest() {

	int bucketSize = 2;
	int numBlocks = pow(2, 20);
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();
	
	RandForOramInterface* random = new RandomForOram();
	
	OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
	//OramInterface* oram = new OramDeterministic(storage, random, bucketSize, numBlocks);

	cout << "Warming up the stash by writing blocks" << endl;
	int bound = numBlocks;
	for(int i = 0; i < bound; i++){
			int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, this->sampleData(i%numBlocks));
			if (i%1000 == 0) {
				cout << "Writing Block " << i << " with value: " << i%numBlocks << " to ORAM. The stash size is: " << to_string(oram->getStashSize()) << std::endl;
			}
	}

	
	
	for(int i = 0; i < bound; i++){
		int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
		string holder = "";
		if (accessed == NULL) {
			cout << "accessed is null" << endl;
			cout << "accessed: " << i%numBlocks << " and i = " << i << endl;
 		}
		for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
			int temp = accessed[j];
			holder += to_string(temp);
			holder += " ";
		}
		if (i%1000 == 0){
			cout << "Reading Block " << i << " from ORAM. Value is : " << holder << std::endl;
		//if (oram->getStashSize() > 0) {
			cout << "Stash size is " << to_string(oram->getStashSize()) << std::endl;
		}	
	}

	// for(int i = 0; i < numBlocks; i++){
	// 		int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, this->sampleData(i));
	// 		cout << "Writing Block " << i % numBlocks << " to ORAM. The stash size is: " << to_string(oram->getStashSize()) << std::endl;
	// }

}
