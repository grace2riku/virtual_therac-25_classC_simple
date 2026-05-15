# 構成アイテム一覧(CI List)

**ドキュメント ID:** CIL-TH25S-001
**バージョン:** 1.1
**最終更新日:** 2026-05-15
**対象製品:** 仮想 Therac-25 Simple / TH25S-SIM-001
**対象リリース:** 1.0.0

| 役割 | 氏名 | 所属 | 日付 | 署名 |
|------|------|------|------|------|
| 管理者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-15 | — |
| 承認者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-15 | — |

---

## 1. 目的と適用範囲

本書は、IEC 62304 箇条 5.1.10 および箇条 8.1 に基づき、構成管理対象となるすべての構成アイテム(CI)を識別・記録する。本書はベースラインごとにスナップショットを取り、アーカイブ(SMS §9)の一部として保管する。

## 2. 記録ルール

- 各 CI には一意の `CI ID` を付与する。
- バージョンは Git タグ(`v1.0.0`)またはコミットハッシュで識別する。SOUP はベンダ提供バージョン。
- 本一覧は変更のたびに更新し、改訂履歴にベースラインとの関連を記録する。

## 3. ソースコード

| CI ID | 名称 | パス | 現行バージョン | 安全クラス |
|-------|------|------|-------------|----------|
| CI-SRC-001 | TH25S-CORE 安全コアライブラリ(UNIT-001〜003) | `src/th25s_core/` | `v1.0.0` | C |
| CI-SRC-002 | ルートビルド構成 | `CMakeLists.txt` | `v1.0.0` | C |
| CI-SRC-003 | th25s_core ビルド構成 | `src/th25s_core/CMakeLists.txt` | `v1.0.0` | C |

## 4. ドキュメント

| CI ID | ドキュメント | パス | 現行バージョン | 状態 |
|-------|-----------|------|-------------|------|
| CI-DOC-SDP | ソフトウェア開発計画書 | `5.1_software_development_planning/` | 1.1 | 承認済(CR-0001 で 1.1 へ昇格) |
| CI-DOC-SRS | ソフトウェア要求仕様書 | `5.2_software_requirements_analysis/` | 1.0 | 承認済 |
| CI-DOC-SAD | ソフトウェアアーキテクチャ設計書 | `5.3_software_architecture_design/` | 1.1 | 承認済(CR-0001 で 1.1 へ昇格) |
| CI-DOC-SDD | ソフトウェア詳細設計書 | `5.4_software_detailed_design/` | 1.0 | 承認済 |
| CI-DOC-UTPR | ユニットテスト計画書/報告書 | `5.5_software_unit_implementation/` | 1.1 | 承認済(CR-0001 で 1.1 へ昇格) |
| CI-DOC-ITPR | 結合試験計画書/報告書 | `5.6_software_integration_testing/` | 1.0 | 承認済 |
| CI-DOC-STPR | システム試験計画書/報告書 | `5.7_software_system_testing/` | 1.0 | 承認済 |
| CI-DOC-SMS | ソフトウェアマスタ仕様書 | `5.8_software_release/` | 1.1 | 承認済(CR-0001 で 1.1 へ昇格) |
| CI-DOC-SMP | ソフトウェア保守計画書 | `6_software_maintenance_process/` | 1.0 | 承認済 |
| CI-DOC-SRMP | ソフトウェアリスクマネジメント計画書 | `7_software_risk_management_process/` | 1.1 | 承認済(CR-0001 で 1.1 へ昇格) |
| CI-DOC-SSC | ソフトウェア安全クラス決定記録 | `7_software_risk_management_process/` | 1.0 | 承認済 |
| CI-DOC-RMF | リスクマネジメントファイル(ISO 14971) | `7_software_risk_management_process/` | 1.0 | 承認済 |
| CI-DOC-SCMP | ソフトウェア構成管理計画書 | `8_software_configuration_management_process/` | 1.0 | 承認済 |
| CI-DOC-CCB | CCB 運用規程 | `8_software_configuration_management_process/` | 1.0 | 承認済 |
| CI-DOC-CRR | 変更要求台帳 | `8_software_configuration_management_process/` | 1.1 | 承認済(CR-0001 で 1.1 へ昇格) |
| CI-DOC-CIL | 構成アイテム一覧(本書、自己参照) | `8_software_configuration_management_process/` | 1.1 | 承認済(CR-0001 で 1.1 へ昇格) |
| CI-DOC-SPRP | ソフトウェア問題解決手順書 | `9_software_problem_resolution_process/` | 1.0 | 承認済 |
| CI-DOC-ACL | IEC 62304 監査チェックリスト | `compliance/` | 1.1 | 承認済(CR-0001 で 1.1 へ昇格) |
| CI-DOC-CLAUDE | プロジェクト指示(CLAUDE.md) | `CLAUDE.md` | 1.0 | 承認済 |
| CI-DOC-README | プロジェクト概要(README.md) | `README.md` | 1.0 | 承認済 |
| CI-DOC-STEPS | 開発ステップ記録 / 日割りガイド | `DEVELOPMENT_STEPS.md` | 1.0 | 承認済 |

