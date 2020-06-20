#include "catch.h"
#include "Bucket.h"
#include "ServerStorage.h"
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "RandomForOram.h"
#include "UntrustedStorageInterface.h"
#include "OramReadPathEviction.h"
#include "OramDeterministic.h"


TEST_CASE("Test deterministic ORAM ReverseBits")
{	
	int bucketSize = 2;
	int numBlocks = 32;
	Bucket::setMaxSize(bucketSize);	

	UntrustedStorageInterface* storage = new ServerStorage();

	RandForOramInterface* random = new RandomForOram();
	OramDeterministic* oram = new OramDeterministic(storage, random, bucketSize, numBlocks);
    REQUIRE(oram->ReverseBits(0, 4) == 0);
    REQUIRE(oram->ReverseBits(1, 4) == 8);
    REQUIRE(oram->ReverseBits(8, 4) == 1);
    REQUIRE(oram->ReverseBits(8, 5) == 2);
    REQUIRE(oram->ReverseBits(8, 6) == 4);
    REQUIRE(oram->ReverseBits(9, 4) == 9);
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}

TEST_CASE("Test deterministic ORAM P")
{   
    int bucketSize = 2;
    int numBlocks = 32;
    Bucket::setMaxSize(bucketSize); 

    UntrustedStorageInterface* storage = new ServerStorage();

    RandForOramInterface* random = new RandomForOram();
    OramDeterministic* oram = new OramDeterministic(storage, random, bucketSize, numBlocks);
    REQUIRE(oram->P(0, 0) == 0);
    REQUIRE(oram->P(0, 1) == 1);
    REQUIRE(oram->P(1, 1) == 1);
    REQUIRE(oram->P(20, 4) == 25);
    REQUIRE(oram->P(31, 5) == 62);
    Bucket::resetState();
    ServerStorage::is_initialized = false;
    ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;
}
