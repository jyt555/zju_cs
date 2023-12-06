        ; R3:head(left) address of the list
        ; R4:tail(right) address of the list
        ; R5:state('1' means loading '+','2' means '[')
        ; R7:pointer in 'START' stack
        .ORIG   x3000
        LD      R3,LIST ;initialization
        LD      R4,LIST
        LD      R7,START
        AND     R5,R5,#0
        STI     R5,LIST  ; '\0'
        STI     R5,START ; '\0'
        ;
READ    TRAP    x20
        TRAP    x21 ; echo
        LD      R2,NEW
        ADD     R2,R2,R0
        BRz     PUTOUT ; newline ?
        ADD     R5,R5,#0 ; state ?
        BRp     STORE
        ;
        ; judge
        LD      R1,ADDI
        ADD     R1,R0,R1
        BRnp    SKIP1 ; if the character is '+'?
        ADD     R5,R5,#1
        BRnzp   READ
SKIP1   LD      R1,DIF
        ADD     R1,R0,R1
        BRnp    SKIP2 ; if '-'?
        BRnzp   DL
SKIP2   LD      R1,LEFT
        ADD     R1,R0,R1
        BRnp    SKIP3 ; if '['?
        ADD     R5,R5,#2
        BRnzp   READ
SKIP3   LD      R1,RIGHT
        ADD     R1,R0,R1
        BRnp    STORE ; if ']'?
        BRnzp   DR
        ;
        ; store
STORE   ADD     R1,R5,#-1 ; R5 = 1 ?
        BRz     AL
        ADD     R1,R5,#-2 ; R5 = 2 ?
        BRz     AR
        ;
AL      ADD     R3,R3,#-1
        STR     R0,R3,#0
        BRnzp   AGA
AR      STR     R0,R4,#0
        ADD     R4,R4,#1
        AND     R1,R1,#0
        STR     R1,R4,#0 ; '\0'
        BRnzp   AGA
        ;
DL      LDR     R1,R3,#0
        BRnp    P
        LD      R0,UNDER
        STR     R0,R7,#0 ; store '_'
        BRnzp   Q
P       STR     R1,R7,#0
        ADD     R3,R3,#1
Q       ADD     R7,R7,#1
        BRnzp   READ
DR      NOT     R1,R3
        ADD     R1,R1,#1 ; -(R3)
        ADD     R1,R4,R1 ; R4-R3
        BRnp    X
        LD      R0,UNDER
        STR     R0,R7,#0 ; store '_'
        BRnzp   Y
X       ADD     R4,R4,#-1
        LDR     R1,R4,#0
        STR     R1,R7,#0
        AND     R1,R1,#0
        STR     R1,R4,#0 ; '/0'
Y       ADD     R7,R7,#1
AGA     AND     R5,R5,#0
        BRnzp   READ
        ;
        ; putout
PUTOUT  AND     R0,R0,#0
        STR     R0,R7,#0  ; '\0'
        LD      R0,START
        TRAP    x22    
        HALT
        ;
        ; data base
LIST    .FILL   x4000 ; stack to store
START   .FILL   x4100 ; stack to putout
NEWLINE .FILL   x000A ; newline
NEW     .FILL   xFFF6 ; -(newline)
UNDER   .FILL   x005F ; underline
ADDI    .FILL   xFFD5 ; -(+)
DIF     .FILL   xFFD3 ; -(-)
LEFT    .FILL   xFFA5 ; -([)
RIGHT   .FILL   xFFA3 ; -(])
        .END