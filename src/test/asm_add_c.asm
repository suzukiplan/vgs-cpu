/*
    ADD C の テストプログラム
 */
start:
    // リテラルの加算テスト
    LD      C, 1000
    ADD     C, 0                // C = C + 0 (1byte)
    CMP     C, 1000
    JNE     test-failed
    JZ      test-failed
    ADD     C, 255              // C = C + 255 (1byte)
    CMP     C, 1255
    JNE     test-failed
    JZ      test-failed
    ADD     C, 256              // C = C + 256 (2byte)
    CMP     C, 1511
    JNE     test-failed
    JZ      test-failed
    ADD     C, 65535            // C = C + 65535 (2byte)
    CMP     C, 67046
    JNE     test-failed
    JZ      test-failed
    ADD     C, 65536            // C = C + 65536 (4byte)
    CMP     C, 132582
    JNE     test-failed
    JZ      test-failed
    ADD     C, -132582          // C = C - 132582 (4byte)
    JNZ     test-failed

    // アドレス格納領域の加算テスト
    LD      C, $12345678
    ST      C, [0]
    LD      C, 0
    ADD     C, [0]O             // C = C + $78(120) (1byte)
    CMP     C, $78
    JNE     test-failed
    JZ      test-failed
    ADD     C, -120             // C = C - 120 (4byte)
    JNZ     test-failed
    ADD     C, [0]H             // C = C + $5678(22136) (2byte)
    CMP     C, $5678
    JNE     test-failed
    JZ      test-failed
    ADD     C, -22136           // C = C - 22136 (4byte)
    JNZ     test-failed
    ADD     C, [0]              // C = C + $12345678(305419896) (4byte)
    CMP     C, $12345678
    JNE     test-failed
    JZ      test-failed
    ADD     C, -305419896       // C = C - 305419896 (4byte)
    JNZ     test-failed

    // レジスタ加算テスト
    LD      A, $11110000
    LD      B, $00001111
    LD      D, $11111111
    ADD     C, A                // C = C + A
    CMP     C, $11110000
    JNE     test-failed
    JZ      test-failed
    ADD     C, B                // C = C + B
    CMP     C, $11111111
    JNE     test-failed
    JZ      test-failed
    ADD     C, D                // C = C + D
    CMP     C, $22222222
    JNE     test-failed
    JZ      test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
