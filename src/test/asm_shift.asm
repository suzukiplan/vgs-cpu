/*
    シフト演算 の テストプログラム
 */
start:
    // SHR A
    LD      A, B10110011100011110000111110000011
    SHR     A, 1
    JZ      test-failed
    CMP     A, B01011001110001111000011111000001
    JNE     test-failed
    SHR     A, 2
    JZ      test-failed
    CMP     A, B00010110011100011110000111110000
    JNE     test-failed
    SHR     A, 4
    JZ      test-failed
    CMP     A, B00000001011001110001111000011111
    JNE     test-failed
    SHR     A, 8
    JZ      test-failed
    CMP     A, B00000000000000010110011100011110
    JNE     test-failed
    SHR     A, 16
    JZ      test-failed
    CMP     A, B00000000000000000000000000000001
    JNE     test-failed
    SHR     A, 1
    JNZ     test-failed

    // SHL A
    LD      A, B10110011100011110000111110000011
    SHL     A, 1
    JZ      test-failed
    CMP     A, B01100111000111100001111100000110
    JNE     test-failed
    SHL     A, 2
    JZ      test-failed
    CMP     A, B10011100011110000111110000011000
    JNE     test-failed
    SHL     A, 4
    JZ      test-failed
    CMP     A, B11000111100001111100000110000000
    JNE     test-failed
    SHL     A, 8
    JZ      test-failed
    CMP     A, B10000111110000011000000000000000
    JNE     test-failed
    SHL     A, 16
    JZ      test-failed
    CMP     A, B10000000000000000000000000000000
    JNE     test-failed
    SHL     A, 1
    JZ      test-succeed

    // SHR B
    LD      B, B10110011100011110000111110000011
    SHR     B, 1
    JZ      test-failed
    CMP     B, B01011001110001111000011111000001
    JNE     test-failed
    SHR     B, 2
    JZ      test-failed
    CMP     B, B00010110011100011110000111110000
    JNE     test-failed
    SHR     B, 4
    JZ      test-failed
    CMP     B, B00000001011001110001111000011111
    JNE     test-failed
    SHR     B, 8
    JZ      test-failed
    CMP     B, B00000000000000010110011100011110
    JNE     test-failed
    SHR     B, 16
    JZ      test-failed
    CMP     B, B00000000000000000000000000000001
    JNE     test-failed
    SHR     B, 1
    JNZ     test-failed

    // SHL B
    LD      B, B10110011100011110000111110000011
    SHL     B, 1
    JZ      test-failed
    CMP     B, B01100111000111100001111100000110
    JNE     test-failed
    SHL     B, 2
    JZ      test-failed
    CMP     B, B10011100011110000111110000011000
    JNE     test-failed
    SHL     B, 4
    JZ      test-failed
    CMP     B, B11000111100001111100000110000000
    JNE     test-failed
    SHL     B, 8
    JZ      test-failed
    CMP     B, B10000111110000011000000000000000
    JNE     test-failed
    SHL     B, 16
    JZ      test-failed
    CMP     B, B10000000000000000000000000000000
    JNE     test-failed
    SHL     B, 1
    JZ      test-succeed

    // SHR C
    LD      C, B10110011100011110000111110000011
    SHR     C, 1
    JZ      test-failed
    CMP     C, B01011001110001111000011111000001
    JNE     test-failed
    SHR     C, 2
    JZ      test-failed
    CMP     C, B00010110011100011110000111110000
    JNE     test-failed
    SHR     C, 4
    JZ      test-failed
    CMP     C, B00000001011001110001111000011111
    JNE     test-failed
    SHR     C, 8
    JZ      test-failed
    CMP     C, B00000000000000010110011100011110
    JNE     test-failed
    SHR     C, 16
    JZ      test-failed
    CMP     C, B00000000000000000000000000000001
    JNE     test-failed
    SHR     C, 1
    JNZ     test-failed

    // SHL C
    LD      C, B10110011100011110000111110000011
    SHL     C, 1
    JZ      test-failed
    CMP     C, B01100111000111100001111100000110
    JNE     test-failed
    SHL     C, 2
    JZ      test-failed
    CMP     C, B10011100011110000111110000011000
    JNE     test-failed
    SHL     C, 4
    JZ      test-failed
    CMP     C, B11000111100001111100000110000000
    JNE     test-failed
    SHL     C, 8
    JZ      test-failed
    CMP     C, B10000111110000011000000000000000
    JNE     test-failed
    SHL     C, 16
    JZ      test-failed
    CMP     C, B10000000000000000000000000000000
    JNE     test-failed
    SHL     C, 1
    JZ      test-succeed

    // SHR D
    LD      D, B10110011100011110000111110000011
    SHR     D, 1
    JZ      test-failed
    CMP     D, B01011001110001111000011111000001
    JNE     test-failed
    SHR     D, 2
    JZ      test-failed
    CMP     D, B00010110011100011110000111110000
    JNE     test-failed
    SHR     D, 4
    JZ      test-failed
    CMP     D, B00000001011001110001111000011111
    JNE     test-failed
    SHR     D, 8
    JZ      test-failed
    CMP     D, B00000000000000010110011100011110
    JNE     test-failed
    SHR     D, 16
    JZ      test-failed
    CMP     D, B00000000000000000000000000000001
    JNE     test-failed
    SHR     D, 1
    JNZ     test-failed

    // SHL D
    LD      D, B10110011100011110000111110000011
    SHL     D, 1
    JZ      test-failed
    CMP     D, B01100111000111100001111100000110
    JNE     test-failed
    SHL     D, 2
    JZ      test-failed
    CMP     D, B10011100011110000111110000011000
    JNE     test-failed
    SHL     D, 4
    JZ      test-failed
    CMP     D, B11000111100001111100000110000000
    JNE     test-failed
    SHL     D, 8
    JZ      test-failed
    CMP     D, B10000111110000011000000000000000
    JNE     test-failed
    SHL     D, 16
    JZ      test-failed
    CMP     D, B10000000000000000000000000000000
    JNE     test-failed
    SHL     D, 1
    JZ      test-succeed

test-failed:
    INT     $00
    LD      D, -1
    BRK

test-succeed:
    LD      D, 1
    BRK
