# Changes

## version 0.1.2
- license の修正（不要文言削除のみ）
- `vgsasm` のテストを追加

## Version 0.1.1
- `vgsasm` のテストを追加: LD, ST
- bugfix: 引数またはレジスタが指定できる第2オペランドにレジスタBを指定する2進数の0が指定されてしまう

## Version 0.1.0 (破壊的変更)
- `VGS` 命令を廃止
- `INT` 命令を追加
- API関数 に `vgscpu_regist_interrupt` を追加
- `UNKNOWN INSTRUCTION` 例外メッセージの書式を変更

## Version 0.0.3
- `vgsasm` のテストを追加: PUSH, POP
- bugfix: acu-b, c, d の命令アセンブル結果が不正になる

## Version 0.0.2
- `vgsasm` コマンドを実装 (注意: テスト未実施)

## Version 0.0.1
- CPU実装完了 (APIはまだ全部揃っていない)

