/*
    LD/ST D の テストプログラム
 */

start:
    // 4バイト store したものを 1, 2, 4 バイト load するテスト
    LD      D, $deadbeef
    ST      D, [0]

    LD      D, [0]O
    CMP     D, $ef
    JNE     test-failed

    LD      D, [0]H
    CMP     D, $beef
    JNE     test-failed

    LD      D, [0]
    CMP     D, $deadbeef
    JNE     test-failed

    // 2バイト store したものを 1, 2, 4 バイト load するテスト
    LD      D, $abcd
    ST      D, [0]H

    LD      D, [0]O
    CMP     D, $cd
    JNE     test-failed

    LD      D, [0]H
    CMP     D, $abcd
    JNE     test-failed

    LD      D, [0]
    CMP     D, $deadabcd
    JNE     test-failed

    // 1バイト store したものを 1, 2, 4 バイト load するテスト
    LD      D, $10
    ST      D, [0]O

    LD      D, [0]O
    CMP     D, $10
    JNE     test-failed

    LD      D, [0]H
    CMP     D, $ab10
    JNE     test-failed

    LD      D, [0]
    CMP     D, $deadab10
    JNE     test-failed

    // レジスタからのload
    LD      A, $A001
    LD      B, $B001
    LD      C, $C001

    LD      D, A
    CMP     D, $A001
    JNE     test-failed

    LD      D, B
    CMP     D, $B001
    JNE     test-failed

    LD      D, C
    CMP     D, $C001
    JNE     test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
