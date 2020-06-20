//
//
//

#ifndef PORAM_UNTRUSTEDSTORAGEINTERFACE_H
#define PORAM_UNTRUSTEDSTORAGEINTERFACE_H
#include "Bucket.h"

class UntrustedStorageInterface {
    //You are required to call setCapacity BEFORE reading or writing any buckets. You are not allowed to change the capacity after you set it.
    //You should only use ONE UntrustedStorage in your ORAM.
public:
    virtual void setCapacity(int totalNumOfBuckets) {};

    virtual Bucket ReadBucket(int position) { return Bucket(); };

    virtual void WriteBucket(int position, const Bucket& bucket_to_write) {};
};


#endif //PORAM_UNTRUSTEDSTORAGEINTERFACE_H
