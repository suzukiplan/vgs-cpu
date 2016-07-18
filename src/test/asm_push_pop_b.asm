/*
    PUSH/POP B の テストプログラム
 */

start:
    LD      B, $deadbeef
    PUSH    B
    PUSH    BH
    PUSH    BO

    POP     BO
    CMP     B, $ef
    JNE     test-failed

    POP     BH
    CMP     B, $beef
    JNE     test-failed

    POP     B
    CMP     B, $deadbeef
    JNE     test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
