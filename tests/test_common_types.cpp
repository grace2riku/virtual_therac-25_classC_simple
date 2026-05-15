/*
 * UNIT-001 CommonTypes ユニット試験 (UTPR-TH25S-001 §7.2 UT-001-xx)
 * 仮想 Therac-25 Simple (TH25S-CORE) / IEC 62304 クラス C 学習プロジェクト
 *
 * 検証対象: th25s_validate_energy / th25s_validate_dose / th25s_safe_counter_* /
 *           th25s_error_message
 * 関連要求: SRS-005, SRS-008 / 関連 RCM: RCM-003, RCM-004 / 対応ハザード: HZ-003
 */
#include "CppUTest/TestHarness.h"
#include "th25s_core/common_types.h"
#include <string.h>

/* ===== th25s_validate_energy: モード別エネルギ範囲検証 (SRS-005) ===== */
TEST_GROUP(CommonTypes_ValidateEnergy){};

/* UT-001-01 正常系: 電子モードの代表エネルギ (10.0 MeV) は受理される。 */
TEST(CommonTypes_ValidateEnergy, ElectronNominalEnergyIsAccepted)
{
    LONGS_EQUAL(TH25S_OK, th25s_validate_energy(TH25S_MODE_ELECTRON, 10.0));
}

/* UT-001-02 境界値: 電子モード下限 1.0 MeV は受理される。 */
TEST(CommonTypes_ValidateEnergy, ElectronLowerBoundIsAccepted)
{
    LONGS_EQUAL(TH25S_OK, th25s_validate_energy(TH25S_MODE_ELECTRON,
                                                TH25S_ELECTRON_ENERGY_MIN_MEV));
}

/* UT-001-03 境界値: 電子モード上限 25.0 MeV は受理される。 */
TEST(CommonTypes_ValidateEnergy, ElectronUpperBoundIsAccepted)
{
    LONGS_EQUAL(TH25S_OK, th25s_validate_energy(TH25S_MODE_ELECTRON,
                                                TH25S_ELECTRON_ENERGY_MAX_MEV));
}

/* UT-001-04 異常系: 電子モード下限未満 (0.99 MeV) は範囲外として拒否される。 */
TEST(CommonTypes_ValidateEnergy, ElectronBelowLowerBoundIsRejected)
{
    LONGS_EQUAL(TH25S_ERR_ENERGY_OUT_OF_RANGE,
                th25s_validate_energy(TH25S_MODE_ELECTRON, 0.99));
}

/* UT-001-05 異常系: 電子モード上限超過 (25.01 MeV) は範囲外として拒否される。 */
TEST(CommonTypes_ValidateEnergy, ElectronAboveUpperBoundIsRejected)
{
    LONGS_EQUAL(TH25S_ERR_ENERGY_OUT_OF_RANGE,
                th25s_validate_energy(TH25S_MODE_ELECTRON, 25.01));
}

/* UT-001-06 境界値: X線モード下限 5.0 MV は受理される。 */
TEST(CommonTypes_ValidateEnergy, XrayLowerBoundIsAccepted)
{
    LONGS_EQUAL(TH25S_OK,
                th25s_validate_energy(TH25S_MODE_XRAY, TH25S_XRAY_ENERGY_MIN_MV));
}

/* UT-001-07 異常系: 4.99 は電子モードでは有効値だが X線モードでは範囲外。
 * モードごとに範囲が異なることを検証する。 */
TEST(CommonTypes_ValidateEnergy, XrayBelowLowerBoundIsRejected)
{
    LONGS_EQUAL(TH25S_OK, th25s_validate_energy(TH25S_MODE_ELECTRON, 4.99));
    LONGS_EQUAL(TH25S_ERR_ENERGY_OUT_OF_RANGE,
                th25s_validate_energy(TH25S_MODE_XRAY, 4.99));
}

