//
//
//

#include "OramReadPathEviction.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

OramReadPathEviction::OramReadPathEviction(UntrustedStorageInterface* storage, RandForOramInterface* rand_gen,
                                           int bucket_size, int num_blocks) {
    this->storage = storage;
    this->rand_gen = rand_gen;
    this->bucket_size = bucket_size;
    this->num_blocks = num_blocks;
    this->num_levels = ceil(log10(num_blocks) / log10(2)) + 1;
    this->num_buckets = pow(2, num_levels)-1;
    if (this->num_buckets*this->bucket_size < this->num_blocks) //deal with precision loss
    {
        throw new runtime_error("Not enough space for the acutal number of blocks.");
    }
    this->num_leaves = pow(2, num_levels-1);
    Bucket::resetState();
    Bucket::setMaxSize(bucket_size);
    this->rand_gen->setBound(num_leaves);
    this->storage->setCapacity(num_buckets);
    this->position_map = new int[this->num_blocks];
    this->stash = vector<Block>();
    
    for (int i = 0; i < this->num_blocks; i++){
        position_map[i] = rand_gen->getRandomLeaf();
    }

    for(int i = 0; i < num_buckets; i++){

        Bucket init_bkt = Bucket();
        for(int j = 0; j < bucket_size; j++){
            init_bkt.addBlock(Block());
        }
        storage->WriteBucket(i, Bucket(init_bkt));
    }

}

int* OramReadPathEviction::access(Operation op, int blockIndex, int *newdata) {

    int *data = new int[Block::BLOCK_SIZE];
    int oldLeaf = position_map[blockIndex];
    position_map[blockIndex] = rand_gen->getRandomLeaf();
    for (int i = 0; i < num_levels; i++) {
        vector<Block> blocks = storage->ReadBucket(OramReadPathEviction::P(oldLeaf, i)).getBlocks();
        for (Block b: blocks) {
            if (b.index != -1) {
                stash.push_back(Block(b));
            }
        }

    }

    Block *targetBlock = NULL;
    int targetPos = 0;
    for (int i = 0; i < stash.size(); i++) {
        Block b = stash[i];
        if (b.index == blockIndex) {
            targetBlock = &b;
            targetPos = i;
            break;
        }
    }


    if (op == Operation::WRITE) {
        if (targetBlock == NULL) {
            Block newBlock = Block(position_map[blockIndex], blockIndex, newdata);
            stash.push_back(newBlock); 
        } else {
            for(int i =0; i < Block::BLOCK_SIZE; i++){
                targetBlock->data[i] = newdata[i];
            }
            stash[targetPos] = Block(*targetBlock);
        }
    } 
    else {
        if(targetBlock == NULL){
            data = NULL;
        }
        else {
            for(int i = 0; i < Block::BLOCK_SIZE; i++){
                data[i] = targetBlock->data[i];
            }
        }
    }


    // Eviction steps: write to the same path that was read from.
    for (int l = num_levels - 1; l >= 0; l--) {

        vector<int> bid_evicted = vector<int>();
        Bucket bucket = Bucket();
        int Pxl = P(oldLeaf, l);
        int counter = 0;

        for (Block b_instash : stash) {
            
            if (counter >= bucket_size) {
                break;
            }
            Block be_evicted = Block(b_instash);
            if (Pxl == P(position_map[be_evicted.index], l)) {
                bucket.addBlock(be_evicted);

                bid_evicted.push_back(be_evicted.index);
                counter++;
            }
        }

        //remove from the stash
        for(int i = 0; i < bid_evicted.size(); i++)
        {   
            
            for(int j=0; j<stash.size(); j++)
            {
                Block b_instash = stash.at(j);
                if(b_instash.index == bid_evicted.at(i))
                {   
                    this->stash.erase(this->stash.begin() + j);

                    break;
                }
            }
            
        }

        while(counter < bucket_size)
        {
            bucket.addBlock(Block()); //dummy block
            counter++;
        }
        storage->WriteBucket(Pxl, bucket);

    }

    return data;
    
}

int OramReadPathEviction::P(int leaf, int level) {
    /*
    * This function should be deterministic. 
    * INPUT: leaf in range 0 to num_leaves - 1, level in range 0 to num_levels - 1. 
    * OUTPUT: Returns the location in the storage of the bucket which is at the input level and leaf.
    */
    return (1<<level) - 1 + (leaf >> (this->num_levels - level - 1));
}


/*
The below functions are to access various parameters, as described by their names.
INPUT: No input
OUTPUT: Value of internal variables given in the name.
*/

int* OramReadPathEviction::getPositionMap() {
    return this->position_map;
}

vector<Block> OramReadPathEviction::getStash() {
    return this->stash;
}
    
int OramReadPathEviction::getStashSize() {
    return (this->stash).size();
}
    
int OramReadPathEviction::getNumLeaves() {
    return this->num_leaves;

}

int OramReadPathEviction::getNumLevels() {
    return this->num_levels;

}

int OramReadPathEviction::getNumBlocks() {
    return this->num_blocks;

}

int OramReadPathEviction::getNumBuckets() {
    return this->num_buckets;

}

