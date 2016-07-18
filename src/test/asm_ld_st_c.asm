/*
    LD/ST C の テストプログラム
 */

start:
    // 4バイト store したものを 1, 2, 4 バイト load するテスト
    LD      C, $deadbeef
    ST      C, [0]

    LD      C, [0]O
    CMP     C, $ef
    JNE     test-failed

    LD      C, [0]H
    CMP     C, $beef
    JNE     test-failed

    LD      C, [0]
    CMP     C, $deadbeef
    JNE     test-failed

    // 2バイト store したものを 1, 2, 4 バイト load するテスト
    LD      C, $abcd
    ST      C, [0]H

    LD      C, [0]O
    CMP     C, $cd
    JNE     test-failed

    LD      C, [0]H
    CMP     C, $abcd
    JNE     test-failed

    LD      C, [0]
    CMP     C, $deadabcd
    JNE     test-failed

    // 1バイト store したものを 1, 2, 4 バイト load するテスト
    LD      C, $10
    ST      C, [0]O

    LD      C, [0]O
    CMP     C, $10
    JNE     test-failed

    LD      C, [0]H
    CMP     C, $ab10
    JNE     test-failed

    LD      C, [0]
    CMP     C, $deadab10
    JNE     test-failed

    // レジスタからのload
    LD      A, $A001
    LD      B, $B001
    LD      D, $D001

    LD      C, A
    CMP     C, $A001
    JNE     test-failed

    LD      C, B
    CMP     C, $B001
    JNE     test-failed

    LD      C, D
    CMP     C, $D001
    JNE     test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
