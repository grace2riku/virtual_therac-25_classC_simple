# ソフトウェアユニットテスト計画書/報告書

**ドキュメント ID:** UTPR-TH25S-001
**バージョン:** 1.2
**作成日:** 2026-05-13
**対象製品:** 仮想 Therac-25 Simple / TH25S-SIM-001
**対象ソフトウェアバージョン:** 1.0.0
**安全クラス:** C(IEC 62304)

| 役割 | 氏名 | 所属 | 日付 | 署名 |
|------|------|------|------|------|
| 作成者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-13 | — |
| レビュー者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-13 | — |
| 承認者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-13 | — |

---

> 本書はユニット試験の **計画**(第 I 部)と **実施結果(報告)**(第 II 部)を一体で管理する。

## 1. 目的と適用範囲

本書は、IEC 62304 箇条 5.5 に基づき、TH25S-CORE の各ソフトウェアユニット(UNIT-001〜003)の実装と検証(ユニット試験)の計画および結果を記録する。

## 2. 参照文書

| ID | 文書名 | バージョン |
|----|--------|----------|
| [1] | ソフトウェア詳細設計書(SDD-TH25S-001) | 1.0 |
| [2] | ソフトウェア開発計画書(SDP-TH25S-001) | 1.0 |

---

# 第 I 部 計画

## 3. ソフトウェアユニットの実装(箇条 5.5.1)

### 3.1 実装ルール
- 使用言語: C11(`-std=c11`、拡張無効)。
- コーディング規約: MISRA C:2012 を参考。全公開関数の冒頭で引数 NULL チェックを行う。
- 静的解析: コンパイラ厳格警告(`-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wcast-align -Wnull-dereference -Wdouble-promotion`)を **警告ゼロ** で通す。
- コードレビュー: Pull Request 上で自己レビューチェックリストにより実施。

### 3.2 実装対象ユニット一覧
| ユニット ID | 名称 | ソースファイル |
|------------|------|---------------|
| UNIT-001 | CommonTypes | `src/th25s_core/include/th25s_core/common_types.h` / `src/th25s_core/src/common_types.c` |
| UNIT-002 | TreatmentSequencer | `src/th25s_core/include/th25s_core/treatment_sequencer.h` / `src/th25s_core/src/treatment_sequencer.c` |
| UNIT-003 | SafetyInterlock | `src/th25s_core/include/th25s_core/safety_interlock.h` / `src/th25s_core/src/safety_interlock.c` |

## 4. ソフトウェアユニット検証プロセスの確立(箇条 5.5.2)

| 方法 | 適用範囲 | ツール |
|------|---------|-------|
| コードレビュー | 全ユニット | Pull Request + 自己レビューチェックリスト |
| 静的解析 | 全ユニット | コンパイラ厳格警告(警告ゼロを必須) |
| ユニット試験 | 全ユニット | CppUTest 4.0(SOUP-001) |

## 5. ソフトウェアユニット受入基準(箇条 5.5.3)

各ユニットは以下を **すべて** 満たすことを受入条件とする。

1. 詳細設計書(SDD §4)の公開 API と実装が一致している。
2. コーディング規約違反がない(コンパイラ厳格警告ゼロ)。
3. ユニット試験が全件合格している。

## 6. 追加のユニット受入基準(箇条 5.5.4 ― クラス C)

クラス C のため、以下を追加の受入基準とする。

- [x] **正常系の動作確認:** 正常入力で期待出力が得られること(UT-xxx-01 系等)
- [x] **境界値試験:** 入力値域の最小・最大・境界±(エネルギ・線量・カウンタ上限)
- [x] **異常系・エラー入力:** 値域外、NULL、不正状態からの呼出
- [x] **資源使用:** 動的メモリ確保なし・再帰なし。固定サイズ構造体のみ
- [x] **制御フロー網羅:** 各公開関数の全分岐を試験ケースで通過(分岐網羅 100%)
- [x] **データフロー:** 未初期化変数なし(厳格警告で担保)
- [x] **障害の検出と処置:** NULL・範囲外・順序違反・カウンタオーバーフローの検出と処置が設計どおり
- [x] **並行処理:** 本ソフトウェアは単一スレッド設計であり、競合は操作順序依存の問題として扱う(RCM-001 で対応)。マルチスレッドは適用しない
- [x] **タイミング:** 全操作が同期的に即座に戻る(SRS-101)

