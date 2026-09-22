add_executable(qgb qgb.cpp)

target_include_directories(qgb PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(qgb PRIVATE core)