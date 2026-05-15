/*
 * UNIT-002 TreatmentSequencer 実装
 * 仮想 Therac-25 Simple (TH25S-CORE) / IEC 62304 クラス C 学習プロジェクト
 * 関連設計: SDD-TH25S-001 §4.2-4.3 / 関連 RCM: RCM-001, RCM-003 / 対応ハザード: HZ-002, HZ-003
 */
#include "th25s_core/treatment_sequencer.h"
#include "th25s_core/safety_interlock.h"

void th25s_seq_init(th25s_sequencer_t *seq)
{
    if (seq == NULL) {
        return;
    }
    seq->state = TH25S_SEQ_IDLE;
    seq->rx.mode = TH25S_MODE_NONE;
    seq->rx.energy = 0.0;
    seq->rx.dose_cgy = 0.0;
    seq->turntable = TH25S_TT_UNKNOWN;
    th25s_safe_counter_init(&seq->consistency_checks, TH25S_SEQ_MAX_CONSISTENCY_CHECKS);
}

th25s_seq_state_t th25s_seq_state(const th25s_sequencer_t *seq)
{
    if (seq == NULL) {
        return TH25S_SEQ_FAULT;
    }
    return seq->state;
}

th25s_error_t th25s_seq_select_mode(th25s_sequencer_t *seq, th25s_treatment_mode_t mode)
{
    if (seq == NULL) {
        return TH25S_ERR_NULL_ARG;
    }
    if (mode == TH25S_MODE_NONE) {
        return TH25S_ERR_MODE_NONE;
    }
    /* ビーム照射中・FAULT 中はモード変更を受け付けない (abort が必要)。 */
    if (seq->state == TH25S_SEQ_BEAM_ON || seq->state == TH25S_SEQ_FAULT) {
        return TH25S_ERR_SEQUENCE_VIOLATION;
    }
    /*
     * RCM-001 の中核 (HZ-002 への対応):
     * モードを (再) 選択すると、処方とターンテーブル確定をすべて破棄して
     * MODE_SELECTED へ戻す。Therac-25 の「設定完了後に素早くモードを編集する」
     * 操作によって古いターンテーブル状態が引き継がれる経路を、状態機械の構造で
     * 不可能にする。同じモードを選び直した場合も同様に破棄する (保守的設計)。
     */
    seq->rx.mode = mode;
    seq->rx.energy = 0.0;
    seq->rx.dose_cgy = 0.0;
    seq->turntable = TH25S_TT_UNKNOWN;
    seq->state = TH25S_SEQ_MODE_SELECTED;
    return TH25S_OK;
}

th25s_error_t th25s_seq_set_prescription(th25s_sequencer_t *seq, th25s_prescription_t rx)
{
    th25s_error_t err;

    if (seq == NULL) {
        return TH25S_ERR_NULL_ARG;
    }
    if (seq->state != TH25S_SEQ_MODE_SELECTED) {
        return TH25S_ERR_SEQUENCE_VIOLATION;
    }
    /* 処方のモードは選択済みモードと一致しなければならない。 */
    if (rx.mode != seq->rx.mode) {
        return TH25S_ERR_SEQUENCE_VIOLATION;
    }
    err = th25s_validate_energy(rx.mode, rx.energy);
    if (err != TH25S_OK) {
        return err;
    }
    err = th25s_validate_dose(rx.dose_cgy);
    if (err != TH25S_OK) {
        return err;
    }
    seq->rx = rx;
    seq->state = TH25S_SEQ_PRESCRIPTION_SET;
    return TH25S_OK;
}

th25s_error_t th25s_seq_confirm_turntable(th25s_sequencer_t *seq, th25s_turntable_position_t pos)
{
    if (seq == NULL) {
        return TH25S_ERR_NULL_ARG;
    }
    if (seq->state != TH25S_SEQ_PRESCRIPTION_SET) {
        return TH25S_ERR_SEQUENCE_VIOLATION;
    }
    if (pos == TH25S_TT_UNKNOWN) {
        return TH25S_ERR_TURNTABLE_NOT_CONFIRMED;
    }
    /*
     * 操作者が確定した位置をそのまま記録する。モードとの整合性はここで判定せず、
     * ビームオン時に独立した SafetyInterlock が判定する (多重防御)。これにより
     * 「誤った位置を確定してしまった」場合でもビームオン時に拒否される。
     */
    seq->turntable = pos;
    seq->state = TH25S_SEQ_TURNTABLE_CONFIRMED;
    return TH25S_OK;
}

th25s_error_t th25s_seq_request_ready(th25s_sequencer_t *seq)
{
    if (seq == NULL) {
        return TH25S_ERR_NULL_ARG;
    }
    if (seq->state != TH25S_SEQ_TURNTABLE_CONFIRMED) {
        return TH25S_ERR_SEQUENCE_VIOLATION;
    }
    seq->state = TH25S_SEQ_READY;
    return TH25S_OK;
}

th25s_error_t th25s_seq_request_beam_on(th25s_sequencer_t *seq)
{
    th25s_error_t err;
    th25s_interlock_input_t in;

    if (seq == NULL) {
        return TH25S_ERR_NULL_ARG;
    }
    if (seq->state != TH25S_SEQ_READY) {
        return TH25S_ERR_SEQUENCE_VIOLATION;
    }

    /*
     * RCM-003 (HZ-003 への対応): 整合性チェックの実施回数を飽和カウンタで数える。
     * カウンタがオーバーフローした場合、安全チェックを「省略」するのではなく
     * FAULT へ遷移して照射を拒否する。Therac-25 ではカウンタの巻き戻りが
     * チェックのバイパスに直結したが、本実装ではバイパス経路を持たない。
     */
    err = th25s_safe_counter_increment(&seq->consistency_checks);
    if (err != TH25S_OK) {
        seq->state = TH25S_SEQ_FAULT;
        return err;
    }

    /* RCM-002 (HZ-001 への対応): 独立した SafetyInterlock による最終整合性判定。 */
    in.mode = seq->rx.mode;
    in.energy = seq->rx.energy;
    in.dose_cgy = seq->rx.dose_cgy;
    in.turntable = seq->turntable;

    err = th25s_interlock_check_beam_on(&in);
    if (err != TH25S_OK) {
        seq->state = TH25S_SEQ_FAULT;
        return err;
    }

    seq->state = TH25S_SEQ_BEAM_ON;
    return TH25S_OK;
}

void th25s_seq_abort(th25s_sequencer_t *seq)
{
    if (seq == NULL) {
        return;
    }
    /*
     * IDLE へ戻すが、整合性チェックカウンタは機器の生涯にわたる累積値として
     * 保持する (治療ごとにリセットしない)。
     */
    seq->state = TH25S_SEQ_IDLE;
    seq->rx.mode = TH25S_MODE_NONE;
    seq->rx.energy = 0.0;
    seq->rx.dose_cgy = 0.0;
    seq->turntable = TH25S_TT_UNKNOWN;
}
