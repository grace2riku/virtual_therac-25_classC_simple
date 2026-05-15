# 仮想 Therac-25 Simple — プロジェクト指示(Claude 向け)

## 概要

本リポジトリは **IEC 62304:2006+A1:2015「医療機器ソフトウェア ― ソフトウェアライフサイクルプロセス」** の箇条 5(開発)、箇条 6(保守)、箇条 7(リスクマネジメント)、箇条 8(構成管理)、箇条 9(問題解決)に基づく成果物を、歴史的医療機器 **Therac-25** を題材に **1 週間で一巡体験できる規模** に絞って作成した学習プロジェクトである。

姉妹プロジェクト `virtual_therac-25_classC`(3 プロセス・約 21 ユニット・大規模)の **シンプル版** であり、安全コア 1 モジュール・3 ユニットに集中している。

- **対象安全クラス: クラス C**(死亡又は重傷の可能性)
- **実装言語: C11**(ISO/IEC 9899:2011)
- **試験フレームワーク: CppUTest 4.0**(SOUP-001、既定)または **GoogleTest**(SOUP-002、CR-0001 で追加、CR-0002 で環境別バージョン正確化: macOS Homebrew = 1.17.0 / Ubuntu 24.04 apt `libgtest-dev` = 1.14.0)。CMake オプション `-DTH25S_TEST_FRAMEWORK=cpputest|gtest|both` で選択。両フレームワークで同一の試験ケースが走る(`tests/test_framework.h` 互換層)。
- **ファイル形式: Markdown**(Git での差分管理を前提)
- **ベーステンプレート: [grace2riku/iec62304_template](https://github.com/grace2riku/iec62304_template)**

## 安全クラスについて

| クラス | 定義 |
|--------|------|
| A | 傷害又は健康被害はあり得ない |
| B | 重傷には至らない傷害があり得る |
| C | 死亡又は重傷があり得る |

本プロジェクトはクラス C を前提とし、クラス A/B の要求事項もすべて包含する。クラス C のみで追加適用される要求(5.3.5 分離、5.4.2〜5.4.4 詳細設計、5.5.4 追加ユニット受入基準、5.7.4 システム試験妥当性確認)を漏れなく収録する。

## ディレクトリ構造

```
.
├── CLAUDE.md / README.md / DEVELOPMENT_STEPS.md   # プロジェクト指示・概要・日割りガイド
├── CMakeLists.txt                                 # ルートビルド構成
├── src/th25s_core/                                # 安全コアライブラリ (UNIT-001〜003, C11)
│   ├── include/th25s_core/{common_types,treatment_sequencer,safety_interlock}.h
│   └── src/{common_types,treatment_sequencer,safety_interlock}.c
├── tests/                                         # CppUTest ユニット・結合・システム試験
├── 5.1_software_development_planning/              # 5.1 開発計画 (SDP)
├── 5.2_software_requirements_analysis/             # 5.2 要求仕様 (SRS)
├── 5.3_software_architecture_design/               # 5.3 アーキテクチャ設計 (SAD)
├── 5.4_software_detailed_design/                   # 5.4 詳細設計 (SDD)
├── 5.5_software_unit_implementation/               # 5.5 ユニットテスト計画/報告 (UTPR)
├── 5.6_software_integration_testing/               # 5.6 結合試験計画/報告 (ITPR)
├── 5.7_software_system_testing/                    # 5.7 システム試験計画/報告 (STPR)
├── 5.8_software_release/                           # 5.8 リリース (SMS)
├── 6_software_maintenance_process/                 # 6  保守計画 (SMP)
├── 7_software_risk_management_process/             # 7  SRMP / SSC / RMF
├── 8_software_configuration_management_process/    # 8  SCMP / CIL / CCB / CRR
├── 9_software_problem_resolution_process/          # 9  問題解決手順書 (SPRP)
├── compliance/audit_checklist.md                   # IEC 62304 条項別チェックリスト (ACL)
├── .github/workflows/                              # CI (docs-check.yml / build-test.yml)
├── .markdownlint-cli2.yaml / lychee.toml           # 文書 lint / リンクチェック設定
└── .clang-format                                   # C11 コーディングスタイル
```

## ドキュメント間のトレーサビリティ

| 種別 | プレフィックス | 例 |
|------|--------------|-----|
| ソフトウェア要求事項 | `SRS-` | SRS-001 |
| アーキテクチャ要素 | `ARCH-` | ARCH-001 |
| ソフトウェアユニット | `UNIT-` | UNIT-001 |
| ソフトウェア項目間 IF | `IF-U-` | IF-U-001 |
| ソフトウェア外部 IF | `IF-E-` | IF-E-001 |
| ユニット試験 | `UT-` | UT-001-01 |
| 結合試験 | `IT-` | IT-001 |
| システム試験 | `ST-` | ST-001 |
| リスクコントロール手段 | `RCM-` | RCM-001 |
| ハザード | `HZ-` | HZ-001 |
| 事象シーケンス(ISO 14971) | `EV-` | EV-001 |
| SOUP | `SOUP-` | SOUP-001 |
| 分離(アーキテクチャ) | `SEP-` | SEP-001 |
| 構成アイテム | `CI-` | CI-SRC-001 |
| 問題報告 | `PRB-` | PRB-0001 |
| 変更要求 | `CR-` | CR-0001 |
| 残留異常 | `ANOM-` | ANOM-001 |
| ベースライン | `BL-` | BL-20260515-003 |
| Q&A 記録エントリ(学習プロジェクト固有、CR-0004 で追加) | `QA-` | QA-001 |

成果物ドキュメントの ID プレフィックス: `SDP- / SRS- / SAD- / SDD- / UTPR- / ITPR- / STPR- / SMS- / SMP- / SRMP- / SSC- / SCMP- / CIL- / CCB- / CRR- / RMF- / SPRP- / ACL-` および `QAR-`(Q&A 記録、学習プロジェクト固有、CR-0004 で追加)。製品コードは **`TH25S`**(例: SDP-TH25S-001)。

各略語の英語フル・日本語訳・対応文書へのリンクは [README §略語](./README.md#略語本リポジトリで使われる略称の辞書) を正本とする(監査用ミラー: [`compliance/audit_checklist.md §10`](./compliance/audit_checklist.md))。

## 編集時のガイドライン

### 記述スタイル
- 見出しレベルは `#`(H1)をドキュメントタイトル、`##`(H2)を箇条番号に対応させる。
- 表は GitHub Flavored Markdown。**列数はヘッダー・区切り行・全データ行で必ず一致させる**(markdownlint MD056)。
- 未記入項目は削除せず `TBD` と明記する。

### 日付の書式(必須)
- **ISO 8601 拡張表記のみ**: `YYYY-MM-DD`(月単位は `YYYY-MM` を許容)。
- スラッシュ区切り・英語月名・年省略は **使用禁止**。CI(`docs-check.yml`)が自動検出する。

### 改訂履歴・レビュー承認
- 各ドキュメント末尾に「改訂履歴」テーブルを設ける。バージョンは `MAJOR.MINOR`。
- 各ドキュメント冒頭に「作成者 / レビュー者 / 承認者」欄を設ける。

## 本プロジェクト固有のルール

### 題材と学習目的(最重要・判断基準)

題材は実在した医療用リニアアクセラレータ **Therac-25**(AECL、1982 年発売)である。1985〜1987 年に米国・カナダで 6 件の放射線過剰照射事故(6 名重症または死亡)が発生した。本プロジェクトの成果物は、IEC 62304 のプロセスをこの歴史的事故を教材として再構成するものであり、以下を **設計・記述の第一原理** とする。

- **「もし IEC 62304 のプロセスが当時正しく回っていたら、どの成果物・どの活動が事故を防いだか?」** の視点で記述する。
- 事故原因のうち、本シンプル版では以下の 3 つに集中して教材化する。
  - **モードとビーム形成機構(ターンテーブル)の不整合**(HZ-001 / RCM-002)
  - **操作順序依存**(当時の race condition を単一スレッドの順序依存バグとして再現。HZ-002 / RCM-001)
  - **カウンタオーバーフローによる安全チェックのバイパス**(HZ-003 / RCM-003)
  - 横断: **暗号的エラーメッセージ**("MALFUNCTION 54" 型)の排除(RCM-004)
- 本リポジトリは学術・教育目的の **再構成(仮想)** であり、当時の AECL 社内文書ではない。成果物冒頭にその旨を明記する。
- Therac-25 の事故に関する記述は学術文献(Leveson & Turner 1993 ほか)に基づく事実関係のみとし、個人・企業への責任帰属を断定する表現は避ける。

### 言語・実装技術の固定

- 実装言語は **C11** に固定する。理由: 当時の Therac-25 は PDP-11 アセンブリで実装されたが、本プロジェクトは「現代の C で同等機能を IEC 62304 に従って実装する」学習文脈である。
- 試験は **CppUTest**(既定)または **GoogleTest** を用いる(C コードを C++ の試験フレームワークから検証。ヘッダは `extern "C"` ガード付き)。試験ソースは `tests/test_framework.h` 経由で両フレームワーク共通の単一ソースとし、CMake オプションでフレームワークを選択する。
- コーディング規約は MISRA C:2012 を第一参考、CERT C を補助とする。コンパイラ厳格警告を **警告ゼロ** で通すことを必須とする。
- 並行処理は導入しない。Therac-25 の race condition は **単一スレッドの操作順序依存バグ** として状態機械(RCM-001)で教材化する。

### 開発ステップ記録の更新義務(最重要)

**開発ステップが一つ完了するごとに `DEVELOPMENT_STEPS.md` を必ず更新する。** これは IEC 62304 §5.1.8 の延長としたプロジェクト固有の追加要件である(姉妹プロジェクトと同方針)。

- 更新トリガ: 新しいドキュメントの作成・承認、既存ドキュメントの重大改訂、フェーズ遷移。
- 記録項目: Day 番号 / 作業日 / 作業内容 / 成果物(ドキュメント ID・パス) / 採用根拠(IEC 62304 条項と、可能なら Therac-25 事故との対応)。
- 成果物コミットと同一(または直後の)コミットで更新する。

### ID 体系の固有規則

- 問題報告 ID は **`PRB-NNNN`** を使用する(`PR-NNNN` は使わない。GitHub の Pull Request との衝突回避)。
- 製品コードは **`TH25S`** を用いる。

### 単独開発下の独立性擬制

本プロジェクトは単独開発のため、CCB・品質責任者・レビュアは作成者(開発者A)が兼任する。これは Therac-25 事故の主要因「単独開発者 + 独立レビューの不在」を学習目的で意図的に擬制で補うものである。独立性の代替手段:

1. 審議インターバル(重大変更は 24 時間以上。CCB-TH25S-001 §5.4)
2. 自己レビューチェックリスト(Pull Request 上で実施)
3. CI による機械的検証(docs-check: 構造・lint・リンク・日付 / build-test: ビルド・全試験)

## CI(GitHub Actions)

Pull Request・push ごとに以下を自動検証する。

- `docs-check.yml`: (1) 箇条 5.1〜9 の必須ディレクトリ・主要ファイルの存在、(2) Markdown lint、(3) 内部リンク切れ、(4) 日付書式(ISO 8601)。
- `build-test.yml`: CMake によるビルド + CppUTest によるユニット・結合・システム試験(`ctest`)。

ローカル事前確認:
```bash
npx markdownlint-cli2 "**/*.md"
lychee --offline --include-fragments './**/*.md'
cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build && ctest --test-dir build --output-on-failure
```

## AI アシスタントへの指示

本ディレクトリで作業する際は:

1. **規格の箇条番号と章立てを絶対に変更しない**(監査時のトレーサビリティを損なうため)。
2. 要求事項を追加・変更した場合は、関連するトレーサビリティマトリクス(SRS §9、SAD §11、SDD §7、各試験文書)も併せて更新する。
3. **クラス C 固有の要求事項(5.3.5, 5.4.2, 5.4.3, 5.4.4, 5.5.4, 5.7.4)は削除しない。**
4. 差分レビューを容易にするため、既存行を不必要に再フォーマットしない。
5. 日付はすべて **`YYYY-MM-DD` 書式** を使用する。
6. **ドキュメントを新規作成・重大改訂した場合は、必ず `DEVELOPMENT_STEPS.md` を更新する。**
7. 問題報告 ID は `PRB-NNNN` を使用する。
8. 実装言語は **C11**、試験は **CppUTest または GoogleTest**(`TH25S_TEST_FRAMEWORK` で選択)とする。試験ソースは `tests/test_framework.h` 経由の互換マクロ(`TH25S_TEST` / `TH25S_LONGS_EQUAL` 等)を用い、両フレームワークで同一動作するよう書く。本番コードにテスト用の条件分岐(`if (test_mode)` 等)を入れない。
9. テストは実際の機能を検証する。無意味なアサーション(`CHECK(true)` 等)やテストを通すためだけのハードコードは禁止。正常系・境界値・異常系を網羅する。
