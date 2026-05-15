/*
 * CppUTest テストランナーのエントリポイント
 * 仮想 Therac-25 Simple (TH25S-CORE) / UTPR-TH25S-001 §7
 */
#include "CppUTest/CommandLineTestRunner.h"

int main(int argc, char **argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
