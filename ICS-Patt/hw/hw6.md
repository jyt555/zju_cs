# hw6

8.2
With the model in Figure 8.9, data entries do not move, while with the model in Figure 8.8, data entries move. And because the stack is implemented in memory, it is more convenient to plus the SP(Figure 8.9), rather than move all data entries in it.

8.8
(a) the stack contains A F after the PUSH F
(b) the stack contains the most elements after PUSH J and PUSH K.

| INSTRUCTION | DATA        |
| :---------: | :---------- |
|   PUSH G    | A F G       |
|   PUSH H    | A F G H     |
|   PUSH I    | A F G H I   |
|  *  PUSH J  | A F G H I J |
|     POP     | A F G H I   |
|  *  PUSH K  | A F G H I K |
|     POP     | A F G H I   |
|     POP     | A F G H     |
|     POP     | A F G       |
|   PUSH L    | A F G L     |
|     POP     | A F G       |
|     POP     | A F         |
|   PUSH M    | A F M       |

(c) A F M

7.30
(a) BRnp SKIP
(b) ADD R5, R5, #1
(c) STR R0, R6, #0
(d) LD R1, SAVER