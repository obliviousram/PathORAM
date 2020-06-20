#include "catch.h"
#include "Bucket.h"
#include "ServerStorage.h"
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "RandomForOram.h"
#include "UntrustedStorageInterface.h"
#include "OramReadPathEviction.h"
#include "OramDeterministic.h"

int* sampleDataRand(int i) {
    int* newArray = new int[Block::BLOCK_SIZE];
    for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
        newArray[j] = i;
    }
    return newArray;
}

TEST_CASE("Test deterministic ORAM write and read randomly, very small numBlocks")
{	
	int bucketSize = 2;
	int numBlocks = 1;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramInterface* oram = new OramDeterministic(storage, random, bucketSize, numBlocks);

    int naiveRAM[numBlocks][Block::BLOCK_SIZE];
    vector<int> written = vector<int>();

    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int acc = rand() % 2;
        if (acc == 0) {
            int* data = sampleDataRand(i%numBlocks);
            // Copy data into the naive RAM
            for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
                naiveRAM[i][j] = data[j];
            }
            int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, data);
            written.push_back(i);
        }
    }

    
    
    for(int n = 0; n < written.size(); ++n){
        int i = written[n]; 
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == naiveRAM[i][j]);
        }
        
    }

    
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}

TEST_CASE("Test deterministic ORAM write and read randomly, larger numBlocks")
{   
    int bucketSize = 2;
    int numBlocks = 1024;
    Bucket::setMaxSize(bucketSize); 

    UntrustedStorageInterface* storage = new ServerStorage();

    RandForOramInterface* random = new RandomForOram();
    OramInterface* oram = new OramDeterministic(storage, random, bucketSize, numBlocks);

    int naiveRAM[numBlocks][Block::BLOCK_SIZE];
    vector<int> written = vector<int>();

    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int acc = rand() % 2;
        if (acc == 0) {
            int* data = sampleDataRand(i%numBlocks);
            // Copy data into the naive RAM
            for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
                naiveRAM[i][j] = data[j];
            }
            int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, data);
            written.push_back(i);
        }
    }

    
    
    for(int n = 0; n < written.size(); ++n){
        int i = written[n]; 
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == naiveRAM[i][j]);
        }
        
    }

    
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}

TEST_CASE("Test deterministic ORAM rewrite randomly and read, larger numBlocks")
{   
    int bucketSize = 2;
    int numBlocks = 1024;
    Bucket::setMaxSize(bucketSize); 

    UntrustedStorageInterface* storage = new ServerStorage();

    RandForOramInterface* random = new RandomForOram();
    OramInterface* oram = new OramDeterministic(storage, random, bucketSize, numBlocks);

    int naiveRAM[numBlocks][Block::BLOCK_SIZE];
    vector<int> written = vector<int>();

    int bound = 4*numBlocks;
    for(int i = 0; i < bound; i++){
        int acc = rand() % 5;
        if (acc == 0) {
            int* data = sampleDataRand(i%numBlocks);
            // Copy data into the naive RAM
            for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
                naiveRAM[i%numBlocks][j] = data[j];
            }
            int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, data);
            written.push_back(i%numBlocks);
        }
    }

    
    
    for(int n = 0; n < written.size(); ++n){
        int i = written[n]; 
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == naiveRAM[i][j]);
        }
        
    }

    
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}

