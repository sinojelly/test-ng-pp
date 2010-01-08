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

#ifndef __TESTNGPP_ASSERTER_H
#define __TESTNGPP_ASSERTER_H

#include <testngpp/testngpp.h>
#include <testngpp/AssertionFailure.h>
#include <testngpp/utils/Formatter.h>
#include <sstream>

TESTNGPP_NS_START

//////////////////////////////////////////////////////////////////
#define TS_ASSERT(expr) do { \
   if(!(expr)) {\
      throw TESTNGPP_NS::AssertionFailure(__FILE__, __LINE__, \
         "expected (" #expr ") being TRUE, but it's actually FALSE"); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_TRUE(expr) TS_ASSERT(expr)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_FALSE(expr) do { \
   if(expr) {\
      throw TESTNGPP_NS::AssertionFailure(__FILE__, __LINE__, \
         "expected (" #expr ") being FALSE, but it's actually TRUE"); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_EQUALS(expected, value) do { \
   typeof(expected) __expected = (expected); \
   typeof(value) __value = (value); \
   if(__expected != __value) { \
      std::stringstream ss; \
      ss << "expected (" #expected " == " #value "), but actually got (" \
         << TESTNGPP_NS::toTypeAndValueString(__expected) \
         << " != " \
         << TESTNGPP_NS::toTypeAndValueString(__value) \
         << ")"; \
      throw TESTNGPP_NS::AssertionFailure(__FILE__, __LINE__, ss.str()); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_NOT_EQUALS(expected, value) do { \
   typeof(expected) __expected = (expected); \
   typeof(value) __value = (value); \
   if(__expected == __value) {\
      std::stringstream ss; \
      ss << "expected (" #expected " != " #value "), but actually got (" \
         << TESTNGPP_NS::toTypeAndValueString(__expected) \
         << " == " \
         << TESTNGPP_NS::toTypeAndValueString(__value) \
         << ")"; \
      throw TESTNGPP_NS::AssertionFailure(__FILE__, __LINE__, ss.str()); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_THROWS(expr, except) do { \
   bool testngpp_caught_exception = false; \
   try { \
      expr; \
   }catch(except&){ \
      testngpp_caught_exception = true; \
   } \
   if(!testngpp_caught_exception) { \
      throw TESTNGPP_NS::AssertionFailure(__FILE__, __LINE__, \
         "expected " #expr " will throw an exception of type " #except ", but actually not."); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_THROWS_ANYTHING(expr) do { \
   bool testngpp_caught_exception = false; \
   try { \
      expr; \
   }catch(...){ \
      testngpp_caught_exception = true; \
   } \
   if(!testngpp_caught_exception) { \
      throw TESTNGPP_NS::AssertionFailure(__FILE__, __LINE__, \
         "expected " #expr " will throw an exception of any type, but actually not."); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_THROWS_NOTHING(expr) do { \
   try { \
      expr; \
   }catch(...){ \
      throw TESTNGPP_NS::AssertionFailure(__FILE__, __LINE__, \
          "expected " #expr " will not throw any exceptions, but it actually did."); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_THROWS_EQUALS(expr, except, expected, value) do { \
   try { \
      expr; \
      throw TESTNGPP_NS::AssertionFailure(__FILE__, __LINE__, \
          "expected " #expr " will throw an exception of type " #except ", but actually not."); \
   }catch(except){ \
      TS_ASSERT_EQUALS(expected, value); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_FAIL(msg) do { \
    throw TESTNGPP_NS::AssertionFailure(__FILE__, __LINE__, msg); \
}while(0)

//////////////////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

