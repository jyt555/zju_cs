    .ORIG x3000
        LDI R0,A1
        ST  R0,PTR1
        LDI R0,A2
        ST  R0,PTR2 
    ; Uppercase to Lowercase
    ; str1
        LDI R2,PTR1 ; R2 <- str1
        LD  R4,PTR1 ; R4 <- addr1
        AND R5,R5,#0; R5/offset = 0
UTL1    LD  R3,P1
        ADD R3,R2,R3 ; R2-96
        BRn SKIP1
        LD  R3,P2
        ADD R2,R2,R3 ; R2-32
        LD  R3,PTR1
        ADD R3,R3,R5
        STR R2,R3,#0
SKIP1   ADD R4,R4,#1
        ADD R5,R5,#1
        LDR R2,R4,#0
        BRp UTL1
    ; str2
        AND R4,R4,#0; R4/offset = 0
        LDI R3,PTR2 ; R3 <- str2
        LD  R5,PTR2 ; R5 <- addr2
UTL2    LD  R2,P1
        ADD R2,R3,R2
        BRn SKIP2
        LD  R2,P2
        ADD R3,R2,R3
        LD  R2,PTR2
        ADD R2,R2,R4
        STR R3,R2,#0
SKIP2   ADD R4,R4,#1
        ADD R5,R5,#1
        LDR R3,R5,#0
        BRp UTL2
    ;
    ; initialization
        LD  R4,PTR1 ; R4 <- addr1
        LD  R5,PTR2 ; R5 <- addr2
        AND R6,R6,#0; R6/offset1 = 0
        AND R7,R7,#0; R7/offset2 = 0
        ST  R6,NUM1 ; NUM1 = 0
        ST  R6,NUM2 ; NUM2 = 0
    ;
    ; search
AGA     LD  R0,PTR1
        ADD R0,R0,R6
        LDR R2,R0,#0 ; reset R2/str1
        BRz CHARGE
        LD  R0,P2
        ADD R0,R0,R2 ; sp?
        BRz ADD1
        LD  R0,PTR2
        ADD R0,R0,R7
        LDR R3,R0,#0 ; reset R3/str2
        BRz DIF
        LD  R0,P2
        ADD R0,R0,R3 ; sp?
        BRz ADD2
        NOT R3,R3
        ADD R3,R3,#1 ; R3' = -R3
        ADD R0,R2,R3 ; R2-R3
        BRnp ADD2
        LD  R3,SP   ; store
        LD  R0,PTR2
        ADD R0,R0,R7
        STR R3,R0,#0 ; str2 -> sp
        LD  R0,NUM2  ; v
        ADD R0,R0,#1 ; v
        ST  R0,NUM2  ; NUM2++
        BRnzp SETR1
    ;
SETR1   AND R7,R7,#0
        LD  R0,NUM1  ; v
        ADD R0,R0,#1 ; v
        ST  R0,NUM1  ; NUM1++
ADD1    ADD R6,R6,#1
        BRnzp AGA
ADD2    ADD R7,R7,#1
        BRnzp AGA
    ;
    ; charge
CHARGE  LD  R6,NUM1 ; test
        LD  R7,NUM2 ; special case:
        NOT R7,R7   ; str1 is longer
        ADD R7,R7,#1; than str2.
        ADD R0,R6,R7
        BRnp DIF
        LD  R5,PTR2 ; R5 <- addr2
        AND R7,R7,#0; offset2 = 0
RE      LD  R0,PTR2
        ADD R0,R0,R7
        LDR R3,R0,#0 ; reset R3/str2
        BRz SAME
        LD  R0,P2
        ADD R0,R0,R3
        BRnp DIF
        ADD R7,R7,#1
        BRnzp RE
    ;
    ; output
DIF     LD  R0,N
        BRnzp STOP
SAME    LD  R0,Y
        BRnzp STOP
    ;
STOP    TRAP x22 ; put
        TRAP x25 ; halt
    ;
    ; data base
P1      .FILL #-96
P2      .FILL #-32
SP      .FILL #32
NUM1    .BLKW #1
NUM2    .BLKW #1
A1      .FILL x4000
A2      .FILL x4001
PTR1    .BLKW #1
PTR2    .BLKW #1
Y       .FILL YES
N       .FILL NO
YES     .STRINGZ "YES"
NO      .STRINGZ "NO"
        .END
;
    .ORIG x4000
        .FILL STR1
        .FILL STR2
Str1    .STRINGZ "Lids efH dls  "
STR2    .STRINGZ "sLi dfes hld "
    .END