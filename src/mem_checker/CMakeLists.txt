
#this file is not used, these files are included in testngpp.lib CMakeLists.txt directily.

SET(SRC_LIST	
	bool_array.cpp       
	mem_pool_base.cpp
	debug_new.cpp        
	static_mem_pool.cpp
	interface_4xunit.cpp
	reporter.cpp)

	

ADD_LIBRARY(mem_checker ${SRC_LIST})