/* UT-001-08 異常系: モード未選択 (NONE) は TH25S_ERR_MODE_NONE。 */
TEST(CommonTypes_ValidateEnergy, ModeNoneIsRejected)
{
    LONGS_EQUAL(TH25S_ERR_MODE_NONE, th25s_validate_energy(TH25S_MODE_NONE, 10.0));
}

/* ===== th25s_validate_dose: 線量範囲検証 (SRS-008) ===== */
TEST_GROUP(CommonTypes_ValidateDose){};

/* UT-001-09 正常系: 代表線量 (200.0 cGy) は受理される。 */
TEST(CommonTypes_ValidateDose, NominalDoseIsAccepted)
{
    LONGS_EQUAL(TH25S_OK, th25s_validate_dose(200.0));
}

/* UT-001-10 境界値: 下限 0.01 cGy は受理される。 */
TEST(CommonTypes_ValidateDose, LowerBoundIsAccepted)
{
    LONGS_EQUAL(TH25S_OK, th25s_validate_dose(TH25S_DOSE_MIN_CGY));
}

/* UT-001-11 境界値: 上限 10000.0 cGy は受理される。 */
TEST(CommonTypes_ValidateDose, UpperBoundIsAccepted)
{
    LONGS_EQUAL(TH25S_OK, th25s_validate_dose(TH25S_DOSE_MAX_CGY));
}

/* UT-001-12 異常系: 下限未満 (0.009 cGy) は拒否される。 */
TEST(CommonTypes_ValidateDose, BelowLowerBoundIsRejected)
{
    LONGS_EQUAL(TH25S_ERR_DOSE_OUT_OF_RANGE, th25s_validate_dose(0.009));
}

/* UT-001-13 異常系: 上限超過 (10000.01 cGy) は拒否される。Therac-25 の
 * 過剰照射 (処方の 100 倍超) に相当する値域逸脱を検出できることを示す。 */
TEST(CommonTypes_ValidateDose, AboveUpperBoundIsRejected)
{
    LONGS_EQUAL(TH25S_ERR_DOSE_OUT_OF_RANGE, th25s_validate_dose(10000.01));
}

/* ===== th25s_safe_counter_*: 飽和カウンタ (RCM-003 / HZ-003) ===== */
TEST_GROUP(CommonTypes_SafeCounter){};

/* UT-001-14 正常系: 初期化直後は value=0, overflowed=false, is_valid=true。 */
TEST(CommonTypes_SafeCounter, InitYieldsZeroAndValid)
{
    th25s_safe_counter_t c;
    th25s_safe_counter_init(&c, 3U);
    UNSIGNED_LONGS_EQUAL(0U, c.value);
    UNSIGNED_LONGS_EQUAL(3U, c.limit);
    CHECK_FALSE(c.overflowed);
    CHECK_TRUE(th25s_safe_counter_is_valid(&c));
}

/* UT-001-15 正常系: 上限未満のインクリメントは value を +1 し TH25S_OK。 */
TEST(CommonTypes_SafeCounter, IncrementBelowLimitAdvancesValue)
{
    th25s_safe_counter_t c;
    th25s_safe_counter_init(&c, 3U);
    LONGS_EQUAL(TH25S_OK, th25s_safe_counter_increment(&c));
    UNSIGNED_LONGS_EQUAL(1U, c.value);
    LONGS_EQUAL(TH25S_OK, th25s_safe_counter_increment(&c));
    UNSIGNED_LONGS_EQUAL(2U, c.value);
}

/* UT-001-16 境界値・異常系: 上限到達後のインクリメントは TH25S_ERR_COUNTER_OVERFLOW
 * を返し、value は limit を超えない (HZ-003 / RCM-003 の中核)。 */
