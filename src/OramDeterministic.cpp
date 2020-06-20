//
//
//

#include "OramDeterministic.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <bitset>

using namespace std;

OramDeterministic::OramDeterministic(UntrustedStorageInterface* storage, RandForOramInterface* rand_gen,
                                           int bucket_size, int num_blocks) {
    this->G = 0;
    this->storage = storage;
    this->rand_gen = rand_gen;
    this->bucket_size = bucket_size;
    this->num_blocks = num_blocks;
    this->num_levels = ceil(log10(num_blocks) / log10(2)) + 1;
    this->num_buckets = pow(2, num_levels)-1;
    if (this->num_buckets*this->bucket_size < this->num_blocks) //deal with precision loss
    {
        throw new runtime_error("Not enough space for the actual number of blocks.");
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


    // Initialize all blocks in all buckets as empty.
    for(int i = 0; i < num_buckets; i++){

        Bucket init_bkt = Bucket();
        for(int j = 0; j < bucket_size; j++){
            init_bkt.addBlock(Block());
        }
        storage->WriteBucket(i, Bucket(init_bkt));
    }
}

int* OramDeterministic::access(Operation op, int blockIndex, int *newdata) {
    int *data = new int[Block::BLOCK_SIZE];
    int oldLeaf = position_map[blockIndex];
    position_map[blockIndex] = rand_gen->getRandomLeaf();
    Block *targetBlock = NULL;
    for (int i = 0; i < num_levels; i++) {
        vector<Block> blocks = storage->ReadBucket(P(oldLeaf, i)).getBlocks();
        Bucket write_back = Bucket();
        for (Block b: blocks) {
            if (b.index != blockIndex) {
                write_back.addBlock(Block(b));
            } else {
                targetBlock = new Block(b);
                write_back.addBlock(Block());

            }
        }
        storage->WriteBucket(P(oldLeaf, i), &write_back);
    }


    if (op == Operation::WRITE) {

        if (targetBlock == NULL) {
            Block newBlock = Block(position_map[blockIndex], blockIndex, newdata);
            stash.push_back(newBlock);
        } else {
            for (int i = 0; i < Block::BLOCK_SIZE; i++) {
                targetBlock->data[i] = newdata[i];
            }
            stash.push_back(Block(*targetBlock));

        }
    } else { 
        if (targetBlock == NULL) {
            data = NULL;
        }
        else {
            for (int i = 0; i < Block::BLOCK_SIZE; i++) {
                data[i] = targetBlock->data[i];
            }
            stash.push_back(Block(*targetBlock));
        }
    }

    /* 
    Eviction steps: Two paths will be evicted and re-written
    as determined by ReverseBits
    */
    for (int repeat = 0; repeat < 2; repeat++) {

        int g = ReverseBits(this->G, num_levels - 1);
        this->G = this->G + 1;
        for (int l = 0; l < num_levels; l++) {
            
            vector<Block> blocks = vector<Block>();
            /*
            Read blocks on current level in the path and write to stash
            */
            try {
                blocks = storage->ReadBucket(P(g, l)).getBlocks();
            }
            catch (exception& e) {
                cout << e.what() << endl;
            }
            
            for (Block b : blocks) {
                if (b.index != -1) {
                    stash.push_back(Block(b));
                }
            }
        }
        
        /*
        Re-write blocks from the stash to current path whenever possible, starting at the deepest level
        */
        for (int l = num_levels - 1; l >= 0; l--) {
            vector<int> bid_evicted = vector<int>();
            Bucket bucket = Bucket();
            int Pxl = P(g, l);
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
                    }
                }
                
            }
            
            while (counter < bucket_size) {
                bucket.addBlock(Block()); //dummy block
                counter++;
            }
            storage->WriteBucket(Pxl, bucket);

        }
    }

    return data;
}




int OramDeterministic::ReverseBits(int g, int bits_length) {
    /*
    INPUT: Integers g and bits_length.
    OUTPUT: Integer reverse of length bits_length consisting of reversed bits of g.
    To be used to traverse the leaves and run eviction in reverse lexicographical order.
    */
    int g_mod = g%num_leaves;
    int reverse = 0;
    while(g_mod) {
        reverse <<= 1;
        reverse |= g_mod & 1;
        g_mod >>= 1;
        bits_length--;
    }
    
    reverse <<= bits_length;
    return reverse;
}


int OramDeterministic::P(int leaf, int level) {
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


int *OramDeterministic::getPositionMap() {
    return this->position_map;
}

vector<Block> OramDeterministic::getStash() {
    return this->stash;
}

int OramDeterministic::globalG() {
    return this->G;
}

    
int OramDeterministic::getStashSize() {
    return (this->stash).size();
}
    
int OramDeterministic::getNumLeaves() {
    return this->num_leaves;

}

int OramDeterministic::getNumLevels() {
    return this->num_levels;

}

int OramDeterministic::getNumBlocks() {
    return this->num_blocks;

}

int OramDeterministic::getNumBuckets() {
    return this->num_buckets;

}

