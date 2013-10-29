project(tests)

add_executable(unit-test-01 unit-test-01.cpp)
target_link_libraries(unit-test-01 ${LINKLIBS})
set_target_properties(unit-test-01 PROPERTIES COMPILE_FLAGS "-std=c++11")

add_executable(unit-test-02 unit-test-02.cpp)
target_link_libraries(unit-test-02 ${LINKLIBS})
set_target_properties(unit-test-02 PROPERTIES COMPILE_FLAGS "-std=c++11")

add_executable(unit-test-03 unit-test-03.cpp)
target_link_libraries(unit-test-03 ${LINKLIBS})
set_target_properties(unit-test-03 PROPERTIES COMPILE_FLAGS "-std=c++11")

add_executable(unit-test-04 unit-test-04.cpp)
target_link_libraries(unit-test-04 ${LINKLIBS})
set_target_properties(unit-test-04 PROPERTIES COMPILE_FLAGS "-std=c++11")

add_executable(unit-test-05 unit-test-05.cpp)
target_link_libraries(unit-test-05 ${LINKLIBS})
set_target_properties(unit-test-05 PROPERTIES COMPILE_FLAGS "-std=c++11")

add_executable(unit-test-06 unit-test-06.cpp)
target_link_libraries(unit-test-06 ${LINKLIBS})
set_target_properties(unit-test-06 PROPERTIES COMPILE_FLAGS "-std=c++11")

add_test(unit-test-01 unit-test-01 "")
add_test(unit-test-02 unit-test-02 "")
add_test(unit-test-03 unit-test-03 "")
add_test(unit-test-04 unit-test-04 "")
add_test(unit-test-05 unit-test-05 "")
add_test(unit-test-06 unit-test-06 "${tests_SOURCE_DIR}")
