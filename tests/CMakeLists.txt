set(TEST_TARGETS RegistersTest UtilsTest RomsTest TimerTest)

include(GoogleTest)

foreach(test ${TEST_TARGETS})
    add_executable(${test} ${test}.cpp)
    target_link_libraries(${test} PRIVATE core GTest::gtest_main)
    target_include_directories(${test} PUBLIC ${PROJECT_SOURCE_DIR})
    gtest_discover_tests(${test})
endforeach()