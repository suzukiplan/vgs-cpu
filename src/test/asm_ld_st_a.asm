/*
    LD/ST A の テストプログラム
 */

start:
    // 4バイト store したものを 1, 2, 4 バイト load するテスト
    LD      A, $deadbeef
    ST      A, [0]

    LD      A, [0]O
    CMP     A, $ef
    JNE     test-failed

    LD      A, [0]H
    CMP     A, $beef
    JNE     test-failed

    LD      A, [0]
    CMP     A, $deadbeef
    JNE     test-failed

    // 2バイト store したものを 1, 2, 4 バイト load するテスト
    LD      A, $abcd
    ST      A, [0]H

    LD      A, [0]O
    CMP     A, $cd
    JNE     test-failed

    LD      A, [0]H
    CMP     A, $abcd
    JNE     test-failed

    LD      A, [0]
    CMP     A, $deadabcd
    JNE     test-failed

    // 1バイト store したものを 1, 2, 4 バイト load するテスト
    LD      A, $10
    ST      A, [0]O

    LD      A, [0]O
    CMP     A, $10
    JNE     test-failed

    LD      A, [0]H
    CMP     A, $ab10
    JNE     test-failed

    LD      A, [0]
    CMP     A, $deadab10
    JNE     test-failed

    // レジスタからのload
    LD      B, $B001
    LD      C, $C001
    LD      D, $D001

    LD      A, B
    CMP     A, $B001
    JNE     test-failed

    LD      A, C
    CMP     A, $C001
    JNE     test-failed

    LD      A, D
    CMP     A, $D001
    JNE     test-failed

    // レジスタ指定アドレスからのload
    LD      A, $12345678
    ST      A, [0]
    LD      A, $90abcdef
    ST      A, [4]

    LD      A, 4
    LD      A, [A]O
    CMP     A, $ef
    JNE     test-failed
    LD      A, 4
    LD      A, [A]H
    CMP     A, $cdef
    JNE     test-failed
    LD      A, 4
    LD      A, [A]
    CMP     A, $90abcdef
    JNE     test-failed

    LD      B, 4
    LD      A, [B]O
    CMP     A, $ef
    JNE     test-failed
    LD      A, [B]H
    CMP     A, $cdef
    JNE     test-failed
    LD      A, [B]
    CMP     A, $90abcdef
    JNE     test-failed

    LD      C, 4
    LD      A, [C]O
    CMP     A, $ef
    JNE     test-failed
    LD      A, [C]H
    CMP     A, $cdef
    JNE     test-failed
    LD      A, [C]
    CMP     A, $90abcdef
    JNE     test-failed

    LD      D, 4
    LD      A, [D]O
    CMP     A, $ef
    JNE     test-failed
    LD      A, [D]H
    CMP     A, $cdef
    JNE     test-failed
    LD      A, [D]
    CMP     A, $90abcdef
    JNE     test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
