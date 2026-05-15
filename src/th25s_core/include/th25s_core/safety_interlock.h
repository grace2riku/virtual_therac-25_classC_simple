/*
 * UNIT-003 SafetyInterlock — ビーム照射可否の最終整合性判定
 * 仮想 Therac-25 Simple (TH25S-CORE) / IEC 62304 クラス C 学習プロジェクト
 *
 * 関連設計: SDD-TH25S-001 §4.4 / 関連要求: SRS-TH25S-001 §5
 * 関連 RCM: RCM-002 (独立した最終整合性判定) / 対応ハザード: HZ-001
 *
 * 分離 (SEP-001): 本ユニットは UNIT-002 TreatmentSequencer から論理的に分離された
 * 防御層である。可変状態を一切持たず、TreatmentSequencer の内部構造体にも依存せず、
 * 判定に必要な全情報を引数 (th25s_interlock_input_t) で受け取る純関数として実装する。
 * これにより Sequencer 側の状態管理に欠陥があっても、不整合なビーム照射を
 * 独立に拒否できる (多重防御)。
 */
#ifndef TH25S_CORE_SAFETY_INTERLOCK_H
#define TH25S_CORE_SAFETY_INTERLOCK_H

#include "th25s_core/common_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * インターロック判定の入力。ビーム照射可否判定に必要な全情報を保持する。
 * TreatmentSequencer の内部表現には依存しない (SEP-001)。
 */
typedef struct {
    th25s_treatment_mode_t mode;
    double energy;
    double dose_cgy;
    th25s_turntable_position_t turntable;
} th25s_interlock_input_t;

/*
 * ビーム照射可否の最終整合性判定 (RCM-002, HZ-001)。
 *
 * 以下をすべて満たすときのみ TH25S_OK を返す。不整合が複数ある場合は
 * 最初に検出した不整合に対応するエラーコードを返す。
 *   1. in が非 NULL                      → TH25S_ERR_NULL_ARG
 *   2. mode が NONE でない                → TH25S_ERR_MODE_NONE
 *   3. energy がモード別許容範囲内         → TH25S_ERR_ENERGY_OUT_OF_RANGE
 *   4. dose_cgy が許容範囲内               → TH25S_ERR_DOSE_OUT_OF_RANGE
 *   5. mode とターンテーブル位置が整合      → TH25S_ERR_MODE_TURNTABLE_MISMATCH
 *        (ELECTRON ⇔ TT_ELECTRON_POS / XRAY ⇔ TT_XRAY_POS)
 *
 * 可変状態を持たないため、同一入力に対し常に同一の結果を返す。
 */
th25s_error_t th25s_interlock_check_beam_on(const th25s_interlock_input_t *in);

#ifdef __cplusplus
}
#endif

#endif /* TH25S_CORE_SAFETY_INTERLOCK_H */
