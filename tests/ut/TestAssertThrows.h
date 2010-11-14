#include <testngppst/testngppst.hpp>
#include <mockcpp/mokc.h>

FIXTURE(TestAssertThrows)
{
	struct Expected{};
	struct Actual{};
	
    TEST(ASSERT_THROWS can rethrow unexpected exception)
    {
	    ASSERT_THROWS(ASSERT_THROWS(throw Actual(), Expected), Actual); // only one FAILURE info is OK!
    }
};

