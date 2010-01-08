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

#ifndef __TESTCPP_TEST_CASE_H
#define __TESTCPP_TEST_CASE_H

#include <testcpp/testcpp.h>
#include <string>

#include <testcpp/internal/TestCaseInfoReader.h>


TESTCPP_NS_START

struct TestCase
   : public TestCaseInfoReader
{
	TestCase( const std::string& nameOfCase
           , const std::string& nameOfFixture
           , const std::string& file
           , unsigned int line)
		: name(nameOfCase)
      , fixtureName(nameOfFixture)
      , fileName(file)
      , lineOfFile(line)
   {}

	const std::string& getName() const
	{ return name; }

	const std::string& getNameOfFixture() const
	{ return fixtureName; }

	const std::string& getFileName() const
	{ return fileName; }

	const unsigned int getLineOfFile() const
	{ return lineOfFile; }

   virtual void run() = 0;

	virtual ~TestCase() {}

private:
	std::string name;
	std::string fixtureName;
   std::string fileName;
	unsigned int lineOfFile;
};

TESTCPP_NS_END

#endif

