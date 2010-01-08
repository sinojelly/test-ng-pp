/**
    CxxTestPlus is a practical, easy-to-use C/C++ xUnit framework.
    Copyright (C) <2009>  <Arthur Yuan: arthur.ii.yuan@gmail.com>

    CxxTestPlus is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CxxTestPlus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CxxTestPlus.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef __TESTCPP_TEST_FIXTURE_DESC_H
#define __TESTCPP_TEST_FIXTURE_DESC_H

#include <string>

#include <testcpp/testcpp.h>

#include <testcpp/internal/TestFixtureInfoReader.h>
#include <testcpp/TestFixture.h>

TESTCPP_NS_START

struct TestCase;

struct TestFixtureDesc
   : public TestFixtureInfoReader
{
   TestFixtureDesc( const std::string& fixtureName
                  , const std::string fileName
                  , TestFixture* fixture
                  , TestCase** arrayOfTestCases
                  , unsigned int sizeOfArray)
      : nameOfFixture(fixtureName)
      , nameOfFile(fileName)
      , testCases(arrayOfTestCases)
      , numberOfTestCases(sizeOfArray)
      , testFixture(fixture)
   {}

   const std::string& getName() const
   { return nameOfFixture; }

   const std::string& getFileName() const
   { return nameOfFile; }

   const unsigned int getNumberOfTestCases() const
   { return numberOfTestCases; }

   TestFixture* getFixture() const
   { return testFixture; }

   TestCase* getTestCase(unsigned int index) const
   {
      if(index >= numberOfTestCases)
      {
         return 0;
      }

      return testCases[index];
   }

private:
   TestCase** testCases;
   unsigned int numberOfTestCases;
   TestFixture* testFixture;
   std::string nameOfFixture;
   std::string nameOfFile;
};

TESTCPP_NS_END

#endif

