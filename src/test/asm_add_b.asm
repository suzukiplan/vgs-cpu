/*
    ADD B の テストプログラム
 */
start:
    // リテラルの加算テスト
    LD      B, 1000
    ADD     B, 0                // B = B + 0 (1byte)
    CMP     B, 1000
    JNE     test-failed
    JZ      test-failed
    ADD     B, 255              // B = B + 255 (1byte)
    CMP     B, 1255
    JNE     test-failed
    JZ      test-failed
    ADD     B, 256              // B = B + 256 (2byte)
    CMP     B, 1511
    JNE     test-failed
    JZ      test-failed
    ADD     B, 65535            // B = B + 65535 (2byte)
    CMP     B, 67046
    JNE     test-failed
    JZ      test-failed
    ADD     B, 65536            // B = B + 65536 (4byte)
    CMP     B, 132582
    JNE     test-failed
    JZ      test-failed
    ADD     B, -132582          // B = B - 132582 (4byte)
    JNZ     test-failed

    // アドレス格納領域の加算テスト
    LD      B, $12345678
    ST      B, [0]
    LD      B, 0
    ADD     B, [0]O             // B = B + $78(120) (1byte)
    CMP     B, $78
    JNE     test-failed
    JZ      test-failed
    ADD     B, -120             // B = B - 120 (4byte)
    JNZ     test-failed
    ADD     B, [0]H             // B = B + $5678(22136) (2byte)
    CMP     B, $5678
    JNE     test-failed
    JZ      test-failed
    ADD     B, -22136           // B = B - 22136 (4byte)
    JNZ     test-failed
    ADD     B, [0]              // B = B + $12345678(305419896) (4byte)
    CMP     B, $12345678
    JNE     test-failed
    JZ      test-failed
    ADD     B, -305419896       // B = B - 305419896 (4byte)
    JNZ     test-failed

    // レジスタ加算テスト
    LD      A, $11110000
    LD      C, $00001111
    LD      D, $11111111
    ADD     B, A                // B = B + A
    CMP     B, $11110000
    JNE     test-failed
    JZ      test-failed
    ADD     B, C                // B = B + C
    CMP     B, $11111111
    JNE     test-failed
    JZ      test-failed
    ADD     B, D                // B = B + D
    CMP     B, $22222222
    JNE     test-failed
    JZ      test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
