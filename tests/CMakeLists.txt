add_executable(RegistersTest RegistersTest.cpp)
add_executable(UtilsTest UtilsTest.cpp)

target_link_libraries(RegistersTest PRIVATE core GTest::gtest_main)
target_link_libraries(UtilsTest PRIVATE core GTest::gtest_main)

target_include_directories(RegistersTest PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(UtilsTest PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(RegistersTest)
gtest_discover_tests(UtilsTest)