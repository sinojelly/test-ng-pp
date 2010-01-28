
import os

from Message import *

from Phase1Result import *

def output(content, file):
   if file == None:
      return

   lines = content + "\n"
   file.writelines(lines.encode('utf-8'))

def get_base_name(file):
    return os.path.basename(file).split('.')[0]

def get_fixture_name_in_src(fixture):
   return "TestFixture_" 

def get_testcase_name(testcase):
   if testcase.get_traditional_name():
      return testcase.get_traditional_name()

   return "test_" + str(testcase.get_line_number())

def get_testcase_class_name(fixture, testcase):
   return "TESTCASE_" + get_fixture_id(fixture) + "_" + get_testcase_name(testcase)

def get_testcase_instance_name(fixture, testcase):
   return "testcase_instance_" + get_fixture_id(fixture) + "_" + get_testcase_name(testcase)

def get_fixture_desc_class():
   return "TESTNGPP_NS::TestFixtureDesc"

def get_fixture_desc_var(fixture):
   return "test_fixture_desc_instance_" + get_fixture_id(fixture)

def get_file_name(testcase):
   return os.path.basename(testcase.get_file_name())

def get_depends_var(fixture, testcase):
   depends = testcase.get_depends()
   if depends == None: return "0"
   return get_testcase_instance_name(fixture, depends)

def get_fixture_id(fixture):
   return fixture.get_id()

testcase_template = '''
static struct %s 
   : public TESTNGPP_NS::TestCase
{
   %s()
      : TESTNGPP_NS::TestCase
        ( "%s"
        , "%s"
        , %s
        , "%s"
        , %d)
   {}

   void setUp(TESTNGPP_NS::TestFixture* fixture)
   {
      if(fixture == 0)
      {
         belongedFixture = new %s();
         belongedFixture->setUp();
      }
      else
      {
         belongedFixture = dynamic_cast<%s*>(fixture);
      }
   }

   void tearDown()
   {
      belongedFixture->tearDown();
   }

   void run()
   {
      belongedFixture->%s();
   }

   TESTNGPP_NS::TestFixture* getFixture() const
   {
      return belongedFixture;
   }

private:
   %s* belongedFixture; 
} %s ;

'''

################################################
class TestCaseDefGenerator:
   #############################################
   def __init__(self, file, testcase, fixture):
      self.fixture = fixture
      self.testcase = testcase
      self.file = file
   #############################################
   def __generate(self):
      testcase_def = testcase_template % ( \
         get_testcase_class_name(self.fixture, self.testcase), \
         get_testcase_class_name(self.fixture, self.testcase), \
         self.testcase.get_name(), \
         self.fixture.get_name(), \
         get_depends_var(self.fixture, self.testcase), \
         get_file_name(self.testcase), \
         self.testcase.get_line_number(), \
         get_fixture_id(self.fixture), \
         get_fixture_id(self.fixture), \
         get_testcase_name(self.testcase), \
         get_fixture_id(self.fixture), \
         get_testcase_instance_name(self.fixture, self.testcase) \
         )
      output(testcase_def, self.file)

   def generate(self):
      if self.testcase.has_been_generated():
         return

      depends = self.testcase.get_depends()
      if depends:
         TestCaseDefGenerator(self.file, depends, self.fixture).generate()

      self.__generate()
      self.testcase.mark_as_generated()
         
################################################
def get_testcase_array_var(fixture):
   return "g_TESTCASEARRAY_" + get_fixture_id(fixture)

################################################
class TestCaseArrayGenerator:
   #############################################
   def __init__(self, file, testcase, fixture):
      self.testcase = testcase
      self.file = file
      self.fixture = fixture

   #############################################
   def generate(self):
      testcase_in_array = '''&%s,''' % (get_testcase_instance_name(self.fixture, self.testcase))
      output(testcase_in_array, self.file)

################################################
class TestCaseDependsVerifier:
   #############################################
   def __init__(self, testcase):
      self.testcase = testcase

   #############################################
   def generate(self):
      depends = self.testcase.get_depends()
      temp = depends
      while temp != None:
        temp = temp.get_depends()
        if temp == depends:
           self.testcase.report_cyclic_depend_error()


################################################
class TestCaseSeeker:
   #############################################
   def __init__(self, fixture):
      self.fixture = fixture

   #############################################
   def generate(self):
      ScopesGenerator([self.fixture.get_scope()], None) \
         .generate(lambda file, elem: TestCaseDependsVerifier(elem))


testcase_array_template_begin = '''
static TESTNGPP_NS::TestCase* %s[] = {'''

array_template_end = '''0
};


'''

################################################
class FixtureGenerator:
   #############################################
   def __init__(self, file, fixture):
      self.fixture = fixture
      self.file = file

   #############################################
   def generate_testcases(self):
      ScopesGenerator([self.fixture.get_scope()], self.file) \
         .generate(lambda file, elem: TestCaseDefGenerator(file, elem, self.fixture))

   #############################################
   def generate_testcase_array_content(self):
      ScopesGenerator([self.fixture.get_scope()], self.file) \
         .generate(lambda file, elem: TestCaseArrayGenerator(file, elem, self.fixture))

   #############################################
   def generate_testcase_array(self):
      begin = testcase_array_template_begin % (get_testcase_array_var(self.fixture))

      output(begin, self.file)

      self.generate_testcase_array_content()

      output(array_template_end, self.file)

   #############################################
   def generate(self):
      self.generate_testcases()
      self.generate_testcase_array()
      