## 5. SOUP(Software of Unknown Provenance)

| CI ID | 名称 | 供給元 | バージョン | ライセンス | 入手日 | 備考 |
|-------|------|-------|----------|----------|-------|------|
| CI-SOUP-001 | CppUTest | パッケージマネージャ(Homebrew / apt) | 4.0 | BSD-3-Clause | 2026-05-13 | 試験コードのみにリンク。製品成果物 `libth25s_core.a` には非リンク |
| CI-SOUP-002 | GoogleTest | パッケージマネージャ(Homebrew / apt) | 1.17.0(Homebrew) / Ubuntu apt の libgtest-dev 提供版 | BSD-3-Clause | 2026-05-15(CR-0001 で追加) | 試験コードのみにリンク。製品成果物には非リンク。CppUTest と相互排他で選択する(`TH25S_TEST_FRAMEWORK=gtest` または `both`) |

## 6. 開発・検証ツール

| CI ID | 種別 | ツール名 | バージョン | 役割 |
|-------|------|--------|----------|------|
| CI-TOOL-001 | コンパイラ | GCC または Clang | C11 / C++17 対応版 | ビルド |
| CI-TOOL-002 | ビルドシステム | CMake | 3.20 以上 | ビルド構成 |
| CI-TOOL-003 | 試験フレームワーク(主) | CppUTest | 4.0 | ユニット・結合・システム試験(`TH25S_TEST_FRAMEWORK=cpputest` 既定) |
| CI-TOOL-003b | 試験フレームワーク(副) | GoogleTest | 1.17.0 | 同上 (`TH25S_TEST_FRAMEWORK=gtest` または `both` で選択、CR-0001 で追加) |
| CI-TOOL-004 | バージョン管理 | Git | 2.x | 構成管理 |
| CI-TOOL-005 | CI/CD | GitHub Actions | — | 自動ビルド・試験・文書検証 |
| CI-TOOL-006 | パッケージ検出 | pkg-config | — | CppUTest の検出・リンク |

## 7. 試験データ・試験資産

| CI ID | 名称 | パス | 現行バージョン |
|-------|------|------|-------------|
| CI-TD-001 | ユニット試験コード(UNIT-001) | `tests/test_common_types.cpp` | `v1.0.0` |
| CI-TD-002 | ユニット試験コード(UNIT-002) | `tests/test_treatment_sequencer.cpp` | `v1.0.0` |
| CI-TD-003 | ユニット試験コード(UNIT-003) | `tests/test_safety_interlock.cpp` | `v1.0.0` |
| CI-TD-004 | テストランナー・試験ビルド構成 | `tests/test_main.cpp`, `tests/CMakeLists.txt` | `v1.0.0`(CR-0001 で両フレームワーク対応に更新) |
| CI-TD-005 | 試験フレームワーク互換マクロヘッダ | `tests/test_framework.h` | `v1.0.0`(CR-0001 で新規追加) |

## 8. 成果バイナリ・配布物

| CI ID | 名称 | 形式 | 備考 |
|-------|------|------|------|
| CI-BIN-001 | TH25S-CORE 製品ライブラリ | `libth25s_core.a`(静的ライブラリ) | ビルド成果物。タグ `v1.0.0` から再現可能 |

## 9. ベースライン履歴

| ベースライン ID | 日付 | 目的 | 承認者 | 関連 CR |
|--------------|------|------|-------|---------|
| BL-20260510-001 | 2026-05-10 | 要求凍結(SRS 承認) | 開発者A | — |
| BL-20260512-002 | 2026-05-12 | 設計凍結(SAD / SDD 承認) | 開発者A | — |
| BL-20260515-003 | 2026-05-15 | リリース `v1.0.0` | 開発者A | — |
| BL-20260515-004 | 2026-05-15 | 試験ツリー拡張(GoogleTest 追加) | 開発者A | CR-0001 |

## 10. 改訂履歴

| バージョン | 日付 | 変更内容 | 変更者 |
|----------|------|---------|--------|
| 1.0 | 2026-05-15 | 初版作成。ソースコード・全文書・SOUP・ツール・試験資産・成果バイナリ・ベースライン履歴を登録。 | 開発者A |
| 1.1 | 2026-05-15 | CR-0001 反映: §5 に CI-SOUP-002 GoogleTest を追加、§6 に試験フレームワーク副を追加、§7 に CI-TD-005 互換マクロヘッダを追加、§9 にベースライン BL-20260515-004 を追加、§10 自己参照を 1.1 へ昇格。 | 開発者A |

## 付録 A: CIL 更新時チェックリスト

CIL を更新する際は、以下を全走査すること。

- [ ] 今回の CR で直接変更する CI の現行バージョン・状態を更新した
- [ ] 自己参照(CI-DOC-CIL)の現行バージョンと状態を本 CIL 自身の昇格後バージョンに更新した
- [ ] §3〜§8 を全走査し、未反映の CI が無いか確認した
- [ ] ベースライン履歴(§9)への追加要否を判定し、必要なら追記した
- [ ] 改訂履歴(§10)に今回の昇格エントリを追加した
- [ ] 関連する CRR のエントリと CI バージョンが整合していることを確認した
