/*
    MOD の テストプログラム
 */
start-a:
    // リテラルのテスト
    LD      A, 1000
    MOD     A, 64               // A = A mod 100 (1byte)
    CMP     A, 40
    JNE     test-failed
    JZ      test-failed
    MOD     A, 40               // A = A mod 40 (1byte)
    JNZ     test-failed
    LD      A, 259
    MOD     A, 256              // A = A mod 256 (2byte)
    JZ      test-failed
    CMP     A, 3
    JNE     test-failed
    LD      A, 65540
    MOD     A, 65536            // A = A mod 65536 (4byte)
    CMP     A, 4
    JNE     test-failed
    JZ      test-failed

    // アドレス格納領域のテスト
    LD      A, $10203040
    ST      A, [0]
    LD      A, $48
    MOD     A, [0]O             // A = A mod $40 (1byte)
    CMP     A, 8
    JNE     test-failed
    JZ      test-failed
    LD      A, $4040
    MOD     A, [0]H             // A = A / $3040 (2byte)
    CMP     A, $1000
    JNE     test-failed
    JZ      test-failed
    LD      A, $20406081
    MOD     A, [0]              // A = A mod $10203040 (4byte)
    CMP     A, 1
    JNE     test-failed
    JZ      test-failed

    // レジスタ演算テスト
    LD      A, 67
    LD      B, 64
    MOD     A, B                // A = A mod B
    CMP     A, 3
    JNE     test-failed
    JZ      test-failed

    LD      A, 48
    LD      C, 7
    MOD     A, C                // A = A mod C
    CMP     A, 6
    JNE     test-failed
    JZ      test-failed

    LD      A, 33
    LD      D, 12
    MOD     A, D                // A = A mod D
    CMP     A, 9
    JNE     test-failed
    JZ      test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
