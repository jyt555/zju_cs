# hw4

5.2
64 bits in MDR, but we can know nothing about the size of MAR.

5.4
a. 8
b. 6
c. 6

5.9
(c) can be used as NOP, because it is never taken.
(a): ADD R1, R1, #0
(b): BRnzp #1
(c): never taken.
ADD instruction fetches operands and stores result, that others do not do.

5.15
1110 001 000100000  (LEA R1, 0x20) R1<-0x3121
0010 010 000100000 (LD R2, 0x20) R2<-Mem[0x3122] = 0x4566
1010 011 000100001(LDI R3, 0x20) R3<Mem[Mem[0x3123]] = 0xabcd
0110 100 010 000001( LDR R4, R2, 0x1) R4<- Mem[R2 + 0x1] = 0xabcd
1111 0000 0010 0101(TRAP 0x25) halt

5.16
a. PC-relative mode. Because the offset isn’t beyond 2^8^, and this way is convenient.
b. Indirect mode. Because in this way, the address can be anywhere.
c. Base+offset mode. Because in this way, the address can be anywhere, and it’s convenient to find consistent address.