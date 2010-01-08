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

#ifndef __TESTCPP_H
#define __TESTCPP_H

#if !TESTCPP_NO_NAMESPACE
# define TESTCPP_NS testcpp
# define TESTCPP_NS_START namespace TESTCPP_NS {
# define TESTCPP_NS_END }
# define USING_TESTCPP_NS using namespace TESTCPP_NS;
#else
# define TESTCPP_NS 
# define TESTCPP_NS_START 
# define TESTCPP_NS_END 
# define USING_TESTCPP_NS 
#endif


#endif

