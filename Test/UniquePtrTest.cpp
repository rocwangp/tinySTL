
#include "UniquePtrTest.h"

#include "../string.h"

namespace tinystl{
	namespace UniquePtrTest{
		void testCase1(){
			unique_ptr<int> sp1(new int(10));
			assert(*(sp1.get()) == 10);
			
            auto p = sp1.release();
            assert(sp1.get() == nullptr);
            assert(*p == 10);

            p = new int(100);
            sp1.reset(p);
            assert(*(sp1.get()) == 100);
            assert(sp1.get() == p);

            assert(*sp1 == 100);
            
            auto sp2 = make_unique<string>(10, '0');
            assert(*sp2 == "0000000000");

            unique_ptr<int> spp;
            assert(spp == nullptr);
            assert(!(spp != nullptr));
		}

		void testAllCases(){
			testCase1();
		}
	}
}


