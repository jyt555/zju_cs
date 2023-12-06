        ; R2:the initial position of 'bird'
        ; R3:the current position to print
        ; R7:counter
        ;
; system booting code
        .ORIG   x0200
        LD      R6,OS_SP
        ;
        LD      R0,USER_PSR
        ADD     R6,R6,#-1
        STR     R0,R6,#0
        ;
        LD      R0,USER_PC
        ADD     R6,R6,#-1
        STR     R0,R6,#0
        ;
        LD      R1,ENTRY
        STI     R1,IVA  ; mem[IVA] <- ENTRY
        ;
        LD      R1,MASK
        LDI     R4,JKBSR
        STR     R1,R4,#0 ; IE
        RTI
        ;
        ; data base
OS_SP    .FILL  x3000
USER_PSR .FILL  x8002
USER_PC  .FILL  x3000
JKBSR    .FILL  KBSR
MASK     .FILL  x4000   ; to set IE
IVA      .FILL  x0180   ; the address from INTV
ENTRY    .FILL  x2000   ; the address of interrupt service routine
        .END
        ;
; interrupt service routine
        .ORIG   x2000
READY   LDI     R5,KBSR
        BRzp    READY   ; interrupt ?
        LDI     R5,KBDR ; input -> R5
        LD      R4,NUMS
        ADD     R4,R5,R4
        BRn     READY   ; R5 < '0'
        LD      R4,NUML
        ADD     R4,R5,R4
        BRnz    MOVE    ; ('0' <=) R5 <= '9'
        LD      R4,CHAS
        ADD     R4,R5,R4
        BRn     READY   ; R5 < 'a'
        LD      R4,CHAL
        ADD     R4,R5,R4
        BRnz    CHANGE  ; ('a' <=) R5 <= 'z'
        BRnzp   READY
        ;
MOVE    LD      R4,NUMS
        ADD     R5,R5,R4
        ADD     R2,R2,R5
        RTI
CHANGE  LD      R4,CHAS
        ADD     R1,R5,R4 
        ADD     R1,R1,#1 ; offset (from 1)
        STI     R1,JBIRD
        RTI    
        ;
        ; data base
KBSR    .FILL   xFE00
KBDR    .FILL   xFE02
JBIRD   .FILL   BIRD
NUMS    .FILL   xFFD0 ; -48  neg'0'
NUML    .FILL   xFFC7 ; -57  neg'9'
CHAS    .FILL   xFF9F ; -97  neg'a'
CHAL    .FILL   xFF86 ; -122 neg'z'
        .END
        ;
; user program
        .ORIG   x3000
        AND     R3,R3,#0
        ADD     R3,R3,#1 ; R3 = 1
        ST      R3,BIRD
    ;[?]    LDR     R1,R3,#0 ; R1 = 1
        LD      R2,RMOST ; start from right
        ;
PRINT   LD      R7,NUM   ; counter = 20
        LD      R5,SIDE  ; R5 = -18
        ADD     R5,R5,R2
        BRnz    SKIP1    ; R2 <= 18
        LD      R2,RMOST ; R2 = 18
SKIP1   ADD     R5,R2,#0
        BRp     SKIP2    ; R2 >= 1
        AND     R2,R2,#0
        ADD     R2,R2,#1 ; R2 = 1
SKIP2   AND     R3,R3,#0 
        ADD     R3,R3,#1 ; initialization R3 = 1
        NOT     R4,R2
        ADD     R4,R4,#1 ; R4 = -R2
AGA     ADD     R7,R7,#0
        BRz     OVER     ; counter = 0
        ADD     R5,R3,R4
        BRn     PAIR     ; R3 < R2
        ADD     R5,R5,#-2
        BRp     PAIR     ; R3 > R2 + 2
        LD      R0,BASE
        LD      R1,BIRD
        ADD     R0,R0,R1
        TRAP    x21      ; print 'bird'
        ADD     R7,R7,#-1
        ADD     R3,R3,#1
        BRnzp   AGA
PAIR    LD      R0,AIR
        TRAP    x21
        ADD     R7,R7,#-1
        ADD     R3,R3,#1
        BRnzp   AGA
        ;
OVER    LD      R0,NEWLINE
        TRAP    x21
        JSR     DELAY
        ADD     R2,R2,#-1
        BRnzp   PRINT
        ;
DELAY   ST      R1,SAVER1
        LD      R1,COUNT
LOOP    ADD     R1,R1,#-1
        BRnp    LOOP
        LD      R1,SAVER1
        RET
        ;
        ; data base
NUM     .FILL   #20     ; for counter
BASE    .FILL   #96
SIDE    .FILL   #-18
RMOST   .FILL   #18
BIRD    .BLKW   #1
NEWLINE .FILL   x000A   ; ASCII for '/n'
AIR     .FILL   x002E   ; ASCII for '.'
COUNT   .FILL   #5000   ; for delay
SAVER1  .BLKW   #1
        .END
