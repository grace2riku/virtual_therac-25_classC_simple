/*
 * UNIT-002 TreatmentSequencer ユニット試験 (UTPR-TH25S-001 §7.2 UT-002-xx)
 * 仮想 Therac-25 Simple (TH25S-CORE) / IEC 62304 クラス C 学習プロジェクト
 *
 * 検証対象: th25s_seq_*
 * 関連要求: SRS-002, SRS-003, SRS-006 / 関連 RCM: RCM-001, RCM-003
 * 対応ハザード: HZ-002 (操作順序依存), HZ-001 (モード不整合), HZ-003 (カウンタ)
 *
 * 試験フレームワーク: CppUTest または GoogleTest (CR-0001、test_framework.h 経由)。
 */
#include "test_framework.h"
#include "th25s_core/treatment_sequencer.h"

/* 整合した電子モード処方を組み立てるヘルパ。 */
static th25s_prescription_t electron_rx(void)
{
    th25s_prescription_t rx;
    rx.mode = TH25S_MODE_ELECTRON;
    rx.energy = 10.0;
    rx.dose_cgy = 200.0;
    return rx;
}

/* 整合した X線モード処方を組み立てるヘルパ。 */
static th25s_prescription_t xray_rx(void)
{
    th25s_prescription_t rx;
    rx.mode = TH25S_MODE_XRAY;
    rx.energy = 18.0;
    rx.dose_cgy = 200.0;
    return rx;
}

TH25S_TEST_FIXTURE_BEGIN(TreatmentSequencer)
    th25s_sequencer_t seq;

    TH25S_FIXTURE_SETUP()
    {
        th25s_seq_init(&seq);
    }
};

/* UT-002-01 正常系: 初期化直後は IDLE 状態。 */
TH25S_TEST_F(TreatmentSequencer, InitYieldsIdle)
{
    TH25S_LONGS_EQUAL(TH25S_SEQ_IDLE, th25s_seq_state(&seq));
}

/* UT-002-02 正常系: 正しい順序の操作でビームオンまで到達できる。 */
TH25S_TEST_F(TreatmentSequencer, FullValidFlowReachesBeamOn)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, electron_rx()));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_confirm_turntable(&seq, TH25S_TT_ELECTRON_POS));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_ready(&seq));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_beam_on(&seq));
    TH25S_LONGS_EQUAL(TH25S_SEQ_BEAM_ON, th25s_seq_state(&seq));
}

/* UT-002-03 異常系・HZ-002: IDLE 状態で処方設定を試みると順序違反で拒否される。 */
TH25S_TEST_F(TreatmentSequencer, SetPrescriptionFromIdleIsSequenceViolation)
{
    TH25S_LONGS_EQUAL(TH25S_ERR_SEQUENCE_VIOLATION,
                      th25s_seq_set_prescription(&seq, electron_rx()));
    TH25S_LONGS_EQUAL(TH25S_SEQ_IDLE, th25s_seq_state(&seq));
}

/* UT-002-04 異常系・HZ-002: IDLE 状態でいきなりビームオン要求 → 順序違反。 */
TH25S_TEST_F(TreatmentSequencer, BeamOnFromIdleIsSequenceViolation)
{
    TH25S_LONGS_EQUAL(TH25S_ERR_SEQUENCE_VIOLATION, th25s_seq_request_beam_on(&seq));
}

/* UT-002-05 異常系・HZ-002: 処方未設定でターンテーブル確定を試みる → 順序違反。 */
TH25S_TEST_F(TreatmentSequencer, ConfirmTurntableBeforePrescriptionIsSequenceViolation)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_ERR_SEQUENCE_VIOLATION,
                      th25s_seq_confirm_turntable(&seq, TH25S_TT_ELECTRON_POS));
}

/* UT-002-06 異常系・HZ-002: ターンテーブル未確定でビームオン要求 → 順序違反。 */
TH25S_TEST_F(TreatmentSequencer, BeamOnBeforeTurntableConfirmIsSequenceViolation)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, electron_rx()));
    TH25S_LONGS_EQUAL(TH25S_ERR_SEQUENCE_VIOLATION, th25s_seq_request_beam_on(&seq));
}

/*
 * UT-002-07 RCM-001 中核 (HZ-002): READY 到達後にモードを再選択すると、
 * 処方とターンテーブル確定が破棄され MODE_SELECTED へ戻る。Therac-25 の
 * 「設定完了後に素早くモードを編集する」操作で古い状態が引き継がれる経路を、
 * 状態機械が構造的に断ち切ることを検証する。
 */
TH25S_TEST_F(TreatmentSequencer, ReselectingModeFromReadyResetsToModeSelected)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_XRAY));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, xray_rx()));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_confirm_turntable(&seq, TH25S_TT_XRAY_POS));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_ready(&seq));
    TH25S_LONGS_EQUAL(TH25S_SEQ_READY, th25s_seq_state(&seq));

    /* ここでモードを編集する。 */
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_SEQ_MODE_SELECTED, th25s_seq_state(&seq));
    /* ターンテーブル確定が破棄されている。 */
    TH25S_LONGS_EQUAL(TH25S_TT_UNKNOWN, seq.turntable);
}

