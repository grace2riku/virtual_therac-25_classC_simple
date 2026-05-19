# IEC 62304 監査チェックリスト(クラス C)

**ドキュメント ID:** ACL-TH25S-001
**バージョン:** 1.7
**最終更新日:** 2026-05-20
**対象規格:** IEC 62304:2006+A1:2015 / JIS T 2304:2017
**対象製品:** 仮想 Therac-25 Simple / TH25S-SIM-001
**対象リリース:** 1.0.0

| 役割 | 氏名 | 所属 | 日付 | 署名 |
|------|------|------|------|------|
| 作成者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-20 | — |
| レビュー者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-20 | — |
| 承認者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-20 | — |

---

## 1. 目的と使い方

本書は IEC 62304 箇条 5〜9 の要求事項を 1 行ずつ列挙し、本リポジトリのどの成果物で対応しているかを対応表として示す。

### 記号
- ✅ 対応済み(根拠文書が存在)
- ⚠️ 部分対応 / N/A 適用外

### 安全クラス適用表示
- **[A]** クラス A 以上で要求 / **[B,C]** クラス B 及び C で要求 / **[C]** クラス C のみで要求

## 2. 箇条 4 一般要求事項

| 条項 | 要求 | 対応文書 | 状態 | 備考 |
|------|------|--------|------|------|
| 4.1 | 品質マネジメントシステム | — | ⚠️ | 学習プロジェクトのため外部 QMS なし。SDP §1 で学習目的を明示 |
| 4.2 | リスクマネジメント(ISO 14971) | `7_.../risk_management_file.md` | ✅ | RMF-TH25S-001 |
| 4.3 | ソフトウェア安全クラス分類 | `7_.../software_safety_class_determination_record.md` | ✅ | クラス C 決定 |
| 4.4 | 遺留ソフトウェア(Legacy Software) | — | N/A | 新規開発 |

## 3. 箇条 5 ソフトウェア開発プロセス

> **対応文書列の表記(CR-0006 反映):** 章番号(`§N`)に加えて当該章のタイトルを併記し、リンクをクリックすると対応文書の該当章にジャンプできる(GitHub 上の Markdown レンダリング前提)。`§番号` を持たない補助参照(SRMP / SCMP / CIL / RMF / SPRP の単独参照、`src/th25s_core/` 等)はリンク化していない。

