## 关于递归

这两天验收注意到大家还不是很注意保护寄存器，怕大家 lab5 心态写炸，给大家一些例子。

首先，调用 函数/trap/interrupt 前保护寄存器是常识，你应该习惯性的先把所有 (除了R6) 的寄存器存一份，就算你在这个函数里不用它，也能提供一些安全感x

其次，目前我看到的大多数人是这么保护寄存器的：

```assembly
; 两种写法，分别用FILL和BLKW，没什么差别
; before
ST R1, SAVER1
ST R2, SAVER2

...

; after
LD R1, SAVER1
LD R2, SAVER2

SAVER1	.FILL #0
SAVER2	.BLKW 1
...
```

这样保护是保护了，但是当你想在一个函数中再调别的函数，或者一份代码中有多个函数，这种写法就会变得离谱起来。在验收过程中我也看到有人最后在代码里有一堆 `SAVE_R10`,`SAVE_R11`，来为不同函数存寄存器状态。

这种写法既麻烦可读性又差。最严重的是，如果你要写一个递归函数，这样根本没法写。**因为重复调用同一个函数，它们将把寄存器状态存在同一个地方**。



在验收的时候我给大家修没保存寄存器 bug 时，都是用 Stack Pointer 来存寄存器状态的。

在此给大家一个 sample code，如果函数调用的压栈还完全不会写的话，可以进行参考。



## Sample

这个 sample 是原 lab5 费案的一个写了一半的代码x

该程序可以读入两个整数，并输出它们的**最大正公约数 (GCD)**。

有两部分代码可以供大家参考。

其一是位于 `x4000` 的 GCD 函数，其中有较为完整的递归函数压栈写法。（算法就是简单的更相减损法，毕竟 lc3 没有除/右移，这个不是重点）

其二是自己写的 `TRAP x02`，实现了一个输入十进制数的功能，这个lab5可能也会用到。这个输入是支持输入负数的，但是毕竟是半成品代码，gcd 部分懒了没判断负数的情况。

输出十进制数的部分也挺麻烦的，这里也懒了，直接将结果存在了 `R3`, 不过思路大致就是输入反过来。

代码如下，希望我注释写的足够阳间了。

（预祝大家lab5写的愉快x）

