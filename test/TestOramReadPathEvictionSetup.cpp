#include "catch.h"
#include "Bucket.h"
#include "ServerStorage.h"
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "RandomForOram.h"
#include "UntrustedStorageInterface.h"
#include "OramReadPathEviction.h"
#include "OramDeterministic.h"


TEST_CASE("Test ORAM with read path eviction setup very small numBlocks")
{	
	int bucketSize = 2;
	int numBlocks = 1;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramInterface* oram = new OramDeterministic(storage, random, bucketSize, numBlocks);
    REQUIRE(oram->getNumBlocks() == 1);
    REQUIRE(oram->getNumLevels() == 1);
    REQUIRE(oram->getNumLeaves() == 1);
    REQUIRE(oram->getNumBuckets() == 1);
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}

TEST_CASE("Test ORAM with read path eviction setup small numBlocks")
{	
	int bucketSize = 2;
	int numBlocks = 32;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramInterface* oram = new OramDeterministic(storage, random, bucketSize, numBlocks);
    REQUIRE(oram->getNumBlocks() == 32);
    REQUIRE(oram->getNumLevels() == 6);
    REQUIRE(oram->getNumLeaves() == 32);
    REQUIRE(oram->getNumBuckets() == 63);
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}


TEST_CASE("Test ORAM with read path eviction setup larger numBlocks")
{	
	int bucketSize = 2;
	int numBlocks = 1024;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    REQUIRE(oram->getNumBlocks() == 1024);
    REQUIRE(oram->getNumLevels() == 11);
    REQUIRE(oram->getNumLeaves() == 1024);
    REQUIRE(oram->getNumBuckets() == 2047);
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}

TEST_CASE("Test ORAM with read path eviction setup very large numBlocks")
{	
	int bucketSize = 2;
	int numBlocks = 1048576;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    REQUIRE(oram->getNumBlocks() == 1048576);
    REQUIRE(oram->getNumLevels() == 21);
    REQUIRE(oram->getNumLeaves() == 1048576);
    REQUIRE(oram->getNumBuckets() == 2097151);
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}


TEST_CASE("Test ORAM with read path eviction setup numBlocks not power of 2")
{	
	int bucketSize = 2;
	int numBlocks = 30;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramInterface* oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    REQUIRE(oram->getNumBlocks() == 30);
    REQUIRE(oram->getNumLevels() == 6);
    REQUIRE(oram->getNumLeaves() == 32);
    REQUIRE(oram->getNumBuckets() == 63);
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}