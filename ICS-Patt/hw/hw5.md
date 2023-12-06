# hw5

5.37
IR, PC, Reg File, SEXT with IR[8:0], 
ADDR2MUX, ADDR1MUX set to PC, the corresponding ADDER,
MARMUX, GateMARMUX, NZP(CC), 
Memory, MDR, MAR.

* `STI dose NOT set CC`

5.39
IR, PC, Reg File, SEXT with IR[8:0],
ADDR2MUX, ADDR1MUX set to PC, the corresponding ADDER,
MARMUX, GateMARMUX, NZP(CC).

* `LEA does NOT set CC`

6.24
0110 **000 001 000001** ; (LDR R0, R1, #1)

7.32
  symbol table

| Symbol | Address |
| :----: | :-----: |
|  SKIP  |  x8009  |
|   A    |  x800A  |
|   B    |  x8011  |
| BANNER |  x8012  |
|   C    |  x801F  |

x8006: 0010 0010 0000 0011
x8007: 0000 0100 0000 0001
x8008: 0011 0000 0000 1001

The contents of line 7 causes the value of R0 to be in location B, and in this case, it happens to be 5.
The contents of line 10 causes the value 5 to be in location B whatever other registers’ values are.

* `x8008 0011 000 000001000 (x8011 - x8008 -1)`
  `line 7 store 5 in B at runtime, line 10 set 5 in B after loading.`

7.34
(a) NOT R2, R0
(b) ADD R2, R2, #1  ; R2 = -R0
(c) BRz DONE
(d) ADD R0, R0, #1