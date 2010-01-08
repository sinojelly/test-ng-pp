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

#ifndef __TESTCPP_HPP
#define __TESTCPP_HPP

#include <testcpp/testcpp.h>
#include <testcpp/Asserter.h>
#include <testcpp/TestFixture.h>
#include <testcpp/ResourceCheckPoint.h>
#include <testcpp/TestCaseSuccess.h>

#define __BEGIN_TEST     try {
#define __CLEANUP        throw TESTCPP_NS::TestCaseSuccess(); } catch(...) {
#define __END_TEST       throw; }

#endif

