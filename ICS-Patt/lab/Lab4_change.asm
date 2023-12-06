        ;注释后面带[]的是后来加的FLAG，用来防止在一行内输出的时候被打断
        ;但是现在还是有点问题，懒得改了，再说吧
        ;除了这个FLAG，还有一种防打断方式是修改KBSR的14位
        ;但是用户模式下不能修改这个
        ;所以要自己编写一条TRAP用来改这个
        ;
        ;x2000里面，原来跳转READY的全部改成R（地址变）了，这里不太适合用循环
        ;
        ;R1可以直接做全局变量，里面存字母的ASCII码，没必要用offset多此一举
        
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
        STR     R4,R6,#-1
        STR     R5,R6,#-2
        ;
        LDI     R5,KBDR ; input -> R5
        LDI     R4,JFLAG ; []
        BRz     R       ; FLAG = 0
        LD      R4,NUMS
        ADD     R4,R5,R4
        BRn     R       ; R5 < '0'
        LD      R4,NUML
        ADD     R4,R5,R4
        BRnz    MOVE    ; ('0' <=) R5 <= '9'
        LD      R4,CHAS
        ADD     R4,R5,R4
        BRn     R       ; R5 < 'a'
        LD      R4,CHAL
        ADD     R4,R5,R4
        BRnz    CHANGE  ; ('a' <=) R5 <= 'z'
        BR      R
        ;
MOVE    LD      R4,NUMS
        ADD     R5,R5,R4
        ADD     R2,R2,R5
        BR      R
CHANGE  LD      R4,CHAS
        ADD     R1,R5,R4 
        ADD     R1,R1,#1 ; offset (from 1)
        STI     R1,JBIRD
R       LDR     R5,R6,#2
        LDR     R4,R6,#1
        RTI    
        ;
        ; data base
KBSR    .FILL   xFE00
KBDR    .FILL   xFE02
JBIRD   .FILL   BIRD
JFLAG   .FILL   FLAG
NUMS    .FILL   xFFD0 ; -48  neg'0'
NUML    .FILL   xFFC7 ; -57  neg'9'
CHAS    .FILL   xFF9F ; -97  neg'a'
CHAL    .FILL   xFF86 ; -122 neg'z'
        .END
        ;
; user program
        .ORIG   x3000
        AND     R3,R3,#0
        ST      R3,FLAG  ; FLAG = 0 []
        ADD     R3,R3,#1 ; R3 = 1
        ST      R3,BIRD  ; BIRD = 1
        LD      R2,RMOST ; start from right
        ;
PRINT   AND     R4,R4,#0 ; []
        ST      R4,FLAG
        LD      R7,NUM   ; counter = 20
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
        BRz     OVER     ; counter = 0,over print
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
        AND     R4,R4,#0 ; []
        ADD     R4,R4,#1
        ST      R4,FLAG  ; FLAG = 1
        JSR     DELAY
        ADD     R2,R2,#-1
        BRnzp   PRINT
        ;
DELAY   ST      R1,SAVER1
        LD      R1,COUNT
LOOP    ADD     R1,R1,#-1
        BRnp    LOOP
        LD      R1,SAVER1
        RET             ; counterR7 = 0 -> JSR.RET -> initialization不会影响计数器
        ;
        ; data base
NUM     .FILL   #20     ; for counter
BASE    .FILL   #96
SIDE    .FILL   #-18
RMOST   .FILL   #18
BIRD    .BLKW   #1
FLAG    .BLKW   #1 ; []
NEWLINE .FILL   x000A   ; ASCII for '/n'
AIR     .FILL   x002E   ; ASCII for '.'
COUNT   .FILL   #5000   ; for delay
SAVER1  .BLKW   #1
        .END
