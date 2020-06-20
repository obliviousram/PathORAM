

#include "OramInterface.h"
#include "OramReadPathEviction.h"
#include "UntrustedStorageInterface.h"
#include "ServerStorage.h"

#include <vector>


class CorrectnessTester5 {
	public:

		CorrectnessTester5();
		int* sampleData(int i);
		void runCorrectnessTest();

};