```assembly
.ORIG x3000
MAIN
    LD R6, USP          ; Init stack pointer
    TRAP x01            ; and registers
    TRAP x02
    ADD R1, R0, #0      ;
    TRAP x02
    ADD R2, R0, #0
    LD R0, GCD_ADDR
    JSRR R0   
    ADD R3, R0, #0
    HALT

USP         .FILL   xFDFF
GCD_ADDR    .FILL x4000
.END


.ORIG x4000
GCD_V1
    ; R0: Result
    ; R1: Parameter x
    ; R2: Parameter y
    ; R3: Temp
    ; R4: Temp
    ; R5: Temp
    ; This function will calculate the GCD of two numbers
    ; (ONLY ACCEPT POSITIVE NUMBERS)
    ; using the Euclidean algorithm.
    ; (but being limited by lc3, we could only use minus instead of modulo)
    ;
    ; Caller should store two numbers in R1 and R2.
    ; Result will be stored in R0.

    ; Store registers
    STR R1, R6, #-1
    STR R2, R6, #-2
    STR R3, R6, #-3
    STR R4, R6, #-4
    STR R5, R6, #-5
    STR R7, R6, #-6
    ADD R6, R6, #-6

;   Check if x == y
    NOT R0, R2
    ADD R0, R0, #1
    ADD R0, R0, R1     ; If x < y, swap x and y
    BRzp GCD_V1_EDGE
;   Swap x and y
    ADD R3, R1, #0     ; R3 = x
    ADD R1, R2, #0     ; R1 = y
    ADD R2, R3, #0     ; R2 = x

;   Recursion exit condition
GCD_V1_EDGE
    ; If y == 0, return x
    ADD R0, R1, #0
    ADD R2, R2, #0
    BRz GCD_V1_RETURN

;   Euclidean algorithm
GCD_V1_MINUS
    NOT R0, R2
    ADD R0, R0, #1
    ADD R1, R0, R1     ; x = x - y until x < 0
    BRzp GCD_V1_MINUS
    ADD R1, R1, R2     ; x = x + y

;   recursive call
    JSR GCD_V1

GCD_V1_RETURN
    ADD R6, R6, #6
    LDR R1, R6, #-1
    LDR R2, R6, #-2
    LDR R3, R6, #-3
    LDR R4, R6, #-4
    LDR R5, R6, #-5
    LDR R7, R6, #-6
    RET
.END







.ORIG x0001
INIT    .FILL x1000
GETINT  .FILL x2000
.END


.ORIG x1000
TRAP_INIT
    AND R0, R0, #0
    AND R1, R1, #0
    AND R2, R2, #0
    AND R3, R3, #0
    AND R4, R4, #0
    AND R5, R5, #0
    AND R7, R7, #0
    RTI
.END


.ORIG x2000
TRAP_GETINT
;   Store registers
;   R0: Input & Result
;   R1: Sign Flag (0 = positive, 1 = negative)
;   R2: Temp
;   R3: Temprary Result
;
;   This trap will only work for integers between -32768 and 32767,
;   and will store result in R0.
    STR R1, R6, #-1
    STR R2, R6, #-2
    STR R3, R6, #-3
    STR R7, R6, #-4
    ADD R6, R6, #-3
    AND R1, R1, #0  ;   Clear sign flag
    AND R3, R3, #0  ;   Clear result

;   Print prompt
    LEA R0, PROMPT
    PUTS

;   Get input
INPUT_INT
    GETC
    OUT

; Switch (R0):
;   case "\n":
CASE_NEWLINE
    LD R2, NEWLINE_NEG
    ADD R2, R0, R2
    BRnp CASE_SIGN
    AND R0, R0, #0              ;   Clear input
    ADD R0, R0, R3              ;   Copy result to R0
    ADD R1, R1, #0
    BRnz GETINT_RETURN          ;   Return
    NOT R0, R0                  ;   If sign flag is set, negate result
    ADD R0, R0, #1
    BR GETINT_RETURN



;  case "-":
CASE_SIGN
    LD R2, NEGATIVE_NEG
    ADD R2, R0, R2
    BRnp CASE_DIGIT
    ADD R1, R1, #0              ;   Check sign flag,
    BRp EXCEPTION_INVALID_INPUT ;   if negative sign is already set, throw an exception
    ADD R1, R1, #1              ;   Otherwise, set sign flag
    BR INPUT_INT

;   case "0"-"9":
CASE_DIGIT
    LD R2, ZERO_NEG
    ADD R2, R0, R2
    BRn EXCEPTION_INVALID_INPUT
    LD R2, NINE_NEG
    ADD R2, R0, R2
    BRp EXCEPTION_INVALID_INPUT
    JSR MULTI_10                ;   Multiply result by 10
    LD R2, ZERO_NEG
    ADD R2, R0, R2        ;   Subtract '0' from input
    ADD R3, R3, R2              ;   Add to result
    BR INPUT_INT
    
    


GETINT_RETURN
    ADD R6, R6, #3
    LDR R1, R6, #-1
    LDR R2, R6, #-2
    LDR R3, R6, #-3
    LDR R7, R6, #-4
    RTI

EXCEPTION_INVALID_INPUT
    LEA R0, EXCEPTION_PROMP
    PUTS
    HALT

MULTI_10
;   Help function to multiply R3 by 10
;   will use R2 as temp.


    ADD R2, R3, R3      ; R2 = 2 * x
    ADD R3, R2, R2      ; R3 = 4 * x
    ADD R3, R3, R3      ; R3 = 8 * x
    ADD R3, R3, R2      ; R3 = 10 * x
    RET
    
NEWLINE_NEG     .FILL xFFF6 ; -"\n"
NEGATIVE_NEG    .FILL xFFD3 ; -"-"
ZERO_NEG        .FILL xFFD0 ; -"0"
NINE_NEG        .FILL xFFC7 ; -"9"
PROMPT          .STRINGZ "Please enter an integer: (press enter to continue)\n"
EXCEPTION_PROMP .STRINGZ "Invalid input. Please enter an integer!"
.END
```





