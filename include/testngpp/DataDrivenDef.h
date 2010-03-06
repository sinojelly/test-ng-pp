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

#ifndef __TESTNGPP_DATA_DRIVEN_DEF_H
#define __TESTNGPP_DATA_DRIVEN_DEF_H

#include <testngpp/testngpp.h>
#include <testngpp/Any.h>

TESTNGPP_NS_START

struct DataProvider
{
   virtual const unsigned int size() const = 0;
   virtual const unsigned int numberOfItems() const = 0;
   virtual TESTNGPP_NS::any data(unsigned int index, unsigned int item) const = 0;
   virtual const char* toString(unsigned int index) const = 0;
   virtual const char* getName() const = 0;

   virtual ~DataProvider() {}
};

#define __TESTNGPP_DATA_GROUP(...) {(const char*)#__VA_ARGS__, __VA_ARGS__ }
#define DATA_GROUP(data1, ...) __TESTNGPP_DATA_GROUP(data1, ##__VA_ARGS__)

#define DATA_PROVIDER(name, items, data1, ...)\
struct __TESTNGPP_DataProvider_##name : public TESTNGPP_NS::DataProvider \
{ \
   const unsigned int size() const\
   { \
      const TESTNGPP_NS::any name[][items+1] = {data1, ##__VA_ARGS__ }; \
      return sizeof(name)/sizeof(name[0]); \
   } \
   const unsigned int numberOfItems() const \
   { \
      return items; \
   } \
   \
   TESTNGPP_NS::any data(unsigned int index, unsigned int item) const \
   { \
      const TESTNGPP_NS::any name[][items+1] = {data1, ##__VA_ARGS__ }; \
      return name[index][item+1]; \
   } \
   \
   const char* toString(unsigned int index) const  \
   { \
      const TESTNGPP_NS::any name[][items+1] = {data1, ##__VA_ARGS__ }; \
      return TESTNGPP_NS::any_cast<const char*>(name[index][0]); \
   } \
   const char * getName() const \
   { return #name; } \
} name

TESTNGPP_NS_END

#endif

