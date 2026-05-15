/*
 * UNIT-001 CommonTypes — 共通型・検証関数・飽和カウンタ
 * 仮想 Therac-25 Simple (TH25S-CORE) / IEC 62304 クラス C 学習プロジェクト
 *
 * 関連設計: SDD-TH25S-001 §4.1 / 関連要求: SRS-TH25S-001 §4
 * 関連 RCM: RCM-003 (飽和カウンタ) / RCM-004 (明確なエラーメッセージ)
 */
#ifndef TH25S_CORE_COMMON_TYPES_H
#define TH25S_CORE_COMMON_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 治療モード (SRS-001)。電子モードは低出力、X線モードは X線ターゲット挿入を伴う高出力。 */
typedef enum {
    TH25S_MODE_NONE = 0,
    TH25S_MODE_ELECTRON,
    TH25S_MODE_XRAY
} th25s_treatment_mode_t;

/*
 * ターンテーブル位置 (SRS-006)。Therac-25 ではターンテーブルが
 * ビーム経路上の機構 (散乱箔 / X線ターゲット+フラットニングフィルタ) を切り替える。
 * モードとターンテーブル位置の不整合が過剰照射事故の直接原因となった (HZ-001)。
 */
typedef enum {
    TH25S_TT_UNKNOWN = 0,        /* 位置未確定 */
    TH25S_TT_FIELD_LIGHT_POS,    /* フィールドライト位置 (患者位置決め用、ビーム照射不可) */
    TH25S_TT_ELECTRON_POS,       /* 電子モード位置 (散乱箔) */
    TH25S_TT_XRAY_POS            /* X線モード位置 (X線ターゲット + フラットニングフィルタ) */
} th25s_turntable_position_t;

/*
 * エラーコード体系 (RCM-004)。
 * Therac-25 の "MALFUNCTION 54" のような暗号的メッセージが操作者の誤判断と
 * バイパス常態化を招いた教訓から、各コードに人間可読のメッセージを対応付ける
 * (th25s_error_message)。
 */
typedef enum {
    TH25S_OK = 0,
    TH25S_ERR_NULL_ARG,                 /* 引数が NULL */
    TH25S_ERR_MODE_NONE,                /* 治療モードが未選択 */
    TH25S_ERR_ENERGY_OUT_OF_RANGE,      /* エネルギがモード別許容範囲外 */
    TH25S_ERR_DOSE_OUT_OF_RANGE,        /* 線量が許容範囲外 */
    TH25S_ERR_SEQUENCE_VIOLATION,       /* 状態機械の許可されない遷移 (HZ-002) */
    TH25S_ERR_TURNTABLE_NOT_CONFIRMED,  /* ターンテーブル位置が未確定 */
    TH25S_ERR_MODE_TURNTABLE_MISMATCH,  /* モードとターンテーブル位置の不整合 (HZ-001) */
    TH25S_ERR_NOT_READY,                /* READY 状態でない */
    TH25S_ERR_COUNTER_OVERFLOW          /* 整合性チェックカウンタのオーバーフロー (HZ-003) */
} th25s_error_t;

/* 処方 (治療パラメータ)。energy は電子モードで MeV、X線モードで MV。 */
typedef struct {
    th25s_treatment_mode_t mode;
    double energy;
    double dose_cgy;
} th25s_prescription_t;

/* エネルギ許容範囲 (SRS-005)。 */
#define TH25S_ELECTRON_ENERGY_MIN_MEV 1.0
#define TH25S_ELECTRON_ENERGY_MAX_MEV 25.0
#define TH25S_XRAY_ENERGY_MIN_MV 5.0
#define TH25S_XRAY_ENERGY_MAX_MV 25.0

/* 線量許容範囲 (SRS-008)。 */
#define TH25S_DOSE_MIN_CGY 0.01
#define TH25S_DOSE_MAX_CGY 10000.0

/*
 * 飽和カウンタ (RCM-003 — HZ-003 への対応)。
 *
 * Therac-25 では 1 バイトのカウンタが 256 回ごとに 0 へ巻き戻り、その瞬間に
 * 安全チェックがバイパスされた。本カウンタは上限到達時に 0 へ巻き戻らず
 * 上限値を保持し、overflowed フラグを立てる。これにより「カウンタ値が特定値に
 * なったら安全チェックを省略する」という危険な構造を成立させない。
 */
typedef struct {
    uint32_t value;
    uint32_t limit;
    bool overflowed;
} th25s_safe_counter_t;

/* エラーコードに対応する人間可読メッセージを返す (RCM-004)。NULL は返さない。 */
const char *th25s_error_message(th25s_error_t code);

/* モード別のエネルギ範囲を検証する (SRS-005)。 */
th25s_error_t th25s_validate_energy(th25s_treatment_mode_t mode, double energy);

/* 線量範囲を検証する (SRS-008)。 */
th25s_error_t th25s_validate_dose(double dose_cgy);

/* 飽和カウンタを初期化する。value=0, overflowed=false, limit=limit。 */
void th25s_safe_counter_init(th25s_safe_counter_t *counter, uint32_t limit);

/*
 * 飽和カウンタをインクリメントする。
 * - 上限未満なら value を +1 して TH25S_OK を返す。
 * - 上限に達している場合は value を変更せず overflowed を true にし、
 *   TH25S_ERR_COUNTER_OVERFLOW を返す (0 へ巻き戻らない)。
 */
th25s_error_t th25s_safe_counter_increment(th25s_safe_counter_t *counter);

/* オーバーフローしていなければ true。counter が NULL の場合は false。 */
bool th25s_safe_counter_is_valid(const th25s_safe_counter_t *counter);

#ifdef __cplusplus
}
#endif

#endif /* TH25S_CORE_COMMON_TYPES_H */
