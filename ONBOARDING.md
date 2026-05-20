# 仮想 Therac-25 Simple — リリース後保守ワークフロー オンボーディングガイド

**ドキュメント ID:** ONB-TH25S-001
**バージョン:** 1.2
**作成日:** 2026-05-20
**対象製品:** 仮想 Therac-25 Simple / TH25S-SIM-001

| 役割 | 氏名 | 所属 | 日付 | 署名 |
|------|------|------|------|------|
| 作成者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-20 | — |
| レビュー者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-20 | — |
| 承認者 | 開発者A(全ロール兼任) | 学習プロジェクト | 2026-05-20 | — |

---

このガイドは、本プロジェクト(IEC 62304 クラス C 学習プロジェクト)で **リリース後の保守(GitHub Issue 対応)** を引き継ぐ人のためのものです。2026-05-20 に Issue #1〜#6 + #11 を **CR-0006〜CR-0012** として処理した実績に基づいて作成し、以降の保守(本ガイド自身の追加 CR-0013 ほか CR-0014 以降)も本手順に沿って実施しています。再現可能な手順として §9 に実績を蓄積しています。

> プロジェクトの目的・構成・安全クラスは [README.md](./README.md) と [CLAUDE.md](./CLAUDE.md) を、ハザードの背景は [THERAC25_HAZARD_ANALYSIS.md](./THERAC25_HAZARD_ANALYSIS.md) を参照してください。本ガイドは「どう直すか」の手順書です。

## 0. 大原則

- リリース後の変更は、たとえ文書 1 行の修正でも **変更要求(CR-NNNN)として構成管理プロセスを通す**。これは「プロセスを一巡体験する」という本プロジェクトの教材目的そのもの。
- 単独開発のため独立性は擬制する: **審議インターバル(CCB §5.4)+ 自己レビュー(PR 上)+ CI による機械的検証**。Issue Forms の必須入力もこの機械的検証の一部。
- 正本の方向を取り違えない: **設計(SDD/SRS)が正本 → 実装(コード)はその反映**。トレーサビリティの正本は各成果物文書(分散方式)で、`TRACEABILITY_MATRIX.md` はその派生ビュー(参考)。

## 1. 全体フロー(9 ステップ)

```text
1. Issue 受領・分析       →  2. 方針確認(スコープ/形式/CR運用)
3. ブランチ作成           →  4. 実装(成果物の作成・修正)
5. 影響文書の連鎖更新     →  6. 検証(機械的検証)
7. コミット・push・PR     →  8. レビュー(指摘あれば fixup)
9. マージ(Squash)
```

## 2. CR(変更要求)の起票ルール

- GitHub Issue は **SPRP §3.1** に従い「市販後フィードバック」として取扱う。**PRB は新規採番せず**、GitHub Issue 番号をそのまま起点とする。
- CR ID は [CRR §4](./8_software_configuration_management_process/change_request_register.md) 台帳の **末尾 + 1** で採番。
- 区分は [CCB §5.2](./8_software_configuration_management_process/ccb_operating_rules.md) 基準。文書のみ・製品挙動不変なら **MINOR**。RCM 実装 CI(UNIT-001〜003)・安全クラス・分離設計(SEP-001)・外部 IF・SOUP に触れるなら **MAJOR**。
- 起票テンプレートは [`.github/ISSUE_TEMPLATE/change-request.yml`](./.github/ISSUE_TEMPLATE/change-request.yml)(GitHub の Issue 作成画面で「変更要求 (CR)」を選択)。

## 3. ブランチ・コミット規約

- **main へ直接コミットしない。** 必ずブランチ → PR 経由。
- ブランチ名: `docs/issue-<N>-<短い英語スラッグ>`(例: `docs/issue-5-sdd-unit002-state-diagram`)。
- コミットメッセージ: `docs(CR-NNNN): 概要 (Closes #N)`。本文に変更点・検証結果・IEC 62304 根拠を記し、末尾に必ず:

  ```text
  Co-Authored-By: Claude Opus 4.7 (1M context) <noreply@anthropic.com>
  ```

- PR 本文末尾には `🤖 Generated with [Claude Code](https://claude.com/claude-code)`。

## 4. 影響文書の連鎖更新(最重要・漏らさない)

成果物を 1 つ変更したら、**必ず**以下を同一 PR で同期させる。漏れると監査時にトレーサビリティ不整合(IEC 62304 §8.2.4 違反)になる。

