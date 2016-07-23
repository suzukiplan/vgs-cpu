/*
    ADD A の テストプログラム
 */
start:
    // リテラルの加算テスト
    LD      A, 1000
    ADD     A, 0                // A = A + 0 (1byte)
    CMP     A, 1000
    JNE     test-failed
    JZ      test-failed
    ADD     A, 255              // A = A + 255 (1byte)
    CMP     A, 1255
    JNE     test-failed
    JZ      test-failed
    ADD     A, 256              // A = A + 256 (2byte)
    CMP     A, 1511
    JNE     test-failed
    JZ      test-failed
    ADD     A, 65535            // A = A + 65535 (2byte)
    CMP     A, 67046
    JNE     test-failed
    JZ      test-failed
    ADD     A, 65536            // A = A + 65536 (4byte)
    CMP     A, 132582
    JNE     test-failed
    JZ      test-failed
    ADD     A, -132582          // A = A - 132582 (4byte)
    JNZ     test-failed

    // アドレス格納領域の加算テスト
    LD      A, $12345678
    ST      A, [ 0 ]
    LD      A, 0
    ADD     A, [0]O             // A = A + $78(120) (1byte)
    CMP     A, $78
    JNE     test-failed
    JZ      test-failed
    ADD     A, -120             // A = A - 120 (4byte)
    JNZ     test-failed
    ADD     A, [ 0   ]H         // A = A + $5678(22136) (2byte)
    CMP     A, $5678
    JNE     test-failed
    JZ      test-failed
    ADD     A, -22136           // A = A - 22136 (4byte)
    JNZ     test-failed
    ADD     A, [0]              // A = A + $12345678(305419896) (4byte)
    CMP     A, $12345678
    JNE     test-failed
    JZ      test-failed
    ADD     A, -305419896       // A = A - 305419896 (4byte)
    JNZ     test-failed

    // レジスタ加算テスト
    LD      B, $11110000
    LD      C, $00001111
    LD      D, $11111111
    ADD     A, B                // A = A + B
    CMP     A, $11110000
    JNE     test-failed
    JZ      test-failed
    ADD     A, C                // A = A + C
    CMP     A, $11111111
    JNE     test-failed
    JZ      test-failed
    ADD     A, D                // A = A + D
    CMP     A, $22222222
    JNE     test-failed
    JZ      test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
