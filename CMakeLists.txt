
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(testngpp)

SET(PACKAGE_NAME \"testngpp\")
SET(PACKAGE_STRING \"testngpp\")
SET(PACKAGE_BUGREPORT \"arthur.ii.yuan@gmail.com\")
SET(PACKAGE_URL \"http://code.google.com/p/test-ng-pp\")
SET(PACKAGE \"testngpp\")
SET(VERSION \"0.4\")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(src/listeners)
ADD_SUBDIRECTORY(samples)
