/*
    ADD D の テストプログラム
 */
start:
    // リテラルの加算テスト
    LD      D, 1000
    ADD     D, 0                // D = D + 0 (1byte)
    CMP     D, 1000
    JNE     test-failed
    JZ      test-failed
    ADD     D, 255              // D = D + 255 (1byte)
    CMP     D, 1255
    JNE     test-failed
    JZ      test-failed
    ADD     D, 256              // D = D + 256 (2byte)
    CMP     D, 1511
    JNE     test-failed
    JZ      test-failed
    ADD     D, 65535            // D = D + 65535 (2byte)
    CMP     D, 67046
    JNE     test-failed
    JZ      test-failed
    ADD     D, 65536            // D = D + 65536 (4byte)
    CMP     D, 132582
    JNE     test-failed
    JZ      test-failed
    ADD     D, -132582          // D = D - 132582 (4byte)
    JNZ     test-failed

    // アドレス格納領域の加算テスト
    LD      D, $12345678
    ST      D, [0]
    LD      D, 0
    ADD     D, [0]O             // D = D + $78(120) (1byte)
    CMP     D, $78
    JNE     test-failed
    JZ      test-failed
    ADD     D, -120             // D = D - 120 (4byte)
    JNZ     test-failed
    ADD     D, [0]H             // D = D + $5678(22136) (2byte)
    CMP     D, $5678
    JNE     test-failed
    JZ      test-failed
    ADD     D, -22136           // D = D - 22136 (4byte)
    JNZ     test-failed
    ADD     D, [0]              // D = D + $12345678(305419896) (4byte)
    CMP     D, $12345678
    JNE     test-failed
    JZ      test-failed
    ADD     D, -305419896       // D = D - 305419896 (4byte)
    JNZ     test-failed

    // レジスタ加算テスト
    LD      A, $11110000
    LD      B, $00001111
    LD      C, $11111111
    ADD     D, A                // D = D + A
    CMP     D, $11110000
    JNE     test-failed
    JZ      test-failed
    ADD     D, B                // D = D + B
    CMP     D, $11111111
    JNE     test-failed
    JZ      test-failed
    ADD     D, C                // D = D + C
    CMP     D, $22222222
    JNE     test-failed
    JZ      test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
