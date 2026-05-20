# 変更要求台帳(CR Register)

**ドキュメント ID:** CRR-TH25S-001
**バージョン:** 1.12
**最終更新日:** 2026-05-20
**対象製品:** 仮想 Therac-25 Simple / TH25S-SIM-001

| 役割 | 氏名 | 所属 | 日付 | 署名 |
|------|------|------|------|------|
| 管理者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-20 | — |
| 承認者(品質保証) | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-20 | — |

---

## 1. 目的と適用範囲

本台帳は、IEC 62304 箇条 8.2(変更制御)に基づき、TH25S-CORE に対するすべての変更要求(CR)を一元的に記録・追跡する。起票からクローズまでの状態遷移を記録し、問題報告(PRB)およびリリースとのトレーサビリティを維持する。

## 2. 参照文書

| ID | 文書名 | バージョン |
|----|--------|----------|
| [1] | CCB 運用規程(CCB-TH25S-001) | 1.0 |
| [2] | ソフトウェア構成管理計画書(SCMP-TH25S-001) | 1.0 |
| [3] | ソフトウェア問題解決手順書(SPRP-TH25S-001) | 1.0 |

## 3. 記録ルール

### 3.1 CR ID の採番
- 形式: `CR-NNNN`(4 桁連番)
- 採番元: GitHub Issues(変更要求テンプレート)

### 3.2 状態(State)の定義
| 状態 | 定義 |
|------|------|
| OPEN | 起票直後、区分判定前 |
| TRIAGED | 区分判定済み、CCB 審議待ち |
| APPROVED | CCB 承認済み、実装待ち・実装中 |
| REJECTED | CCB により却下 |
| VERIFICATION | 実装完了、検証中 |
| CLOSED | 検証合格、マージ・リリース済み |
| REOPENED | クローズ後、問題再発で再オープン |

### 3.3 区分(Category)
| 区分 | 判定基準 | 承認フロー |
|------|---------|----------|
| MINOR | 軽微(誤字・コメント・文書のみ) | 開発者A 承認 |
| MODERATE | 中程度(非安全関連) | CCB 簡易審議(擬制) |
| MAJOR | 重大(安全関連・アーキ変更・SOUP 変更) | CCB 正式審議(擬制) |

## 4. CR 台帳

