/*
 * UNIT-003 SafetyInterlock ユニット試験 (UTPR-TH25S-001 §7.2 UT-003-xx)
 * 仮想 Therac-25 Simple (TH25S-CORE) / IEC 62304 クラス C 学習プロジェクト
 *
 * 検証対象: th25s_interlock_check_beam_on
 * 関連要求: SRS-005, SRS-008, SRS-RCM-002 / 関連 RCM: RCM-002 / 対応ハザード: HZ-001
 *
 * 試験フレームワーク: CppUTest または GoogleTest (CR-0001、test_framework.h 経由)。
 */
#include "test_framework.h"
#include "th25s_core/safety_interlock.h"

/* 整合した電子モード入力を組み立てるヘルパ。 */
static th25s_interlock_input_t electron_ok(void)
{
    th25s_interlock_input_t in;
    in.mode = TH25S_MODE_ELECTRON;
    in.energy = 10.0;
    in.dose_cgy = 200.0;
    in.turntable = TH25S_TT_ELECTRON_POS;
    return in;
}

/* 整合した X線モード入力を組み立てるヘルパ。 */
static th25s_interlock_input_t xray_ok(void)
{
    th25s_interlock_input_t in;
    in.mode = TH25S_MODE_XRAY;
    in.energy = 18.0;
    in.dose_cgy = 200.0;
    in.turntable = TH25S_TT_XRAY_POS;
    return in;
}

TH25S_TEST_GROUP(SafetyInterlock);

/* UT-003-01 正常系: 電子モード + 電子位置 + 範囲内パラメータ → TH25S_OK。 */
TH25S_TEST(SafetyInterlock, ElectronConsistentInputIsAccepted)
{
    th25s_interlock_input_t in = electron_ok();
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_interlock_check_beam_on(&in));
}

/* UT-003-02 正常系: X線モード + X線位置 + 範囲内パラメータ → TH25S_OK。 */
TH25S_TEST(SafetyInterlock, XrayConsistentInputIsAccepted)
{
    th25s_interlock_input_t in = xray_ok();
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_interlock_check_beam_on(&in));
}

/* UT-003-03 異常系・HZ-001 中核: X線モード (高出力) なのにターンテーブルが
 * 電子位置 → TH25S_ERR_MODE_TURNTABLE_MISMATCH。Therac-25 East Texas / Tyler
 * 事故型 (X線ターゲット非挿入のまま高出力ビーム照射) の発現経路を拒否する。 */
TH25S_TEST(SafetyInterlock, XrayModeWithElectronPositionIsRejected)
{
    th25s_interlock_input_t in = xray_ok();
    in.turntable = TH25S_TT_ELECTRON_POS;
    TH25S_LONGS_EQUAL(TH25S_ERR_MODE_TURNTABLE_MISMATCH,
                      th25s_interlock_check_beam_on(&in));
}

/* UT-003-04 異常系・HZ-001: 電子モードなのにターンテーブルが X線位置 → 拒否。 */
TH25S_TEST(SafetyInterlock, ElectronModeWithXrayPositionIsRejected)
{
    th25s_interlock_input_t in = electron_ok();
    in.turntable = TH25S_TT_XRAY_POS;
    TH25S_LONGS_EQUAL(TH25S_ERR_MODE_TURNTABLE_MISMATCH,
                      th25s_interlock_check_beam_on(&in));
}

/* UT-003-05 異常系: ターンテーブル位置未確定 (UNKNOWN) → 拒否。 */
TH25S_TEST(SafetyInterlock, UnknownTurntableIsRejected)
{
    th25s_interlock_input_t in = electron_ok();
    in.turntable = TH25S_TT_UNKNOWN;
    TH25S_LONGS_EQUAL(TH25S_ERR_MODE_TURNTABLE_MISMATCH,
                      th25s_interlock_check_beam_on(&in));
}

/* UT-003-06 異常系: ターンテーブルがフィールドライト位置 (患者位置決め用、
 * ビーム照射不可) → 拒否。 */
