

from Message import *

from Phase1Result import *

def get_testcase_base_name():
   return "TESTNGPP_NS::TestCase"

def get_fixture_base_name():
   return "TESTNGPP_NS::TestFixture"

def get_fixture_var():
   return "fixture"

def get_fixture_var_decl():
   return get_fixture_base_name() + "* " + get_fixture_var()

def get_testcase_class_name(testcase):
   return "TESTCASE_" + testcase.get_name()

def get_testcase_instance_name(testcase):
   return "testcase_instance_" + testcase.get_name()

################################################
class TestCaseGenerator:
   #############################################
   def __init__(self, testcase, fixture, file):
      self.fixture = fixture
      self.testcase = testcase
      self.file = file

   #############################################
   def get_content(self):
      return "static struct " + get_testcase_class_name(self.testcase) + "\n" + \
             "   : public " + get_testcase_base_name() + "\n" + \
             "{" + "\n" + \
             "   " + get_testcase_class_name(self.testcase) + "(" + get_fixture_var_decl() + ")" + "\n" + \
             "     :" + get_testcase_base_name() +                   "\n" + \
             "        (\"" + self.testcase.get_name() + "\"" +       "\n" + \
             "        , \"" + self.fixture.get_name() + "\"" +       "\n" + \
             "        , " + get_fixture_var()  +                     "\n" + \
             "        , \"" + self.testcase.get_file_name() + "\"" + "\n" + \
             "        , " + str(self.testcase.get_line_number())  + ")" +   "\n" + \
             "   {" +                                                "\n" + \
             "      if(" + get_fixture_var() + " == 0)" +            "\n" + \
             "         " + get_fixture_var() + " = new " + self.fixture.get_name() + "();\n" + \
             "   }\n" +                                              "\n" + \
             "} " + get_testcase_instance_name(self.testcase) + ";"+ "\n" + \
             ""

   def generate(self):
      print self.get_content().encode('utf-8')


################################################
def get_testcase_array_var(fixture):
   return "g_TESTCASEARRAY_" + fixture.get_name()

################################################
class FixtureGenerator:
   #############################################
   def __init__(self, fixture, file):
      self.fixture = fixture
      self.file = file

   #############################################
   def generate_testcases(self):
      for testcase in self.fixture.get_scope().get_elements():
         TestCaseGenerator(testcase, self.fixture, self.file).generate()

   def get_testcase_array(self):
      return "static " + get_testcase_base_name() + "* " + \
                 get_testcase_array_var(self.fixture) + "[] = {" + "\n" + \
             "};" + "\n" 

   #############################################
   def generate_testcase_array(self):
      print self.get_testcase_array().encode('utf-8')

   #############################################
   def generate(self):
      self.generate_testcases()
      self.generate_testcase_array()
      
################################################
class ScopeGenerator:
   #############################################
   def __init__(self, scope, file):
      self.scope = scope
      self.file = file

   #############################################
   def generate_begin(self):
      inst = self.scope.get_inst()
      if inst != "::":
         print "#" + inst

   #############################################
   def generate_end(self):
      if self.scope.is_root_scope():
         print "#endif", "//", self.scope.get_inst()

   #############################################
   def generate_scopes(self, scopes):
      ScopesGenerator(scopes, self.file).generate(self.__class__)

   #############################################
   def generate_sub_scopes(self):
      self.generate_scopes(self.scope.get_sub_scopes())

   #############################################
   def generate_siblings(self):
      self.generate_scopes(self.scope.get_siblings())

   #############################################
   def generate_content(self):
      pass

   #############################################
   def generate(self):
      self.generate_begin()
      self.generate_content()
      self.generate_sub_scopes()
      self.generate_siblings()
      self.generate_end()

################################################
class FixtureScopeGenerator(ScopeGenerator):
   #############################################
   def __init__(self, scope, file):
      ScopeGenerator.__init__(self, scope, file)

   #############################################
   def generate_content(self):
      for fixture in self.scope.get_elements():
         FixtureGenerator(fixture, self.file).generate()

################################################
class TestCaseScopeGenerator(ScopeGenerator):
   #############################################
   def __init__(self, scope, file):
      ScopeGenerator.__init__(self, scope.get_scope(), file)

   #############################################
   def generate_content(self):
      for testcase in self.scope.get_elements():
         TestCaseGenerator(testcase, self.file).generate()

################################################
class ScopesGenerator:
   #############################################
   def __init__(self, scopes, file):
     self.scopes = scopes
     self.file = file

   #############################################
   def generate(self, cls):
      for scope in self.scopes:
         cls(scope, self.file).generate()
     
################################################
def phase4(target, scopes):
   file = None
   ScopesGenerator(scopes, file).generate(FixtureScopeGenerator)

