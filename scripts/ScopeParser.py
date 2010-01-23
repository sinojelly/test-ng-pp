#!/usr/bin/python

##########################################################
class ScopeParser:
   #######################################################
   def __init__(self, scope):
      self.scope = scope
      self.scopes = []

      self.fixtures = FixturesInOneFile(scope.get_name(), scope.get_expr())
      self.parser = None

   #######################################################
   def show(self):
      self.fixtures.show()

   #######################################################
   def handle_tag(self, tag):
      pass

   #######################################################
   def look_for_fixture(self, line):
      if isinstance(line, Tag):
         self.handle_tag(line)
         return

      if isinstance(line, Scope):
         self.scopes.append(line)
         return

      fixture_name = is_fixture_def(line)
      if fixture_name:
         self.parser = FixtureParser(fixture_name)

   #######################################################
   def parse_line(self, line):
      if self.parser:
         fixture = self.parser.parse(line)
         if fixture:
            self.fixtures.add_fixture(fixture)
            self.parser = None
         return

      self.look_for_fixture(line)      
      
   #######################################################
   def parse(self):
      for line in self.scope.get_lines():
         self.parse_line(line)

      if self.parser:
         fatal(self.scope.get_lines()[-1], "incompleted fixture definition")

      # sub scope
      for scope in self.scopes:
         self.fixtures.add_scope(Phase3Parser(scope).parse())

      for scope in self.scope.get_elses():
         self.fixtures.add_sibling(Phase3Parser(scope).parse())
         
      return self.fixtures

##########################################################
def phase3(globalScope):
   ''' return: Fixtures'''
   return Phase3Parser(globalScope).parse()

