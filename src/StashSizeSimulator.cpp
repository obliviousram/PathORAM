
#include "StashSizeSimulator.h"
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
#include <fstream>


using namespace std;

StashSizeSimulator::StashSizeSimulator(int bucketSize, int numBlocks, int numAccesses, int simNumber, int type) {
	this->numAccesses = numAccesses;
	
	this->stashSizes = new int[numBlocks+2];
	this->stashSizes[0] = numAccesses;
	this->simNumber = simNumber;
	
	for (int i=1; i < numBlocks+2; i++) {
		this->stashSizes[i] = 0;
	}
	this->numBlocks = numBlocks;
	this->bucketSize = bucketSize;
	this->type = type;
}	

int* StashSizeSimulator::sampleData(int i) {
	int* newArray = new int[Block::BLOCK_SIZE];
	for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
		newArray[j] = i;
	}
	return newArray;
}

void StashSizeSimulator::updateStashSizes(int recorded, int times) {
	for (int i = 1; i<recorded+1;i++){
		this->stashSizes[i] += times;
	}
}

void StashSizeSimulator::writeSimulation() {
	ofstream sim;
	sim.open("simulations/simulation"+to_string(simNumber)+".txt");
	for (int i = 0; i<numBlocks+2; i++) {
		if (this->stashSizes[i] == 0) {
			break;
		}
		sim << to_string(i-1) << "," << to_string(this->stashSizes[i]) << endl;
	}
	sim.close();
}

void StashSizeSimulator::runSimulation() {
	
	int bSize = this->bucketSize;
	int nBlocks = this->numBlocks;
	int nAccesses = this->numAccesses;

	int* recSizes = new int[nBlocks+1];

	for (int i=0; i < nBlocks+1; i++) {
		recSizes[i] = 0;
	}

	Bucket::setMaxSize(bSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();

	OramInterface* oram;

	if (type == 0) {
		oram = new OramReadPathEviction(storage, random, bSize, nBlocks);	
	}
	else {
		oram = new OramDeterministic(storage, random, bSize, nBlocks);
	}

	cout << "Warming up the stash by writing blocks" << endl;

	for(int i = 0; i < 3000000; i++){
		int* accessed = oram->access(OramInterface::Operation::WRITE, i % nBlocks, this->sampleData(i));
	}

	cout << "Stash warmed up: starting to record results." << endl;
	for(int i = 0; i < nAccesses; i++){
		int* accessed = oram->access(OramInterface::Operation::READ, i % nBlocks, NULL);
		int rec = oram->getStashSize();
		recSizes[rec]++;
		if (i%1000000 == 0) {
			cout << "Accessed " << to_string(i) << " Recorded size = " << to_string(rec) << endl;
		}
		
	}

	cout << "Writing results" << endl;
	for (int i=0; i<nBlocks+1;i++) {
		updateStashSizes(i, recSizes[i]);
	}
	this->writeSimulation();

}
