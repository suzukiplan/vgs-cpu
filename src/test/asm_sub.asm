/*
    SUB の テストプログラム
 */
start-a:
    // リテラルのテスト
    LD      A, 1000
    SUB     A, 100              // A = A - 100 (1byte)
    CMP     A, 900
    JNE     test-failed
    JZ      test-failed
    SUB     A, 255              // A = A - 255 (1byte)
    CMP     A, 645
    JNE     test-failed
    JZ      test-failed
    SUB     A, 256              // A = A - 256 (2byte)
    CMP     A, 389
    JNE     test-failed
    JZ      test-failed
    SUB     A, 389              // A = A - 389 (2byte)
    JNZ     test-failed
    SUB     A, 65535            // A = A + 65535 (2byte)
    CMP     A, -65535
    JNE     test-failed
    JZ      test-failed
    SUB     A, 65536            // A = A - 65536 (4byte)
    CMP     A, -131071
    JNE     test-failed
    JZ      test-failed
    SUB     A, -131071          // A = A - (-131071) (4byte)
    JNZ     test-failed

    // アドレス格納領域のテスト
    LD      A, $12345678
    ST      A, [0]
    LD      A, 0
    SUB     A, [0]O             // A = A - $78(120) (1byte)
    CMP     A, -120
    JNE     test-failed
    JZ      test-failed
    SUB     A, -120             // A = A - (-120) (4byte)
    JNZ     test-failed
    SUB     A, [0]H             // A = A - $5678(22136) (2byte)
    CMP     A, -22136
    JNE     test-failed
    JZ      test-failed
    SUB     A, -22136           // A = A - (-22136) (4byte)
    JNZ     test-failed
    SUB     A, [0]              // A = A - $12345678(305419896) (4byte)
    CMP     A, -305419896
    JNE     test-failed
    JZ      test-failed
    SUB     A, -305419896       // A = A - (-305419896) (4byte)
    JNZ     test-failed

    // レジスタテスト
    LD      A, $22222222
    LD      B, $11110000
    LD      C, $00001111
    LD      D, $11111111
    SUB     A, B                // A = A - B
    CMP     A, $11112222
    JNE     test-failed
    JZ      test-failed
    SUB     A, C                // A = A - C
    CMP     A, $11111111
    JNE     test-failed
    JZ      test-failed
    SUB     A, D                // A = A - D
    JNZ     test-failed

start-b:
    // リテラルのテスト
    LD      B, 1000
    SUB     B, 100              // B = B - 100 (1byte)
    CMP     B, 900
    JNE     test-failed
    JZ      test-failed
    SUB     B, 255              // B = B - 255 (1byte)
    CMP     B, 645
    JNE     test-failed
    JZ      test-failed
    SUB     B, 256              // B = B - 256 (2byte)
    CMP     B, 389
    JNE     test-failed
    JZ      test-failed
    SUB     B, 389              // B = B - 389 (2byte)
    JNZ     test-failed
    SUB     B, 65535            // B = B + 65535 (2byte)
    CMP     B, -65535
    JNE     test-failed
    JZ      test-failed
    SUB     B, 65536            // B = B - 65536 (4byte)
    CMP     B, -131071
    JNE     test-failed
    JZ      test-failed
    SUB     B, -131071          // B = B - (-131071) (4byte)
    JNZ     test-failed

    // アドレス格納領域のテスト
    LD      B, $12345678
    ST      B, [0]
    LD      B, 0
    SUB     B, [0]O             // B = B - $78(120) (1byte)
    CMP     B, -120
    JNE     test-failed
    JZ      test-failed
    SUB     B, -120             // B = B - (-120) (4byte)
    JNZ     test-failed
    SUB     B, [0]H             // B = B - $5678(22136) (2byte)
    CMP     B, -22136
    JNE     test-failed
    JZ      test-failed
    SUB     B, -22136           // B = B - (-22136) (4byte)
    JNZ     test-failed
    SUB     B, [0]              // B = B - $12345678(305419896) (4byte)
    CMP     B, -305419896
    JNE     test-failed
    JZ      test-failed
    SUB     B, -305419896       // B = B - (-305419896) (4byte)
    JNZ     test-failed

    // レジスタテスト
    LD      B, $22222222
    LD      A, $11110000
    LD      C, $00001111
    LD      D, $11111111
    SUB     B, A                // B = B - A
    CMP     B, $11112222
    JNE     test-failed
    JZ      test-failed
    SUB     B, C                // B = B - C
    CMP     B, $11111111
    JNE     test-failed
    JZ      test-failed
    SUB     B, D                // B = B - D
    JNZ     test-failed

