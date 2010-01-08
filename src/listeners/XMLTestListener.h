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

#ifndef __TESTCPP_XML_TEST_LISTENER_H
#define __TESTCPP_XML_TEST_LISTENER_H

#include <testcpp/testcpp.h>
#include <testcpp/runner/TestListener.h>
#include <sstream>

TESTCPP_NS_START

struct TestSuiteInfoReader;
struct TestResultReporter;
struct TestSuiteResultReporter;
struct TestCaseResultReporter;
struct XMLTestListenerImpl;

struct XMLTestListener
   : public TestListener
{
   XMLTestListener( TestResultReporter*
                     , TestSuiteResultReporter*
                     , TestCaseResultReporter*
                     , std::ostream& os);
   ~XMLTestListener();

   void addCaseCrash(TestCaseInfoReader*);
   void addCaseError(TestCaseInfoReader*, const std::string&);
   void addCaseFailure(TestCaseInfoReader*, const AssertionFailure&);
   void startTestCase(TestCaseInfoReader*);
   void endTestCase(TestCaseInfoReader*);

   void startTestFixture(TestFixtureInfoReader*);
   void endTestFixture(TestFixtureInfoReader*);
   void addFixtureError(TestFixtureInfoReader*, const std::string&);
   void addFixtureFailure(TestFixtureInfoReader*, const AssertionFailure&);

   void startTestSuite(TestSuiteInfoReader*);
   void endTestSuite(TestSuiteInfoReader*);
   void addSuiteError(TestSuiteInfoReader*, const std::string&);

   void startTest();
   void endTest();
   void addError(const std::string&);

private:

	XMLTestListenerImpl* This;
};

TESTCPP_NS_END

#endif
