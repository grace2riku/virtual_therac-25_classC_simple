/*
 * UNIT-001 CommonTypes 実装
 * 仮想 Therac-25 Simple (TH25S-CORE) / IEC 62304 クラス C 学習プロジェクト
 * 関連設計: SDD-TH25S-001 §4.1
 */
#include "th25s_core/common_types.h"

const char *th25s_error_message(th25s_error_t code)
{
    /* RCM-004: すべてのコードに人間可読のメッセージを対応付ける。 */
    switch (code) {
    case TH25S_OK:
        return "OK: 整合性チェックに合格";
    case TH25S_ERR_NULL_ARG:
        return "ERROR: 引数が NULL です";
    case TH25S_ERR_MODE_NONE:
        return "ERROR: 治療モードが選択されていません";
    case TH25S_ERR_ENERGY_OUT_OF_RANGE:
        return "ERROR: エネルギがモード別の許容範囲外です";
    case TH25S_ERR_DOSE_OUT_OF_RANGE:
        return "ERROR: 線量が許容範囲 (0.01-10000.0 cGy) 外です";
    case TH25S_ERR_SEQUENCE_VIOLATION:
        return "ERROR: 治療シーケンスの操作順序が不正です";
    case TH25S_ERR_TURNTABLE_NOT_CONFIRMED:
        return "ERROR: ターンテーブル位置が確定されていません";
    case TH25S_ERR_MODE_TURNTABLE_MISMATCH:
        return "ERROR: 治療モードとターンテーブル位置が不整合です (ビーム照射不可)";
    case TH25S_ERR_NOT_READY:
        return "ERROR: READY 状態ではありません";
    case TH25S_ERR_COUNTER_OVERFLOW:
        return "ERROR: 整合性チェックカウンタがオーバーフローしました";
    default:
        return "ERROR: 未定義のエラーコード";
    }
}

th25s_error_t th25s_validate_energy(th25s_treatment_mode_t mode, double energy)
{
    switch (mode) {
    case TH25S_MODE_ELECTRON:
        if (energy < TH25S_ELECTRON_ENERGY_MIN_MEV ||
            energy > TH25S_ELECTRON_ENERGY_MAX_MEV) {
            return TH25S_ERR_ENERGY_OUT_OF_RANGE;
        }
        return TH25S_OK;
    case TH25S_MODE_XRAY:
        if (energy < TH25S_XRAY_ENERGY_MIN_MV ||
            energy > TH25S_XRAY_ENERGY_MAX_MV) {
            return TH25S_ERR_ENERGY_OUT_OF_RANGE;
        }
        return TH25S_OK;
    case TH25S_MODE_NONE:
    default:
        return TH25S_ERR_MODE_NONE;
    }
}

th25s_error_t th25s_validate_dose(double dose_cgy)
{
    if (dose_cgy < TH25S_DOSE_MIN_CGY || dose_cgy > TH25S_DOSE_MAX_CGY) {
        return TH25S_ERR_DOSE_OUT_OF_RANGE;
    }
    return TH25S_OK;
}

void th25s_safe_counter_init(th25s_safe_counter_t *counter, uint32_t limit)
{
    if (counter == NULL) {
        return;
    }
    counter->value = 0U;
    counter->limit = limit;
    counter->overflowed = false;
}

th25s_error_t th25s_safe_counter_increment(th25s_safe_counter_t *counter)
{
    if (counter == NULL) {
        return TH25S_ERR_NULL_ARG;
    }
    /*
     * RCM-003 / HZ-003: 上限到達時に 0 へ巻き戻さず、値を保持して overflowed を立てる。
     * Therac-25 の 8bit カウンタは巻き戻った瞬間に安全チェックがバイパスされたが、
     * 本実装は巻き戻りそのものを発生させない。
     */
    if (counter->value >= counter->limit) {
        counter->overflowed = true;
        return TH25S_ERR_COUNTER_OVERFLOW;
    }
    counter->value++;
    return TH25S_OK;
}

bool th25s_safe_counter_is_valid(const th25s_safe_counter_t *counter)
{
    if (counter == NULL) {
        return false;
    }
    return !counter->overflowed;
}
