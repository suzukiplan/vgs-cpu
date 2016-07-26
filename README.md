# VGS CPU [![Build Status](https://travis-ci.org/suzukiplan/vgs-cpu.svg?branch=master)](https://travis-ci.org/suzukiplan/vgs-cpu)
- SUZUKI PLAN - Video Game System - Central Processing Unit
- VGS-CPU は 8bit CPU 並に _フルアセンブリ言語でのコーディングがし易い_ 32bit の CISC仮想CPU です
- 本リポジトリでは VGS-CPU モジュールに加え, [アセンブラ](#assembler) `(vgsasm)` や [デバッガ](#debugger) `(vgsdrun)` など, VGS-CPU向けアプリケーションの開発に必要なプログラミングツール(ミドルウェア)も提供しています

## License
[2-clause BSD](https://github.com/suzukiplan/vgs-cpu/blob/master/LICENSE.txt)

## Support environment
- `make test` を実行して全ての項目が success になる環境であれば動作できます
- big-endian の環境には対応していません

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

### vgscpu_regist_interrupt
```c
void vgscpu_regist_interrupt(void *ctx, unsigned char n, int (*interrupt)(struct vgscpu_context *));
```
- `INT` 命令で実行される 割り込み番号n の 関数を登録する
- 割り込み関数 VGS-CPU を利用するコンピュータシステム上 で コンテキスト毎 に 最大256個 登録することができる
- Windows で利用する場合, 割り込み関数 の 呼び出し規約 が `__cdecl` (C規約) である点に注意すること

_例: `HELLO, VGS-CPU` と標準出力する割り込み (番号0) を登録_
```c
int hello_world(struct vgscpu_context *c)
{
    printf("HELLO, VGS-CPU");
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    vgscpu_regist_interrupt(c, 0, hello_world);
    :
```

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
- `[GR]` : general register address
  - `[GR]` : 通常, アドレス (GR値) が指す 32bit の領域を意味する
  - `[GR]H` : 通常, アドレス (GR値) が指す 16bit の領域を意味する
  - `[GR]O` : 通常, アドレス (GR値) が指す 8bit の領域を意味する
  - 例外的にアドレスの先頭を単純に指し示す場合は単に `[GR]` と表記

### memory operations
|operand|z|q|outline|
|---|:---:|:---:|---|
|`PUSH GR`|-|-|汎用レジスタ の 値 を スタック領域 に積む |
|`POP GR`|-|-|スタック領域(s) の 内容 を 汎用レジスタ に戻す|
|`LD GR, n`|-|-|汎用レジスタ `<-` 定数|
|`LD GR, [n]`|-|-|汎用レジスタ `<-` 主記憶のn番地の内容|
|`LD GR1, GR2`|-|-|汎用レジスタ1 に 汎用レジスタ2 の内容をロード|
|`LD GR1, [GR2]`|-|-|汎用レジスタ1 に 主記憶の 汎用レジスタ2 が指す番地の内容をロード|
|`ST GR, [n]`|-|-|汎用レジスタ の 値 `->` 主記憶のn番地|
|`ST GR, [GR2]`|-|-|汎用レジスタ の 値 `->` 主記憶の　汎用レジスタ2 が指す番地|

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
|`INT n`|`?`|`?`|割り込み番号n を実行|

- `CAL` を実行すると, スタック領域に 戻りアドレス(4byte) を PUSH して アドレスn へジャンプする
- `RET` を実行すると, スタック領域から 戻りアドレス(4byte) を POP して 戻りアドレス へジャンプする
- _アプリケーションにより, 誤って 戻りアドレス が `POP` されると stack underflow または 不正アドレス へジャンプする恐れがある_
- `INT` で 呼び出す割り込み は `vgscpu_regist_interrupt` で予め登録してなければならない
- `INT` で 登録した割り込み関数 の 戻り値 (int型) は __レジスタD__ にセットされる
- `INT` の 戻り値以外 の 呼び出し規約の取り決め は 割り込みの実装者 に委ねられるが, 原則以下の割当を推奨する
  - レジスタA: 算術演算の類のパラメタ (accumulater)
  - レジスタB: 主記憶アドレスの起点 (base)
  - レジスタC: 個数 (counter)

## Assembler
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

## Debugger
### vgsdrun command
```
$ vgsdrun [-s] input-source
```
- アセンブリ言語ソース (.asm) ファイルを実際に vgs-cpu で実行した時の流れを表示
- `-s` オプションを指定すればステップ実行

#### record format
```
<address>: opcodes : line#<number> > assembly-source
```

#### example: run all
```
$ ./vgsdrun src/test/asm_add_a.asm 
assembling: src/test/asm_add_a.asm on memory
starting debug run.
00000000: ----- start -----
00000000: 08 E8 03               : line#00006 > LD   A, 1000
00000003: 5D 00                  : line#00007 > ADD  A, 0
00000005: A6 E8 03               : line#00008 > CMP  A, 1000
00000008: F8 02 01 00 00         : line#00009 > JNE  $102 (test-failed)
0000000d: F5 02 01 00 00         : line#00010 > JZ   $102 (test-failed)
00000012: 5D FF                  : line#00011 > ADD  A, 255
00000014: A6 E7 04               : line#00012 > CMP  A, 1255
00000017: F8 02 01 00 00         : line#00013 > JNE  $102 (test-failed)
0000001c: F5 02 01 00 00         : line#00014 > JZ   $102 (test-failed)
00000021: 5E 00 01               : line#00015 > ADD  A, 256
00000024: A6 E7 05               : line#00016 > CMP  A, 1511
00000027: F8 02 01 00 00         : line#00017 > JNE  $102 (test-failed)
0000002c: F5 02 01 00 00         : line#00018 > JZ   $102 (test-failed)
00000031: 5E FF FF               : line#00019 > ADD  A, 65535
00000034: A7 E6 05 01 00         : line#00020 > CMP  A, 67046
00000039: F8 02 01 00 00         : line#00021 > JNE  $102 (test-failed)
0000003e: F5 02 01 00 00         : line#00022 > JZ   $102 (test-failed)
00000043: 5F 00 00 01 00         : line#00023 > ADD  A, 65536
00000048: A7 E6 05 02 00         : line#00024 > CMP  A, 132582
0000004d: F8 02 01 00 00         : line#00025 > JNE  $102 (test-failed)
00000052: F5 02 01 00 00         : line#00026 > JZ   $102 (test-failed)
00000057: 5F 1A FA FD FF         : line#00027 > ADD  A, -132582
0000005c: F6 02 01 00 00         : line#00028 > JNZ  $102 (test-failed)
00000061: 09 78 56 34 12         : line#00031 > LD   A, $12345678
00000066: 12 00 00 00 00         : line#00032 > ST   A, [0]
0000006b: 07 00                  : line#00033 > LD   A, 0
0000006d: 63 00 00 00 00         : line#00034 > ADD  A, [0]O
00000072: A5 78                  : line#00035 > CMP  A, $78
00000074: F8 02 01 00 00         : line#00036 > JNE  $102 (test-failed)
00000079: F5 02 01 00 00         : line#00037 > JZ   $102 (test-failed)
0000007e: 5F 88 FF FF FF         : line#00038 > ADD  A, -120
00000083: F6 02 01 00 00         : line#00039 > JNZ  $102 (test-failed)
00000088: 64 00 00 00 00         : line#00040 > ADD  A, [0]H
0000008d: A6 78 56               : line#00041 > CMP  A, $5678
00000090: F8 02 01 00 00         : line#00042 > JNE  $102 (test-failed)
00000095: F5 02 01 00 00         : line#00043 > JZ   $102 (test-failed)
0000009a: 5F 88 A9 FF FF         : line#00044 > ADD  A, -22136
0000009f: F6 02 01 00 00         : line#00045 > JNZ  $102 (test-failed)
000000a4: 65 00 00 00 00         : line#00046 > ADD  A, [0]
000000a9: A7 78 56 34 12         : line#00047 > CMP  A, $12345678
000000ae: F8 02 01 00 00         : line#00048 > JNE  $102 (test-failed)
000000b3: F5 02 01 00 00         : line#00049 > JZ   $102 (test-failed)
000000b8: 5F 88 A9 CB ED         : line#00050 > ADD  A, -305419896
000000bd: F6 02 01 00 00         : line#00051 > JNZ  $102 (test-failed)
000000c2: 1E 00 00 11 11         : line#00054 > LD   B, $11110000
000000c7: 32 11 11               : line#00055 > LD   C, $00001111
000000ca: 48 11 11 11 11         : line#00056 > LD   D, $11111111
000000cf: 60                     : line#00057 > ADD  A, B
000000d0: A7 00 00 11 11         : line#00058 > CMP  A, $11110000
000000d5: F8 02 01 00 00         : line#00059 > JNE  $102 (test-failed)
000000da: F5 02 01 00 00         : line#00060 > JZ   $102 (test-failed)
000000df: 61                     : line#00061 > ADD  A, C
000000e0: A7 11 11 11 11         : line#00062 > CMP  A, $11111111
000000e5: F8 02 01 00 00         : line#00063 > JNE  $102 (test-failed)
000000ea: F5 02 01 00 00         : line#00064 > JZ   $102 (test-failed)
000000ef: 62                     : line#00065 > ADD  A, D
000000f0: A7 22 22 22 22         : line#00066 > CMP  A, $22222222
000000f5: F8 02 01 00 00         : line#00067 > JNE  $102 (test-failed)
000000fa: F5 02 01 00 00         : line#00068 > JZ   $102 (test-failed)
000000ff: 46 01                  : line#00070 > LD   D, 1
00000101: 00                     : line#00071 > BRK 

registers:
c->r.a = 22222222, c->r.b = 11110000, c->r.c = 00001111, c->r.d = 00000001
c->r.p = 00000101, c->r.s = 00000000, c->f.z = 00000000, c->f.q = 00000000
$
```

#### example: step execution
```
$ ./vgsdrun src/test/asm_mul.asm  -s
assembling: src/test/asm_mul.asm on memory
starting debug run.
00000000: ----- start-a -----
00000000: 08 E8 03               : line#00006 > LD   A, 1000
00000003: 6F 64                  : line#00007 > MUL  A, 100
00000005: A7 A0 86 01 00         : line#00008 > CMP  A, 100000
0000000a: F8 08 03 00 00         : line#00009 > JNE  $308 (test-failed)
> h
command usage:
- h     ... help
- r     ... show registers
- q     ... quit
- other ... execute next line
> r
registers:
c->r.a = 000186A0, c->r.b = 00000000, c->r.c = 00000000, c->r.d = 00000000
c->r.p = 0000000A, c->r.s = 00000000, c->f.z = 00000000, c->f.q = 00000000
> q
$
```