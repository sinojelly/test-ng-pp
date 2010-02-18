/***
    mockcpp is a generic C/C++ mock framework.
    Copyright (C) <2009>  <Darwin Yuan: darwin.yuan@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#include <iostream>
#include <mockcpp/DestructorChecker.h>
#include <mockcpp/MethodInfoReader.h>
#include <mockcpp/Asserter.h>
#include <mockcpp/VirtualTableUtils.h>

MOCKCPP_NS_START

///////////////////////////////////////////////////////////////////////
namespace
{

////////////////////////////////////////////////////////////////////////
struct DummyType {};

const unsigned int invalidIndex = 0xFFFFFFFF;

////////////////////////////////////////////////////////////////////////
struct Indices
{
   unsigned int indexOfVPTR;
   unsigned int indexOfVTBL;

   Indices()
      : indexOfVPTR(invalidIndex)
      , indexOfVTBL(invalidIndex)
   {}
};

////////////////////////////////////////////////////////////////////////
struct FakeObject
{
   void* vptr[MOCKCPP_MAX_INHERITANCE];
   void** vtbl;

   Indices indices;

   FakeObject(const std::type_info& info);

   Indices& getIndices() 
   {
      return indices;
   }

   ~FakeObject();
};

FakeObject*
getFakeObject(void* caller, unsigned int vptrIndex)
{
   void** vptr = &((void**)caller)[-(int)vptrIndex];
   return (FakeObject*)vptr;
}

////////////////////////////////////////////////////////////////////////
template <int IndexOfVptr, int IndexOfVtbl, typename T>
struct DestructorChecker
{
   void check(void*)
   {
      FakeObject* fakeObj = getFakeObject(this, IndexOfVptr);
      fakeObj->getIndices().indexOfVPTR = IndexOfVptr;
      fakeObj->getIndices().indexOfVTBL = IndexOfVtbl;
   }
};

///////////////////////////////////////////////////////////////////////
#define MOCKCPP_SET_DESTRUCTOR_CHECKER_VTBL(I, J) do{ \
   vtbl[getRealVtblIndex(I,J)] = getAddrOfMethod(&DestructorChecker<I,J,DummyType>::check); \
}while(0)

////////////////////////////////////////////////////////////////////////
FakeObject::
FakeObject(const std::type_info& info)
{
   vtbl = createVtbls(MOCKCPP_MAX_INHERITANCE);
   #include <mockcpp/DestructorCheckerDef.h>
   initializeVtbls(vptr, vtbl, MOCKCPP_MAX_INHERITANCE, info, false);
}

////////////////////////////////////////////////////////////////////////
FakeObject::
~FakeObject()
{
   freeVtbls(vtbl, MOCKCPP_MAX_INHERITANCE);
}

////////////////////////////////////////////////////////////////////////
}


///////////////////////////////////////////////////////////////////////
void* createDestructorChecker(const std::type_info& info)
{
   return (void*) new FakeObject(info);
}

///////////////////////////////////////////////////////////////////////
#define MOCKCPP_THROW_NO_PURE_VIRTUAL_EXCEPTION() do { \
   MOCKCPP_FAIL("You are trying to mock a non-pure-virtual object"); \
} while(0)

///////////////////////////////////////////////////////////////////////
void getIndicesOfDestructor(void* obj, unsigned int& indexOfVptr, unsigned int& indexOfVtbl)
{
   FakeObject* fakeObj = (FakeObject*) obj;

   indexOfVptr = fakeObj->getIndices().indexOfVPTR;
   indexOfVtbl = fakeObj->getIndices().indexOfVTBL;

   delete fakeObj;

   if(indexOfVptr == invalidIndex)
   {
      MOCKCPP_THROW_NO_PURE_VIRTUAL_EXCEPTION();
   }
}

///////////////////////////////////////////////////////////////////////

MOCKCPP_NS_END

