

#include "OramInterface.h"
#include "OramReadPathEviction.h"
#include "UntrustedStorageInterface.h"
#include "ServerStorage.h"

#include <vector>


class CorrectnessTester1 {
	public:

		CorrectnessTester1();
		int* sampleData(int i);
		void runCorrectnessTest();

};