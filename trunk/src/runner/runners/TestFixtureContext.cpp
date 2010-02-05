
#include <testngpp/internal/TestFixtureDesc.h>

#include <testngpp/runner/TagsFilters.h>
#include <testngpp/runner/FixtureTagsFilter.h>
#include <testngpp/runner/TestFixtureContext.h>


TESTNGPP_NS_START

/////////////////////////////////////////////////////////////////
struct TestFixtureContextImpl
{
   TestFixtureContextImpl(TestFixtureDesc* desc, TagsFilters* filters);

   TestFixtureDesc* fixture; // X
   FixtureTagsFilter tagsFilter;
};

/////////////////////////////////////////////////////////////////
TestFixtureContextImpl::
TestFixtureContextImpl
      ( TestFixtureDesc* desc
      , TagsFilters* filters)
      : fixture(desc)
      , tagsFilter(filters)
{
}

/////////////////////////////////////////////////////////////////
TestFixtureContext::
TestFixtureContext
      ( TestFixtureDesc* desc
      , TagsFilters* filters)
      : This( new TestFixtureContextImpl
               ( desc
               , filters))
{
}

/////////////////////////////////////////////////////////////////
TestFixtureContext::
~TestFixtureContext()
{
   delete This;
}

/////////////////////////////////////////////////////////////////
TestFixtureDesc*
TestFixtureContext::
getFixture() const
{
   return This->fixture;
}

/////////////////////////////////////////////////////////////////
FixtureTagsFilter*
TestFixtureContext::
getTagsFilter() const
{
   return &This->tagsFilter;
}

/////////////////////////////////////////////////////////////////

TESTNGPP_NS_END

