# [WIP] VGS CPU
- SUZUKI PLAN - Video Game System - Central Processing Unit
- VGS-CPU is a 32bit cpu that spacialized for creating games of VGS.

## Developmental status
- version alpha (いろいろ作りかけ)

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
- VGS本体から呼び出し時は `r.s` と `r.p` が常にリセットされる
  - スタック領域(s)の内容は次回ループ時に引き継がれない
  - 次回ループに残すべき情報は主記憶(m)をストアする必要がある

## Operands [WIP]
### g.e.
- `(GR)` : general register
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
|`PUSH (GR)`|-|-|汎用レジスタ の 値 を スタック領域(s) に積む |
|`POP (GR)`|-|-|スタック領域(s) の 内容 を 汎用レジスタ に戻す|
|`LD (GR), n`|-|-|汎用レジスタ `<-` 定数|
|`LD (GR), [n]`|-|-|汎用レジスタ `<-` 主記憶(m)のn番地の内容|
|`LD (GR1), (GR2)`|-|-|汎用レジスタ(GR1) に 汎用レジスタ(GR2) の内容をロード|
|`ST (GR), [n]`|-|-|汎用レジスタ の 値 `->` 主記憶(m)のn番地(32bit)|

### arithmetic calculation
|operand|z|q|outline|
|---|:---:|:---:|---|
|`ADD (GR), n`|o|-|汎用レジスタ `<-` 汎用レジスタ `+` 定数|
|`ADD (GR), [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `+` 主記憶(m)のn番地の内容|
|`ADD (GR1), (GR2)`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `+` 汎用レジスタ2|
|`SUB (GR), n`|o|-|汎用レジスタ `<-` 汎用レジスタ `-` 定数|
|`SUB (GR), [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `-` 主記憶(m)のn番地の内容|
|`SUB (GR1), (GR2)`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `-` 汎用レジスタ2|
|`INC (GR)`|o|-|汎用レジスタ `<-` 汎用レジスタ `+` 1|
|`DEC (GR)`|o|-|汎用レジスタ `<-` 汎用レジスタ `-` 1|
|`MUL (GR), n`|o|-|汎用レジスタ `<-` 汎用レジスタ `×` 定数|
|`MUL (GR), [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `×` 主記憶(m)のn番地の内容|
|`MUL (GR1), (GR2)`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `×` 汎用レジスタ2|
|`DIV (GR), n`|o|-|汎用レジスタ `<-` 汎用レジスタ `÷` 定数|
|`DIV (GR), [n]`|o|-|汎用レジスタ `<-` 汎用レジスタ `÷` 主記憶(m)のn番地の内容|
|`DIV (GR1), (GR2)`|o|-|汎用レジスタ1 `<-` 汎用レジスタ1 `÷` 汎用レジスタ2|


- 演算系命令は極力 `r.a` を使った方が高速になる

### Logical calculation

todo: 書き途中