TEST(CommonTypes_SafeCounter, IncrementAtLimitReportsOverflowAndHoldsValue)
{
    th25s_safe_counter_t c;
    th25s_safe_counter_init(&c, 2U);
    LONGS_EQUAL(TH25S_OK, th25s_safe_counter_increment(&c)); /* value 1 */
    LONGS_EQUAL(TH25S_OK, th25s_safe_counter_increment(&c)); /* value 2 = limit */
    LONGS_EQUAL(TH25S_ERR_COUNTER_OVERFLOW, th25s_safe_counter_increment(&c));
    UNSIGNED_LONGS_EQUAL(2U, c.value); /* limit を超えない */
}

/* UT-001-17 異常系: オーバーフロー後は is_valid が false になる。 */
TEST(CommonTypes_SafeCounter, IsInvalidAfterOverflow)
{
    th25s_safe_counter_t c;
    th25s_safe_counter_init(&c, 1U);
    (void)th25s_safe_counter_increment(&c);
    LONGS_EQUAL(TH25S_ERR_COUNTER_OVERFLOW, th25s_safe_counter_increment(&c));
    CHECK_FALSE(th25s_safe_counter_is_valid(&c));
}

/* UT-001-18 異常系: オーバーフロー後にさらにインクリメントしても 0 へ巻き戻らず、
 * TH25S_ERR_COUNTER_OVERFLOW を返し続ける。Therac-25 の 8bit カウンタが 256 回ごとに
 * 0 へ巻き戻った欠陥との構造的な対比。 */
TEST(CommonTypes_SafeCounter, NeverWrapsAroundAfterOverflow)
{
    th25s_safe_counter_t c;
    th25s_safe_counter_init(&c, 1U);
    (void)th25s_safe_counter_increment(&c);
    for (int i = 0; i < 300; ++i) {
        LONGS_EQUAL(TH25S_ERR_COUNTER_OVERFLOW, th25s_safe_counter_increment(&c));
        UNSIGNED_LONGS_EQUAL(1U, c.value); /* 巻き戻らず limit を保持し続ける */
    }
}

/* UT-001-19 異常系: NULL 引数。increment は TH25S_ERR_NULL_ARG、is_valid は false。 */
TEST(CommonTypes_SafeCounter, NullArgIsHandled)
{
    LONGS_EQUAL(TH25S_ERR_NULL_ARG, th25s_safe_counter_increment(NULL));
    CHECK_FALSE(th25s_safe_counter_is_valid(NULL));
}

/* ===== th25s_error_message: 明確なエラーメッセージ (RCM-004) ===== */
TEST_GROUP(CommonTypes_ErrorMessage){};

/* UT-001-20 正常系: 全エラーコードに非 NULL かつ非空のメッセージが対応する。
 * Therac-25 の "MALFUNCTION 54" 型の暗号的表示の排除を検証する。 */
TEST(CommonTypes_ErrorMessage, EveryCodeHasNonEmptyMessage)
{
    const th25s_error_t codes[] = {
        TH25S_OK, TH25S_ERR_NULL_ARG, TH25S_ERR_MODE_NONE,
        TH25S_ERR_ENERGY_OUT_OF_RANGE, TH25S_ERR_DOSE_OUT_OF_RANGE,
        TH25S_ERR_SEQUENCE_VIOLATION, TH25S_ERR_TURNTABLE_NOT_CONFIRMED,
        TH25S_ERR_MODE_TURNTABLE_MISMATCH, TH25S_ERR_NOT_READY,
        TH25S_ERR_COUNTER_OVERFLOW
    };
    for (size_t i = 0; i < sizeof(codes) / sizeof(codes[0]); ++i) {
        const char *msg = th25s_error_message(codes[i]);
        CHECK(msg != NULL);
        CHECK(strlen(msg) > 0);
    }
}

/* UT-001-21 異常系: 未定義のコード値でも NULL を返さない (防御的実装)。 */
TEST(CommonTypes_ErrorMessage, UndefinedCodeStillReturnsNonNull)
{
    const char *msg = th25s_error_message((th25s_error_t)9999);
    CHECK(msg != NULL);
    CHECK(strlen(msg) > 0);
}
