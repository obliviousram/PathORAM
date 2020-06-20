//
//
//

#ifndef PORAM_ORAMREADPATHEVICTION_H
#define PORAM_ORAMREADPATHEVICTION_H
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "UntrustedStorageInterface.h"
#include <cmath>

class OramReadPathEviction : public OramInterface {
public:
    UntrustedStorageInterface* storage;
    RandForOramInterface* rand_gen;

    int bucket_size;
    int num_levels;
    int num_leaves;
    int num_blocks;
    int num_buckets;

    int *position_map; //array
    vector<Block> stash;

    OramReadPathEviction(UntrustedStorageInterface* storage,
            RandForOramInterface* rand_gen, int bucket_size, int num_blocks);
    int* access(Operation op, int blockIndex, int newdata[]);
    int P(int leaf, int level);
    int* getPositionMap();
    vector<Block> getStash();
    int getStashSize();
    int getNumLeaves();
    int getNumLevels();
    int getNumBlocks();
    int getNumBuckets();

};


#endif 
