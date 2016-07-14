# [WIP] VGS CPU [![Build Status](https://travis-ci.org/suzukiplan/vgs-cpu.svg?branch=master)](https://travis-ci.org/suzukiplan/vgs-cpu)
- SUZUKI PLAN - Video Game System - Central Processing Unit
- VGS-CPU is a 32bit cpu that spacialized for creating games of VGS.

## Developmental status
WIP外しの残todo:
- [x] ACU-B の test case を全て追加
- [x] ACU-C の test case を全て追加
- [x] ACU-D の test case を全て追加
- [ ] CAL operand の実装
- [ ] branch operand の test case を全て追加
- [ ] VGS operand の実装
- [ ] VGS operand の test case を全て実装
- [ ] ドキュメント整備

## About VGS-CPU
- 将来的には Cコンパイラ も作りたいと思っていますが __32bit CPUなのに6502並にフルアセンブリ言語でもコーディングしやすい石__ として, 嘗て私がフルアセでコードを書いていたことを色々思い出しながら設計しています _(単にCコンパイラを作るのは大変だからという説もある)_
- VGSの推奨言語を今まで「C言語」としてきた伏線がようやくここで回収される訳です（ゲーム本体は普通にC++で書いた方が楽ですが、C++コンパイラを作るのは私には荷が重すぎるので... _Cなら標準関数を除けば多分作れる_）

## How to use
- このリポジトリには コード + テスト しかありません
- 利用したいプログラムに `git subversion add` して利用する想定

## API specification
### vgscpu_create_context
```c
void *vgscpu_create_context();
void *vgscpu_create_specific_context(unsigned int ps, unsigned int ss, unsigned int ms);
```
- CPUコンテキストを作成
- `vgscpu_create_context` は `vgscpu_create_specific_context(64KB - 4byte, 256KB, 4MB)` と等価
- `ps` : プログラムサイズの上限（byte）
- `ss` : スタックサイズの上限（byte）
- `ms` : メモリサイズの上限（byte）

### vgscpu_load_program
```c
int vgscpu_load_program(void *ctx, void *pg, size_t size);
```
- CPUコンテキスト に プログラム を ロード
- 成功すると 0 を返し, 失敗すると -1 を返す

### vgscpu_run
```c
int vgscpu_run(void *ctx);
```
- プログラムを実行
- program counter と stack pointer はリセットされる
- `BRK` 命令を検出すると 0 を返す
- 例外を検出すると -1 を返しエラー情報が格納される

### vgscpu_get_last_error
```c
const char* vgscpu_get_last_error(void *ctx);
```
- 最後に発生したエラー情報 ( `\0` で終端するASCIIテキスト) を返す

### vgscpu_release_context
```c
void vgscpu_release_context(void *ctx);
```
- CPUコンテキストを開放

## Specification & Plans (WIP)
- VGS 本体の SLOT に PSLOT (Program Slot) を追加する
- VGS アプリ起動時に PSLOT000 が 1回だけ実行される
- 以降, フレーム間隔毎に PSLOT001 が実行される
- PSLOT は (001 ~ 254 の範囲で) 動的にスイッチできる
- VGS アプリ終了時に PSLOT255 が (あれば) 1回だけ実行される
- todo: アセンブラも後で作る（それまではハンドアセンブルでテストする）

## Architecture
### context
|type|name|size|outline|
|---|---|---|---|
|register|r.a|32bit|general register (accumulater)|
|register|r.b|32bit|general register (base)|
|register|r.c|32bit|general register (counter)|
|register|r.d|32bit|general register (data)|
|register|r.s|32bit|stack pointer|
|register|r.p|32bit|program counter|
|status flag|f.z|32bit|zero flag|
|status flag|f.q|32bit|equal flag (0: euqal, -1: negative, 1:positive)|
|memory|m|variable|main memory|
|memory|s|variable|stack memory|
|memory|p|variable|program context|

- 一般的なCPUと異なる点として以下のような特徴があります
  - フラグは32bitで管理（この方がトータルの演算コストが低くなる）
  - 主記憶を memory / stack / program で分離している
- コンテキスト確保時は全ての領域がゼロクリアされる
- `vgscpu_run` 呼び出し時は `r.s` と `r.p` が常にリセットされる
  - スタック領域(s)の内容は次回ループ時に引き継がれない
  - 次回ループに残すべき情報は主記憶(m)をストアする必要がある

## Operands
### g.e.
- `GR` : general register
  - `A` : 32bit
  - `AH` : 16bit (A and 0xffff)
  - `AO` : 8bit (A and 0xff)
  - B, C, D についても同様
- `n` : literal number
  - `255` : decimal number
  - `$FF` : hexadecimal number
  - `0377` : octal
  - `B11111111` : binary
