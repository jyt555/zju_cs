.ORIG   x3000
MAIN
    LD      R6,USP      ; Init stack pointer
    AND     R0, R0, #0
    AND     R2, R2, #0
    AND     R3, R3, #0
    AND     R4, R4, #0
    ;
    ADD     R2,R2,#1    ; R2 = 1 for counter
    JSR     SAVE
    LDI     R5,N
    NOT     R5,R5
    ADD     R5,R5,#1    ; R5 = -N
    ST      R5,_N
    LDI     R5,M
    NOT     R5,R5
    ADD     R5,R5,#1    ; R5 = -M
    ST      R5,_M
    ;
RE1 LD      R5,_N
    ADD     R1,R3,R5    ; R3 - N
    BRz     PUTOUT
RE2 LD      R5,_M
    ADD     R1,R4,R5    ; R4 - M
    BRn     CON
    AND     R4,R4,#0    ; R4 = 0
    ADD     R3,R3,#1    ; R3++
    BR      RE1
CON JSR     RE
    ADD     R4,R4,#1    ; R4++
    AND     R2,R2,#0
    ADD     R2,R2,#1    ; R2 = 1
    BR      RE2

RE  ; recursion
    STR     R1,R6,#-1   ; save registers
    STR     R2,R6,#-2
    STR     R3,R6,#-3
    STR     R4,R6,#-4
    STR     R5,R6,#-5
    STR     R7,R6,#-6
    ADD     R6,R6,#-6
    JSR     LOADMAP
    ADD     R1,R0,#0    ; R1 = MAP[R3][R4]
    ;
UP  
    ADD     R3,R3,#0    ; R3 > 0 ?
    BRz     DOWN
    ADD     R3,R3,#-1
    JSR     LOADMAP     ; R0 = MAP'
    NOT     R5,R0
    ADD     R5,R5,#1    ; R5 = - MAP'
    ADD     R5,R1,R5    ; MAP - MAP'
    BRnz    U1
    ADD     R2,R2,#1
    JSR     SAVE
    JSR     RE
    ADD     R2,R2,#-1
U1  ADD     R3,R3,#1
DOWN
    LD      R5,_N
    ADD     R5,R5,#1
    ADD     R5,R3,R5    ; R3 - N + 1
    BRz     LEFT
    ADD     R3,R3,#1
    JSR     LOADMAP     ; R0 = MAP'
    NOT     R5,R0
    ADD     R5,R5,#1    ; R5 = - MAP'
    ADD     R5,R1,R5    ; MAP - MAP'
    BRnz    D1
    ADD     R2,R2,#1
    JSR     SAVE
    JSR     RE
    ADD     R2,R2,#-1
D1  ADD     R3,R3,#-1
LEFT
    ADD     R4,R4,#0    ; R4 > 0 ?
    BRz     RIGHT
    ADD     R4,R4,#-1
    JSR     LOADMAP     ; R0 = MAP'
    NOT     R5,R0
    ADD     R5,R5,#1    ; R5 = - MAP'
    ADD     R5,R1,R5    ; MAP - MAP'
    BRnz    L1
    ADD     R2,R2,#1
    JSR     SAVE
    JSR     RE
    ADD     R2,R2,#-1
L1  ADD     R4,R4,#1
RIGHT
    LD      R5,_M
    ADD     R5,R5,#1
    ADD     R5,R4,R5    ; R4 - M + 1
    BRz     OVER
    ADD     R4,R4,#1
    JSR     LOADMAP     ; R0 = MAP'
    NOT     R5,R0
    ADD     R5,R5,#1    ; R5 = - MAP'
    ADD     R5,R1,R5    ; MAP - MAP'
    BRnz    T1
    ADD     R2,R2,#1
    JSR     SAVE
    JSR     RE
    ADD     R2,R2,#-1
T1  ADD     R4,R4,#-1
    ;
OVER ; over this recursion
    ADD     R6,R6,#6    ; restore registers
    LDR     R1,R6,#-1
    LDR     R2,R6,#-2
    LDR     R3,R6,#-3
    LDR     R4,R6,#-4
    LDR     R5,R6,#-5
    LDR     R7,R6,#-6
    RET
    ;
PUTOUT
    LD      R2,MAX      ; R2 <- MAX
    HALT

; subroutine
LOADMAP ; R0 = MAP[R3][R4]
    STR     R1,R6,#-1
    STR     R2,R6,#-2
    STR     R3,R6,#-3
    STR     R5,R6,#-4
    STR     R7,R6,#-5
    ;
    AND     R5,R5,#0    ; offset R5 = 0
    AND     R1,R1,#0
    LDI     R2,M
    NOT     R3,R3
    ADD     R3,R3,#1    ; R3 = -R3
LO1 ADD     R7,R1,R3    ; R1 - R3
    BRz     LO2
    ADD     R5,R5,R2
    ADD     R1,R1,#1
    BR      LO1
LO2 ADD     R5,R5,R4    ; R5 = offset
    LD      R7,MAP
    ADD     R5,R5,R7    ; R5 is the address
    LDR     R0,R5,#0    ; R0 = MAP[R3][R4]
    ;
    LDR     R1,R6,#-1
    LDR     R2,R6,#-2
    LDR     R3,R6,#-3
    LDR     R5,R6,#-4
    LDR     R7,R6,#-5
    RET

SAVE ; if R2 > MAX, MAX <- R2
    STR     R5,R6,#-1
    ;
    LD      R5,MAX
    NOT     R5,R5
    ADD     R5,R5,#1    ; R5 = - max
    ADD     R5,R2,R5    ; R2 - max
    BRnz    S
    ST      R2,MAX
    ;
S   LDR     R5,R6,#-1
    RET

; data base
USP .FILL   xFDFF
N   .FILL   x4000
M   .FILL   x4001
_N  .BLKW   #1
_M  .BLKW   #1
MAP .FILL   x4002
MAX .BLKW   #1
.END

.ORIG   x4000
    .FILL   #3 ; N
    .FILL   #4 ; M
    .FILL   89 ; map
    .FILL   88
    .FILL   86
    .FILL   83
    .FILL   79
    .FILL   73
    .FILL   90
    .FILL   80
    .FILL   60
    .FILL   69
    .FILL   73
    .FILL   77
    .FILL   6
    .FILL   9
    .FILL   13
.END

