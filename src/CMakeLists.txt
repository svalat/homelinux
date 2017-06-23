######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
if (TRANSPORT STREQUAL "MPI")
	set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
endif (TRANSPORT STREQUAL "MPI")

######################################################
set(ENABLE_TESTS         "yes"      CACHE BOOL   "Enable build of internal tests.")
set(ENABLE_COLORS        "yes"      CACHE BOOL   "Enable color in terminal output.")

######################################################
#check for openmp for some parallel tests
#find_package(OpenMP QUIET)
#if (OPENMP_FOUND)
#	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#endif()

######################################################
#find_package(JsonCpp QUIET)
set(JSONCPP_FOUND "yes")
#set(HAVE_JSON_CPP "no")
set(JSON_CPP_LIBRARY libjsoncpp)
set(JSON_CPP_INCLUDE_DIR ../extern-deps/jsoncpp/dist)

######################################################
if (JSONCPP_FOUND)
	include_directories(${JSON_CPP_INCLUDE_DIR})
	set(HAVE_JSON_CPP yes)
endif (JSONCPP_FOUND)

######################################################
include_directories(./)
include_directories(../extern-deps/re2-2017-06-01)

######################################################
set(HOMELINUX_SUB_LIBS $<TARGET_OBJECTS:homelinux-base>
                       $<TARGET_OBJECTS:homelinux-core>
                       $<TARGET_OBJECTS:homelinux-portability>
                       $<TARGET_OBJECTS:homelinux-providers>
					   $<TARGET_OBJECTS:homelinux-crawlers>)

######################################################
if (JSONCPP_FOUND)
	list(APPEND HOMELINUX_EXTERN_LIBS ${JSON_CPP_LIBRARY} re2 -lpthread)
endif (JSONCPP_FOUND)

######################################################
add_library(homelinux-lib STATIC ${HOMELINUX_SUB_LIBS})

######################################################
#setup config file
configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

######################################################
add_executable(homelinux ${HOMELINUX_SUB_LIBS} HomeLinux.cpp main.cpp)
target_link_libraries(homelinux ${HOMELINUX_EXTERN_LIBS})

######################################################
add_subdirectory(base)
add_subdirectory(core)
add_subdirectory(portability)
add_subdirectory(providers)
add_subdirectory(crawlers)

######################################################
#install
INSTALL(TARGETS homelinux RUNTIME DESTINATION bin/)
