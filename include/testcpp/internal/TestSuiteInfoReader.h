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

#ifndef __TESTCPP_TEST_SUITE_INFO_READER_H
#define __TESTCPP_TEST_SUITE_INFO_READER_H

#include <string>

#include <testcpp/testcpp.h>

TESTCPP_NS_START

struct TestSuiteInfoReader
{
	virtual const std::string& getName() const = 0;
	virtual const std::string& getFileName() const = 0;
   virtual const unsigned int getNumberOfTestCases() const = 0;
   virtual const unsigned int getNumberOfTestFixtures() const = 0;

   virtual ~TestSuiteInfoReader() {}
};

TESTCPP_NS_END

#endif

