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

#ifndef __TEST_CPP_ERROR_H
#define __TEST_CPP_ERROR_H

#include <exception>
#include <string>

#include <testcpp/testcpp.h>

TESTCPP_NS_START

struct Error : public std::exception
{
   Error(const std::string& msg)
		: reason(msg)
   {}
   ~Error() throw() {}

   const char* what() const throw()
   {
      return reason.c_str();
   }

private:
   std::string reason;
};

TESTCPP_NS_END

#endif