fixture_desc_template = '''
static TESTNGPP_NS::TestFixtureDesc %s
   ( "%s"
   , "%s"
   , %s
   , (sizeof(%s)/sizeof(%s[0])) - 1
   );

'''
################################################
class FixtureDescGenerator:
   #############################################
   def __init__(self, file, fixture):
      self.fixture = fixture
      self.file = file
   
   #############################################
   def generate(self):
      fixture_desc_def = fixture_desc_template % ( \
          get_fixture_desc_var(self.fixture), \
          self.fixture.get_name(), \
          self.fixture.get_file_name(), \
          get_testcase_array_var(self.fixture), \
          get_testcase_array_var(self.fixture), \
          get_testcase_array_var(self.fixture) )

      output(fixture_desc_def, self.file)

################################################
class FixtureDescArrayGenerator:
   #############################################
   def __init__(self, file, fixture):
      self.fixture = fixture
      self.file = file

   def generate(self):
      output("&" + get_fixture_desc_var(self.fixture) + ",", self.file)

################################################
################################################
class ScopeGenerator:
   #############################################
   def __init__(self, scope, file, generator_getter):
      self.scope = scope
      self.file = file
      self.get_generator = generator_getter

   #############################################
   def generate_begin(self):
      inst = self.scope.get_inst()
      if inst != "::":
         output("#" + inst, self.file)

   #############################################
   def generate_end(self):
      if self.scope.is_root_scope():
         output("#endif // #" + self.scope.get_inst(), self.file)

   #############################################
   def generate_scopes(self, scopes):
      ScopesGenerator(scopes, self.file) \
         .generate(self.get_generator)

   #############################################
   def generate_sub_scopes(self):
      self.generate_scopes(self.scope.get_sub_scopes())

   #############################################
   def generate_siblings(self):
      self.generate_scopes(self.scope.get_siblings())

   #############################################
   def generate_content(self):
      for elem in self.scope.get_elements():
         self.get_generator(self.file, elem).generate()

   #############################################
   def generate(self):
      self.generate_begin()
      self.generate_content()
      self.generate_sub_scopes()
      self.generate_siblings()
      self.generate_end()

################################################
################################################
class ScopesGenerator:
   #############################################
   def __init__(self, scopes, file):
     self.scopes = scopes
     self.file = file

   #############################################
   def generate(self, generator_getter):
      for scope in self.scopes:
         ScopeGenerator(scope, self.file, generator_getter).generate()
     
################################################
def get_suite_desc_name(suite):
   return "test_suite_desc_instance_" + suite

def get_suite_desc_class():
   return "TESTNGPP_NS::TestSuiteDesc"

def get_fixture_array_name(suite):
   return "array_of_fixture_desc_" + suite

def get_suite_getter_name():
   return "___testngpp_test_suite_desc_getter"

##########################################################
dep_headers = [
   "internal/TestCase.h",
   "internal/TestFixtureDesc.h",
   "internal/TestSuiteDesc.h"
]

fixture_array_template_begin = '''
static TESTNGPP_NS::TestFixtureDesc* %s[] = {'''

suite_desc_template = '''
static TESTNGPP_NS::TestSuiteDesc %s
   ( "%s"
   , %s
   , (sizeof(%s)/sizeof(%s[0])) - 1
   );

'''

suite_getter_template = '''
extern "C" TESTNGPP_NS::TestSuiteDesc* %s() {
   return &%s;
}

'''

################################################
class SuiteGenerator:
   #############################################
   def __init__(self, scopes, file, suite, fixture_files):
      self.scopes = scopes
      self.suite = suite
      self.file = file
      self.fixture_files = fixture_files

   #############################################
   def generate_fixtures(self):
      ScopesGenerator(self.scopes, self.file) \
         .generate(lambda file, elem: FixtureGenerator(file, elem) )

   #############################################
   def generate_fixture_descs(self):
      ScopesGenerator(self.scopes, self.file) \
         .generate(lambda file, elem: FixtureDescGenerator(file, elem) )

   #############################################
   def generate_fixture_desc_array(self):
      ScopesGenerator(self.scopes, self.file) \
         .generate(lambda file, elem: FixtureDescArrayGenerator(file, elem) )

   #############################################
   def generate_fixture_array(self):
      fixture_array_def = fixture_array_template_begin % (get_fixture_array_name(self.suite))
      output(fixture_array_def, self.file)
      self.generate_fixture_desc_array()
      output(array_template_end, self.file)

   #############################################
   def generate_suite_desc(self):
      suite_def = suite_desc_template % ( \
         get_suite_desc_name(self.suite), \
         self.suite, \
         get_fixture_array_name(self.suite), \
         get_fixture_array_name(self.suite), \
         get_fixture_array_name(self.suite) )

      output(suite_def, self.file)


   #############################################
   def generate_suite_getter(self):
      suite_getter = suite_getter_template % ( get_suite_getter_name(), get_suite_desc_name(self.suite))
      output(suite_getter, self.file)

   #############################################
   def generate_dep_headers(self):
      for header in dep_headers:
         output("#include <testngpp/" + header + ">", self.file)

   #############################################
   def generate_headers(self):
      self.generate_dep_headers()

      for header in self.fixture_files:
         output("#include \"" + os.path.basename(header) + "\"", self.file)

   #############################################
   def generate(self):
      self.generate_headers()
      self.generate_fixtures()
      self.generate_fixture_descs()
      self.generate_fixture_array()
      self.generate_suite_desc()
      self.generate_suite_getter()

################################################
def verify_testcase_deps(scopes):
   ScopesGenerator(scopes, None) \
         .generate(lambda file, elem: TestCaseSeeker(elem) )

################################################
def phase4(fixture_files, target, scopes):
   verify_testcase_deps(scopes)

   try:
      file = open(target, "w")
   except IOError:
      print >> sys.stderr, "open", target, "failed"
      sys.exit(1)

   SuiteGenerator(scopes, file, get_base_name(target), fixture_files).generate()

   file.close()

################################################
