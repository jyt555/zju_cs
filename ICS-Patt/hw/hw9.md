# hw9(ddl 7.26)

Q1:

| x3FFB |   -   |   -   |   -   |   -   |   -   |   -   |   -   |   -   |   -   |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| x3FFC |   -   |   -   |   -   |   -   |   -   |   -   |   -   |   -   |   -   |
| x3FFD |   -   |   -   |   -   |   -   |   -   |   -   |  205  |  205  |  205  |
| x3FFE |   -   |   -   |  49   |  -49  |  -49  |  172  |  172  |  377  |  377  |
| x3FFF |   -   |  51   |  51   |  51   |   2   |   2   |   2   |   2   |  754  |
|  SP   | x4000 | x3FFF | x3FFE | x3FFE | x3FFF | x3FFE | x3FFD | x3FFE | x3FFF |

| x3FFB |   -   |   -   |   -   |   -   |   -   |      |      |      |      |      |
| :---: | :---: | :---: | :---: | :---: | :---: | ---- | ---- | ---- | ---- | ---- |
| x3FFC |   -   |   -   |   -   |   -   |   -   |      |      |      |      |      |
| x3FFD |  205  |   2   |   2   |   2   |   2   |      |      |      |      |      |
| x3FFE |  17   |  17   |  34   |  -34  |  -34  |      |      |      |      |      |
| x3FFF |  754  |  754  |  754  |  754  |  720  |      |      |      |      |      |
|  SP   | x3FFE | x3FFD | x3FFE | x3FFE | x3FFF |      |      |      |      |      |

Q2:

The address of A[2,4,3,5] is x4392.

To increment Q, we add 1 element, and to increment P, we add 9 elements. Similarly, to increment N, we add 7 * 9 = 63 elements; to increment M, we add 5 * 7 * 9 = 315 elements.
So A[2,4,3,5] is the 2 * 315 + 4 * 63 + 3 * 9 + (5+1) * 1 = 915 th element.
x4000 + #915 - #1 = x4392