| 文書 | 更新内容 |
|------|---------|
| 変更した成果物自身 | バージョン昇格(`MAJOR.MINOR`)+ 末尾「改訂履歴」に 1 行 |
| [CRR](./8_software_configuration_management_process/change_request_register.md) | §4 に CR 行、§6 リリース後保守表、§7 集計(件数 / MINOR 数)、§8 改訂履歴 |
| [CIL](./8_software_configuration_management_process/configuration_item_list.md) | §4 で対象 CI のバージョン、§9 に新規ベースライン `BL-YYYYMMDD-NNN`、§10 改訂履歴 |
| [ACL](./compliance/audit_checklist.md) | §11 改訂履歴(条項状態 ✅ は通常不変) |
| [DEVELOPMENT_STEPS](./DEVELOPMENT_STEPS.md) | 新しい `Day N` 行 + 末尾「改訂履歴」 |

補足ルール:

- **平文文書(README.md / CLAUDE.md)は内容を変更してもバージョン据え置き**(CR-0002 以降の運用)。
- 学習プロジェクト固有の新規文書には ID プレフィックスを付与し、README §略語 と CLAUDE.md に登録する(例: `QAR-` / `HZA-` / `UTM-`)。
- CIL 末尾の **「付録 A: CIL 更新時チェックリスト」を毎回全走査**して漏れを防ぐ。
- 累積バージョン履歴は **全列挙スタイルを維持**(「CR-0001 で 1.1、…、CR-0012 で 1.12 へ昇格」)。省略形(`…`)にしない。

## 5. 検証(コミット前に必ず実行)

```bash
# Markdown 構文
npx markdownlint-cli2 "**/*.md"

# 内部リンク + 見出しアンカー(フラグメント)
lychee --offline --include-fragments './**/*.md'
```

- **Mermaid 図**を追加したら: コードフェンス開閉の一致を確認し、`stateDiagram-v2` のラベルは括弧・`<br/>` を避けてレンダリング確実性を優先(詳細は図の下の凡例に書く)。**実レンダリングは CI 対象外なので、マージ後に GitHub 上で目視確認**。
- **YAML**(Issue Forms 等)を追加したら: `python3 -c "import yaml; yaml.safe_load(open('...'))"` で構文確認。
- CI(GitHub Actions)は PR で 5 ジョブ(CMake build + CppUTest & GoogleTest / Date format / Internal link check / Markdown lint / directory structure)。全 pass を待ってマージ。

## 6. レビュー

- `/review <PR番号>` で多角的にレビュー。
- **自分の PR は GitHub 仕様で approve できない** → `gh pr review <PR#> --comment --body-file <file>` でコメントとして投稿。
- レビューでは「成果物が正本(`.h` コメント / 各文書のトレース列 / 実装)と整合するか」を機械的に照合するのが効く。
- 指摘が出たら **fixup コミット**で反映してから再検証 → マージ。

## 7. マージ

```bash
gh pr merge <PR#> --squash --delete-branch
```

- マージ方式は **Squash + ブランチ削除**(main の線形履歴を維持)。
- コミット/PR の `Closes #N` で Issue が自動クローズされる。
- マージ後、`gh issue list --state open` で残 Issue を確認。

## 8. ハマりどころ(実際に踏んだもの)

- **MD028(blockquote 内の空行):** 連続する `>` 引用の間に空行を置くと markdownlint が落ちる。`>` の空行でつなぐ(1 引用に統合)か、片方を通常段落にする。
- **YAML 内リンクは lychee 検証対象外:** CI の lychee は `.md` グロブのみ。`.github/**/*.yml` 内のリンク・アンカーは手動照合が必要。
- **Issue Forms の `labels:`** は、事前に `gh label create <name>` していないと silent に無視される。
- **日付は ISO 8601(`YYYY-MM-DD`)。** 長い作業中に日付が変わることがあるので、その時点の日付に合わせる。
- **新しい Day 行・改訂履歴行は時系列順に挿入。** 既存行末尾と次セクション見出しを `old_string` に含めて、挿入位置を誤らない。
- **GitHub 見出しアンカー規則:** 小文字化・空白→`-`・`( ) , . :` と em-dash(`―`)は削除・漢字/かな/カナと全角中点(`・`)は保持。既存リンク(例 `#9-トレーサビリティマトリクス箇条-526`)を参照すると確実。

## 9. 実績(このワークフローで処理した CR)

