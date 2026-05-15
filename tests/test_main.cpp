/*
 * テストランナーのエントリポイント (CppUTest / GoogleTest 共通)
 * 仮想 Therac-25 Simple (TH25S-CORE) / UTPR-TH25S-001 §7
 *
 * TH25S_TEST_FRAMEWORK の選択 (CMake) に応じて CppUTest または GoogleTest の
 * テストランナーを起動する (CR-0001 / 2026-05-15)。
 */
#if defined(TH25S_USE_GTEST)
#  include <gtest/gtest.h>
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#elif defined(TH25S_USE_CPPUTEST)
#  include "CppUTest/CommandLineTestRunner.h"
int main(int argc, char **argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
#else
#  error "Define TH25S_USE_GTEST or TH25S_USE_CPPUTEST via CMake"
#endif