## 7. ソフトウェアユニット試験(箇条 5.5.5)

### 7.1 試験環境
- ホスト環境: PC(macOS / Linux)、GCC または Clang(C11 / C++17)
- 試験フレームワーク: **CppUTest 4.0(SOUP-001、既定)** または **GoogleTest(SOUP-002、CR-0001 で追加、CR-0002 で環境別バージョン正確化: macOS Homebrew = 1.17.0 / Ubuntu 24.04 apt `libgtest-dev` = 1.14.0)** のいずれか、または両方。CMake オプション `-DTH25S_TEST_FRAMEWORK=cpputest|gtest|both` で選択する。
- 試験フレームワーク互換層: `tests/test_framework.h` が両フレームワーク間の API 差を吸収する。同一の試験ソース(`tests/test_*.cpp`)が両フレームワークでコンパイル可能。
- 検出: CppUTest は pkg-config 経由、GoogleTest は `find_package(GTest)` 経由でリンクする。
- ビルド: CMake 3.20 以上 / `cmake -B build && cmake --build build`
- 実行: `ctest --test-dir build --output-on-failure`(`both` 選択時は cpputest 版と gtest 版の 2 つの実行ファイルを順に実行する)
- 試験コード: `tests/test_common_types.cpp` / `tests/test_treatment_sequencer.cpp` / `tests/test_safety_interlock.cpp`(両フレームワーク共通の単一ソース)

### 7.2 試験ケース定義

#### UNIT-001 CommonTypes ユニット試験ケース一覧(`tests/test_common_types.cpp`)

| 試験 ID | 対象関数 | 入力 | 期待結果 | 種別 | 関連 SRS / RCM |
|--------|---------|------|---------|------|---------------|
| UT-001-01 | validate_energy | 電子モード 10.0 MeV | TH25S_OK | 正常系 | SRS-005 |
| UT-001-02 | validate_energy | 電子モード 1.0 MeV(下限) | TH25S_OK | 境界値 | SRS-005 |
| UT-001-03 | validate_energy | 電子モード 25.0 MeV(上限) | TH25S_OK | 境界値 | SRS-005 |
| UT-001-04 | validate_energy | 電子モード 0.99 MeV | ENERGY_OUT_OF_RANGE | 異常系 | SRS-005 |
| UT-001-05 | validate_energy | 電子モード 25.01 MeV | ENERGY_OUT_OF_RANGE | 異常系 | SRS-005 |
| UT-001-06 | validate_energy | X線モード 5.0 MV(下限) | TH25S_OK | 境界値 | SRS-005 |
| UT-001-07 | validate_energy | 4.99(電子は OK / X線は範囲外) | OK / ENERGY_OUT_OF_RANGE | 異常系 | SRS-005 |
| UT-001-08 | validate_energy | モード NONE | MODE_NONE | 異常系 | SRS-005 |
| UT-001-09 | validate_dose | 200.0 cGy | TH25S_OK | 正常系 | SRS-008 |
| UT-001-10 | validate_dose | 0.01 cGy(下限) | TH25S_OK | 境界値 | SRS-008 |
| UT-001-11 | validate_dose | 10000.0 cGy(上限) | TH25S_OK | 境界値 | SRS-008 |
| UT-001-12 | validate_dose | 0.009 cGy | DOSE_OUT_OF_RANGE | 異常系 | SRS-008 |
| UT-001-13 | validate_dose | 10000.01 cGy | DOSE_OUT_OF_RANGE | 異常系 | SRS-008 |
| UT-001-14 | safe_counter_init | limit=3 | value=0, overflowed=false, is_valid=true | 正常系 | SRS-009 / RCM-003 |
| UT-001-15 | safe_counter_increment | 上限未満で 2 回 | value が +1 ずつ進む、OK | 正常系 | SRS-009 / RCM-003 |
| UT-001-16 | safe_counter_increment | 上限到達後 | COUNTER_OVERFLOW、value は limit を超えない | 境界値・異常系 | SRS-009 / RCM-003 |
| UT-001-17 | safe_counter_is_valid | オーバーフロー後 | false | 異常系 | SRS-009 / RCM-003 |
| UT-001-18 | safe_counter_increment | オーバーフロー後 300 回 | 常に COUNTER_OVERFLOW、0 へ巻き戻らない | 異常系 | SRS-009 / RCM-003 |
| UT-001-19 | safe_counter_increment / is_valid | NULL | NULL_ARG / false | 異常系 | RCM-003 |
| UT-001-20 | error_message | 全 10 エラーコード | 非 NULL かつ非空のメッセージ | 正常系 | SRS-010 / RCM-004 |
| UT-001-21 | error_message | 未定義コード(9999) | 非 NULL かつ非空 | 異常系 | SRS-010 / RCM-004 |