/*
 * UT-002-08 RCM-001 中核 (HZ-002) の続き: モード再選択でリセットされた後は、
 * ターンテーブルを再確定しない限り READY へ進めない。古いターンテーブル状態の
 * ままビームオンに至る経路が存在しないことを検証する。
 */
TH25S_TEST_F(TreatmentSequencer, AfterModeReselectionMustReconfirmTurntable)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_XRAY));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, xray_rx()));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_confirm_turntable(&seq, TH25S_TT_XRAY_POS));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_ready(&seq));

    /* モード編集 → MODE_SELECTED へリセット。 */
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));

    /* 再確定をスキップして READY を要求しても順序違反で拒否される。 */
    TH25S_LONGS_EQUAL(TH25S_ERR_SEQUENCE_VIOLATION, th25s_seq_request_ready(&seq));
}

/* UT-002-09 異常系: 処方のモードが選択済みモードと不一致 → 順序違反で拒否。 */
TH25S_TEST_F(TreatmentSequencer, PrescriptionModeMismatchIsSequenceViolation)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    /* 選択は電子モードだが X線モードの処方を渡す。 */
    TH25S_LONGS_EQUAL(TH25S_ERR_SEQUENCE_VIOLATION,
                      th25s_seq_set_prescription(&seq, xray_rx()));
}

/* UT-002-10 異常系: 範囲外エネルギの処方 → TH25S_ERR_ENERGY_OUT_OF_RANGE。 */
TH25S_TEST_F(TreatmentSequencer, PrescriptionWithEnergyOutOfRangeIsRejected)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    th25s_prescription_t rx = electron_rx();
    rx.energy = 100.0; /* 電子モード上限 25.0 MeV を超過 */
    TH25S_LONGS_EQUAL(TH25S_ERR_ENERGY_OUT_OF_RANGE,
                      th25s_seq_set_prescription(&seq, rx));
}

/* UT-002-11 異常系: 範囲外線量の処方 → TH25S_ERR_DOSE_OUT_OF_RANGE。 */
TH25S_TEST_F(TreatmentSequencer, PrescriptionWithDoseOutOfRangeIsRejected)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    th25s_prescription_t rx = electron_rx();
    rx.dose_cgy = 50000.0; /* 上限 10000.0 cGy を超過 */
    TH25S_LONGS_EQUAL(TH25S_ERR_DOSE_OUT_OF_RANGE,
                      th25s_seq_set_prescription(&seq, rx));
}

/* UT-002-12 異常系: ターンテーブル確定に UNKNOWN を渡す → 確定不可。 */
TH25S_TEST_F(TreatmentSequencer, ConfirmTurntableWithUnknownIsRejected)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, electron_rx()));
    TH25S_LONGS_EQUAL(TH25S_ERR_TURNTABLE_NOT_CONFIRMED,
                      th25s_seq_confirm_turntable(&seq, TH25S_TT_UNKNOWN));
}

/*
 * UT-002-13 HZ-001: 電子モードなのに X線位置を確定した場合、ビームオン要求時に
 * SafetyInterlock が不整合を検出し、FAULT へ遷移して照射を拒否する。Sequencer 側で
 * 位置の整合性を判定していなくても、独立した防御層が機能することを検証する。
 */
TH25S_TEST_F(TreatmentSequencer, BeamOnRejectsModeTurntableMismatchAndEntersFault)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, electron_rx()));
    /* 操作者が誤って X線位置を確定してしまう。 */
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_confirm_turntable(&seq, TH25S_TT_XRAY_POS));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_ready(&seq));

    TH25S_LONGS_EQUAL(TH25S_ERR_MODE_TURNTABLE_MISMATCH,
                      th25s_seq_request_beam_on(&seq));
    TH25S_LONGS_EQUAL(TH25S_SEQ_FAULT, th25s_seq_state(&seq));
}

/* UT-002-14 HZ-001: X線モードなのに電子位置を確定 → 同様に FAULT で拒否。
 * Therac-25 East Texas / Tyler 事故型 (高出力 X線モードで X線ターゲット非挿入) の
 * 発現経路を Sequencer 結合状態でも拒否することを検証する。 */
TH25S_TEST_F(TreatmentSequencer, BeamOnRejectsXrayModeWithElectronPosition)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_XRAY));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, xray_rx()));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_confirm_turntable(&seq, TH25S_TT_ELECTRON_POS));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_ready(&seq));

    TH25S_LONGS_EQUAL(TH25S_ERR_MODE_TURNTABLE_MISMATCH,
                      th25s_seq_request_beam_on(&seq));
    TH25S_LONGS_EQUAL(TH25S_SEQ_FAULT, th25s_seq_state(&seq));
}