| CR ID | 起票日 | 起票者 | 区分 | 状態 | 概要 | 関連 PRB | 関連 CI | 関連 RCM | 実装ブランチ/PR | 検証 ID | クローズ日 |
|-------|-------|-------|------|------|------|--------|---------|---------|--------------|--------|----------|
| CR-0001 | 2026-05-15 | 開発者A | MAJOR | CLOSED | GoogleTest を SOUP-002 として追加し、CppUTest と試験時に選択可能とする(`TH25S_TEST_FRAMEWORK=cpputest\|gtest\|both`)。両フレームワークで同一の 55 試験ケースが走るよう試験ソースを互換マクロ層(`tests/test_framework.h`)経由に統一。製品コード `libth25s_core.a` は不変。(注: 起票時点の本行は GoogleTest 1.17.0 単一表記としていたが、CR-0002 で環境別に正確化した結果、SOUP-002 の運用上の実バージョンは macOS Homebrew = 1.17.0 / Ubuntu 24.04 apt libgtest-dev = 1.14.0 と判明) | — | CI-SOUP-002(新規), CI-SRC-002(`CMakeLists.txt`), CI-TD-001〜004, `tests/test_framework.h`(新規) | — (試験ツリー拡張、RCM 影響なし) | main 直接コミット(単独開発・本台帳 1.1 同梱) | UT-001-01〜UT-003-14(両フレームワークで合格) | 2026-05-15 |
| CR-0002 | 2026-05-15 | 開発者A | MINOR | CLOSED | SOUP-002 GoogleTest のバージョン記載を環境別に正確化(`1.17.0` 単一表記 → `1.17.0 (macOS Homebrew)` / `1.14.0 (Ubuntu 24.04 apt libgtest-dev)` 併記)。CR-0001 完了後の CI ログ確認で Ubuntu apt 提供版が 1.17.0 と異なることが判明したため、IEC 62304 §8.1.2 SOUP 識別の正確性を担保すべく文書のみを修正。SOUP の追加・削除・実バージョン変更はなく、CCB §5.2 基準では MINOR 区分。 | — | CI-DOC-CLAUDE, CI-DOC-README, CI-DOC-SDP, CI-DOC-SAD, CI-DOC-UTPR, CI-DOC-SMS, CI-DOC-SRMP, CI-DOC-CRR, CI-DOC-CIL, CI-DOC-ACL, CI-DOC-STEPS | — (文書修正、RCM 影響なし) | main 直接コミット | 既存 CI(`build-test.yml`)の両フレームワーク合格を再確認 | 2026-05-15 |
| CR-0003 | 2026-05-15 | 開発者A | MINOR | CLOSED | 略語辞書を README §略語に新設(発見性向上のため)。既存の `compliance/audit_checklist.md §10` は監査用ミラーとして日本語訳・ACL 自身を追加して充実化。`DEVELOPMENT_STEPS.md` および `CLAUDE.md` から README §略語へのリンクを追加。文書修正のみで SOUP・実装・RCM への影響なし(CCB §5.2 基準で MINOR)。 | — | CI-DOC-README, CI-DOC-CLAUDE, CI-DOC-STEPS, CI-DOC-ACL, CI-DOC-CRR, CI-DOC-CIL | — (文書改善、RCM 影響なし) | main 直接コミット | markdownlint 0 エラー / lychee 0 エラー / 構造チェック OK | 2026-05-15 |
| CR-0004 | 2026-05-15 | 開発者A | MINOR | CLOSED | 学習プロジェクト固有の Q&A 制度を確立(新規ファイル `IEC62304_QA.md`、文書 ID `QAR-TH25S-001`、エントリ ID 体系 `QA-NNN`)。Q&A-001 として「RMF §6.1 リスク評価表の見方と判定根拠」を記録。同回答の準備中に、§6.1 表で参照される `EV-001`〜`EV-004` が §4.2 でラベル付き定義されていなかったことが判明したため、RMF §4.2 にラベル付き事象シーケンス定義を追加(RMF v1.0 → v1.1)。既存利用にもかかわらず未登録だった `EV-` プレフィックスを CLAUDE.md / README §略語に追加し、新規の `QAR-` / `QA-` プレフィックスも併せて登録。HZ・RCM・判定値・実装・SOUP への影響なし(CCB §5.2 基準で MINOR)。 | — | CI-DOC-QA(新規), CI-DOC-RMF, CI-DOC-CLAUDE, CI-DOC-README, CI-DOC-STEPS, CI-DOC-ACL, CI-DOC-CRR, CI-DOC-CIL | — (文書改善、RCM 影響なし) | main 直接コミット | markdownlint 0 エラー / lychee 0 エラー / 構造チェック OK | 2026-05-15 |
| CR-0005 | 2026-05-15 | 開発者A | MINOR | CLOSED | 利用者からの質問「SRS §9 トレーサビリティ表のアーキテクチャ列は SRS 初版作成時には書けないはず、後で追記する手順で合っているか」に対する **QA-002** を `IEC62304_QA.md` に追加(v1.0 → v1.1)。回答準備中に、SRS §9 が「v1.0 リリース時の as-released 状態」を提示しているが、その旨が表の前に明示されておらず初学者が「SRS 初版作成時点でアーキテクチャ列を書ける」と誤解しかねないことが判明。SRS §9 冒頭に as-released 状態と反復更新パターンの説明を追加(SRS v1.0 → v1.1)。DEVELOPMENT_STEPS.md に「トレーサビリティの反復更新パターン」節を新設し、各フェーズ完了時の上流文書更新手順を明示。SRS の要求事項本体・トレース内容・実装・SOUP への影響なし(CCB §5.2 基準で MINOR)。 | — | CI-DOC-QA, CI-DOC-SRS, CI-DOC-STEPS, CI-DOC-CRR, CI-DOC-CIL, CI-DOC-ACL | — (文書改善、RCM 影響なし) | main 直接コミット | markdownlint 0 エラー / lychee 0 エラー / 構造チェック OK | 2026-05-15 |
| CR-0006 | 2026-05-20 | 開発者A | MINOR | CLOSED | 利用者からの指摘(GitHub Issue #1)で「`compliance/audit_checklist.md §3` の対応文書列が `SDP §3` のような章番号のみで内容が分かりづらく、当該章への到達性も低い」ことが判明。ACL §3(箇条 5 ソフトウェア開発プロセス、5.1〜5.8 の計 47 行)の対応文書列を「`<略号> §N 章タイトル(箇条 X.Y.Z)`形式 + 該当章へのアンカーリンク」に書き換え、§3 冒頭にも表記ルールの注記を追加した。`§番号` を伴わない補助参照(SRMP / SCMP / CIL / RMF / SPRP の単独参照、`src/th25s_core/` 等)はリンク化していない。対象は ACL §3 のみで、§4〜§7(箇条 6〜9)は本 CR の範囲外(Issue でも箇条 5 のみ指定)。要求事項本体・条項対応関係・状態(✅)・SOUP・実装・RCM への影響なし(CCB §5.2 基準で MINOR)。 | — (Issue #1 で報告。SPRP §3.1 に従い GitHub Issue を市販後フィードバックとして取扱い、PRB 新規採番は行わず CR で対応) | CI-DOC-ACL, CI-DOC-CRR, CI-DOC-CIL, CI-DOC-STEPS | — (文書改善、RCM 影響なし) | `docs/issue-1-acl-correspondence-doc-column` ブランチ | markdownlint 0 エラー / lychee `--offline --include-fragments` 0 エラー(§3 で新規追加した 57 件のアンカーリンクのフラグメント解決を確認) | 2026-05-20 |
| CR-0007 | 2026-05-20 | 開発者A | MINOR | CLOSED | 利用者からの指摘(GitHub Issue #2)で「CCB 運用規程 §4.1 が参照する『GitHub Issues(変更要求テンプレート)』が実体として存在しない」ことが判明。`.github/ISSUE_TEMPLATE/change-request.yml`(YAML Issue Forms 形式)を新規作成し、CCB §4.1 の必須 6 項目に加えて §5.2 安全関連性チェック・SCMP §4.3 検証方針・§7 ロールバック計画・SMP §4.7 通知要否を起票時に網羅する構造化フォームを実装。CCB §4.1 にテンプレート実体への参照リンクを追加し、双方向トレーサビリティを確立(CCB 1.0 → 1.1)。Issue Forms の `validations.required` を「独立性擬制の第 4 の機械的検証」として CCB §4.1 で位置付け。製品コード `libth25s_core.a`・SOUP・実装・RCM・要求事項本体への影響なし(CCB §5.2 基準で MINOR、文書とプロセス補助テンプレートのみ)。 | — (Issue #2 で報告。SPRP §3.1 に従い GitHub Issue を市販後フィードバックとして取扱い、PRB 新規採番は行わず CR で対応) | CI-DOC-CCB(新規 §4.1 リンク追加で 1.0 → 1.1), CI-DOC-ACL, CI-DOC-CRR, CI-DOC-CIL, CI-DOC-STEPS。新規ファイル `.github/ISSUE_TEMPLATE/change-request.yml` は SCMP §3.3「リポジトリ内で版管理」枠組みで管理(CIL §4 個別登録は見送り) | — (文書 + プロセス補助テンプレートの追加、RCM 影響なし) | `docs/issue-2-cr-issue-template` ブランチ | YAML 構文検証(`python -m yaml`)合格 / markdownlint 0 エラー / lychee `--offline --include-fragments` 0 エラー | 2026-05-20 |
| CR-0008 | 2026-05-20 | 開発者A | MINOR | CLOSED | 利用者からの要望(GitHub Issue #3)で「README の Therac-25 ハザード(HZ-001/002/003/横断)について、事故発生メカニズムを詳しく図解して解説してほしい」を受領。学習プロジェクト固有の補助教材 `THERAC25_HAZARD_ANALYSIS.md`(文書 ID `HZA-TH25S-001`)を新規作成。HZ-001〜003 + 横断要因の事故発生メカニズムを Mermaid 図解(flowchart / stateDiagram / sequenceDiagram)付きで解説し、各ハザードに「IEC 62304 プロセスでの防止」「本実装の RCM 対応(ソース抜粋付き)」を対応付け。§8 でハザード → SRS → RCM → 実装 → 検証のトレースを明示し、Issue #3 の「ハザードはシステム要求とも考えられる」観点に応えた。README「題材」セクションに解説リンク + §略語に HZA を追加、CLAUDE.md の ID プレフィックスに HZA- を登録。事故記述は Leveson & Turner 1993 に基づく事実関係のみ。製品コード・SOUP・実装・RCM・要求事項本体・既存規格文書(RMF 等)への影響なし(CCB §5.2 基準で MINOR、補助教材の新規追加)。 | — (Issue #3 で報告。SPRP §3.1 に従い GitHub Issue を市販後フィードバックとして取扱い、PRB 新規採番は行わず CR で対応) | CI-DOC-HZA(新規 `THERAC25_HAZARD_ANALYSIS.md` / HZA-TH25S-001 v1.0), CI-DOC-ACL, CI-DOC-CRR, CI-DOC-CIL, CI-DOC-STEPS。平文文書 README.md / CLAUDE.md(バージョン据え置き) | — (補助教材の追加、RCM 影響なし) | `docs/issue-3-therac25-hazard-analysis` ブランチ | markdownlint 0 エラー / lychee `--offline --include-fragments` 0 エラー / Mermaid 構文確認 | 2026-05-20 |
| CR-0009 | 2026-05-20 | 開発者A | MINOR | CLOSED | 利用者からの要望(GitHub Issue #4)で「QA-002 §5 で言及した統一トレーサビリティマトリクス分離方式について、現在の分散方式(各文書に §トレース列)は変更せず、参考資料として統一マトリクスを作成してほしい」を受領。学習プロジェクト固有の参考資料 `TRACEABILITY_MATRIX.md`(文書 ID `UTM-TH25S-001`)を新規作成。要求駆動(SRS→ARCH→UNIT→UT→IT→ST)・安全駆動(HZ→EV→SRS-RCM→RCM→実装→検証)・RCM 対応の複合マトリクスを各文書(SRS §9 / SAD §11 / SDD §7 / UTPR §10 / ITPR §13 / STPR §11 / RMF §11)から集約し、各セルに出典文書を明記。**本書は派生ビュー(参考資料)であり正本は各文書の分散トレース列である**旨を §1.2 で明示(統一方式を本採用すると正本が逆転する点も解説)。QA-002 §5 に参考実装への参照リンクを追加(QAR 1.1 → 1.2)。Issue #4 の制約どおり既存の各文書 §トレース列は一切変更せず。SOUP・実装・RCM・要求事項本体・トレース内容への影響なし(CCB §5.2 基準で MINOR、参考資料の新規追加)。なお PR #10 レビューで **SRS-004 のシステム試験 ID が SRS §9(ST-004)と STPR §11(ST-001)で食い違う正本間不整合** を検出し、UTM §3 ※1・§6 に注記のうえ **Issue #11** を起票して別 CR で是正することとした(統一ビューが分散方式の隠れた不整合を検出した実例)。 | — (Issue #4 で報告。SPRP §3.1 に従い GitHub Issue を市販後フィードバックとして取扱い、PRB 新規採番は行わず CR で対応。派生検出事項は Issue #11 として起票) | CI-DOC-UTM(新規 `TRACEABILITY_MATRIX.md` / UTM-TH25S-001 v1.0), CI-DOC-QA(QA-002 §5 リンク追加で 1.1 → 1.2), CI-DOC-ACL, CI-DOC-CRR, CI-DOC-CIL, CI-DOC-STEPS。平文文書 README.md / CLAUDE.md(バージョン据え置き) | — (参考資料の追加、RCM 影響なし) | `docs/issue-4-unified-traceability-matrix` ブランチ | markdownlint 0 エラー / lychee `--offline --include-fragments` 0 エラー(統一マトリクス内の各文書アンカーリンク解決を確認) | 2026-05-20 |
| CR-0010 | 2026-05-20 | 開発者A | MINOR | CLOSED | 利用者からの要望(GitHub Issue #5)で「SDD §4 UNIT-002 TreatmentSequencer の状態遷移図が ASCII 表記で、状態とイベント(トリガ)のどちらを表しているかパッと見て分からないため Mermaid 記法で書いてほしい」を受領。SPRP §3.1 に従い市販後フィードバックとして取扱い、CR-0010(MINOR)として処理。SDD §4 UNIT-002 の状態遷移図を ASCII から Mermaid `stateDiagram-v2` に変更(状態=角丸ボックス、イベント=矢印ラベルで視覚的に区別)し、図の下に「状態(7 種)とイベント(トリガ)の凡例」+ request_beam_on の分岐・abort・FAULT・select_mode 再選択リセット(RCM-001)の補足を追加。状態機械のロジック・遷移・既存 RCM-001 注記は不変(表現形式のみの改善)。SDD を 1.0 → 1.1 へ昇格。製品コード・SOUP・実装・RCM・要求事項本体・設計内容への影響なし(CCB §5.2 基準で MINOR、図の表現形式変更)。 | — (Issue #5 で報告。SPRP §3.1 に従い GitHub Issue を市販後フィードバックとして取扱い、PRB 新規採番は行わず CR で対応) | CI-DOC-SDD(状態遷移図の Mermaid 化で 1.0 → 1.1), CI-DOC-ACL, CI-DOC-CRR, CI-DOC-CIL, CI-DOC-STEPS | — (図の表現形式変更、RCM 影響なし) | `docs/issue-5-sdd-unit002-state-diagram` ブランチ | markdownlint 0 エラー / lychee `--offline --include-fragments` 0 エラー / Mermaid 構文確認 | 2026-05-20 |
| CR-0011 | 2026-05-20 | 開発者A | MINOR | CLOSED | 利用者からの要望(GitHub Issue #6)で「SDD §4 UNIT-002 の公開 API 表に関数の役割・引数の意味が書かれておらず、各関数が何をするのかイメージできない(関数名からの推測は厳しい)」を受領。SPRP §3.1 に従い市販後フィードバックとして取扱い、CR-0011(MINOR)として処理。同じ表構造の課題は UNIT-001/003 も共通のため、SDD §4 の **3 ユニットすべて** の公開 API 表の直後に「関数の責務と引数」小節を追加し、各関数の責務(役割)・引数の意味・戻り値の意味を記述。記述は各 `.h`(common_types.h / treatment_sequencer.h / safety_interlock.h)の関数コメントと整合させた(本 SDD が設計の正本、`.h` コメントは実装上の反映)。既存の公開 API 表(事前/事後条件)・設計内容・実装・要求は不変(説明の補強のみ)。SDD を 1.1 → 1.2 へ昇格。製品コード・SOUP・実装・RCM への影響なし(CCB §5.2 基準で MINOR、説明補強)。 | — (Issue #6 で報告。SPRP §3.1 に従い GitHub Issue を市販後フィードバックとして取扱い、PRB 新規採番は行わず CR で対応) | CI-DOC-SDD(関数責務小節の追加で 1.1 → 1.2), CI-DOC-ACL, CI-DOC-CRR, CI-DOC-CIL, CI-DOC-STEPS | — (説明の補強、RCM 影響なし) | `docs/issue-6-sdd-unit-function-responsibilities` ブランチ | markdownlint 0 エラー / lychee `--offline --include-fragments` 0 エラー | 2026-05-20 |
| CR-0012 | 2026-05-20 | 開発者A | MINOR | CLOSED | CR-0009 の統一トレーサビリティマトリクス作成時に検出した正本間不整合(GitHub Issue #11)を是正。SRS-004 のシステム試験 ID が SRS §9(ST-004)と STPR §11(ST-001)で食い違っていた問題に対し、**ST-001 を正**として SRS §9 を ST-001 に修正。さらに STPR §8 試験ケース定義を確認したところ ST-001 のカバー要求リストにも SRS-004 が欠落していたため(レビュー前調査で発見した「もう一段の不整合」)、ST-001 カバー要求に SRS-004 を補完し、**SRS §9 / STPR §8 / STPR §11 の 3 者を ST-001 で完全整合**させた。SRS-004「ビームオン要求は READY 状態でのみ受付」の正常系は ST-001(READY→ビームオン到達)が検証する。統一マトリクス UTM の ※1 注記・§6 を「是正済み」に更新(UTM 1.0 → 1.1)。要求事項本体・試験ケースの実体・試験結果・RCM・実装は不変(トレーサビリティの正確化のみ。CCB §5.2 基準で MINOR)。 | Issue #11(CR-0009 で検出・起票。SPRP §3.1 に従い GitHub Issue として管理、PRB 新規採番なし) | CI-DOC-SRS(§9 SRS-004 是正で 1.1 → 1.2), CI-DOC-STPR(§8 ST-001 カバー要求補完で 1.0 → 1.1), CI-DOC-UTM(注記是正で 1.0 → 1.1), CI-DOC-ACL, CI-DOC-CRR, CI-DOC-CIL, CI-DOC-STEPS | — (トレーサビリティ是正、RCM 影響なし) | `docs/issue-11-srs004-traceability-fix` ブランチ | markdownlint 0 エラー / lychee `--offline --include-fragments` 0 エラー | 2026-05-20 |

> 本台帳は保守プロセス(箇条 6)で発生した変更要求を追記して管理する。CR-0001 は SOUP 追加(CCB §5.2 基準で MAJOR 区分)だが、追加対象が試験フレームワークであり製品リスクコントロール手段への影響はないため、§5 安全関連 CR 抽出には含めない。CCB §5.4 の 24 時間以上インターバルは、本 CR が学習プロジェクトの試験ツリー拡張で患者危害リスクを伴わないことから、議長(開発者A 擬制)判断で短縮した(独立性の代替は CI 両フレームワーク全合格で確保)。

## 5. 安全関連 CR の抽出

安全関連 CR(RCM 影響あり / MAJOR 区分)は、以下の観点で定期レビューする。本バージョンでは該当 CR はない。

| CR ID | 関連ハザード(HZ) | 関連 RCM | リスク再評価日 | RMF 更新反映 |
|-------|----------------|--------|-------------|-----------|
| — | — | — | — | — |

## 6. リリース別 CR サマリ

各リリースに含まれる CR を一覧化し、ソフトウェアマスタ仕様書(SMS)の「リリースバージョンの文書化」に用いる。

### リリース `v1.0.0`(2026-05-15)

| CR ID | 区分 | 概要 | ユーザ通知要否 | 規制当局通知要否 |
|-------|------|------|-------------|----------------|
| — | — | 初回リリース。変更要求に基づかず、開発プロセス(箇条 5)で構築。 | 不要 | 不要(学習プロジェクト) |

### リリース後保守(`v1.0.0` 後、2026-05-15)

| CR ID | 区分 | 概要 | ユーザ通知要否 | 規制当局通知要否 |
|-------|------|------|-------------|----------------|
| CR-0001 | MAJOR | GoogleTest 追加・試験フレームワーク選択化(製品 `libth25s_core.a` は不変、試験ツリー拡張のみ) | 不要(製品挙動不変、学習リポジトリ README の更新で開示) | 不要(学習プロジェクト) |
| CR-0002 | MINOR | SOUP-002 GoogleTest バージョン記載の正確化(環境別併記。SOUP 実バージョン・製品挙動は不変) | 不要(文書修正のみ) | 不要(学習プロジェクト) |
| CR-0003 | MINOR | 略語辞書の新設(README §略語、監査用ミラーは ACL §10)。発見性向上のための文書改善で、SOUP・実装・RCM 影響なし | 不要(文書改善のみ) | 不要(学習プロジェクト) |
| CR-0004 | MINOR | Q&A 制度の確立(IEC62304_QA.md 新設)と RMF §4.2 EV-NNN ラベル定義の補完。SOUP・実装・RCM 影響なし | 不要(学習サポート資料の追加と文書整合の補完) | 不要(学習プロジェクト) |
| CR-0005 | MINOR | QA-002 追加(SRS §9 反復更新パターン)と SRS §9 / DEVELOPMENT_STEPS への as-released 状態の説明追加。SOUP・実装・RCM 影響なし | 不要(文書改善のみ、要求事項本体は不変) | 不要(学習プロジェクト) |
| CR-0006 | MINOR | ACL §3(箇条 5)の対応文書列を章タイトル + アンカーリンク化(57 件)。SOUP・実装・RCM 影響なし | 不要(文書改善のみ、要求事項本体は不変) | 不要(学習プロジェクト) |
| CR-0007 | MINOR | GitHub Issue Forms 変更要求テンプレート `.github/ISSUE_TEMPLATE/change-request.yml` を新規作成、CCB §4.1 にテンプレート参照リンクを追加(CCB 1.0 → 1.1)。SOUP・実装・RCM 影響なし | 不要(プロセス補助の追加、製品挙動不変) | 不要(学習プロジェクト) |
| CR-0008 | MINOR | Therac-25 ハザード解析・事故メカニズム解説 `THERAC25_HAZARD_ANALYSIS.md`(HZA-TH25S-001)を新規作成(Mermaid 図解付き教材)。SOUP・実装・RCM 影響なし | 不要(補助教材の追加、製品挙動不変) | 不要(学習プロジェクト) |
| CR-0009 | MINOR | 統一トレーサビリティマトリクス `TRACEABILITY_MATRIX.md`(UTM-TH25S-001)を参考資料として新規作成(各文書の §トレース列を集約した派生ビュー)。SOUP・実装・RCM 影響なし | 不要(参考資料の追加、製品挙動不変) | 不要(学習プロジェクト) |
| CR-0010 | MINOR | SDD §4 UNIT-002 の状態遷移図を ASCII から Mermaid `stateDiagram-v2` に変更(SDD 1.0 → 1.1)。状態機械ロジックは不変、表現形式の改善。SOUP・実装・RCM 影響なし | 不要(図の表現形式変更、製品挙動不変) | 不要(学習プロジェクト) |
| CR-0011 | MINOR | SDD §4 UNIT-001/002/003 の公開 API 表に「関数の責務と引数」小節を追加(SDD 1.1 → 1.2)。説明の補強のみで設計内容は不変。SOUP・実装・RCM 影響なし | 不要(説明の補強、製品挙動不変) | 不要(学習プロジェクト) |
| CR-0012 | MINOR | SRS-004 トレーサビリティの正本間不整合を是正(SRS §9 を ST-001 に、STPR §8 に SRS-004 補完、SRS 1.1→1.2 / STPR 1.0→1.1 / UTM 1.0→1.1)。要求・試験実体・RCM は不変。SOUP・実装影響なし | 不要(トレーサビリティ是正、製品挙動・試験結果不変) | 不要(学習プロジェクト) |

## 7. 集計・傾向分析(箇条 9.6 連携)

| 期間 | 総 CR | MAJOR | MODERATE | MINOR | 安全関連 | 再発(REOPENED) |
|------|------|-------|----------|-------|--------|----------------|
| 2026-05(初回リリースまで) | 0 | 0 | 0 | 0 | 0 | 0 |
| 2026-05(リリース後 v1.0.0 保守期) | 12 | 1 | 0 | 11 | 0 | 0 |

傾向分析結果のフィードバック先: 開発標準(コーディング規約)、試験計画(追加試験ケース)。CR-0001 は試験ツリーの保守(SOUP 追加)であり、製品コード・RCM への影響はない。本 CR は「学習プロジェクトの教材性向上(複数フレームワークでの試験動作確認)」を目的とし、傾向分析上は外乱要因として扱う。

## 8. 改訂履歴

| バージョン | 日付 | 変更内容 | 変更者 |
|----------|------|---------|--------|
| 1.0 | 2026-05-15 | 初版作成。初回リリース `v1.0.0` 時点のスナップショット(CR 0 件)。 | 開発者A |
| 1.1 | 2026-05-15 | CR-0001(GoogleTest 追加・MAJOR・CLOSED)を §4 に追加。§6 リリース別 CR サマリに「リリース後保守」節を追加。§7 集計表にリリース後保守期の行を追加。 | 開発者A |
| 1.2 | 2026-05-15 | CR-0002(MINOR・CLOSED)を §4 に追加。§4 CR-0001 行に環境別実バージョン判明の注記を追加。§6 リリース後保守に CR-0002 行を追加。§7 集計表をリリース後保守期 2 件・MINOR 1 件に更新。 | 開発者A |
| 1.3 | 2026-05-15 | CR-0003(MINOR・CLOSED)を §4 に追加。§6 リリース後保守に CR-0003 行を追加。§7 集計表をリリース後保守期 3 件・MINOR 2 件に更新。 | 開発者A |
| 1.4 | 2026-05-15 | CR-0004(MINOR・CLOSED)を §4 に追加。§6 リリース後保守に CR-0004 行を追加。§7 集計表をリリース後保守期 4 件・MINOR 3 件に更新。 | 開発者A |
| 1.5 | 2026-05-15 | CR-0005(MINOR・CLOSED)を §4 に追加。§6 リリース後保守に CR-0005 行を追加。§7 集計表をリリース後保守期 5 件・MINOR 4 件に更新。 | 開発者A |
| 1.6 | 2026-05-20 | CR-0006(MINOR・CLOSED)を §4 に追加(ACL §3 対応文書列の章タイトル + アンカーリンク化)。§6 リリース後保守に CR-0006 行を追加。§7 集計表をリリース後保守期 6 件・MINOR 5 件に更新。 | 開発者A |
| 1.7 | 2026-05-20 | CR-0007(MINOR・CLOSED)を §4 に追加(GitHub Issue Forms 変更要求テンプレートの新規作成、CCB §4.1 リンク追加)。§6 リリース後保守に CR-0007 行を追加。§7 集計表をリリース後保守期 7 件・MINOR 6 件に更新。 | 開発者A |
| 1.8 | 2026-05-20 | CR-0008(MINOR・CLOSED)を §4 に追加(Therac-25 ハザード解析・事故メカニズム解説 HZA-TH25S-001 の新規作成)。§6 リリース後保守に CR-0008 行を追加。§7 集計表をリリース後保守期 8 件・MINOR 7 件に更新。 | 開発者A |
| 1.9 | 2026-05-20 | CR-0009(MINOR・CLOSED)を §4 に追加(統一トレーサビリティマトリクス UTM-TH25S-001 の参考資料新規作成)。§6 リリース後保守に CR-0009 行を追加。§7 集計表をリリース後保守期 9 件・MINOR 8 件に更新。 | 開発者A |
| 1.10 | 2026-05-20 | CR-0010(MINOR・CLOSED)を §4 に追加(SDD §4 UNIT-002 状態遷移図の Mermaid 化)。§6 リリース後保守に CR-0010 行を追加。§7 集計表をリリース後保守期 10 件・MINOR 9 件に更新。 | 開発者A |
| 1.11 | 2026-05-20 | CR-0011(MINOR・CLOSED)を §4 に追加(SDD §4 UNIT-001/002/003 への関数責務小節の追加)。§6 リリース後保守に CR-0011 行を追加。§7 集計表をリリース後保守期 11 件・MINOR 10 件に更新。 | 開発者A |
| 1.12 | 2026-05-20 | CR-0012(MINOR・CLOSED)を §4 に追加(SRS-004 トレーサビリティ正本間不整合の是正)。§6 リリース後保守に CR-0012 行を追加。§7 集計表をリリース後保守期 12 件・MINOR 11 件に更新。本 CR は CR-0009 の統一マトリクスが検出した不整合(Issue #11)を是正する初の「規格成果物のトレース内容是正」案件。 | 開発者A |