- `[n]` : address
  - `[n]` : 通常, アドレスが指す 32bit の領域を意味する
  - `[n]H` : 通常, アドレスが指す 16bit の領域を意味する
  - `[n]O` : 通常, アドレスが指す 8bit の領域を意味する
  - 例外的にアドレスの先頭を単純に指し示す場合は単に `[n]` と表記

### memory operations
|operand|z|q|outline|
|---|:---:|:---:|---|
|`PUSH GR`|-|-|汎用レジスタ の 値 を スタック領域 に積む |
|`POP GR`|-|-|スタック領域(s) の 内容 を 汎用レジスタ に戻す|
|`LD GR, n`|-|-|汎用レジスタ `<-` 定数|
|`LD GR, [n]`|-|-|汎用レジスタ `<-` 主記憶のn番地の内容|
|`LD GR1, GR2`|-|-|汎用レジスタ1 に 汎用レジスタ2 の内容をロード|
|`ST GR, [n]`|-|-|汎用レジスタ の 値 `->` 主記憶のn番地|

### arithmetic calculation
|operand|z|q|outline|
|---|:---:|:---:|---|
|`ADD GR, n`|o|-|汎用レジスタ `<-` 汎用レジスタ `+` 定数|
|`ADD GR, [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `+` 主記憶のn番地の内容|
|`ADD GR1, GR2`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `+` 汎用レジスタ2|
|`SUB GR, n`|o|-|汎用レジスタ `<-` 汎用レジスタ `-` 定数|
|`SUB GR, [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `-` 主記憶のn番地の内容|
|`SUB GR1, GR2`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `-` 汎用レジスタ2|
|`INC GR`|o|-|汎用レジスタ `<-` 汎用レジスタ `+` 1|
|`DEC GR`|o|-|汎用レジスタ `<-` 汎用レジスタ `-` 1|
|`MUL GR, n`|o|-|汎用レジスタ `<-` 汎用レジスタ `×` 定数|
|`MUL GR, [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `×` 主記憶のn番地の内容|
|`MUL GR1, GR2`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `×` 汎用レジスタ2|
|`DIV GR, n`|o|-|汎用レジスタ `<-` 汎用レジスタ `÷` 定数|
|`DIV GR, [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `÷` 主記憶のn番地の内容|
|`DIV GR1, GR2`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `÷` 汎用レジスタ2|
|`MOD GR, n`|o|-|汎用レジスタ `<-` 汎用レジスタ `mod` 定数|
|`MOD GR, [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `mod` 主記憶のn番地の内容|
|`MOD GR1, GR2`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `mod` 汎用レジスタ2|

- ADD, SUB, MUL, DIV, MOD の場合 GR, GR1 は A を使うと高速になる
- todo: 補数演算が追加で必要かもしれない

### logical calculation
|operand|z|q|outline|
|---|:---:|:---:|---|
|`AND GR, n`|o|-|汎用レジスタ `<-` 汎用レジスタ `&` 定数|
|`AND GR, [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `&` 主記憶のn番地の内容|
|`AND GR1, GR2`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `&` 汎用レジスタ2|
|`OR GR, n`|o|-|汎用レジスタ `<-` 汎用レジスタ `|` 定数|
|`OR GR, [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `|` 主記憶のn番地の内容|
|`OR GR1, GR2`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `|` 汎用レジスタ2|
|`XOR GR, n`|o|-|汎用レジスタ `<-` 汎用レジスタ `^` 定数|
|`XOR GR, [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `^` 主記憶のn番地の内容|
|`XOR GR1, GR2`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `^` 汎用レジスタ2|
|`NOT GR`|o|-|汎用レジスタ `<-` `not` 汎用レジスタ|
|`SHL GR, n`|o|-|汎用レジスタ を n bit 左シフト _(0 <= n < 256)_|
|`SHR GR, n`|o|-|汎用レジスタ を n bit 右シフト _(0 <= n < 256)_|

- AND, OR, XOR の場合 GR, GR1 は A を使うと高速になる

### comparison
- 2値を比較して `f.q` を更新
- 左辺 `>` 右辺 : `f.q` = `1`
- 左辺 `=` 右辺 : `f.q` = `0`
- 左辺 `<` 右辺 : `f.q` = `-1`
- `CMP` : 符号なしで比較
- `CMP2` : 符号あり（２の補数）で比較

|operand|z|q|outline|
|---|:---:|:---:|---|
|`CMP GR, n`|-|o|汎用レジスタ `:` 定数|
|`CMP GR, [n]`|-|o|汎用レジスタ `:` 主記憶のn番地の内容|
|`CMP GR1, GR2`|-|o|汎用レジスタ1 `:` 汎用レジスタ2|
|`CMP2 GR, n`|-|o|汎用レジスタ `:` 定数|
|`CMP2 GR, [n]`|-|o|汎用レジスタ `:` 主記憶のn番地の内容|
|`CMP2 GR1, GR2`|-|o|汎用レジスタ1 `:` 汎用レジスタ2|

### branch
todo: 書き途中