#### UNIT-002 TreatmentSequencer ユニット試験ケース一覧(`tests/test_treatment_sequencer.cpp`)

| 試験 ID | 対象関数 | 入力・操作 | 期待結果 | 種別 | 関連 SRS / RCM |
|--------|---------|-----------|---------|------|---------------|
| UT-002-01 | seq_init | 初期化 | state=IDLE | 正常系 | SRS-002 |
| UT-002-02 | (全 API) | 正しい順序の全操作 | BEAM_ON に到達 | 正常系 | SRS-001, SRS-101 |
| UT-002-03 | set_prescription | IDLE から呼出 | SEQUENCE_VIOLATION | 異常系 | SRS-002 / RCM-001 |
| UT-002-04 | request_beam_on | IDLE から呼出 | SEQUENCE_VIOLATION | 異常系 | SRS-002, SRS-004 / RCM-001 |
| UT-002-05 | confirm_turntable | 処方未設定で呼出 | SEQUENCE_VIOLATION | 異常系 | SRS-002 / RCM-001 |
| UT-002-06 | request_beam_on | ターンテーブル未確定で呼出 | SEQUENCE_VIOLATION | 異常系 | SRS-002, SRS-004 / RCM-001 |
| UT-002-07 | select_mode | READY 到達後にモード再選択 | state が MODE_SELECTED へ戻り turntable=UNKNOWN | 正常系(RCM 中核) | SRS-003 / RCM-001 |
| UT-002-08 | request_ready | モード再選択後に再確定せず READY 要求 | SEQUENCE_VIOLATION | 異常系(RCM 中核) | SRS-003 / RCM-001 |
| UT-002-09 | set_prescription | 選択モードと処方モードが不一致 | SEQUENCE_VIOLATION | 異常系 | SRS-002 |
| UT-002-10 | set_prescription | 範囲外エネルギ | ENERGY_OUT_OF_RANGE | 異常系 | SRS-005 |
| UT-002-11 | set_prescription | 範囲外線量 | DOSE_OUT_OF_RANGE | 異常系 | SRS-008 |
| UT-002-12 | confirm_turntable | UNKNOWN を渡す | TURNTABLE_NOT_CONFIRMED | 異常系 | SRS-006 |
| UT-002-13 | request_beam_on | 電子モードで X線位置を確定 | MODE_TURNTABLE_MISMATCH、state=FAULT | 異常系(HZ-001) | SRS-007 / RCM-002 |
| UT-002-14 | request_beam_on | X線モードで電子位置を確定 | MODE_TURNTABLE_MISMATCH、state=FAULT | 異常系(HZ-001) | SRS-007 / RCM-002 |
| UT-002-15 | seq_abort | 任意状態 / FAULT から abort | state=IDLE | 正常系 | SRS-002 |
| UT-002-16 | select_mode | BEAM_ON 中にモード変更 | SEQUENCE_VIOLATION | 異常系 | SRS-002 / RCM-001 |
| UT-002-17 | request_beam_on | カウンタ上限到達状態で要求 | COUNTER_OVERFLOW、state=FAULT | 異常系(HZ-003) | SRS-009 / RCM-003 |
| UT-002-18 | (全 API) | NULL 引数 | NULL_ARG / FAULT、クラッシュなし | 異常系 | — |
| UT-002-19 | select_mode | NONE を渡す | MODE_NONE | 異常系 | SRS-001 |
| UT-002-20 | (各 API) | FAULT 状態から abort 以外 | SEQUENCE_VIOLATION、abort のみ受理 | 異常系 | SRS-002 / RCM-001 |

