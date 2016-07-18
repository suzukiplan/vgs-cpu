/*
    PUSH/POP A の テストプログラム
 */

start:
    LD      A, $deadbeef
    PUSH    A
    PUSH    AH
    PUSH    AO

    POP     AO
    CMP     A, $ef
    JNE     test-failed

    POP     AH
    CMP     A, $beef
    JNE     test-failed

    POP     A
    CMP     A, $deadbeef
    JNE     test-failed

    LD      D, 1
    BRK

test-failed:
    INT     $00
    LD      D, -1
    BRK
