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

    // レジスタ指定アドレスからのload
    LD      A, $12345678
    ST      A, [0]
    LD      A, $90abcdef
    ST      A, [4]

    LD      A, 4
    LD      B, [A]O
    CMP     B, $ef
    JNE     test-failed
    LD      B, [A]H
    CMP     B, $cdef
    JNE     test-failed
    LD      B, [A]
    CMP     B, $90abcdef
    JNE     test-failed

    LD      B, 4
    LD      B, [B]O
    CMP     B, $ef
    JNE     test-failed
    LD      B, 4
    LD      B, [B]H
    CMP     B, $cdef
    JNE     test-failed
    LD      B, 4
    LD      B, [B]
    CMP     B, $90abcdef
    JNE     test-failed

    LD      C, 4
    LD      B, [C]O
    CMP     B, $ef
    JNE     test-failed
    LD      B, [C]H
    CMP     B, $cdef
    JNE     test-failed
    LD      B, [C]
    CMP     B, $90abcdef
    JNE     test-failed

    LD      D, 4
    LD      B, [D]O
    CMP     B, $ef
    JNE     test-failed
    LD      B, [D]H
    CMP     B, $cdef
    JNE     test-failed
    LD      B, [D]
    CMP     B, $90abcdef
    JNE     test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
