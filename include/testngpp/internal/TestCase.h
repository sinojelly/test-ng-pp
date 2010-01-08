/**
    TestNG++ is a practical, easy-to-use C/C++ xUnit framework.
    Copyright (C) <2009>  <Arthur Yuan: arthur.ii.yuan@gmail.com>

    TestNG++ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TestNG++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TestNG++.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef __TESTNGPP_TEST_CASE_H
#define __TESTNGPP_TEST_CASE_H

#include <testngpp/testngpp.h>
#include <string>

#include <testngpp/internal/TestCaseInfoReader.h>


TESTNGPP_NS_START

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

TESTNGPP_NS_END

#endif

