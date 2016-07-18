# VGS CPU [![Build Status](https://travis-ci.org/suzukiplan/vgs-cpu.svg?branch=master)](https://travis-ci.org/suzukiplan/vgs-cpu)
- SUZUKI PLAN - Video Game System - Central Processing Unit
- VGS-CPU is a 32bit cpu that spacialized for creating games of VGS.
- This repository also provides assember for VGS-CPU.

## How to use
- UNIX系OS全般(mac/Linuxを含む) の ターミナル で `make` を実行すれば, できることが書いてあります
- `make build` を実行すればアセンブラ (vgsasmコマンド) をビルドできます
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
|operand|z|q|outline|
|---|:---:|:---:|---|
|`JMP n`|-|-|アドレスn へ無条件ジャンプ|
|`JZ n`|x|-|`f.z` が 1 の場合, アドレスn へジャンプ|
|`JNZ n`|x|-|`f.z` が 0 の場合, アドレスn へジャンプ|
|`JE n`|-|x|`f.q` が 0 の場合, アドレスn へジャンプ|
|`JNE n`|-|x|`f.q` が 非0 の場合, アドレスn へジャンプ|
|`JN n`|-|x|`f.q` が -1 の場合, アドレスn へジャンプ|
|`JNN n`|-|x|`f.q` が 非-1 の場合, アドレスn へジャンプ|
|`JP n`|-|x|`f.q` が 1 の場合, アドレスn へジャンプ|
|`JNP n`|-|x|`f.q` が 非1 の場合, アドレスn へジャンプ|
|`CAL n`|-|-|アドレスn を呼び出す|
|`RET`|-|-|`CAL` の呼び出し元へ復帰|
|`BRK`|-|-|プログラムの実行状態を解除|

- `CAL` を実行すると, スタック領域に 戻りアドレス(4byte) を PUSH して アドレスn へジャンプする
- `RET` を実行すると, スタック領域から 戻りアドレス(4byte) を POP して 戻りアドレス へジャンプする
- _アプリケーションにより, 誤って 戻りアドレス が `POP` されると stack underflow または 不正アドレス へジャンプする恐れがある_

### VGS API
|operand|z|q|outline|
|---|:---:|:---:|---|
|`VGS n`|-|-|命令番号n の VGS API を呼び出す|

- VGS の API を呼び出すための特別な命令
- 全てのデバイスアクセスは VGS API が実行するため, VGS-CPU には 通常のコンピュータの `OUT` や `IN` に相当する命令は存在しない 
- 呼び出し規約（引数として利用される項目）は 命令番号 によって異なる
- 戻り値は必ず __レジスタD__ に格納される

## VGS API
### operands
|命令番号|API名|呼び出し規約|戻り値|意味|
|---:|:---|:---|:---:|:---|
|0|NOOP|n/a|0|何も実行せずに 0 を返す|

_※VGS APIは実装途中で、今後拡張していきます_

## VGS ASM
### vgsasm command
```
$ vgsasm [-o output-binary] input-source
```

### syntax
#### comment syntax
C/C++言語形式 の コメント に 対応
```asm
    PUSH A  /* C形式コメント (複数行可能) */
    POP  A  // C++形式コメント
```

#### literal syntax
- 定数の表記方法は 2進数, 8進数, 10進数, 16進数 の 4種類 を サポート
- 負数表記 は 10進数 の 場合 にのみできる
```asm
    LD A, B10101110   // 2進数 (頭に B を付ける)
    LD B, 0765        // 8進数 (頭に 0 を付ける)
    LD C, 12345678    // 10進数 (頭に - または 1〜9 を付ける)
    LD D, $deadbeef   // 16進数 (頭に $ を付ける)
```

主記憶のアドレス表記には `[literal]` を用いる
```asm
    ST A, [$123]      // アドレス $123 から 4byte (32bit) の内容を参照
    ST B, [$123]H     // アドレス $123 から 2byte (16bit) の内容を参照
    ST C, [$123]O     // アドレス $123 から 1byte (8bit) の内容を参照
```

#### label syntax
- サフィックス `:` を付けたものがラベルとなる
- ラベルは 1バイト以上 255バイト以下 の 1トークン文字列 とする
- ブランチ命令の飛び先には必ずラベルを用いる (アドレス値の直指定はできない)
```asm
    ST  A, [$123]
    LD  B, 100
    CMP A, B
    JE  equal       // A = B
    JP  positive    // A > B
    JN  negative    // A < B

equal:
    LD  C, 0
    BRK

positive:
    LD  C, 1
    BRK

negative:
    LD  C, -1
    BRK
```

#### operand syntax
[Operands](#operands) を 参照
