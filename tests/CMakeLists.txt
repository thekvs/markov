project(tests)

add_executable(unit-test-01 unit-test-01.cpp)
target_link_libraries(unit-test-01 ${LINKLIBS})
set_target_properties(unit-test-01 PROPERTIES COMPILE_FLAGS "-std=c++11")

add_executable(unit-test-02 unit-test-02.cpp)
target_link_libraries(unit-test-02 ${LINKLIBS})
set_target_properties(unit-test-02 PROPERTIES COMPILE_FLAGS "-std=c++11")

add_test(unit-test-01 unit-test-01 "")
add_test(unit-test-02 unit-test-02 "")