| Issue | 内容 | CR | 区分 | PR |
|-------|------|-----|------|-----|
| #1 | ACL §3 対応文書列を章タイトル + アンカーリンク化 | CR-0006 | MINOR | #7 |
| #2 | GitHub Issue Forms 変更要求テンプレート作成 | CR-0007 | MINOR | #8 |
| #3 | Therac-25 ハザード解析・事故メカニズム解説(`HZA-`)| CR-0008 | MINOR | #9 |
| #4 | 統一トレーサビリティマトリクス(`UTM-`)| CR-0009 | MINOR | #10 |
| #5 | SDD UNIT-002 状態遷移図の Mermaid 化 | CR-0010 | MINOR | #12 |
| #6 | SDD UNIT-001/002/003 の関数責務明確化 | CR-0011 | MINOR | #13 |
| #11 | SRS-004 トレーサビリティ不整合の是正(#4 で検出)| CR-0012 | MINOR | #14 |
| —(内部) | リリース後保守ワークフロー オンボーディングガイド(`ONB-`、本書)を構成管理下に追加 | CR-0013 | MINOR | #15 |
| —(内部) | QA-003(CIL §3 のソースコード CI 3 分割基準)を Q&A 記録に追記 | CR-0014 | MINOR | #16 |
| —(内部) | CIL §3 CI-SRC-002 への CR-0001 注記補完(CR-0014 のレビューで検出)| CR-0015 | MINOR | #17 |
| —(内部) | 本書 §9 実績に CR-0013〜0015 を追記(学びの一例 2 を追加)| CR-0016 | MINOR | #18 |
| —(内部) | QA-004(SDD UNIT-001 の th25s_error_t 要約記載でも実装・テストできた理由)を Q&A 記録に追記 | CR-0017 | MINOR | #19 |

> **学びの一例 1(俯瞰が不整合を炙り出す):** Issue #4 で統一マトリクスを作った際、SRS §9 と STPR §11 の SRS-004 のシステム試験 ID が食い違う「正本間不整合」を検出した。これを Issue #11 として起票し、CR-0012 で STPR §8 の試験ケース定義まで追跡して 3 者を根本整合させた。**統一ビュー(俯瞰)は分散文書間の隠れた不整合を炙り出す。検出したら一方修正で済ませず、関連文書を追跡して根本是正すること。**
>
> **学びの一例 2(後続作業が記録漏れを炙り出す):** CR-0014(QA-003 = ソースコード CI 分割基準の解説)を書く過程で、ルートビルド構成 CI-SRC-002 が CR-0001 で変更されているのに CIL §3 に注記が欠落していた(試験側 CI-TD-004 のみ注記済みだった)記録漏れを検出し、CR-0015 で是正した。Issue #11(CR-0012)・STEPS 版数乖離(CR-0013)に続く 3 例目で、**質問対応・ガイド化・レビューといった「後から俯瞰する作業」は、過去の CR が残した記録漏れを発見する良い契機になる。検出した記録漏れは、当時の履歴行は改変せず追補行で遡及是正すること(過去ログは書き換えない)。**

## 10. 参照文書(プロセスの正本)

| 文書 | 役割 |
|------|------|
| [CCB 運用規程](./8_software_configuration_management_process/ccb_operating_rules.md) | CR 区分・承認フロー・審議インターバル |
| [構成管理計画書 SCMP](./8_software_configuration_management_process/software_configuration_management_plan.md) | 構成識別・変更制御・ベースライン |
| [変更要求台帳 CRR](./8_software_configuration_management_process/change_request_register.md) | CR の一元記録(採番はここの末尾 +1)|
| [構成アイテム一覧 CIL](./8_software_configuration_management_process/configuration_item_list.md) | CI バージョン・ベースライン履歴・更新チェックリスト |
| [問題解決手順書 SPRP](./9_software_problem_resolution_process/software_problem_resolution_procedure.md) | 問題報告の取扱い(GitHub Issue = 市販後フィードバック)|
| [監査チェックリスト ACL](./compliance/audit_checklist.md) | IEC 62304 条項別の適合状況 |
| [開発ステップ記録 DEVELOPMENT_STEPS](./DEVELOPMENT_STEPS.md) | 日割りガイド + 全 Day の保守履歴 |

## 11. 改訂履歴

| バージョン | 日付 | 変更内容 | 変更者 |
|----------|------|---------|--------|
| 1.0 | 2026-05-20 | 初版作成(CR-0013)。CR-0006〜0012 の実績に基づくリリース後保守ワークフローのオンボーディングガイドを構成管理下に追加。 | 開発者A |
| 1.1 | 2026-05-20 | CR-0016 反映: §9 実績に CR-0013〜0015(本書追加・QA-003 追記・CIL CI-SRC-002 注記補完)を追記。あわせて「学びの一例 2(後続作業が記録漏れを炙り出す)」を追加し、導入文を以降の保守も本手順で実施している旨に更新。手順本体(§0〜§8)は不変。 | 開発者A |
| 1.2 | 2026-05-20 | CR-0018 反映: §9 実績に CR-0016(本書 §9 実績更新)・CR-0017(QA-004 追記)を追記。導入文の対象範囲を「CR-0014 以降」と一般化(以後の追記で陳腐化しないため)。手順本体(§0〜§8)・学びの一例は不変。 | 開発者A |
