//
//
//

#ifndef PORAM_ORAMDETERMINISTIC_H
#define PORAM_ORAMDETERMINISTIC_H
#include "OramInterface.h"
#include "OramReadPathEviction.h"


class OramDeterministic : public OramInterface {
public:
    UntrustedStorageInterface* storage;
    RandForOramInterface* rand_gen;
    int G;
    int bucket_size;
    int num_levels;
    int num_leaves;
    int num_blocks;
    int num_buckets;
    int *position_map; //array
    vector<Block> stash;

    OramDeterministic(UntrustedStorageInterface *storage,
                      RandForOramInterface *rand_gen, int bucket_size, int num_blocks);
    int* access(Operation op, int block_id, int newdata[]);
    int* getPositionMap();
    vector<Block> getStash();
    int globalG();
    int ReverseBits(int G, int bits_length);
    static string ConvertToBase2(int num);
    int P(int leaf, int level);
    int getStashSize();
    int getNumLeaves();
    int getNumLevels();
    int getNumBlocks();
    int getNumBuckets();

};



#endif //PORAM_ORAMDETERMINISTIC_H
