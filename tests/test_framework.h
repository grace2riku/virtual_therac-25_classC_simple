/*
 * 試験フレームワーク互換マクロ
 * 仮想 Therac-25 Simple (TH25S-CORE) / IEC 62304 クラス C 学習プロジェクト
 *
 * CppUTest と GoogleTest のどちらでも同一のテストソースをコンパイル・実行できる
 * ようにする薄い互換層。CMake オプション TH25S_TEST_FRAMEWORK の設定に応じて
 * TH25S_USE_CPPUTEST または TH25S_USE_GTEST が定義され、本ヘッダが各フレーム
 * ワークの API に展開する (CR-0001 / 2026-05-15)。
 *
 * フィクスチャを伴うグループは TH25S_TEST_FIXTURE_BEGIN(Name) で開き、本体を
 * 通常の C++ 文として記述し、`};` で閉じる。フィクスチャ内では
 * TH25S_FIXTURE_SETUP() / TH25S_FIXTURE_TEARDOWN() でセットアップ・後処理を
 * 定義し、TH25S_TEST_F(Name, Case) でテストを宣言する。
 *
 * フィクスチャを伴わないグループは TH25S_TEST_GROUP(Name); で宣言し、
 * TH25S_TEST(Name, Case) でテストを宣言する。
 */
#ifndef TH25S_TEST_FRAMEWORK_H
#define TH25S_TEST_FRAMEWORK_H

#if defined(TH25S_USE_GTEST) && defined(TH25S_USE_CPPUTEST)
#  error "TH25S_USE_GTEST and TH25S_USE_CPPUTEST are mutually exclusive"
#endif

#if defined(TH25S_USE_GTEST)
#  include <gtest/gtest.h>

   /* フィクスチャ無しグループ: GTest は TEST(group, name) で自動登録するため
    * 宣言は不要。trailing `;` を吸収するためダミー struct を展開する。 */
#  define TH25S_TEST_GROUP(name) struct th25s_gtest_no_fixture_##name {}

#  define TH25S_TEST(group, name) TEST(group, name)

   /* フィクスチャグループ: ::testing::Test 派生クラスを開く。
    * 本体は通常の C++ 文として記述し、利用側で `};` で閉じる。 */
#  define TH25S_TEST_FIXTURE_BEGIN(name) \
       class name : public ::testing::Test {                                  \
       protected:

#  define TH25S_FIXTURE_SETUP() void SetUp() override
#  define TH25S_FIXTURE_TEARDOWN() void TearDown() override

#  define TH25S_TEST_F(fixture, name) TEST_F(fixture, name)

   /* アサーション: 列挙体と整数の比較で型安全性を確保するため明示キャストする。 */
#  define TH25S_LONGS_EQUAL(expected, actual) \
       EXPECT_EQ(static_cast<long>(expected), static_cast<long>(actual))
#  define TH25S_UNSIGNED_LONGS_EQUAL(expected, actual) \
       EXPECT_EQ(static_cast<unsigned long>(expected), static_cast<unsigned long>(actual))
#  define TH25S_CHECK(cond) EXPECT_TRUE(cond)
#  define TH25S_CHECK_TRUE(cond) EXPECT_TRUE(cond)
#  define TH25S_CHECK_FALSE(cond) EXPECT_FALSE(cond)

#elif defined(TH25S_USE_CPPUTEST)
#  include "CppUTest/TestHarness.h"

   /* フィクスチャ無しグループ: 空の TEST_GROUP を宣言する。 */
#  define TH25S_TEST_GROUP(name) TEST_GROUP(name) {}

#  define TH25S_TEST(group, name) TEST(group, name)

   /* フィクスチャグループ: TEST_GROUP は struct (デフォルト public) を開く。
    * 本体は struct メンバとして記述し、利用側で `};` で閉じる。 */
#  define TH25S_TEST_FIXTURE_BEGIN(name) TEST_GROUP(name) {

#  define TH25S_FIXTURE_SETUP() void setup() override
#  define TH25S_FIXTURE_TEARDOWN() void teardown() override

#  define TH25S_TEST_F(fixture, name) TEST(fixture, name)

#  define TH25S_LONGS_EQUAL(expected, actual) LONGS_EQUAL(expected, actual)
#  define TH25S_UNSIGNED_LONGS_EQUAL(expected, actual) UNSIGNED_LONGS_EQUAL(expected, actual)
#  define TH25S_CHECK(cond) CHECK(cond)
#  define TH25S_CHECK_TRUE(cond) CHECK_TRUE(cond)
#  define TH25S_CHECK_FALSE(cond) CHECK_FALSE(cond)

#else
#  error "Define TH25S_USE_GTEST or TH25S_USE_CPPUTEST via CMake (-DTH25S_TEST_FRAMEWORK=cpputest|gtest|both)"
#endif

#endif /* TH25S_TEST_FRAMEWORK_H */
