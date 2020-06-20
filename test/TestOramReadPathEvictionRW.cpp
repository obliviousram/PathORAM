#include "catch.h"
#include "Bucket.h"
#include "ServerStorage.h"
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "RandomForOram.h"
#include "UntrustedStorageInterface.h"
#include "OramReadPathEviction.h"
#include "OramReadPathEviction.h"

int* sampleDataReadPathEviction(int i) {
    int* newArray = new int[Block::BLOCK_SIZE];
    for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
        newArray[j] = i;
    }
    return newArray;
}

TEST_CASE("Test ORAM with read path eviction read very small numBlocks")
{	
	int bucketSize = 2;
	int numBlocks = 1;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);



    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, sampleDataReadPathEviction(i%numBlocks));
    }

    
    
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == i%numBlocks);
        }
        
    }

    
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}

TEST_CASE("Test ORAM with read path eviction read small numBlocks")
{	
	int bucketSize = 2;
	int numBlocks = 32;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, sampleDataReadPathEviction(i%numBlocks));
    }

    
    
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == i%numBlocks);
        }
        
    }

    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}


TEST_CASE("Test ORAM with read path eviction read larger numBlocks")
{	
	int bucketSize = 2;
	int numBlocks = 1024;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    
    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, sampleDataReadPathEviction(i%numBlocks));
    }

    
    
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == i%numBlocks);
        }
        
    }

    
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;

}


TEST_CASE("Test ORAM with read path eviction read numBlocks not power of 2")
{   
    int bucketSize = 2;
    int numBlocks = 30;
    Bucket::setMaxSize(bucketSize); 

    UntrustedStorageInterface* storage = new ServerStorage();

    RandForOramInterface* random = new RandomForOram();
    OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    
    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, sampleDataReadPathEviction(i%numBlocks));
    }

    
    
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == i%numBlocks);
        }
        
    }

    
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}

TEST_CASE("Test ORAM with read path eviction read very small numBlocks, bucketSize = 4")
{   
    int bucketSize = 4;
    int numBlocks = 1;
    Bucket::setMaxSize(bucketSize); 

    UntrustedStorageInterface* storage = new ServerStorage();

    RandForOramInterface* random = new RandomForOram();
    OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);



    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, sampleDataReadPathEviction(i%numBlocks));
    }

    
    
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == i%numBlocks);
        }
        
    }

    
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}

TEST_CASE("Test ORAM with read path eviction read small numBlocks, bucketSize = 4")
{   
    int bucketSize = 4;
    int numBlocks = 32;
    Bucket::setMaxSize(bucketSize); 

    UntrustedStorageInterface* storage = new ServerStorage();

    RandForOramInterface* random = new RandomForOram();
    OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, sampleDataReadPathEviction(i%numBlocks));
    }

    
    
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == i%numBlocks);
        }
        
    }

    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}


TEST_CASE("Test ORAM with read path eviction read larger numBlocks, bucketSize = 4")
{   
    int bucketSize = 4;
    int numBlocks = 1024;
    Bucket::setMaxSize(bucketSize); 

    UntrustedStorageInterface* storage = new ServerStorage();

    RandForOramInterface* random = new RandomForOram();
    OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    
    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, sampleDataReadPathEviction(i%numBlocks));
    }

    
    
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == i%numBlocks);
        }
        
    }

    
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;

}


TEST_CASE("Test ORAM with read path eviction read numBlocks not power of 2, bucketSize = 4")
{   
    int bucketSize = 4;
    int numBlocks = 30;
    Bucket::setMaxSize(bucketSize); 

    UntrustedStorageInterface* storage = new ServerStorage();

    RandForOramInterface* random = new RandomForOram();
    OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    
    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, sampleDataReadPathEviction(i%numBlocks));
    }

    
    
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == i%numBlocks);
        }
        
    }

    
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}

// Uncomment the below test case to run for when the number of blocks is very large (=2^20).

/*
TEST_CASE("Test ORAM read path eviction read very large numBlocks", "[setup_det2]")
{	
	int bucketSize = 2;
	int numBlocks = 1048576;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    
    int bound = numBlocks;
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::WRITE, i % numBlocks, sampleDataReadPathEviction(i%numBlocks));
    }

    
    
    for(int i = 0; i < bound; i++){
        int* accessed = oram->access(OramInterface::Operation::READ, i % numBlocks, NULL);
        
        for (unsigned int j = 0; j<Block::BLOCK_SIZE; ++j) {
            int temp = accessed[j];
            REQUIRE(temp == i%numBlocks);
        }
        
    }

    
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}
*/