/* UT-002-15 正常系: abort はどの状態からも IDLE へ戻す。 */
TH25S_TEST_F(TreatmentSequencer, AbortReturnsToIdleFromAnyState)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, electron_rx()));
    th25s_seq_abort(&seq);
    TH25S_LONGS_EQUAL(TH25S_SEQ_IDLE, th25s_seq_state(&seq));

    /* FAULT 状態からも IDLE へ戻せる。 */
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, electron_rx()));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_confirm_turntable(&seq, TH25S_TT_XRAY_POS));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_ready(&seq));
    (void)th25s_seq_request_beam_on(&seq); /* FAULT へ */
    TH25S_LONGS_EQUAL(TH25S_SEQ_FAULT, th25s_seq_state(&seq));
    th25s_seq_abort(&seq);
    TH25S_LONGS_EQUAL(TH25S_SEQ_IDLE, th25s_seq_state(&seq));
}

/* UT-002-16 異常系: ビーム照射中のモード変更は順序違反で拒否される。 */
TH25S_TEST_F(TreatmentSequencer, ModeChangeDuringBeamOnIsRejected)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, electron_rx()));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_confirm_turntable(&seq, TH25S_TT_ELECTRON_POS));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_ready(&seq));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_beam_on(&seq));
    TH25S_LONGS_EQUAL(TH25S_ERR_SEQUENCE_VIOLATION,
                      th25s_seq_select_mode(&seq, TH25S_MODE_XRAY));
}

/*
 * UT-002-17 HZ-003 / RCM-003: 整合性チェックカウンタを上限に設定した状態で
 * ビームオンを要求すると、安全チェックを「省略」せず TH25S_ERR_COUNTER_OVERFLOW
 * を返して FAULT へ遷移する。Therac-25 のカウンタ巻き戻りによるチェックバイパスとは
 * 逆に、カウンタ異常時は照射を拒否する側に倒れることを検証する。
 * (consistency_checks は公開フィールド。試験での状態準備であり、本番コードに
 *  試験用分岐を入れているわけではない。)
 */
TH25S_TEST_F(TreatmentSequencer, BeamOnRefusesWhenConsistencyCounterOverflowed)
{
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, electron_rx()));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_confirm_turntable(&seq, TH25S_TT_ELECTRON_POS));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_ready(&seq));

    /* カウンタを上限到達状態に準備する。 */
    seq.consistency_checks.value = seq.consistency_checks.limit;

    TH25S_LONGS_EQUAL(TH25S_ERR_COUNTER_OVERFLOW, th25s_seq_request_beam_on(&seq));
    TH25S_LONGS_EQUAL(TH25S_SEQ_FAULT, th25s_seq_state(&seq));
}

/* UT-002-18 異常系: NULL 引数。状態取得は FAULT、各操作は TH25S_ERR_NULL_ARG。 */
TH25S_TEST_F(TreatmentSequencer, NullArgIsHandled)
{
    TH25S_LONGS_EQUAL(TH25S_SEQ_FAULT, th25s_seq_state(NULL));
    TH25S_LONGS_EQUAL(TH25S_ERR_NULL_ARG,
                      th25s_seq_select_mode(NULL, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_ERR_NULL_ARG, th25s_seq_request_beam_on(NULL));
    th25s_seq_abort(NULL); /* クラッシュしないこと */
}

/* UT-002-19 異常系: モード選択に NONE を渡す → TH25S_ERR_MODE_NONE。 */
TH25S_TEST_F(TreatmentSequencer, SelectModeNoneIsRejected)
{
    TH25S_LONGS_EQUAL(TH25S_ERR_MODE_NONE,
                      th25s_seq_select_mode(&seq, TH25S_MODE_NONE));
    TH25S_LONGS_EQUAL(TH25S_SEQ_IDLE, th25s_seq_state(&seq));
}

/* UT-002-20 異常系: FAULT 状態では abort 以外の操作を受け付けない。 */
TH25S_TEST_F(TreatmentSequencer, FaultStateRejectsOperationsExceptAbort)
{
    /* FAULT へ遷移させる。 */
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_select_mode(&seq, TH25S_MODE_XRAY));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_set_prescription(&seq, xray_rx()));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_confirm_turntable(&seq, TH25S_TT_ELECTRON_POS));
    TH25S_LONGS_EQUAL(TH25S_OK, th25s_seq_request_ready(&seq));
    (void)th25s_seq_request_beam_on(&seq);
    TH25S_LONGS_EQUAL(TH25S_SEQ_FAULT, th25s_seq_state(&seq));

    /* FAULT 中は各操作が順序違反で拒否される。 */
    TH25S_LONGS_EQUAL(TH25S_ERR_SEQUENCE_VIOLATION,
                      th25s_seq_select_mode(&seq, TH25S_MODE_ELECTRON));
    TH25S_LONGS_EQUAL(TH25S_ERR_SEQUENCE_VIOLATION, th25s_seq_request_beam_on(&seq));

    /* abort のみ受け付けて IDLE へ復帰する。 */
    th25s_seq_abort(&seq);
    TH25S_LONGS_EQUAL(TH25S_SEQ_IDLE, th25s_seq_state(&seq));
}
