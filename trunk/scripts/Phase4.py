

from Message import *

from Phase1Result import *

def get_testcase_base_name():
   return "TESTNGPP_NS::TestCase"

def get_fixture_base_name():
   return "TESTNGPP_NS::TestFixture"

def get_fixture_var():
   return "fixture"

def get_fixture_para():
   return "pFixture"

def get_fixture_para_decl():
   return get_fixture_base_name() + "* " + get_fixture_para()

def get_testcase_class_name(testcase):
   return "TESTCASE_" + testcase.get_name()

def get_testcase_instance_name(testcase):
   return "testcase_instance_" + testcase.get_name()

################################################
class TestCaseDefGenerator:
   #############################################
   def __init__(self, file, testcase, fixture):
      self.fixture = fixture
      self.testcase = testcase
      self.file = file

   #############################################
   def generate(self):
      output("static struct " + get_testcase_class_name(self.testcase), self.file)
      output("   : public " + get_testcase_base_name(), self.file)
      output("{", self.file)
      output("   " + get_testcase_class_name(self.testcase) + "()", self.file)
      output("     :" + get_testcase_base_name(), self.file)
      output("        (\"" + self.testcase.get_name() + "\"", self.file)
      output("        , \"" + self.fixture.get_name() + "\"", self.file)
      output("        , \"" + self.testcase.get_file_name() + "\"", self.file)
      output("        , " + str(self.testcase.get_line_number())  + ")", self.file)
      output("   {}", self.file)
      output("   void setUp(" + get_fixture_para_decl() + ")", self.file)
      output("   {", self.file)
      output("      if(" + get_fixture_para() + " == 0)", self.file)
      output("         " + get_fixture_var() + " = new " + self.fixture.get_name() + "();", self.file)
      output("      else", self.file)
      output("         " + get_fixture_var() + " = dynamic_cast<" + self.fixture.get_name() + "*>(" + \
                       get_fixture_para() + ");", self.file)
      output("     " + get_fixture_var() + "->setUp(); ", self.file)
      output("   }", self.file)
      output("   void tearDown()", self.file)
      output("   {", self.file)
      output("     " + get_fixture_var() + "->tearDown(); ", self.file)
      output("   }", self.file)
      output("   void run()", self.file)
      output("   {", self.file)
      output("     " + get_fixture_var() + "->" + self.testcase.get_name() + "();", self.file)
      output("   }", self.file)
      output("   " + get_fixture_base_name() + "* getFixture() const", self.file)
      output("   {", self.file)
      output("      return " + get_fixture_var() + ";", self.file)
      output("   }", self.file)
      output("private:", self.file)
      output("   " + self.fixture.get_name() + "* " + get_fixture_var() + ";", self.file)
      output("} " + get_testcase_instance_name(self.testcase) + ";", self.file)

################################################
def get_testcase_array_var(fixture):
   return "g_TESTCASEARRAY_" + fixture.get_name()

################################################
class TestCaseArrayGenerator:
   #############################################
   def __init__(self, file, testcase, fixture):
      self.testcase = testcase
      self.file = file
      self.fixture = fixture

   #############################################
   def generate(self):
      output("&" + get_testcase_instance_name(self.testcase) + ",", self.file)

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

   def generate_testcase_array_content(self):
      ScopesGenerator([self.fixture.get_scope()], self.file) \
         .generate(lambda file, elem: TestCaseArrayGenerator(file, elem, self.fixture))

   #############################################
   def generate_testcase_array(self):
      output("static " + get_testcase_base_name() + "* " + \
                 get_testcase_array_var(self.fixture) + "[] = {", self.file)

      self.generate_testcase_array_content()

      output("0", self.file)
      output("};", self.file)

   #############################################
   def generate(self):
      self.generate_testcases()
      self.generate_testcase_array()
      
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
         print "#" + inst

   #############################################
   def generate_end(self):
      if self.scope.is_root_scope():
         print "#endif", "//", self.scope.get_inst()

   #############################################
   def generate_scopes(self, scopes):
      ScopesGenerator(scopes, self.file).generate(self.get_generator)

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
class SuiteGenerator:
   #############################################
   def __init__(self, scopes, file, suite):
      self.scopes = scopes
      self.suite = suite
      self.file = file

   #############################################
   def generate_fixtures(self):
      ScopesGenerator(self.scopes, self.file) \
         .generate(lambda file, elem: FixtureGenerator(file, elem) )

   #############################################
   def generate(self):
      self.generate_fixtures()

################################################
################################################
def phase4(target, scopes):
   file = None
   SuiteGenerator(scopes, file, target).generate()

