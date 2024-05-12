######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0                        #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

#part of this file is imported from project MALT developped by Sebastien Valat.
#at exascale lab / university of versailles into the MAQAO

######################################################
#Setup paths to gtest/gmock headers and library
MACRO(setup_internal_gmock_and_gtest)
	set(GMOCK_SOURCE_DIR ${CMAKE_SOURCE_DIR}/extern-deps/googletest-1.14.0)
	set(GMOCK_INCLUDE_DIR ${GMOCK_SOURCE_DIR}/include)
	set(GMOCK_INCLUDE_DIRS ${GMOCK_SOURCE_DIR}/include)
	set(GMOCK_BOTH_LIBRARIES gmock gmock_main)
	set(GTEST_BOTH_LIBRARIES gtest)
	set(GTEST_INCLUDE_DIR ${GMOCK_SOURCE_DIR}/gtest/include/)
	set(GTEST_INCLUDE_DIRS ${GMOCK_SOURCE_DIR}/gtest/include/)
ENDMACRO(setup_internal_gmock_and_gtest)

######################################################
MACRO(homelinux_enable_gcc_coverage)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fprofile-arcs -ftest-coverage")
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O0 -fprofile-arcs -ftest-coverage")
	set(CMAKE_EXE_LINKER_FLAGS_FLAGS "${CMAKE_C_FLAGS} -O0 -fprofile-arcs -ftest-coverage")
ENDMACRO(homelinux_enable_gcc_coverage)

######################################################
MACRO(homelinux_enable_cxx_11)
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	if(COMPILER_SUPPORTS_CXX11)
		message(STATUS "Using -std=c++11")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	else()
		CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
		if(COMPILER_SUPPORTS_CXX11)
			message(STATUS "Using -std=c++0x")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
		else()
			message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 or C++0x support. Please use a different C++ compiler.")
		endif()
	endif()
ENDMACRO(homelinux_enable_cxx_11)

######################################################
#form https://stackoverflow.com/questions/35765106/symbolic-links-cmake
#thanks to Rian Quinn
macro(install_symlink filepath sympath)
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filepath} ${sympath})")
    install(CODE "message(\"-- Created symlink: ${sympath} -> ${filepath}\")")
endmacro(install_symlink)
