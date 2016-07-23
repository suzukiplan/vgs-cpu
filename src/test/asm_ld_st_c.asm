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

    // レジスタ指定アドレスからのload
    LD      A, $12345678
    ST      A, [0]
    LD      A, $90abcdef
    ST      A, [4]

    LD      A, 4
    LD      C, [A]O
    CMP     C, $ef
    JNE     test-failed
    LD      C, [A]H
    CMP     C, $cdef
    JNE     test-failed
    LD      C, [A]
    CMP     C, $90abcdef
    JNE     test-failed

    LD      B, 4
    LD      C, [B]O
    CMP     C, $ef
    JNE     test-failed
    LD      C, [B]H
    CMP     C, $cdef
    JNE     test-failed
    LD      C, [B]
    CMP     C, $90abcdef
    JNE     test-failed

    LD      C, 4
    LD      C, [C]O
    CMP     C, $ef
    JNE     test-failed
    LD      C, 4
    LD      C, [C]H
    CMP     C, $cdef
    JNE     test-failed
    LD      C, 4
    LD      C, [C]
    CMP     C, $90abcdef
    JNE     test-failed

    LD      D, 4
    LD      C, [D]O
    CMP     C, $ef
    JNE     test-failed
    LD      C, [D]H
    CMP     C, $cdef
    JNE     test-failed
    LD      C, [D]
    CMP     C, $90abcdef
    JNE     test-failed

    // レジスタ指定アドレスへのstore
    LD      C, $55555555
    ST      C, [16]
    LD      C, 16
    LD      A, 16
    ST      C, [A]o
    LD      C, [16]
    CMP     C, $55555510
    JNE     test-failed
    LD      C, 16
    LD      A, 16
    ST      C, [A]h
    LD      C, [16]
    CMP     C, $55550010
    JNE     test-failed
    LD      C, 16
    LD      A, 16
    ST      C, [A]
    LD      C, [16]
    CMP     C, $00000010
    JNE     test-failed

    LD      C, $55555555
    ST      C, [16]
    LD      C, 16
    LD      B, 16
    ST      C, [B]o
    LD      C, [16]
    CMP     C, $55555510
    JNE     test-failed
    LD      C, 16
    LD      B, 16
    ST      C, [B]h
    LD      C, [16]
    CMP     C, $55550010
    JNE     test-failed
    LD      C, 16
    LD      B, 16
    ST      C, [B]
    LD      C, [16]
    CMP     C, $00000010
    JNE     test-failed

    LD      C, $55555555
    ST      C, [16]
    LD      C, 16
    ST      C, [C]o
    LD      C, [16]
    CMP     C, $55555510
    JNE     test-failed
    LD      C, 16
    ST      C, [C]h
    LD      C, [16]
    CMP     C, $55550010
    JNE     test-failed
    LD      C, 16
    ST      C, [C]
    LD      C, [16]
    CMP     C, $00000010
    JNE     test-failed

    LD      C, $55555555
    ST      C, [16]
    LD      C, 16
    LD      D, 16
    ST      C, [D]o
    LD      C, [16]
    CMP     C, $55555510
    JNE     test-failed
    LD      C, 16
    LD      D, 16
    ST      C, [D]h
    LD      C, [16]
    CMP     C, $55550010
    JNE     test-failed
    LD      C, 16
    LD      D, 16
    ST      C, [D]
    LD      C, [16]
    CMP     C, $00000010
    JNE     test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
