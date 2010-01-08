
MOCKCPP_SRC_PATH=$(top_builddir)/src
MOCKCPP_INCLUDE_PATH = $(top_builddir)/include
MOCKCPP_HEADERS_PATH = $(top_builddir)/include/mockcpp

AM_CPPFLAGS = \
 -I$(MOCKCPP_INCLUDE_PATH) \
 -I$(top_builddir)/3rdparty \
 -Wall

AM_LDFLAGS = \
 @MOCKCPP_LDFLAGS_FOR_DIST@ \
 -rpath /dev/null \
 -no-undefined \
 $(top_builddir)/../../src/libtestcpp.la

noinst_LTLIBRARIES = libmockcpp.la

ARG_RELATED_HEADER_FILES =  \
 $(MOCKCPP_HEADERS_PATH)/DelegatedMethodDef.h \
 $(MOCKCPP_HEADERS_PATH)/ArgumentsListDef.h  \
 $(MOCKCPP_HEADERS_PATH)/MethodTypeTraitsDef.h

VBTL_RELATED_HEADER_FILES = \
 $(MOCKCPP_HEADERS_PATH)/DelegatedMethodGetDef.h \
 $(MOCKCPP_HEADERS_PATH)/DelegatedMethodGetByVptrDef.h \
 $(MOCKCPP_HEADERS_PATH)/GenericMethodCheckerDef.h \
 $(MOCKCPP_HEADERS_PATH)/DestructorAddrGetterDef.h \
 $(MOCKCPP_HEADERS_PATH)/DestructorCheckerDef.h

EXTRA_HEADERS = \
 $(ARG_RELATED_HEADER_FILES) \
 $(VBTL_RELATED_HEADER_FILES)

BUILT_SOURCES = $(EXTRA_HEADERS)
CLEANFILES = $(EXTRA_HEADERS)

MOCKCPP_SRCS = \
 AfterMatcher.cpp \
 AnyBase.cpp \
 Any.cpp \
 Asserter.cpp \
 AssertionFailedError.cpp \
 BeforeMatcher.cpp \
 CallerMatcher.cpp \
 ChainableMockMethodCore.cpp \
 ChainableMockObjectBase.cpp \
 ChainableMockObject.cpp \
 MockObjectBase.cpp \
 ChainingMockHelper.cpp \
 ConstraintSet.cpp \
 DecoratedConstraint.cpp \
 DefaultMatcher.cpp \
 DefaultStub.cpp \
 RepeatStub.cpp \
 Exception.cpp \
 ExpectsMatcher.cpp \
 Formatter.cpp \
 GlobalMockObject.cpp \
 IdentityBuilder.cpp \
 IgnoreResultHandler.cpp \
 IgnoreResultHandlerFactory.cpp \
 IgnoreReturnStub.cpp \
 Invocation.cpp \
 InvocationId.cpp \
 InvocationMocker.cpp \
 InvocationMockerSet.cpp \
 InvocationTimesMatcher.cpp \
 InvokedAtLeast.cpp \
 InvokedAtMost.cpp \
 InvokedExactly.cpp \
 InvokedOnce.cpp \
 InvokedTimesMatcher.cpp \
 IsAnythingHelper.cpp \
 MismatchResultHandler.cpp \
 MismatchResultHandlerFactory.cpp \
 NormalResultHandler.cpp \
 NormalResultHandlerFactory.cpp \
 InvocationMockBuilderGetter.cpp \
 OutBoundPointer.cpp \
 PendingMatcher.cpp \
 ProcStub.cpp \
 RefAny.cpp \
 Result.cpp \
 ReturnObjectList.cpp \
 ReturnStub.cpp \
 SimpleInvocationRecorder.cpp \
 StubsMatcher.cpp \
 TestFailureMatcher.cpp \
 TypelessConstraintAdapter.cpp \
 TypelessStubAdapter.cpp \
 TypeString.cpp \
 VoidResultHandler.cpp \
 VoidResultHandlerFactory.cpp \
 WillStub.cpp \
 ThenStub.cpp \
 VirtualTableUtils.cpp \
 VirtualTable.cpp \
 ChainableMockMethodContainer.cpp \
 ChainableMockMethodNameKey.cpp \
 ChainableMockMethodIndexKey.cpp \
 StubContainer.cpp \
 StringConstraint.cpp \
 IsStringStartWith.cpp \
 IsStringEndWith.cpp \
 IsStringContains.cpp \
 DelegatedMethodGetter.cpp \
 DieStub.cpp \
 InterfaceInfo.cpp \
 GenericMethodIndicesChecker.cpp \
 DestructorChecker.cpp

libmockcpp_la_SOURCES = $(MOCKCPP_SRCS)

ALLOW_MI = @MOCKCPP_ALLOW_MI@
MAX_INHERITANCE = @MOCKCPP_MI@
MAX_VTBL_SIZE = @MOCKCPP_MVS@
MAX_PARAMETERS = @MOCKCPP_MAX_PARAMS@

DIFF = diff
COPY = cp
RM = rm -f

EXTRA_VTBL_HEADERS_DEFS = --allow-mi=$(ALLOW_MI) --max-inheritance=$(MAX_INHERITANCE) \
                        --max-vtbl-size=$(MAX_VTBL_SIZE) --include-path=$(MOCKCPP_SRC_PATH)

EXTRA_PARAMS_HEADERS_DEFS = --max-parameters=$(MAX_PARAMETERS) --include-path=$(MOCKCPP_SRC_PATH)

$(VBTL_RELATED_HEADER_FILES) : 
	$(PYTHON) $(top_builddir)/src/generate_vtbl_related_files.py $(EXTRA_VTBL_HEADERS_DEFS) $(@F)
	@$(DIFF) $@ $(@F) >/dev/null 2>&1 || $(COPY) $(MOCKCPP_SRC_PATH)/$(@F) $@
	@$(RM) $(MOCKCPP_SRC_PATH)/$(@F)

$(ARG_RELATED_HEADER_FILES) : 
	$(PYTHON) $(top_builddir)/src/generate_arg_related_files.py $(EXTRA_PARAMS_HEADERS_DEFS) $(@F)
	@$(DIFF) $@ $(@F) >/dev/null 2>&1 || $(COPY) $(MOCKCPP_SRC_PATH)/$(@F) $@
	@$(RM) $(MOCKCPP_SRC_PATH)/$(@F)

