
#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestCaseHierarchyImpl;
struct TestFixtureDesc;
struct TestCaseFilter;

struct TestCaseHierarchy
{
   TestCaseHierarchy
      ( const TestFixtureDesc* fixture
      , const TestCaseFilter* filter);

   ~TestCaseHierarchy();

private:
   TestCaseHierarchyImpl *This;
};


TESTNGPP_NS_END

