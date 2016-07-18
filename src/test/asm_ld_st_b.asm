/*
    LD/ST B の テストプログラム
 */

start:
    // 4バイト store したものを 1, 2, 4 バイト load するテスト
    LD      B, $deadbeef
    ST      B, [0]

    LD      B, [0]O
    CMP     B, $ef
    JNE     test-failed

    LD      B, [0]H
    CMP     B, $beef
    JNE     test-failed

    LD      B, [0]
    CMP     B, $deadbeef
    JNE     test-failed

    // 2バイト store したものを 1, 2, 4 バイト load するテスト
    LD      B, $abcd
    ST      B, [0]H

    LD      B, [0]O
    CMP     B, $cd
    JNE     test-failed

    LD      B, [0]H
    CMP     B, $abcd
    JNE     test-failed

    LD      B, [0]
    CMP     B, $deadabcd
    JNE     test-failed

    // 1バイト store したものを 1, 2, 4 バイト load するテスト
    LD      B, $10
    ST      B, [0]O

    LD      B, [0]O
    CMP     B, $10
    JNE     test-failed

    LD      B, [0]H
    CMP     B, $ab10
    JNE     test-failed

    LD      B, [0]
    CMP     B, $deadab10
    JNE     test-failed

    // レジスタからのload
    LD      A, $A001
    LD      C, $C001
    LD      D, $D001

    LD      B, A
    CMP     B, $A001
    JNE     test-failed

    LD      B, C
    CMP     B, $C001
    JNE     test-failed

    LD      B, D
    CMP     B, $D001
    JNE     test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
