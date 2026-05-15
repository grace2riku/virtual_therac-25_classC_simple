# CppUTest によるユニット試験ツリー (UTPR-TH25S-001)。
#
# CppUTest (SOUP-001) は pkg-config 経由で検出する。
#   macOS  : brew install cpputest
#   Ubuntu : sudo apt-get install libcpputest-dev
# いずれも cpputest.pc を提供する。

find_package(PkgConfig REQUIRED)
pkg_check_modules(CPPUTEST REQUIRED cpputest)

add_executable(th25s_tests
    test_main.cpp
    test_common_types.cpp
    test_treatment_sequencer.cpp
    test_safety_interlock.cpp
)

target_include_directories(th25s_tests PRIVATE ${CPPUTEST_INCLUDE_DIRS})
target_link_directories(th25s_tests PRIVATE ${CPPUTEST_LIBRARY_DIRS})
target_link_libraries(th25s_tests PRIVATE th25s_core ${CPPUTEST_LIBRARIES})

add_test(NAME th25s_unit_tests COMMAND th25s_tests)
