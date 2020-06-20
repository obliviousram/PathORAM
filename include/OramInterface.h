//
//
//

#ifndef PORAM_ORAMINTERFACE_H
#define PORAM_ORAMINTERFACE_H
#include "Block.h"
#include <vector>

class OramInterface {
public:
    enum Operation {READ,WRITE};
    virtual int* access(Operation op, int blockIndex,
                                    int newdata[]) { 
                                    int dummy = 1000;
                                    int* dummy_ptr = &dummy;
                                    return 0; };
    virtual int P(int leaf, int level) { return 0; };
    virtual int* getPositionMap() { int dummy = 1000;
                                    int* dummy_ptr = &dummy;
                                    return 0; };
    virtual vector<Block> getStash() { return vector<Block>(); };
    virtual int getStashSize() { return 0; };
    virtual int getNumLeaves() { return 0; };
    virtual int getNumLevels() { return 0; };
    virtual int getNumBlocks() { return 0; };
    virtual int getNumBuckets() { return 0; };
};


#endif //PORAM_ORAMINTERFACE_H
