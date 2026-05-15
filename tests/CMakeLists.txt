# 試験ツリー (UTPR-TH25S-001)。
#
# 試験フレームワークは TH25S_TEST_FRAMEWORK で選択する (CR-0001 / 2026-05-15):
#   cpputest : CppUTest のみ (SOUP-001、デフォルト)
#   gtest    : GoogleTest のみ (SOUP-002)
#   both     : 両方を別実行ファイルとしてビルド・登録(CI で両系統の合格を確認)
#
# CppUTest は pkg-config 経由で検出する:
#   macOS  : brew install cpputest    /  Ubuntu : sudo apt-get install libcpputest-dev
# GoogleTest は CMake config 経由で検出する:
#   macOS  : brew install googletest  /  Ubuntu : sudo apt-get install libgtest-dev

string(TOLOWER "${TH25S_TEST_FRAMEWORK}" _th25s_tf)
if(NOT _th25s_tf MATCHES "^(cpputest|gtest|both)$")
    message(FATAL_ERROR
        "TH25S_TEST_FRAMEWORK は cpputest / gtest / both のいずれかを指定してください "
        "(指定値: ${TH25S_TEST_FRAMEWORK})")
endif()

# 両フレームワークで同一の試験ソースをコンパイルする (tests/test_framework.h が
# CppUTest / GoogleTest の API 差を吸収する)。
set(TH25S_TEST_SOURCES
    test_main.cpp
    test_common_types.cpp
    test_treatment_sequencer.cpp
    test_safety_interlock.cpp
)

# ===== CppUTest 試験 (SOUP-001) =====
if(_th25s_tf STREQUAL "cpputest" OR _th25s_tf STREQUAL "both")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CPPUTEST REQUIRED cpputest)

    add_executable(th25s_tests_cpputest ${TH25S_TEST_SOURCES})
    target_compile_definitions(th25s_tests_cpputest PRIVATE TH25S_USE_CPPUTEST)
    target_include_directories(th25s_tests_cpputest PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CPPUTEST_INCLUDE_DIRS}
    )
    target_link_directories(th25s_tests_cpputest PRIVATE ${CPPUTEST_LIBRARY_DIRS})
    target_link_libraries(th25s_tests_cpputest PRIVATE th25s_core ${CPPUTEST_LIBRARIES})
    add_test(NAME th25s_unit_tests_cpputest COMMAND th25s_tests_cpputest)
endif()

# ===== GoogleTest 試験 (SOUP-002) =====
if(_th25s_tf STREQUAL "gtest" OR _th25s_tf STREQUAL "both")
    find_package(GTest REQUIRED)

    add_executable(th25s_tests_gtest ${TH25S_TEST_SOURCES})
    target_compile_definitions(th25s_tests_gtest PRIVATE TH25S_USE_GTEST)
    target_include_directories(th25s_tests_gtest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(th25s_tests_gtest PRIVATE th25s_core GTest::gtest)
    add_test(NAME th25s_unit_tests_gtest COMMAND th25s_tests_gtest)
endif()
