/*
    MUL の テストプログラム
 */
start-a:
    // リテラルのテスト
    LD      A, 1000
    MUL     A, 100              // A = A * 100 (1byte)
    CMP     A, 100000
    JNE     test-failed
    JZ      test-failed
    MUL     A, 0                // A = A * 0 (1byte)
    JNZ     test-failed
    LD      A, 4
    MUL     A, 256              // A = A * 256 (2byte)
    CMP     A, 1024
    JZ      test-failed
    MUL     A, -4               // A = A * (-4) (4byte)
    CMP     A, -4096
    JNE     test-failed
    JZ      test-failed

    // アドレス格納領域のテスト
    LD      A, $10203040
    ST      A, [0]
    LD      A, 2
    MUL     A, [0]O             // A = A - $40 (1byte)
    CMP     A, $80
    JNE     test-failed
    JZ      test-failed
    MUL     A, [0]H             // A = A * $3040 (2byte)
    CMP     A, $182000
    JNE     test-failed
    JZ      test-failed
    LD      A, 2
    MUL     A, [0]              // A = A * $10203040 (4byte)
    CMP     A, $20406080
    JNE     test-failed
    JZ      test-failed

    // レジスタ演算テスト
    LD      A, 2
    LD      B, 4
    LD      C, 6
    LD      D, 8
    MUL     A, B                // A = A * B
    CMP     A, 8
    JNE     test-failed
    JZ      test-failed
    MUL     A, C                // A = A * C
    CMP     A, 48
    JNE     test-failed
    JZ      test-failed
    MUL     A, D                // A = A * D
    CMP     A, 384
    JNE     test-failed
    JZ      test-failed

start-b:
    // リテラルのテスト
    LD      B, 1000
    MUL     B, 100              // B = B * 100 (1byte)
    CMP     B, 100000
    JNE     test-failed
    JZ      test-failed
    MUL     B, 0                // B = B * 0 (1byte)
    JNZ     test-failed
    LD      B, 4
    MUL     B, 256              // B = B * 256 (2byte)
    CMP     B, 1024
    JZ      test-failed
    MUL     B, -4               // B = B * (-4) (4byte)
    CMP     B, -4096
    JNE     test-failed
    JZ      test-failed

    // アドレス格納領域のテスト
    LD      B, $10203040
    ST      B, [0]
    LD      B, 2
    MUL     B, [0]O             // B = B - $40 (1byte)
    CMP     B, $80
    JNE     test-failed
    JZ      test-failed
    MUL     B, [0]H             // B = B * $3040 (2byte)
    CMP     B, $182000
    JNE     test-failed
    JZ      test-failed
    LD      B, 2
    MUL     B, [0]              // B = B $10203040 (4byte)
    CMP     B, $20406080
    JNE     test-failed
    JZ      test-failed

    // レジスタ演算テスト
    LD      A, 2
    LD      B, 4
    LD      C, 6
    LD      D, 8
    MUL     B, A                // B = B * B
    CMP     B, 8
    JNE     test-failed
    JZ      test-failed
    MUL     B, C                // B = B * C
    CMP     B, 48
    JNE     test-failed
    JZ      test-failed
    MUL     B, D                // B = B * D
    CMP     B, 384
    JNE     test-failed
    JZ      test-failed

start-c:
    // リテラルのテスト
    LD      C, 1000
    MUL     C, 100              // C = C * 100 (1byte)
    CMP     C, 100000
    JNE     test-failed
    JZ      test-failed
    MUL     C, 0                // C = C * 0 (1byte)
    JNZ     test-failed
    LD      C, 4
    MUL     C, 256              // C = C * 256 (2byte)
    CMP     C, 1024
    JZ      test-failed
    MUL     C, -4               // C = C * (-4) (4byte)
    CMP     C, -4096
    JNE     test-failed
    JZ      test-failed

    // アドレス格納領域のテスト
    LD      C, $10203040
    ST      C, [0]
    LD      C, 2
    MUL     C, [0]O             // C = C - $40 (1byte)
    CMP     C, $80
    JNE     test-failed
    JZ      test-failed
    MUL     C, [0]H             // C = C * $3040 (2byte)
    CMP     C, $182000
    JNE     test-failed
    JZ      test-failed
    LD      C, 2
    MUL     C, [0]              // C = C $10203040 (4byte)
    CMP     C, $20406080
    JNE     test-failed
    JZ      test-failed

    // レジスタ演算テスト
    LD      A, 2
    LD      B, 4
    LD      C, 6
    LD      D, 8
    MUL     C, A                // C = C * A
    CMP     C, 12
    JNE     test-failed
    JZ      test-failed
    MUL     C, B                // C = C * B
    CMP     C, 48
    JNE     test-failed
    JZ      test-failed
    MUL     C, D                // C = C * D
    CMP     C, 384
    JNE     test-failed
    JZ      test-failed

start-d:
    // リテラルのテスト
    LD      D, 1000
    MUL     D, 100              // D = D * 100 (1byte)
    CMP     D, 100000
    JNE     test-failed
    JZ      test-failed
    MUL     D, 0                // D = D * 0 (1byte)
    JNZ     test-failed
    LD      D, 4
    MUL     D, 256              // D = D * 256 (2byte)
    CMP     D, 1024
    JZ      test-failed
    MUL     D, -4               // D = D * (-4) (4byte)
    CMP     D, -4096
    JNE     test-failed
    JZ      test-failed

    // アドレス格納領域のテスト
    LD      D, $10203040
    ST      D, [0]
    LD      D, 2
    MUL     D, [0]O             // D = D - $40 (1byte)
    CMP     D, $80
    JNE     test-failed
    JZ      test-failed
    MUL     D, [0]H             // D = D * $3040 (2byte)
    CMP     D, $182000
    JNE     test-failed
    JZ      test-failed
    LD      D, 2
    MUL     D, [0]              // D = D $10203040 (4byte)
    CMP     D, $20406080
    JNE     test-failed
    JZ      test-failed

    // レジスタ演算テスト
    LD      A, 2
    LD      B, 4
    LD      C, 6
    LD      D, 8
    MUL     D, A                // D = D * A
    CMP     D, 16
    JNE     test-failed
    JZ      test-failed
    MUL     D, B                // D = D * B
    CMP     D, 64
    JNE     test-failed
    JZ      test-failed
    MUL     D, C                // D = D * C
    CMP     D, 384
    JNE     test-failed
    JZ      test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
