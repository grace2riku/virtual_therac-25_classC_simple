# 仮想 Therac-25 Simple — IEC 62304 クラス C を 1 週間で一巡体験する学習プロジェクト

**本リポジトリは、IEC 62304:2006+A1:2015(JIS T 2304:2017)に基づく医療機器ソフトウェア開発プロセスを、約 1 週間で箇条 5〜9 まで一巡体験するための学習・参考実装プロジェクトです。**

実機・ハードウェア・実患者は存在せず、PC ホスト単体で動作する仮想的な安全コアを対象に、ソフトウェア安全クラス **C**(死亡又は重傷の可能性)の要求事項に則ったライフサイクル成果物を作成します。

## 位置づけ — シンプル版

本プロジェクトは、以下の姉妹プロジェクトの **シンプル版** です。

| プロジェクト | 規模 | 想定学習期間 |
|------------|------|------------|
| [virtual_therac-25_classC](https://github.com/grace2riku/virtual_therac-25_classC) | 3 プロセス構成・約 21 ユニット・46 ステップ超 | 数週間〜 |
| **virtual_therac-25_classC_simple(本リポジトリ)** | **安全コア 1 モジュール・3 ユニット・7 日** | **約 1 週間** |
| [iec62304_template](https://github.com/grace2riku/iec62304_template) | ドキュメントテンプレートのみ | — |

大規模版は規模ゆえに全体像の把握に時間がかかるため、本シンプル版は **IEC 62304 のステップ全体を最短で体験する** ことを目的に、ソフトウェア規模を Therac-25 の代表的 3 ハザードに集中させています。

## 題材: Therac-25 と歴史的事故

Therac-25 は 1982 年に AECL が開発した放射線治療装置です。1985〜1987 年に米国・カナダで **計 6 件の放射線過剰照射事故(6 名重症または死亡)** が発生し、医療機器ソフトウェアの安全性を論じる最も著名な事例となっています。

本プロジェクトは、事故原因のうち以下の 3 つに集中し、「IEC 62304 のプロセスが正しく回っていれば、どの成果物・どの活動が事故を防いだか」を追体験します。

| ハザード | Therac-25 事故での内容 | 本プロジェクトのリスクコントロール |
|---------|----------------------|--------------------------------|
| HZ-001 | ビームモードとビーム形成機構(ターンテーブル)の不整合状態での照射 | RCM-002: シーケンサから論理分離した独立インターロックによる最終整合性判定 |
| HZ-002 | 操作者入力とビーム設定のタイミング競合(race condition) | RCM-001: 明示的状態機械による操作順序の構造的強制(単一スレッドの順序依存バグとして教材化) |
| HZ-003 | 共有カウンタのオーバーフローによる安全チェックのバイパス | RCM-003: 飽和カウンタによるオーバーフロー時 0 復帰の構造的排除 |
| 横断 | 暗号的エラーメッセージ("MALFUNCTION 54")による誤判断 | RCM-004: 全エラーコードへの人間可読メッセージの対応付け |

> 本リポジトリは学術・教育目的の再構成(仮想)であり、当時の AECL 社内文書ではありません。事故に関する記述は学術文献(N. G. Leveson & C. S. Turner, "An Investigation of the Therac-25 Accidents", IEEE Computer, 1993 ほか)に基づきます。

**各ハザードの事故発生メカニズムの詳細解説(Mermaid 図解付き)** は [Therac-25 ハザード解析・事故メカニズム解説(`THERAC25_HAZARD_ANALYSIS.md`)](./THERAC25_HAZARD_ANALYSIS.md) を参照してください。「IEC 62304 のプロセスを踏めば事故を防げたか」「ハザードがどうシステム要求(SRS / RCM)へ展開されるか」の観点で HZ-001〜003 + 横断要因を掘り下げています。

## 対象製品の概要

| 項目 | 内容 |
|------|------|
| 製品名 | 仮想 Therac-25 Simple(Virtual Therac-25 Simple) |
| 製品コード / 型式 | TH25S / TH25S-SIM-001 |
| ソフトウェア名称 | Therac-25 Simple Beam Interlock Software(TH25S-CORE) |
| ソフトウェア安全クラス | **C**(IEC 62304) |
| 実装言語 | **C11** |
| 試験フレームワーク | **CppUTest 4.0**(SOUP-001、既定)または **GoogleTest**(SOUP-002、CR-0001 で追加。実バージョンは macOS Homebrew で 1.17.0、Ubuntu 24.04 apt で 1.14.0)を CMake オプションで選択 |
| ライフサイクルモデル | V 字モデル(単一インクリメント・7 日) |
| リリースバージョン | 1.0.0 |

## ソフトウェア構成(安全コア 1 モジュール・3 ユニット)

```
ARCH-001  TH25S-CORE 安全コア (libth25s_core.a)
├── UNIT-001  CommonTypes        共通型・モード別範囲検証・飽和カウンタ・エラーメッセージ
├── UNIT-002  TreatmentSequencer 治療シーケンス状態機械(操作順序を構造的に強制)
└── UNIT-003  SafetyInterlock    ビーム照射可否の独立した最終整合性判定(純関数)
```

依存方向は常に上位→下位の一方向で、SafetyInterlock(防御層)を TreatmentSequencer(制御層)から論理分離(SEP-001)しています。

## 収録ドキュメント(IEC 62304 箇条 5〜9)

| 箇条 | ドキュメント | パス |
|------|-----------|------|
| 5.1 | ソフトウェア開発計画書(SDP) | [`5.1_.../software_development_plan.md`](./5.1_software_development_planning/software_development_plan.md) |
| 5.2 | ソフトウェア要求仕様書(SRS) | [`5.2_.../software_requirements_specification.md`](./5.2_software_requirements_analysis/software_requirements_specification.md) |
| 5.3 | ソフトウェアアーキテクチャ設計書(SAD) | [`5.3_.../software_architecture_design.md`](./5.3_software_architecture_design/software_architecture_design.md) |
| 5.4 | ソフトウェア詳細設計書(SDD) | [`5.4_.../software_detailed_design.md`](./5.4_software_detailed_design/software_detailed_design.md) |
| 5.5 | ユニットテスト計画書/報告書(UTPR) | [`5.5_.../software_unit_test_plan_report.md`](./5.5_software_unit_implementation/software_unit_test_plan_report.md) |
| 5.6 | 結合試験計画書/報告書(ITPR) | [`5.6_.../software_integration_test_plan_report.md`](./5.6_software_integration_testing/software_integration_test_plan_report.md) |
| 5.7 | システム試験計画書/報告書(STPR) | [`5.7_.../software_system_test_plan_report.md`](./5.7_software_system_testing/software_system_test_plan_report.md) |
| 5.8 | ソフトウェアマスタ仕様書(SMS) | [`5.8_.../software_master_specification.md`](./5.8_software_release/software_master_specification.md) |
| 6 | ソフトウェア保守計画書(SMP) | [`6_.../software_maintenance_plan.md`](./6_software_maintenance_process/software_maintenance_plan.md) |
| 7 | リスクマネジメント計画書(SRMP) | [`7_.../software_risk_management_plan.md`](./7_software_risk_management_process/software_risk_management_plan.md) |
| 7 | ソフトウェア安全クラス決定記録(SSC) | [`7_.../software_safety_class_determination_record.md`](./7_software_risk_management_process/software_safety_class_determination_record.md) |
| 7 | リスクマネジメントファイル(RMF) | [`7_.../risk_management_file.md`](./7_software_risk_management_process/risk_management_file.md) |
| 8 | 構成管理計画書(SCMP) | [`8_.../software_configuration_management_plan.md`](./8_software_configuration_management_process/software_configuration_management_plan.md) |
| 8 | 構成アイテム一覧(CIL) | [`8_.../configuration_item_list.md`](./8_software_configuration_management_process/configuration_item_list.md) |
| 8 | CCB 運用規程(CCB) | [`8_.../ccb_operating_rules.md`](./8_software_configuration_management_process/ccb_operating_rules.md) |
| 8 | 変更要求台帳(CRR) | [`8_.../change_request_register.md`](./8_software_configuration_management_process/change_request_register.md) |
| 9 | ソフトウェア問題解決手順書(SPRP) | [`9_.../software_problem_resolution_procedure.md`](./9_software_problem_resolution_process/software_problem_resolution_procedure.md) |
| 補助 | IEC 62304 監査チェックリスト(ACL) | [`compliance/audit_checklist.md`](./compliance/audit_checklist.md) |

進め方の手引きは [`DEVELOPMENT_STEPS.md`](./DEVELOPMENT_STEPS.md)(Day1〜Day7 の日割りガイド + Day8〜 保守ステップ実績)を参照してください。

IEC 62304 のプロセス・用語に関する質問と回答の蓄積は [`IEC62304_QA.md`](./IEC62304_QA.md) を参照してください。

## ビルドと試験

### 前提

- C11 / C++17 対応のコンパイラ(GCC または Clang)
- CMake 3.20 以上
- 試験フレームワーク (どちらか、または両方)
  - **CppUTest 4.0** + pkg-config — macOS: `brew install cpputest` / Ubuntu/Debian: `sudo apt-get install libcpputest-dev`
  - **GoogleTest 1.x** — macOS: `brew install googletest`(2026-05-15 時点で 1.17.0)/ Ubuntu/Debian: `sudo apt-get install libgtest-dev`(Ubuntu 24.04 では 1.14.0)。両環境で本プロジェクトの試験 API(`TEST`/`TEST_F`/`EXPECT_EQ`/`EXPECT_TRUE`/`EXPECT_FALSE` 等)は互換

### 手順

試験フレームワークは CMake オプション `TH25S_TEST_FRAMEWORK` で選択します(`cpputest` 既定 / `gtest` / `both`)。

```bash
# 既定: CppUTest で試験 (TH25S_TEST_FRAMEWORK=cpputest 相当)
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure

# GoogleTest で試験
cmake -B build-gtest -DCMAKE_BUILD_TYPE=Debug -DTH25S_TEST_FRAMEWORK=gtest
cmake --build build-gtest
ctest --test-dir build-gtest --output-on-failure

# 両フレームワークで試験 (CI と同じ設定)
cmake -B build-both -DCMAKE_BUILD_TYPE=Debug -DTH25S_TEST_FRAMEWORK=both
cmake --build build-both
ctest --test-dir build-both --output-on-failure

# 製品ライブラリのみをビルドする場合 (試験フレームワーク不要)
cmake -B build-lib -DTH25S_BUILD_TESTS=OFF
cmake --build build-lib
```

試験は **55 ケース / 760 チェック** を実行し、全件合格します(UTPR §8.2)。GoogleTest 版も同一の 55 ケースを実行します(`tests/test_framework.h` 互換層により試験ロジックは単一ソース)。

## CI(GitHub Actions)

| ワークフロー | 内容 |
|------------|------|
| `docs-check.yml` | 必須ディレクトリ・ファイルの存在 / Markdown lint / 内部リンク切れ / 日付書式(ISO 8601) |
| `build-test.yml` | CMake ビルド + **CppUTest と GoogleTest 両方** によるユニット・結合・システム試験(`TH25S_TEST_FRAMEWORK=both`) |

## 略語(本リポジトリで使われる略称の辞書)

本プロジェクトの文書全体で頻出する略称を一覧する。**正本はここ(README §略語)** とし、監査用の同一表を [`compliance/audit_checklist.md §10`](./compliance/audit_checklist.md) にもミラーで保持する。

### ドキュメント成果物(IEC 62304 箇条 5〜9 の出力)

| 略語 | 英語フル | 日本語 | 所在 |
|------|---------|-------|------|
| SDP | Software Development Plan | ソフトウェア開発計画書 | [`5.1_.../software_development_plan.md`](./5.1_software_development_planning/software_development_plan.md) |
| SRS | Software Requirements Specification | ソフトウェア要求仕様書 | [`5.2_.../software_requirements_specification.md`](./5.2_software_requirements_analysis/software_requirements_specification.md) |
| SAD | Software Architecture Description | ソフトウェアアーキテクチャ設計書 | [`5.3_.../software_architecture_design.md`](./5.3_software_architecture_design/software_architecture_design.md) |
| SDD | Software Design Description | ソフトウェア詳細設計書 | [`5.4_.../software_detailed_design.md`](./5.4_software_detailed_design/software_detailed_design.md) |
| UTPR | Unit Test Plan and Report | ユニットテスト計画書/報告書 | [`5.5_.../software_unit_test_plan_report.md`](./5.5_software_unit_implementation/software_unit_test_plan_report.md) |
| ITPR | Integration Test Plan and Report | 結合試験計画書/報告書 | [`5.6_.../software_integration_test_plan_report.md`](./5.6_software_integration_testing/software_integration_test_plan_report.md) |
| STPR | System Test Plan and Report | システム試験計画書/報告書 | [`5.7_.../software_system_test_plan_report.md`](./5.7_software_system_testing/software_system_test_plan_report.md) |
| SMS | Software Master Specification | ソフトウェアマスタ仕様書(リリース記録) | [`5.8_.../software_master_specification.md`](./5.8_software_release/software_master_specification.md) |
| SMP | Software Maintenance Plan | ソフトウェア保守計画書 | [`6_.../software_maintenance_plan.md`](./6_software_maintenance_process/software_maintenance_plan.md) |
| SRMP | Software Risk Management Plan | ソフトウェアリスクマネジメント計画書 | [`7_.../software_risk_management_plan.md`](./7_software_risk_management_process/software_risk_management_plan.md) |
| SSC | Software Safety Class Determination Record | ソフトウェア安全クラス決定記録 | [`7_.../software_safety_class_determination_record.md`](./7_software_risk_management_process/software_safety_class_determination_record.md) |
| RMF | Risk Management File | リスクマネジメントファイル(ISO 14971) | [`7_.../risk_management_file.md`](./7_software_risk_management_process/risk_management_file.md) |
| SCMP | Software Configuration Management Plan | ソフトウェア構成管理計画書 | [`8_.../software_configuration_management_plan.md`](./8_software_configuration_management_process/software_configuration_management_plan.md) |
| CIL | Configuration Item List | 構成アイテム一覧 | [`8_.../configuration_item_list.md`](./8_software_configuration_management_process/configuration_item_list.md) |
| CCB | Change Control Board Operating Rules | 変更管理委員会(CCB)運用規程 | [`8_.../ccb_operating_rules.md`](./8_software_configuration_management_process/ccb_operating_rules.md) |
| CRR | Change Request Register | 変更要求台帳 | [`8_.../change_request_register.md`](./8_software_configuration_management_process/change_request_register.md) |
| SPRP | Software Problem Resolution Procedure | ソフトウェア問題解決手順書 | [`9_.../software_problem_resolution_procedure.md`](./9_software_problem_resolution_process/software_problem_resolution_procedure.md) |
| ACL | Audit Checklist(IEC 62304 適合性) | IEC 62304 監査チェックリスト | [`compliance/audit_checklist.md`](./compliance/audit_checklist.md) |
| QAR | Q&A Record(学習プロジェクト固有、CR-0004 で追加) | IEC 62304 Q&A 記録(学習過程の質問と回答の蓄積) | [`IEC62304_QA.md`](./IEC62304_QA.md) |
| HZA | Hazard Analysis(学習プロジェクト固有、CR-0008 で追加) | Therac-25 ハザード解析・事故メカニズム解説(図解付き教材) | [`THERAC25_HAZARD_ANALYSIS.md`](./THERAC25_HAZARD_ANALYSIS.md) |
| UTM | Unified Traceability Matrix(学習プロジェクト固有、CR-0009 で追加) | 統一トレーサビリティマトリクス(各文書の §トレース列を集約した参考ビュー。正本は各文書) | [`TRACEABILITY_MATRIX.md`](./TRACEABILITY_MATRIX.md) |

### トレーサビリティ用の ID プレフィックス

| プレフィックス | 意味 | 例 |
|--------------|------|-----|
| `SRS-` | ソフトウェア要求事項 | SRS-001 |
| `ARCH-` | アーキテクチャ要素(ソフトウェア項目) | ARCH-001 |
| `SDD-` | 詳細設計項目 | SDD-001 |
| `UNIT-` | ソフトウェアユニット | UNIT-001 |
| `IF-U-` | ソフトウェア項目間インタフェース | IF-U-001 |
| `IF-E-` | ソフトウェア外部インタフェース | IF-E-001 |
| `UT-` | ユニット試験 | UT-001-01 |
| `IT-` | 結合試験 | IT-001 |
| `ST-` | システム試験 | ST-001 |
| `HZ-` | ハザード(Hazard) | HZ-001 |
| `EV-` | 事象シーケンス(Event Sequence、ISO 14971 §5.4) | EV-001 |
| `RCM-` | リスクコントロール手段(Risk Control Measure) | RCM-001 |
| `SOUP-` | 素性不明のソフトウェア(Software of Unknown Provenance) | SOUP-001 |
| `SEP-` | アーキテクチャ分離(Separation、5.3.5 関連) | SEP-001 |
| `CI-` | 構成アイテム(Configuration Item) | CI-SRC-001 |
| `PRB-` | 問題報告(Problem Report、`PR-` は使わない) | PRB-0001 |
| `CR-` | 変更要求(Change Request) | CR-0001 |
| `ANOM-` | 残留異常(Anomaly) | ANOM-001 |
| `BL-` | ベースライン(Baseline) | BL-20260515-001 |
| `QA-` | Q&A 記録エントリ(学習プロジェクト固有、CR-0004 で追加) | QA-001 |

### その他の用語・略語

| 略語 | 英語フル | 日本語 |
|------|---------|-------|
| SOUP | Software of Unknown Provenance | 素性不明のソフトウェア(供給元・開発履歴が不明なソフト) |
| HZ | Hazard | ハザード(危害の潜在的源泉) |
| RCM | Risk Control Measure | リスクコントロール手段 |
| SEP | Separation | アーキテクチャ分離(クラス C の 5.3.5) |
| CI | Configuration Item | 構成アイテム |
| CCB | Change Control Board | 変更管理委員会(運用規程の CCB-とは別概念) |
| CR | Change Request | 変更要求 |
| PRB | Problem Report | 問題報告(`PR-` は GitHub PR と衝突するため `PRB-` を使う) |
| MC/DC | Modified Condition/Decision Coverage | 改良条件分岐網羅 |
| IEC | International Electrotechnical Commission | 国際電気標準会議 |
| ISO | International Organization for Standardization | 国際標準化機構 |
| JIS | Japanese Industrial Standards | 日本産業規格(JIS T 2304 = IEC 62304 の日本版) |
| TH25S | (本プロジェクトの製品コード) | 仮想 Therac-25 Simple |

## 関連規格

| 規格 | 用途 |
|------|------|
| IEC 62304:2006+A1:2015 / JIS T 2304:2017 | 本プロジェクトの直接の根拠 |
| ISO 14971:2019 | リスクマネジメント(箇条 7・RMF で参照) |

## 免責事項

本プロジェクトは **学習目的の参考実装** であり、実際の規制適合を保証するものではありません。実運用前には規格原本との照合と、QMS・RA 責任者のレビューが必要です。

## ライセンス

{{ライセンス未設定 — プロジェクトに適したライセンスを LICENSE ファイルとして追加してください}}
