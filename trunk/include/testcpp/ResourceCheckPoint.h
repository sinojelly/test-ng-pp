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

#ifndef __TESTCPP_RESOURCE_CHECK_POINT_H
#define __TESTCPP_RESOURCE_CHECK_POINT_H

#include <testcpp/testcpp.h>
#include <testcpp/AssertionFailure.h>
#include <testcpp/Error.h>

TESTCPP_NS_START

///////////////////////////////////////////////////////////
struct ResourceCheckPoint
{
   size_t memory;
   unsigned int fds;
};

#define TESTCPP_RCP TESTCPP_NS::ResourceCheckPoint
///////////////////////////////////////////////////////////
ResourceCheckPoint testcppSetCheckPoint();
void testcppVerifyCheckPoint(const ResourceCheckPoint& rcp
        , const char*, unsigned int) throw (Error, AssertionFailure);

///////////////////////////////////////////////////////////
#define TESTCPP_SET_RESOURCE_CHECK_POINT() \
   TESTCPP_NS::testcppSetCheckPoint()

///////////////////////////////////////////////////////////
#define TESTCPP_VERIFY_RESOURCE_CHECK_POINT(rcp) do { \
   TESTCPP_NS::testcppVerifyCheckPoint(rcp, __FILE__, __LINE__); \
}while(0)

///////////////////////////////////////////////////////////

TESTCPP_NS_END

#endif

