cmake_minimum_required(VERSION 3.20)

project(th25s_core
    VERSION 1.0.0
    DESCRIPTION "Therac-25 Simple Beam Interlock Software (TH25S-CORE) — IEC 62304 Class C learning project"
    LANGUAGES C CXX
)

# 実装言語は C11 (SDP-TH25S-001 §6.1)。CXX は CppUTest ベースの試験ツリーのみで使用する。
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# clang-tidy / IDE 連携のため compile_commands.json を生成する。
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# プリセット未指定時のデフォルトビルドタイプ。
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Default build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# 厳格な警告 (SDP-TH25S-001 §6.1 / §14 共通ソフトウェア欠陥の識別と回避)。
# th25s_core の各ターゲットに個別付与する (SOUP=CppUTest には付与しない)。
set(TH25S_C_WARNINGS
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion -Wsign-conversion
    -Wcast-align -Wnull-dereference -Wdouble-promotion
)

# 安全コアライブラリ。
add_subdirectory(src/th25s_core)

# 試験ツリー。試験フレームワーク不在の環境では OFF にできる:
#   cmake -B build -DTH25S_BUILD_TESTS=OFF
option(TH25S_BUILD_TESTS "Build the unit test tree" ON)

# 試験フレームワーク選択 (CR-0001 / SOUP-002 GoogleTest 追加で導入)。
# CppUTest と GoogleTest のいずれか、または両方で同一の試験ケースを実行できる。
# 両方選んだ場合、ctest は両フレームワークの試験を続けて実行する。
set(TH25S_TEST_FRAMEWORK "cpputest" CACHE STRING
    "Test framework: cpputest, gtest, or both")
set_property(CACHE TH25S_TEST_FRAMEWORK PROPERTY STRINGS "cpputest" "gtest" "both")

if(TH25S_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "TH25S_BUILD_TESTS=OFF — 試験ツリーをスキップします (CI では ON 必須)。")
endif()
