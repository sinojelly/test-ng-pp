
#ifndef __TESTNGPP_MY_TEST_CASE_H__
#define __TESTNGPP_MY_TEST_CASE_H__

#include <testngpp/internal/TestCase.h>
#include <testngpp/TestFixture.h>

struct MyTestCase : public TESTNGPP_NS::TestCase
{
   MyTestCase( const std::string& nameOfCase
           , const std::string& nameOfFixture
           , const std::string& file
           , unsigned int line)
           : TESTNGPP_NS::TestCase(nameOfCase, nameOfFixture, file, line)
   {}

   TESTNGPP_NS::TestFixture* getFixture() const { return 0; }

   void run() {}
   void setUp(TESTNGPP_NS::TestFixture* ) {}
   void tearDown() {}
};

#endif