### 5.1 ソフトウェア開発計画

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 5.1.1 | ソフトウェア開発計画 | [A] | [SDP §3 ソフトウェア開発プロセスの概要(箇条 5.1.1)](../5.1_software_development_planning/software_development_plan.md#3-ソフトウェア開発プロセスの概要箇条-511) | ✅ |
| 5.1.2 | 計画の維持 | [A] | [SDP §4 計画の維持(箇条 5.1.2)](../5.1_software_development_planning/software_development_plan.md#4-計画の維持箇条-512) | ✅ |
| 5.1.3 | システム設計との関連 | [A] | [SDP §5 システム設計及び開発との関連(箇条 5.1.3)](../5.1_software_development_planning/software_development_plan.md#5-システム設計及び開発との関連箇条-513) | ✅ |
| 5.1.4 | 開発標準・方法・ツール | [B,C] | [SDP §6 ソフトウェア開発標準・方法・ツール計画(箇条 5.1.4 ― クラス B, C)](../5.1_software_development_planning/software_development_plan.md#6-ソフトウェア開発標準方法ツール計画箇条-514--クラス-b-c) | ✅ |
| 5.1.5 | 統合及び統合試験の計画 | [A] | [SDP §7 ソフトウェア統合及び統合試験の計画(箇条 5.1.5)](../5.1_software_development_planning/software_development_plan.md#7-ソフトウェア統合及び統合試験の計画箇条-515) | ✅ |
| 5.1.6 | 検証の計画 | [A] | [SDP §8 ソフトウェア検証の計画(箇条 5.1.6)](../5.1_software_development_planning/software_development_plan.md#8-ソフトウェア検証の計画箇条-516) | ✅ |
| 5.1.7 | リスクマネジメント計画 | [A] | [SDP §9 ソフトウェアリスクマネジメント計画(箇条 5.1.7)](../5.1_software_development_planning/software_development_plan.md#9-ソフトウェアリスクマネジメント計画箇条-517) / SRMP | ✅ |
| 5.1.8 | ドキュメント作成計画 | [A] | [SDP §10 ドキュメント作成計画(箇条 5.1.8)](../5.1_software_development_planning/software_development_plan.md#10-ドキュメント作成計画箇条-518) | ✅ |
| 5.1.9 | 構成管理計画 | [A] | [SDP §11 ソフトウェア構成管理計画(箇条 5.1.9)](../5.1_software_development_planning/software_development_plan.md#11-ソフトウェア構成管理計画箇条-519) / SCMP | ✅ |
| 5.1.10 | 管理対象となる支援項目 | [A] | [SDP §12 管理対象となる支援項目(箇条 5.1.10)](../5.1_software_development_planning/software_development_plan.md#12-管理対象となる支援項目箇条-5110) / CIL | ✅ |
| 5.1.11 | 検証前の構成アイテム管理 | [A] | [SDP §13 検証前の構成アイテムの制御(箇条 5.1.11)](../5.1_software_development_planning/software_development_plan.md#13-検証前の構成アイテムの制御箇条-5111) / SCMP | ✅ |
| 5.1.12 | 共通ソフトウェア欠陥の識別と回避 | [B,C] | [SDP §14 共通ソフトウェア欠陥の識別と回避(箇条 5.1.12 ― クラス B, C)](../5.1_software_development_planning/software_development_plan.md#14-共通ソフトウェア欠陥の識別と回避箇条-5112--クラス-b-c) | ✅ |

### 5.2 ソフトウェア要求事項分析

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 5.2.1 | システム要求からソフトウェア要求への展開 | [A] | [SRS §3 システム要求事項からソフトウェア要求事項への展開(箇条 5.2.1)](../5.2_software_requirements_analysis/software_requirements_specification.md#3-システム要求事項からソフトウェア要求事項への展開箇条-521) | ✅ |
| 5.2.2 | ソフトウェア要求事項の内容(a)〜(l) | [A] | [SRS §4 ソフトウェア要求事項(箇条 5.2.2)](../5.2_software_requirements_analysis/software_requirements_specification.md#4-ソフトウェア要求事項箇条-522)(§4.1〜§4.12) | ✅ |
| 5.2.3 | リスクコントロール手段の SRS への組込み | [A] | [SRS §5 リスクコントロールに関連する要求事項(箇条 5.2.3)](../5.2_software_requirements_analysis/software_requirements_specification.md#5-リスクコントロールに関連する要求事項箇条-523) | ✅ |
| 5.2.4 | 医療機器リスク分析の再評価 | [A] | [SRS §6 医療機器リスク分析の再評価(箇条 5.2.4)](../5.2_software_requirements_analysis/software_requirements_specification.md#6-医療機器リスク分析の再評価箇条-524) / RMF | ✅ |
| 5.2.5 | システム要求事項の更新 | [A] | [SRS §7 ソフトウェア要求事項の更新(箇条 5.2.5)](../5.2_software_requirements_analysis/software_requirements_specification.md#7-ソフトウェア要求事項の更新箇条-525) | ✅ |
| 5.2.6 | 要求事項の検証 | [A] | [SRS §8 要求事項の検証(箇条 5.2.6)](../5.2_software_requirements_analysis/software_requirements_specification.md#8-要求事項の検証箇条-526), [§9 トレーサビリティマトリクス(箇条 5.2.6)](../5.2_software_requirements_analysis/software_requirements_specification.md#9-トレーサビリティマトリクス箇条-526) | ✅ |

### 5.3 ソフトウェアアーキテクチャの設計

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 5.3.1 | 要求事項からアーキテクチャへの変換 | [A] | [SAD §3 ソフトウェア要求事項のソフトウェアアーキテクチャへの変換(箇条 5.3.1)](../5.3_software_architecture_design/software_architecture_design.md#3-ソフトウェア要求事項のソフトウェアアーキテクチャへの変換箇条-531) | ✅ |
| 5.3.2 | ソフトウェア項目間のインタフェース | [A] | [SAD §5 ソフトウェア項目間のインタフェース(箇条 5.3.2)](../5.3_software_architecture_design/software_architecture_design.md#5-ソフトウェア項目間のインタフェース箇条-532) | ✅ |
| 5.3.3 | SOUP の機能的・性能的要求事項の指定 | [B,C] | [SAD §7 SOUP の機能的及び性能的要求事項の指定(箇条 5.3.3 ― クラス B, C)](../5.3_software_architecture_design/software_architecture_design.md#7-soup-の機能的及び性能的要求事項の指定箇条-533--クラス-b-c) | ✅ |
| 5.3.4 | SOUP に必要なシステム HW/SW の指定 | [B,C] | [SAD §8 SOUP に必要なシステム上のハードウェア及びソフトウェアの指定(箇条 5.3.4 ― クラス B, C)](../5.3_software_architecture_design/software_architecture_design.md#8-soup-に必要なシステム上のハードウェア及びソフトウェアの指定箇条-534--クラス-b-c) | ✅ |
| 5.3.5 | リスクコントロールのためのソフトウェア項目の分離 | [C] | [SAD §9 リスクコントロール手段のためのソフトウェア項目の分離(箇条 5.3.5 ― クラス C)](../5.3_software_architecture_design/software_architecture_design.md#9-リスクコントロール手段のためのソフトウェア項目の分離箇条-535--クラス-c)(SEP-001、論理分離。物理分離不採用を明記) | ✅ |
| 5.3.6 | アーキテクチャの検証 | [A] | [SAD §10 ソフトウェアアーキテクチャの検証(箇条 5.3.6)](../5.3_software_architecture_design/software_architecture_design.md#10-ソフトウェアアーキテクチャの検証箇条-536) | ✅ |

### 5.4 ソフトウェア詳細設計

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 5.4.1 | ソフトウェアユニットへの改良 | [B,C] | [SDD §3 ソフトウェア項目のソフトウェアユニットへの改良(箇条 5.4.1)](../5.4_software_detailed_design/software_detailed_design.md#3-ソフトウェア項目のソフトウェアユニットへの改良箇条-541) | ✅ |
| 5.4.2 | 各ソフトウェアユニットの詳細設計 | [C] | [SDD §4 ソフトウェアユニットの詳細設計(箇条 5.4.2 ― クラス C)](../5.4_software_detailed_design/software_detailed_design.md#4-ソフトウェアユニットの詳細設計箇条-542--クラス-c) | ✅ |
| 5.4.3 | インタフェースの詳細設計 | [C] | [SDD §5 インタフェースの詳細設計(箇条 5.4.3 ― クラス C)](../5.4_software_detailed_design/software_detailed_design.md#5-インタフェースの詳細設計箇条-543--クラス-c) | ✅ |
| 5.4.4 | 詳細設計の検証 | [C] | [SDD §6 詳細設計の検証(箇条 5.4.4 ― クラス C)](../5.4_software_detailed_design/software_detailed_design.md#6-詳細設計の検証箇条-544--クラス-c) | ✅ |

### 5.5 ソフトウェアユニットの実装

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 5.5.1 | ソフトウェアユニットの実装 | [A] | [UTPR §3 ソフトウェアユニットの実装(箇条 5.5.1)](../5.5_software_unit_implementation/software_unit_test_plan_report.md#3-ソフトウェアユニットの実装箇条-551) / `src/th25s_core/` | ✅ |
| 5.5.2 | ユニット検証プロセスの確立 | [B,C] | [UTPR §4 ソフトウェアユニット検証プロセスの確立(箇条 5.5.2)](../5.5_software_unit_implementation/software_unit_test_plan_report.md#4-ソフトウェアユニット検証プロセスの確立箇条-552) | ✅ |
| 5.5.3 | ユニット受入基準 | [B,C] | [UTPR §5 ソフトウェアユニット受入基準(箇条 5.5.3)](../5.5_software_unit_implementation/software_unit_test_plan_report.md#5-ソフトウェアユニット受入基準箇条-553) | ✅ |
| 5.5.4 | 追加のユニット受入基準 | [C] | [UTPR §6 追加のユニット受入基準(箇条 5.5.4 ― クラス C)](../5.5_software_unit_implementation/software_unit_test_plan_report.md#6-追加のユニット受入基準箇条-554--クラス-c) | ✅ |
| 5.5.5 | ユニット検証 | [B,C] | [UTPR §7 ソフトウェアユニット試験(箇条 5.5.5)](../5.5_software_unit_implementation/software_unit_test_plan_report.md#7-ソフトウェアユニット試験箇条-555), [§8 試験実施結果](../5.5_software_unit_implementation/software_unit_test_plan_report.md#8-試験実施結果) | ✅ |

### 5.6 ソフトウェア結合及び結合試験

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 5.6.1 | ソフトウェアユニットの結合 | [B,C] | [ITPR §3 ソフトウェアユニットの結合(箇条 5.6.1)](../5.6_software_integration_testing/software_integration_test_plan_report.md#3-ソフトウェアユニットの結合箇条-561) | ✅ |
| 5.6.2 | ソフトウェア結合の検証 | [B,C] | [ITPR §4 ソフトウェア結合の検証(箇条 5.6.2)](../5.6_software_integration_testing/software_integration_test_plan_report.md#4-ソフトウェア結合の検証箇条-562) | ✅ |
| 5.6.3 | ソフトウェア結合試験 | [B,C] | [ITPR §5 ソフトウェア結合試験(箇条 5.6.3)](../5.6_software_integration_testing/software_integration_test_plan_report.md#5-ソフトウェア結合試験箇条-563) | ✅ |
| 5.6.4 | 結合試験の内容 | [B,C] | [ITPR §6 結合試験の内容(箇条 5.6.4)](../5.6_software_integration_testing/software_integration_test_plan_report.md#6-結合試験の内容箇条-564) | ✅ |
| 5.6.5 | 試験手順の評価 | [B,C] | [ITPR §7 結合試験手順の評価(箇条 5.6.5)](../5.6_software_integration_testing/software_integration_test_plan_report.md#7-結合試験手順の評価箇条-565) | ✅ |
| 5.6.6 | 回帰試験の実施 | [B,C] | [ITPR §8 回帰試験の実施(箇条 5.6.6)](../5.6_software_integration_testing/software_integration_test_plan_report.md#8-回帰試験の実施箇条-566) | ✅ |
| 5.6.7 | 結合試験記録の内容 | [B,C] | [ITPR §9 結合試験記録の内容(箇条 5.6.7)](../5.6_software_integration_testing/software_integration_test_plan_report.md#9-結合試験記録の内容箇条-567), [§11 試験実施結果](../5.6_software_integration_testing/software_integration_test_plan_report.md#11-試験実施結果) | ✅ |
| 5.6.8 | ソフトウェア問題解決プロセスの使用 | [B,C] | [ITPR §10 ソフトウェア問題解決プロセスの使用(箇条 5.6.8)](../5.6_software_integration_testing/software_integration_test_plan_report.md#10-ソフトウェア問題解決プロセスの使用箇条-568) / SPRP | ✅ |

### 5.7 ソフトウェアシステム試験

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 5.7.1 | 要求事項検証のための試験の確立 | [B,C] | [STPR §3 ソフトウェア要求事項を検証する試験の設定(箇条 5.7.1)](../5.7_software_system_testing/software_system_test_plan_report.md#3-ソフトウェア要求事項を検証する試験の設定箇条-571) | ✅ |
| 5.7.2 | ソフトウェア問題解決プロセスの使用 | [B,C] | [STPR §5 ソフトウェア問題解決プロセスの使用(箇条 5.7.2)](../5.7_software_system_testing/software_system_test_plan_report.md#5-ソフトウェア問題解決プロセスの使用箇条-572) / SPRP | ✅ |
| 5.7.3 | 変更後の再試験 | [B,C] | [STPR §6 変更後の再試験(箇条 5.7.3)](../5.7_software_system_testing/software_system_test_plan_report.md#6-変更後の再試験箇条-573) | ✅ |
| 5.7.4 | システム試験の妥当性確認 | [C] | [STPR §7 ソフトウェアシステム試験の妥当性確認(箇条 5.7.4 ― クラス C)](../5.7_software_system_testing/software_system_test_plan_report.md#7-ソフトウェアシステム試験の妥当性確認箇条-574--クラス-c) | ✅ |
| 5.7.5 | 試験記録の内容 | [B,C] | [STPR §9 システム試験記録の内容(箇条 5.7.5)](../5.7_software_system_testing/software_system_test_plan_report.md#9-システム試験記録の内容箇条-575) | ✅ |

### 5.8 ソフトウェアリリース

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 5.8.1 | 検証完了の確保 | [B,C] | [SMS §3 ソフトウェア検証完了の確認(箇条 5.8.1)](../5.8_software_release/software_master_specification.md#3-ソフトウェア検証完了の確認箇条-581) | ✅ |
| 5.8.2 | 既知の残留異常の文書化 | [B,C] | [SMS §4 既知の残留異常のドキュメント化(箇条 5.8.2)](../5.8_software_release/software_master_specification.md#4-既知の残留異常のドキュメント化箇条-582) | ✅ |
| 5.8.3 | 既知の残留異常の評価 | [B,C] | [SMS §5 既知の残留異常の評価(箇条 5.8.3)](../5.8_software_release/software_master_specification.md#5-既知の残留異常の評価箇条-583) | ✅ |
| 5.8.4 | リリースバージョンの文書化 | [B,C] | [SMS §6 リリースされたバージョンの文書化(箇条 5.8.4)](../5.8_software_release/software_master_specification.md#6-リリースされたバージョンの文書化箇条-584) | ✅ |
| 5.8.5 | リリース作成方法の文書化 | [B,C] | [SMS §7 リリースソフトウェアの作成方法の文書化(箇条 5.8.5)](../5.8_software_release/software_master_specification.md#7-リリースソフトウェアの作成方法の文書化箇条-585) | ✅ |
| 5.8.6 | 活動及び作業の完了 | [B,C] | [SMS §8 活動および作業の完了(箇条 5.8.6)](../5.8_software_release/software_master_specification.md#8-活動および作業の完了箇条-586) | ✅ |
| 5.8.7 | ソフトウェアのアーカイブ | [B,C] | [SMS §9 ソフトウェアのアーカイブ(箇条 5.8.7)](../5.8_software_release/software_master_specification.md#9-ソフトウェアのアーカイブ箇条-587) | ✅ |
| 5.8.8 | ソフトウェアリリースの再現可能性の保証 | [B,C] | [SMS §10 ソフトウェアリリースの再現可能性の保証(箇条 5.8.8)](../5.8_software_release/software_master_specification.md#10-ソフトウェアリリースの再現可能性の保証箇条-588) | ✅ |

## 4. 箇条 6 ソフトウェア保守プロセス

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 6.1 | 保守計画の確立 | [A] | SMP §3 | ✅ |
| 6.2.1.1 | フィードバックの監視 | [A] | SMP §4.1 | ✅ |
| 6.2.1.2 | フィードバックの文書化・評価 | [A] | SMP §4.2 | ✅ |
| 6.2.1.3 | 問題報告の安全性影響評価 | [A] | SMP §4.3 | ✅ |
| 6.2.2 | ソフトウェア問題解決プロセスの使用 | [A] | SMP §4.4 / SPRP | ✅ |
| 6.2.3 | 変更要求の分析 | [A] | SMP §4.5 | ✅ |
| 6.2.4 | 変更要求の承認 | [A] | SMP §4.6 / CCB | ✅ |
| 6.2.5 | ユーザ・規制当局への通知 | [A] | SMP §4.7 | ✅ |
| 6.3.1 | 確立されたプロセスによる修正実施 | [A] | SMP §5.1 | ✅ |
| 6.3.2 | 修正済みソフトウェアの再リリース | [A] | SMP §5.2 | ✅ |

## 5. 箇条 7 ソフトウェアリスクマネジメントプロセス

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 7.1.1 | 寄与するソフトウェア項目の識別 | [A] | SRMP §3.1 / RMF | ✅ |
| 7.1.2 | 潜在原因の識別 | [A] | SRMP §3.2 | ✅ |
| 7.1.3 | SOUP 公開異常リストの評価 | [A] | SRMP §3.3 | ✅ |
| 7.1.4 | 潜在原因の文書化 | [A] | SRMP §3.4 | ✅ |
| 7.1.5 | 事象シーケンスの文書化 | [A] | SRMP §3.5 / RMF §4.2 | ✅ |
| 7.2.1 | リスクコントロール手段の定義 | [A] | SRMP §4.1 / RMF §6 | ✅ |
| 7.2.2 | ソフトウェアで実装される RCM | [A] | SRMP §4.2 | ✅ |
| 7.3.1 | RCM の検証 | [A] | SRMP §5.1 / RMF §7 | ✅ |
| 7.3.2 | 新たな事象シーケンスの文書化 | [A] | SRMP §5.2 | ✅ |
| 7.3.3 | トレーサビリティの文書化 | [A] | SRMP §5.3, §7 | ✅ |
| 7.4.1 | 変更がソフトウェアに与える影響の解析 | [A] | SRMP §6.1 | ✅ |
| 7.4.2 | 既存 RCM への影響解析 | [A] | SRMP §6.2 | ✅ |
| 7.4.3 | 解析結果に基づくリスクマネジメント | [A] | SRMP §6.3 | ✅ |

## 6. 箇条 8 ソフトウェア構成管理プロセス

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 8.1.1 | 構成アイテム識別手段の確立 | [A] | SCMP §3.1 / CIL | ✅ |
| 8.1.2 | SOUP の識別 | [A] | SCMP §3.2 / CIL §5 | ✅ |
| 8.1.3 | システム構成ドキュメンテーションの識別 | [A] | SCMP §3.3 | ✅ |
| 8.2.1 | 変更要求の承認 | [A] | SCMP §4.1 / CCB | ✅ |
| 8.2.2 | 変更の実装 | [A] | SCMP §4.2 | ✅ |
| 8.2.3 | 変更の検証 | [A] | SCMP §4.3 | ✅ |
| 8.2.4 | 変更のトレーサビリティ | [A] | SCMP §4.4 / CRR | ✅ |
| 8.3 | 構成状態記録 | [A] | SCMP §5 / CIL §9 | ✅ |

## 7. 箇条 9 ソフトウェア問題解決プロセス

| 条項 | 要求 | 適用 | 対応文書 | 状態 |
|------|------|------|--------|------|
| 9.1 | 問題報告の作成 | [A] | SPRP §3 | ✅ |
| 9.2 | 問題の調査 | [A] | SPRP §4 | ✅ |
| 9.3 | 関係者への通知 | [A] | SPRP §5 | ✅ |
| 9.4 | 変更管理プロセスの使用 | [A] | SPRP §6 / SCMP | ✅ |
| 9.5 | 記録の維持 | [A] | SPRP §7 | ✅ |
| 9.6 | 問題の傾向分析 | [A] | SPRP §8 / CRR §7 | ✅ |
| 9.7 | ソフトウェア問題解決の検証 | [A] | SPRP §9 | ✅ |
| 9.8 | 試験文書の内容 | [A] | SPRP §10 | ✅ |

## 8. 監査所見

### 8.1 指摘事項サマリ
| ID | 条項 | 所見 | 重大度 | 是正期限 | 是正ステータス |
|----|------|------|-------|---------|-------------|
| — | — | 初回リリース `v1.0.0` の自己監査では指摘事項なし。全条項が対応文書を持つ。 | — | — | — |

### 8.2 是正記録
是正を要する所見はない。学習プロジェクト固有の制約(QMS 不在、単独開発の独立性擬制、物理分離不採用)は、各文書(SDP §1、SCMP §4.1.1、SAD §9)で明示済みであり、意図的な学習上の簡略化として記録されている。

## 9. トレーサビリティ

- 本チェックリストは CIL(CI-DOC-ACL)の一部として管理し、リリースごとにスナップショットを取得する。
- 各対応文書の変更(バージョン更新)は、本書の「状態」を再評価する契機とする。

## 10. 略語

> 本表は監査用に自己完結化したもの。同一内容は [`README.md §略語`](../README.md#略語本リポジトリで使われる略称の辞書) に正本として掲載されている(CR-0003 で正本場所を README に統一)。

| 略語 | 英語フル | 日本語 |
|------|---------|-------|
| SDP | Software Development Plan | ソフトウェア開発計画書 |
| SRS | Software Requirements Specification | ソフトウェア要求仕様書 |
| SAD | Software Architecture Description | ソフトウェアアーキテクチャ設計書 |
| SDD | Software Design Description | ソフトウェア詳細設計書 |
| UTPR | Unit Test Plan and Report | ユニットテスト計画書/報告書 |
| ITPR | Integration Test Plan and Report | 結合試験計画書/報告書 |
| STPR | System Test Plan and Report | システム試験計画書/報告書 |
| SMS | Software Master Specification | ソフトウェアマスタ仕様書(リリース記録) |
| SMP | Software Maintenance Plan | ソフトウェア保守計画書 |
| SRMP | Software Risk Management Plan | ソフトウェアリスクマネジメント計画書 |
| SSC | Software Safety Class Determination Record | ソフトウェア安全クラス決定記録 |
| RMF | Risk Management File | リスクマネジメントファイル(ISO 14971) |
| SCMP | Software Configuration Management Plan | ソフトウェア構成管理計画書 |
| CIL | Configuration Item List | 構成アイテム一覧 |
| CRR | Change Request Register | 変更要求台帳 |
| CCB | Change Control Board Operating Rules | 変更管理委員会(CCB)運用規程 |
| SPRP | Software Problem Resolution Procedure | ソフトウェア問題解決手順書 |
| ACL | Audit Checklist(IEC 62304 適合性) | IEC 62304 監査チェックリスト(本書) |

## 11. 改訂履歴

| バージョン | 日付 | 変更内容 | 変更者 |
|----------|------|---------|--------|
| 1.0 | 2026-05-15 | 初版作成。箇条 4〜9 の全条項と対応文書のマッピングを記録。自己監査で指摘事項なし。 | 開発者A |
| 1.1 | 2026-05-15 | CR-0001(GoogleTest 追加)後の再確認。全条項の対応文書バージョン更新(SDP/SAD/UTPR/SMS/SRMP/CIL/CRR を 1.1 へ昇格)を反映。条項の状態(✅)は不変、指摘事項なしを維持。 | 開発者A |
| 1.2 | 2026-05-15 | CR-0002(SOUP-002 GoogleTest 環境別バージョン正確化)後の再確認。条項 5.1.10 / 5.3.3 / 5.3.4 / 7.1.3 / 8.1.2(SOUP 識別関連)が対応文書の正確化により強化された。対応文書バージョン更新(SDP/SAD/UTPR/SMS/SRMP/CIL/CRR を 1.2 へ昇格)を反映。条項の状態(✅)は不変、指摘事項なしを維持。 | 開発者A |
| 1.3 | 2026-05-15 | CR-0003(略語辞書の充実化)反映: §10 略語表に日本語訳・ACL 自身を追加。正本は README §略語、本書は監査用ミラーである旨を冒頭注記に明示。CRR/CIL/STEPS の 1.3 昇格に伴う再確認、条項状態(✅)・指摘事項なしを維持。 | 開発者A |
| 1.4 | 2026-05-15 | CR-0004(Q&A 制度確立 + RMF §4.2 EV-NNN ラベル定義)反映: 条項 5.2.4 / 7.1.5(事象シーケンス文書化)・8.2.4(変更のトレーサビリティ)の対応文書がより正確になった旨を確認。RMF 1.1、CRR/CIL/STEPS の 1.4 昇格に同期。条項状態(✅)・指摘事項なしを維持。新規 CI-DOC-QA(学習プロジェクト固有)は IEC 62304 規定外の補助文書のため適合性評価対象には含めない。 | 開発者A |
| 1.5 | 2026-05-15 | CR-0005(QA-002 + SRS §9 as-released 説明追加)反映: 条項 5.2.6(要求事項の検証 — 上流トレースのみ要求)の理解が SRS §9 説明と DEVELOPMENT_STEPS の新節で明示化された。SRS 1.1、IEC62304_QA 1.1、CRR/CIL/STEPS の 1.5 昇格に同期。条項状態(✅)・指摘事項なしを維持。 | 開発者A |
| 1.6 | 2026-05-20 | CR-0006(MINOR)反映: 利用者からの指摘(GitHub Issue #1)に基づき、§3 箇条 5(5.1〜5.8 計 47 条項)の「対応文書」列を「章番号(`§N`)のみ」から「`§N 章タイトル(箇条 X.Y.Z)`形式 + 該当章へのアンカーリンク」に変更し、監査時の対応文書到達性を改善。§3 冒頭に表記ルールを示す注記を追記。CRR/CIL/STEPS の 1.6 昇格に同期。要求事項本体・条項対応関係・状態(✅)は不変、指摘事項なしを維持。 | 開発者A |
| 1.7 | 2026-05-20 | CR-0007(MINOR)反映: 利用者からの指摘(GitHub Issue #2)に基づき、CCB §4.1 が参照する「GitHub Issues(変更要求テンプレート)」を `.github/ISSUE_TEMPLATE/change-request.yml`(YAML Issue Forms)として新規実装。CCB §4.1 にテンプレート参照リンクと追加収集項目を明示し、CCB を 1.0 → 1.1 へ昇格。条項 8.2.1(変更要求の承認)・8.2.4(変更のトレーサビリティ)・6.2.3(変更要求の分析)に対応するプロセスが起票時点で機械的に補強されるようになった旨を確認。CRR/CIL/STEPS の 1.7 昇格に同期。条項状態(✅)・指摘事項なしを維持。 | 開発者A |
