/*
 * UNIT-003 SafetyInterlock 実装
 * 仮想 Therac-25 Simple (TH25S-CORE) / IEC 62304 クラス C 学習プロジェクト
 * 関連設計: SDD-TH25S-001 §4.4 / 関連 RCM: RCM-002 / 対応ハザード: HZ-001
 *
 * 本ファイルは可変なグローバル状態・静的状態を一切持たない (SEP-001 論理分離)。
 */
#include "th25s_core/safety_interlock.h"

/* モードに対して整合するターンテーブル位置を返す。 */
static th25s_turntable_position_t expected_turntable_for(th25s_treatment_mode_t mode)
{
    switch (mode) {
    case TH25S_MODE_ELECTRON:
        return TH25S_TT_ELECTRON_POS;
    case TH25S_MODE_XRAY:
        return TH25S_TT_XRAY_POS;
    case TH25S_MODE_NONE:
    default:
        return TH25S_TT_UNKNOWN;
    }
}

th25s_error_t th25s_interlock_check_beam_on(const th25s_interlock_input_t *in)
{
    th25s_error_t err;

    if (in == NULL) {
        return TH25S_ERR_NULL_ARG;
    }

    /* 1. モード未選択の拒否。 */
    if (in->mode == TH25S_MODE_NONE) {
        return TH25S_ERR_MODE_NONE;
    }

    /* 2. エネルギ範囲の検証 (SRS-005)。 */
    err = th25s_validate_energy(in->mode, in->energy);
    if (err != TH25S_OK) {
        return err;
    }

    /* 3. 線量範囲の検証 (SRS-008)。 */
    err = th25s_validate_dose(in->dose_cgy);
    if (err != TH25S_OK) {
        return err;
    }

    /*
     * 4. モードとターンテーブル位置の整合判定 (HZ-001 の中核防御)。
     * Therac-25 の East Texas / Tyler 事故は、X線モード (高出力) でありながら
     * ターンテーブルが X線ターゲット位置に無い状態でビームが照射されたことによる。
     * ここで両者の一致を独立に確認することで、Sequencer 側の状態管理に欠陥が
     * あっても不整合照射を拒否する。
     */
    if (in->turntable != expected_turntable_for(in->mode)) {
        return TH25S_ERR_MODE_TURNTABLE_MISMATCH;
    }

    return TH25S_OK;
}