#### UNIT-003 SafetyInterlock ユニット試験ケース一覧(`tests/test_safety_interlock.cpp`)

| 試験 ID | 対象関数 | 入力 | 期待結果 | 種別 | 関連 SRS / RCM |
|--------|---------|------|---------|------|---------------|
| UT-003-01 | interlock_check_beam_on | 電子モード + 電子位置 + 範囲内 | TH25S_OK | 正常系 | SRS-007, SRS-101 |
| UT-003-02 | interlock_check_beam_on | X線モード + X線位置 + 範囲内 | TH25S_OK | 正常系 | SRS-007 |
| UT-003-03 | interlock_check_beam_on | X線モード + 電子位置 | MODE_TURNTABLE_MISMATCH | 異常系(HZ-001 中核) | SRS-007 / RCM-002 |
| UT-003-04 | interlock_check_beam_on | 電子モード + X線位置 | MODE_TURNTABLE_MISMATCH | 異常系(HZ-001) | SRS-007 / RCM-002 |
| UT-003-05 | interlock_check_beam_on | ターンテーブル UNKNOWN | MODE_TURNTABLE_MISMATCH | 異常系 | SRS-007 / RCM-002 |
| UT-003-06 | interlock_check_beam_on | ターンテーブル FIELD_LIGHT_POS | MODE_TURNTABLE_MISMATCH | 異常系 | SRS-007 / RCM-002 |
| UT-003-07 | interlock_check_beam_on | モード NONE | MODE_NONE | 異常系 | SRS-007 |
| UT-003-08 | interlock_check_beam_on | 範囲外エネルギ(電子 30.0 MeV) | ENERGY_OUT_OF_RANGE | 異常系 | SRS-005 |
| UT-003-09 | interlock_check_beam_on | 範囲外線量(20000.0 cGy) | DOSE_OUT_OF_RANGE | 異常系 | SRS-008 |
| UT-003-10 | interlock_check_beam_on | 電子モード下限エネルギ + 整合 | TH25S_OK | 境界値 | SRS-005 |
| UT-003-11 | interlock_check_beam_on | X線モード上限エネルギ + 整合 | TH25S_OK | 境界値 | SRS-005 |
| UT-003-12 | interlock_check_beam_on | 線量下限・上限 + 整合 | TH25S_OK | 境界値 | SRS-008 |
| UT-003-13 | interlock_check_beam_on | NULL | NULL_ARG | 異常系 | — |
| UT-003-14 | interlock_check_beam_on | 同一入力を 5 回呼出 | 毎回同一結果(純関数性) | 正常系(SEP-001) | RCM-002 |

### 7.3 カバレッジ目標
| 指標 | 目標値 |
|------|-------|
| ステートメントカバレッジ | 100% |
| 分岐(decision)カバレッジ | 100%(安全関連の全ユニット) |

---

# 第 II 部 報告

## 8. 試験実施結果

### 8.1 実施サマリ
- 実施日: 2026-05-13(CppUTest)、2026-05-15(GoogleTest 追加検証 ― CR-0001)
- 実施者: 開発者A(全ロール兼任)
- ソフトウェアバージョン: 1.0.0(製品コード不変)
- 試験環境: PC ホスト、CMake 3.20 以上 + CppUTest 4.0 + GoogleTest(macOS Homebrew 1.17.0 / Ubuntu 24.04 apt 1.14.0)、`ctest`
- 実行コマンド(CR-0001 後): `cmake -B build -DCMAKE_BUILD_TYPE=Debug -DTH25S_TEST_FRAMEWORK=both && cmake --build build && ctest --test-dir build --output-on-failure`

