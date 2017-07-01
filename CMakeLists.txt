######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#Usage of OBJECT keyword in add_library require 2.8.8 (or something like that, need to check)
cmake_minimum_required(VERSION 2.8.8)
project(homelinux)

######################################################
#global setup
enable_testing()

######################################################
#Add internal search path for scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

######################################################
#Delegate some cmake scripts
include(${CMAKE_SOURCE_DIR}/cmake/macros.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/unittest-macros.cmake)

######################################################
#Some global setup
setup_internal_gmock_and_gtest()
if (ENABLE_GCC_COVERAGE)
        homelinux_enable_gcc_coverage()
endif(ENABLE_GCC_COVERAGE)

######################################################
homelinux_enable_cxx_11()

######################################################
add_subdirectory(src)
add_subdirectory(extern-deps)

######################################################
install(FILES bin/hl-py-env.sh DESTINATION bin/)
install(DIRECTORY compilers DESTINATION homelinux)
install(DIRECTORY packages DESTINATION homelinux)
install(DIRECTORY DESTINATION homelinux/install-db)
install(DIRECTORY DESTINATION homelinux/distfiles)
install(DIRECTORY DESTINATION lib)
install(FILES manpages/hl.1.bz2 DESTINATION share/man/man1)
install(FILES manpages/hl-config.5.bz2 DESTINATION share/man/man5)

######################################################
#install config file only if not already exist
install(CODE "
  if (NOT EXISTS \"${CMAKE_INSTALL_PREFIX}/homelinux.json\")
        file(INSTALL \"${CMAKE_SOURCE_DIR}/configs/homelinux.json\" DESTINATION \"${CMAKE_INSTALL_PREFIX}\")
    endif()
 ")

######################################################
#we create a symlink for lib64 to avoid detection issues
install_symlink(lib ${CMAKE_INSTALL_PREFIX}/lib64)
