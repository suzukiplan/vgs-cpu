/*
    PUSH/POP D の テストプログラム
 */

start:
    LD      D, $deadbeef
    PUSH    D
    PUSH    DH
    PUSH    DO

    POP     DO
    CMP     D, $ef
    JNE     test-failed

    POP     DH
    CMP     D, $beef
    JNE     test-failed

    POP     D
    CMP     D, $deadbeef
    JNE     test-failed

    LD      D, 1
    BRK

test-failed:
    LD      D, -1
    BRK