### 8.2 試験ケース結果

| ユニット | 試験 ID 範囲 | 件数 | 結果 |
|---------|------------|------|------|
| UNIT-001 | UT-001-01〜UT-001-21 | 21 | 全件合格 |
| UNIT-002 | UT-002-01〜UT-002-20 | 20 | 全件合格 |
| UNIT-003 | UT-003-01〜UT-003-14 | 14 | 全件合格 |
| **合計** | — | **55** | **全件合格** |

CppUTest 実行結果(`th25s_tests_cpputest`)の総括:

```
OK (55 tests, 55 ran, 760 checks, 0 ignored, 0 filtered out)
```

GoogleTest 実行結果(`th25s_tests_gtest`、CR-0001 後):

```
[==========] 55 tests from 6 test suites ran.
[  PASSED  ] 55 tests.
```

`TH25S_TEST_FRAMEWORK=both` での ctest 集計:

```
100% tests passed, 0 tests failed out of 2  (cpputest 版 + gtest 版の 2 ctest エントリ)
```

ビルド時のコンパイラ厳格警告: **0 件**(警告ゼロ。受入基準 5 章 -2 を満たす)。両フレームワークで同一の試験ケース集合が合格することを確認した(`tests/test_framework.h` 互換層により試験ロジックは単一ソース)。

### 8.3 カバレッジ実績
| ユニット ID | ステートメント | 分岐 | 備考 |
|------------|--------------|------|------|
| UNIT-001 | 100% | 100% | 全公開関数の正常・境界・異常分岐を UT-001-01〜21 で通過 |
| UNIT-002 | 100% | 100% | 全状態遷移・全順序違反経路を UT-002-01〜20 で通過 |
| UNIT-003 | 100% | 100% | 全整合性判定分岐を UT-003-01〜14 で通過 |

### 8.4 不具合・逸脱
| 問題 ID | 内容 | 重大度 | 対応 | ステータス |
|--------|------|-------|------|----------|
| — | ユニット試験で検出された不具合はなし | — | — | — |

### 8.5 未達項目と処置
未達項目なし。すべての受入基準(5 章・6 章)を満たした。

## 9. 結論

- [x] 全ユニット(UNIT-001〜003)が受入基準(5 章)および追加受入基準(6 章)を満たしている。
- [x] 未解決問題はない。リリース時点の残留異常(ANOM)はない(SMS §4 参照)。

## 10. トレーサビリティマトリクス

| ユニット ID | 試験 ID | 関連 SRS / RCM | 結果 |
|------------|--------|--------------|------|
| UNIT-001 | UT-001-01〜21 | SRS-005, SRS-008, SRS-009, SRS-010 / RCM-003, RCM-004 | 全件合格 |
| UNIT-002 | UT-002-01〜20 | SRS-001〜004, SRS-006, SRS-009 / RCM-001, RCM-003 | 全件合格 |
| UNIT-003 | UT-003-01〜14 | SRS-005, SRS-007, SRS-008 / RCM-002 | 全件合格 |

## 11. 改訂履歴

| バージョン | 日付 | 変更内容 | 変更者 |
|----------|------|---------|--------|
| 1.0 | 2026-05-13 | 初版作成(計画 + 報告)。UNIT-001〜003 の実装と全 55 試験ケースの合格を記録。 | 開発者A |
| 1.1 | 2026-05-15 | CR-0001 反映: §7.1 試験環境に GoogleTest(SOUP-002)選択を追加、§8.1 / §8.2 に GoogleTest 実行結果と両フレームワーク合格を追記。試験ケース定義(§7.2)・カバレッジ(§7.3 / §8.3)・トレーサビリティ(§10)は不変(同一ケース集合)。 | 開発者A |
| 1.2 | 2026-05-15 | CR-0002 反映: §7.1 / §8.1 の GoogleTest バージョン記載を環境別(macOS Homebrew 1.17.0 / Ubuntu 24.04 apt 1.14.0)に正確化。試験結果・カバレッジ・トレーサビリティは不変。 | 開発者A |