TH25S_TEST(SafetyInterlock, FieldLightPositionIsRejected)
{
    th25s_interlock_input_t in = xray_ok();
    in.turntable = TH25S_TT_FIELD_LIGHT_POS;
    TH25S_LONGS_EQUAL(TH25S_ERR_MODE_TURNTABLE_MISMATCH,
                      th25s_interlock_check_beam_on(&in));
}

/* UT-003-07 異常系: モード未選択 (NONE) → TH25S_ERR_MODE_NONE。 */
TH25S_TEST(SafetyInterlock, ModeNoneIsRejected)
{
    th25s_interlock_input_t in = electron_ok();
    in.mode = TH25S_MODE_NONE;
    TH25S_LONGS_EQUAL(TH25S_ERR_MODE_NONE, th25s_interlock_check_beam_on(&in));
}

/* UT-003-08 異常系: エネルギ範囲外 (電子モード 30.0 MeV) → 範囲外として拒否。 */
TH25S_TEST(SafetyInterlock, EnergyOutOfRangeIsRejected)
{
    th25s_interlock_input_t in = electron_ok();
    in.energy = 30.0;
    TH25S_LONGS_EQUAL(TH25S_ERR_ENERGY_OUT_OF_RANGE,
                      th25s_interlock_check_beam_on(&in));
}

/* UT-003-09 異常系: 線量範囲外 (20000.0 cGy) → 範囲外として拒否。 */
TH25S_TEST(SafetyInterlock, DoseOutOfRangeIsRejected)
{
    th25s_interlock_input_t in = xray_ok();
    in.dose_cgy = 20000.0;
    TH25S_LONGS_EQUAL(TH25S_ERR_DOSE_OUT_OF_RANGE,
                      th25s_interlock_check_beam_on(&in));
}

/* UT-003-10 境界値: 電子モード下限エネルギ + 整合構成 → TH25S_OK。 */
TH25S_TEST(SafetyInterlock, ElectronLowerBoundEnergyIsAccepted)
{
    th25s_interlock_input_t in = electron_ok();
    in.energy = TH25S_ELECTRON_ENERGY_MIN_MEV;
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_interlock_check_beam_on(&in));
}

/* UT-003-11 境界値: X線モード上限エネルギ + 整合構成 → TH25S_OK。 */
TH25S_TEST(SafetyInterlock, XrayUpperBoundEnergyIsAccepted)
{
    th25s_interlock_input_t in = xray_ok();
    in.energy = TH25S_XRAY_ENERGY_MAX_MV;
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_interlock_check_beam_on(&in));
}

/* UT-003-12 境界値: 線量の下限・上限 + 整合構成 → いずれも TH25S_OK。 */
TH25S_TEST(SafetyInterlock, DoseBoundsAreAccepted)
{
    th25s_interlock_input_t in = electron_ok();
    in.dose_cgy = TH25S_DOSE_MIN_CGY;
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_interlock_check_beam_on(&in));
    in.dose_cgy = TH25S_DOSE_MAX_CGY;
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_interlock_check_beam_on(&in));
}

/* UT-003-13 異常系: NULL 引数 → TH25S_ERR_NULL_ARG。 */
TH25S_TEST(SafetyInterlock, NullArgIsRejected)
{
    TH25S_LONGS_EQUAL(TH25S_ERR_NULL_ARG, th25s_interlock_check_beam_on(NULL));
}

/* UT-003-14 純関数性 (SEP-001): 同一入力に対する複数回呼び出しは同一結果を返す。
 * SafetyInterlock が可変状態を持たない独立判定であることを確認する。 */
TH25S_TEST(SafetyInterlock, IsPureFunctionForSameInput)
{
    th25s_interlock_input_t ok = xray_ok();
    th25s_interlock_input_t ng = xray_ok();
    ng.turntable = TH25S_TT_ELECTRON_POS;
    for (int i = 0; i < 5; ++i) {
        TH25S_LONGS_EQUAL(TH25S_OK, th25s_interlock_check_beam_on(&ok));
        TH25S_LONGS_EQUAL(TH25S_ERR_MODE_TURNTABLE_MISMATCH,
                          th25s_interlock_check_beam_on(&ng));
    }
}
