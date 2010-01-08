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

#ifndef __TESTCPP_ASSERTER_H
#define __TESTCPP_ASSERTER_H

#include <testcpp/testcpp.h>
#include <testcpp/AssertionFailure.h>
#include <testcpp/utils/Formatter.h>
#include <sstream>

TESTCPP_NS_START

//////////////////////////////////////////////////////////////////
#define TS_ASSERT(expr) do { \
   if(!(expr)) {\
      throw TESTCPP_NS::AssertionFailure(__FILE__, __LINE__, \
         "expected (" #expr ") being TRUE, but it's actually FALSE"); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_TRUE(expr) TS_ASSERT(expr)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_FALSE(expr) do { \
   if(expr) {\
      throw TESTCPP_NS::AssertionFailure(__FILE__, __LINE__, \
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
         << TESTCPP_NS::toTypeAndValueString(__expected) \
         << " != " \
         << TESTCPP_NS::toTypeAndValueString(__value) \
         << ")"; \
      throw TESTCPP_NS::AssertionFailure(__FILE__, __LINE__, ss.str()); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_NOT_EQUALS(expected, value) do { \
   typeof(expected) __expected = (expected); \
   typeof(value) __value = (value); \
   if(__expected == __value) {\
      std::stringstream ss; \
      ss << "expected (" #expected " != " #value "), but actually got (" \
         << TESTCPP_NS::toTypeAndValueString(__expected) \
         << " == " \
         << TESTCPP_NS::toTypeAndValueString(__value) \
         << ")"; \
      throw TESTCPP_NS::AssertionFailure(__FILE__, __LINE__, ss.str()); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_THROWS(expr, except) do { \
   bool testcpp_caught_exception = false; \
   try { \
      expr; \
   }catch(except&){ \
      testcpp_caught_exception = true; \
   } \
   if(!testcpp_caught_exception) { \
      throw TESTCPP_NS::AssertionFailure(__FILE__, __LINE__, \
         "expected " #expr " will throw an exception of type " #except ", but actually not."); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_THROWS_ANYTHING(expr) do { \
   bool testcpp_caught_exception = false; \
   try { \
      expr; \
   }catch(...){ \
      testcpp_caught_exception = true; \
   } \
   if(!testcpp_caught_exception) { \
      throw TESTCPP_NS::AssertionFailure(__FILE__, __LINE__, \
         "expected " #expr " will throw an exception of any type, but actually not."); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_THROWS_NOTHING(expr) do { \
   try { \
      expr; \
   }catch(...){ \
      throw TESTCPP_NS::AssertionFailure(__FILE__, __LINE__, \
          "expected " #expr " will not throw any exceptions, but it actually did."); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_ASSERT_THROWS_EQUALS(expr, except, expected, value) do { \
   try { \
      expr; \
      throw TESTCPP_NS::AssertionFailure(__FILE__, __LINE__, \
          "expected " #expr " will throw an exception of type " #except ", but actually not."); \
   }catch(except){ \
      TS_ASSERT_EQUALS(expected, value); \
   } \
}while(0)

//////////////////////////////////////////////////////////////////
#define TS_FAIL(msg) do { \
    throw TESTCPP_NS::AssertionFailure(__FILE__, __LINE__, msg); \
}while(0)

//////////////////////////////////////////////////////////////////

TESTCPP_NS_END

#endif

