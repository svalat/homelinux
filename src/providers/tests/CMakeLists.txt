######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#Use gtest
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})

######################################################
add_definitions(-DTEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data")

######################################################
set(TEST_NAMES TestProviderModels TestProviderHomelinux TestProviderUrls)

######################################################
FOREACH(test_name ${TEST_NAMES})
	add_executable(${test_name} ${test_name}.cpp)
	target_link_libraries(${test_name} ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} homelinux-lib ${HOMELINUX_EXTERN_LIBS})
	homelinux_add_test(${test_name} ${test_name})
ENDFOREACH(test_name)
