/*
    PUSH/POP C の テストプログラム
 */

start:
    LD      C, $deadbeef
    PUSH    C
    PUSH    CH
    PUSH    CO

    POP     CO
    CMP     C, $ef
    JNE     test-failed

    POP     CH
    CMP     C, $beef
    JNE     test-failed

    POP     C
    CMP     C, $deadbeef
    JNE     test-failed

    LD      D, 1
    BRK

test-failed:
    LD      D, -1
    BRK
