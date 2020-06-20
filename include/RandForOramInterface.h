//
//
//

#ifndef PORAM_RANDFORORAMINTERFACE_H
#define PORAM_RANDFORORAMINTERFACE_H

using namespace std;

class RandForOramInterface {
public:
    virtual int getRandomLeaf() {return 0;};

    virtual void setBound(int num_leaves) {};
};


#endif 
