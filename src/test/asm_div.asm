/*
    DIV の テストプログラム
 */
start-a:
    // リテラルのテスト
    LD      A, 1000
    DIV     A, 100              // A = A / 100 (1byte)
    CMP     A, 10
    JNE     test-failed
    JZ      test-failed
    DIV     A, 11               // A = A / 11 (1byte)
    JNZ     test-failed
    LD      A, 1024
    DIV     A, 256              // A = A / 256 (2byte)
    JZ      test-failed
    CMP     A, 4
    JNE     test-failed
    LD      A, 1048576
    DIV     A, 65536            // A = A / 65536 (4byte)
    CMP     A, 16
    JNE     test-failed
    JZ      test-failed

    // アドレス格納領域のテスト
    LD      A, $10203040
    ST      A, [0]
    LD      A, $80
    DIV     A, [0]O             // A = A / $40 (1byte)
    CMP     A, 2
    JNE     test-failed
    JZ      test-failed
    LD      A, $90C0
    DIV     A, [0]H             // A = A / $3040 (2byte)
    CMP     A, 3
    JNE     test-failed
    JZ      test-failed
    LD      A, $20406080
    DIV     A, [0]              // A = A / $10203040 (4byte)
    CMP     A, 2
    JNE     test-failed
    JZ      test-failed

    // レジスタ演算テスト
    LD      A, 67
    LD      B, 7
    DIV     A, B                // A = A / B
    CMP     A, 9
    JNE     test-failed
    JZ      test-failed

    LD      A, 48
    LD      C, 6
    DIV     A, C                // A = A / C
    CMP     A, 8
    JNE     test-failed
    JZ      test-failed

    LD      A, 33
    LD      D, 3
    DIV     A, D                // A = A / D
    CMP     A, 11
    JNE     test-failed
    JZ      test-failed

start-b:
    // リテラルのテスト
    LD      B, 1000
    DIV     B, 100              // B = B / 100 (1byte)
    CMP     B, 10
    JNE     test-failed
    JZ      test-failed
    DIV     B, 11               // B = B / 11 (1byte)
    JNZ     test-failed
    LD      B, 1024
    DIV     B, 256              // B = B / 256 (2byte)
    JZ      test-failed
    CMP     B, 4
    JNE     test-failed
    LD      B, 1048576
    DIV     B, 65536            // B = B / 65536 (4byte)
    CMP     B, 16
    JNE     test-failed
    JZ      test-failed

    // アドレス格納領域のテスト
    LD      B, $10203040
    ST      B, [0]
    LD      B, $80
    DIV     B, [0]O             // B = B / $40 (1byte)
    CMP     B, 2
    JNE     test-failed
    JZ      test-failed
    LD      B, $90C0
    DIV     B, [0]H             // B = B / $3040 (2byte)
    CMP     B, 3
    JNE     test-failed
    JZ      test-failed
    LD      B, $20406080
    DIV     B, [0]              // B = B / $10203040 (4byte)
    CMP     B, 2
    JNE     test-failed
    JZ      test-failed

    // レジスタ演算テスト
    LD      B, 67
    LD      A, 7
    DIV     B, A                // B = B / A
    CMP     B, 9
    JNE     test-failed
    JZ      test-failed

    LD      B, 48
    LD      C, 6
    DIV     B, C                // B = B / C
    CMP     B, 8
    JNE     test-failed
    JZ      test-failed

    LD      B, 33
    LD      D, 3
    DIV     B, D                // B = B / D
    CMP     B, 11
    JNE     test-failed
    JZ      test-failed

start-c:
    // リテラルのテスト
    LD      C, 1000
    DIV     C, 100              // C = C / 100 (1byte)
    CMP     C, 10
    JNE     test-failed
    JZ      test-failed
    DIV     C, 11               // C = C / 11 (1byte)
    JNZ     test-failed
    LD      C, 1024
    DIV     C, 256              // C = C / 256 (2byte)
    JZ      test-failed
    CMP     C, 4
    JNE     test-failed
    LD      C, 1048576
    DIV     C, 65536            // C = C / 65536 (4byte)
    CMP     C, 16
    JNE     test-failed
    JZ      test-failed

    // アドレス格納領域のテスト
    LD      C, $10203040
    ST      C, [0]
    LD      C, $80
    DIV     C, [0]O             // C = C / $40 (1byte)
    CMP     C, 2
    JNE     test-failed
    JZ      test-failed
    LD      C, $90C0
    DIV     C, [0]H             // C = C / $3040 (2byte)
    CMP     C, 3
    JNE     test-failed
    JZ      test-failed
    LD      C, $20406080
    DIV     C, [0]              // C = C / $10203040 (4byte)
    CMP     C, 2
    JNE     test-failed
    JZ      test-failed

    // レジスタ演算テスト
    LD      C, 67
    LD      A, 7
    DIV     C, A                // C = C / A
    CMP     C, 9
    JNE     test-failed
    JZ      test-failed

    LD      C, 48
    LD      B, 6
    DIV     C, B                // C = C / B
    CMP     C, 8
    JNE     test-failed
    JZ      test-failed

    LD      C, 33
    LD      D, 3
    DIV     C, D                // C = C / D
    CMP     C, 11
    JNE     test-failed
    JZ      test-failed

start-d:
    // リテラルのテスト
    LD      D, 1000
    DIV     D, 100              // D = D / 100 (1byte)
    CMP     D, 10
    JNE     test-failed
    JZ      test-failed
    DIV     D, 11               // D = D / 11 (1byte)
    JNZ     test-failed
    LD      D, 1024
    DIV     D, 256              // D = D / 256 (2byte)
    JZ      test-failed
    CMP     D, 4
    JNE     test-failed
    LD      D, 1048576
    DIV     D, 65536            // D = D / 65536 (4byte)
    CMP     D, 16
    JNE     test-failed
    JZ      test-failed

    // アドレス格納領域のテスト
    LD      D, $10203040
    ST      D, [0]
    LD      D, $80
    DIV     D, [0]O             // D = D / $40 (1byte)
    CMP     D, 2
    JNE     test-failed
    JZ      test-failed
    LD      D, $90C0
    DIV     D, [0]H             // D = D / $3040 (2byte)
    CMP     D, 3
    JNE     test-failed
    JZ      test-failed
    LD      D, $20406080
    DIV     D, [0]              // D = D / $10203040 (4byte)
    CMP     D, 2
    JNE     test-failed
    JZ      test-failed

    // レジスタ演算テスト
    LD      D, 67
    LD      A, 7
    DIV     D, A                // D = D / A
    CMP     D, 9
    JNE     test-failed
    JZ      test-failed

    LD      D, 48
    LD      B, 6
    DIV     D, B                // D = D / B
    CMP     D, 8
    JNE     test-failed
    JZ      test-failed

    LD      D, 33
    LD      C, 3
    DIV     D, C                // D = D / C
    CMP     D, 11
    JNE     test-failed
    JZ      test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
