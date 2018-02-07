#include "test/ThreadsTest.h"
#include "test/ParallerAccumulateTest.h"

int main()
{
    tinystl::parallel::ParallerAccumulateTest::testAllCase();
    /* tinystl::net::ThreadsTest::testAllCase(); */

    return 0;
}
