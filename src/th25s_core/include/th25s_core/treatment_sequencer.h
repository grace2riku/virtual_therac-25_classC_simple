/*
 * UNIT-002 TreatmentSequencer — 治療シーケンス状態機械
 * 仮想 Therac-25 Simple (TH25S-CORE) / IEC 62304 クラス C 学習プロジェクト
 *
 * 関連設計: SDD-TH25S-001 §4.2-4.3 / 関連要求: SRS-TH25S-001 §4
 * 関連 RCM: RCM-001 (状態機械による操作順序の構造的強制) / 対応ハザード: HZ-002, HZ-003
 *
 * Therac-25 では操作者入力処理とビーム設定の間にタイミング競合 (race condition) があり、
 * 「高出力設定の直後に素早くモードを編集する」と古いターンテーブル状態のままビームが
 * オンになった。本ユニットはこの欠陥を「単一スレッドの操作順序依存バグ」として教材化し、
 * 明示的状態機械によって構造的に排除する (RCM-001)。
 */
#ifndef TH25S_CORE_TREATMENT_SEQUENCER_H
#define TH25S_CORE_TREATMENT_SEQUENCER_H

#include "th25s_core/common_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 治療シーケンス状態 (SDD-TH25S-001 §4.2 状態遷移図)。 */
typedef enum {
    TH25S_SEQ_IDLE = 0,            /* 初期状態 */
    TH25S_SEQ_MODE_SELECTED,       /* 治療モード選択済み */
    TH25S_SEQ_PRESCRIPTION_SET,    /* 処方 (エネルギ・線量) 設定済み */
    TH25S_SEQ_TURNTABLE_CONFIRMED, /* ターンテーブル位置確定済み */
    TH25S_SEQ_READY,               /* ビームオン要求受付可能 */
    TH25S_SEQ_BEAM_ON,             /* ビーム照射中 */
    TH25S_SEQ_FAULT                /* 安全違反検出。abort 以外を受け付けない */
} th25s_seq_state_t;

/* 整合性チェックカウンタの上限 (RCM-003)。実運用を想定した十分大きな値。 */
#define TH25S_SEQ_MAX_CONSISTENCY_CHECKS 1000000U

/* 治療シーケンサの状態。フィールドは試験での観測・準備のため公開する。 */
typedef struct {
    th25s_seq_state_t state;
    th25s_prescription_t rx;
    th25s_turntable_position_t turntable;
    th25s_safe_counter_t consistency_checks; /* ビームオン整合性チェックの実施回数 (RCM-003) */
} th25s_sequencer_t;

/* シーケンサを初期化する (state=IDLE)。 */
void th25s_seq_init(th25s_sequencer_t *seq);

/* 現在の状態を返す。seq が NULL の場合は TH25S_SEQ_FAULT。 */
th25s_seq_state_t th25s_seq_state(const th25s_sequencer_t *seq);

/*
 * 治療モードを選択する。
 * RCM-001 の中核: IDLE〜READY のいずれの状態からも呼び出せるが、呼び出すと
 * 処方・ターンテーブル確定をすべて破棄して MODE_SELECTED へ戻す。これにより
 * 「モード編集後に古いターンテーブル状態のままビームオン」を構造的に不可能にする。
 * BEAM_ON / FAULT 状態では TH25S_ERR_SEQUENCE_VIOLATION を返す。
 * mode が TH25S_MODE_NONE の場合は TH25S_ERR_MODE_NONE。
 */
th25s_error_t th25s_seq_select_mode(th25s_sequencer_t *seq, th25s_treatment_mode_t mode);

/*
 * 処方を設定する。MODE_SELECTED 状態でのみ許可。
 * rx.mode は選択済みモードと一致しなければならない (不一致は SEQUENCE_VIOLATION)。
 * エネルギ・線量は範囲検証され、範囲外は対応するエラーコードを返す。
 */
th25s_error_t th25s_seq_set_prescription(th25s_sequencer_t *seq, th25s_prescription_t rx);

/*
 * ターンテーブル位置を確定する。PRESCRIPTION_SET 状態でのみ許可。
 * pos が TH25S_TT_UNKNOWN の場合は TH25S_ERR_TURNTABLE_NOT_CONFIRMED。
 * モードとの整合性はここでは判定せず、ビームオン時に SafetyInterlock が判定する
 * (多重防御。確定値そのものは操作者の入力をそのまま記録する)。
 */
th25s_error_t th25s_seq_confirm_turntable(th25s_sequencer_t *seq, th25s_turntable_position_t pos);

/* READY へ遷移する。TURNTABLE_CONFIRMED 状態でのみ許可。 */
th25s_error_t th25s_seq_request_ready(th25s_sequencer_t *seq);

/*
 * ビームオンを要求する。READY 状態でのみ許可。
 * 整合性チェックカウンタをインクリメントし (オーバーフロー時は FAULT へ遷移し
 * TH25S_ERR_COUNTER_OVERFLOW を返す = RCM-003)、SafetyInterlock による最終
 * 整合性判定を行う。判定が不合格なら FAULT へ遷移し対応エラーを返す。
 * 合格時のみ BEAM_ON へ遷移して TH25S_OK を返す。
 */
th25s_error_t th25s_seq_request_beam_on(th25s_sequencer_t *seq);

/* どの状態からも IDLE へ戻す。整合性チェックカウンタは機器の生涯値として保持する。 */
void th25s_seq_abort(th25s_sequencer_t *seq);

#ifdef __cplusplus
}
#endif

#endif /* TH25S_CORE_TREATMENT_SEQUENCER_H */
