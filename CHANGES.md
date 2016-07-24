# Changes

## Version 0.3.0
- `vgsdrun` コマンドを追加

## Version 0.2.0
- cpu 
  - レジスタが指すアドレス値からの load/store に対応
- `vgsasm`
  - LD/ST の 引数 に レジスタアドレス `[GR]` を指定できるようにする
  - ブラケット `[` に対応する `]` が無い場合を syntax error にする
  - エラー時のコマンド戻り値の仕様を変更

## Version 0.1.2
- license の修正（不要文言削除のみ）
- `vgsasm` のテストを追加
- bugfix: SHL, SHR の結果が ゼロフラグ に反映されない

## Version 0.1.1
- `vgsasm` のテストを追加: LD, ST
- bugfix: 引数またはレジスタが指定できる第2オペランドにレジスタBを指定すると2進数の0が指定されたものとみなされる

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