start-c:
    // リテラルのテスト
    LD      C, 1000
    SUB     C, 100              // C = C - 100 (1byte)
    CMP     C, 900
    JNE     test-failed
    JZ      test-failed
    SUB     C, 255              // C = C - 255 (1byte)
    CMP     C, 645
    JNE     test-failed
    JZ      test-failed
    SUB     C, 256              // C = C - 256 (2byte)
    CMP     C, 389
    JNE     test-failed
    JZ      test-failed
    SUB     C, 389              // C = C - 389 (2byte)
    JNZ     test-failed
    SUB     C, 65535            // C = C + 65535 (2byte)
    CMP     C, -65535
    JNE     test-failed
    JZ      test-failed
    SUB     C, 65536            // C = C - 65536 (4byte)
    CMP     C, -131071
    JNE     test-failed
    JZ      test-failed
    SUB     C, -131071          // C = C - (-131071) (4byte)
    JNZ     test-failed

    // アドレス格納領域のテスト
    LD      C, $12345678
    ST      C, [0]
    LD      C, 0
    SUB     C, [0]O             // C = C - $78(120) (1byte)
    CMP     C, -120
    JNE     test-failed
    JZ      test-failed
    SUB     C, -120             // C = C - (-120) (4byte)
    JNZ     test-failed
    SUB     C, [0]H             // C = C - $5678(22136) (2byte)
    CMP     C, -22136
    JNE     test-failed
    JZ      test-failed
    SUB     C, -22136           // C = C - (-22136) (4byte)
    JNZ     test-failed
    SUB     C, [0]              // C = C - $12345678(305419896) (4byte)
    CMP     C, -305419896
    JNE     test-failed
    JZ      test-failed
    SUB     C, -305419896       // C = C - (-305419896) (4byte)
    JNZ     test-failed

    // レジスタテスト
    LD      C, $22222222
    LD      A, $11110000
    LD      B, $00001111
    LD      D, $11111111
    SUB     C, A                // C = C - A
    CMP     C, $11112222
    JNE     test-failed
    JZ      test-failed
    SUB     C, B                // C = C - B
    CMP     C, $11111111
    JNE     test-failed
    JZ      test-failed
    SUB     C, D                // C = C - D
    JNZ     test-failed

start-d:
    // リテラルのテスト
    LD      D, 1000
    SUB     D, 100              // D = D - 100 (1byte)
    CMP     D, 900
    JNE     test-failed
    JZ      test-failed
    SUB     D, 255              // D = D - 255 (1byte)
    CMP     D, 645
    JNE     test-failed
    JZ      test-failed
    SUB     D, 256              // D = D - 256 (2byte)
    CMP     D, 389
    JNE     test-failed
    JZ      test-failed
    SUB     D, 389              // D = D - 389 (2byte)
    JNZ     test-failed
    SUB     D, 65535            // D = D + 65535 (2byte)
    CMP     D, -65535
    JNE     test-failed
    JZ      test-failed
    SUB     D, 65536            // D = D - 65536 (4byte)
    CMP     D, -131071
    JNE     test-failed
    JZ      test-failed
    SUB     D, -131071          // D = D - (-131071) (4byte)
    JNZ     test-failed

    // アドレス格納領域のテスト
    LD      D, $12345678
    ST      D, [0]
    LD      D, 0
    SUB     D, [0]O             // D = D - $78(120) (1byte)
    CMP     D, -120
    JNE     test-failed
    JZ      test-failed
    SUB     D, -120             // D = D - (-120) (4byte)
    JNZ     test-failed
    SUB     D, [0]H             // D = D - $5678(22136) (2byte)
    CMP     D, -22136
    JNE     test-failed
    JZ      test-failed
    SUB     D, -22136           // D = D - (-22136) (4byte)
    JNZ     test-failed
    SUB     D, [0]              // D = D - $12345678(305419896) (4byte)
    CMP     D, -305419896
    JNE     test-failed
    JZ      test-failed
    SUB     D, -305419896       // D = D - (-305419896) (4byte)
    JNZ     test-failed

    // レジスタテスト
    LD      D, $22222222
    LD      A, $11110000
    LD      B, $00001111
    LD      C, $11111111
    SUB     D, A                // D = D - A
    CMP     D, $11112222
    JNE     test-failed
    JZ      test-failed
    SUB     D, B                // D = D - B
    CMP     D, $11111111
    JNE     test-failed
    JZ      test-failed
    SUB     D, C                // D = D - C
    JNZ     test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
