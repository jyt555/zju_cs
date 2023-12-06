# repo-lab6

## Algorithm (recursion)

<img src="C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20230921165414238.png" alt="image-20230921165414238" style="zoom: 67%;" />

```mermaid
graph TD
a[handle instructions] -.-> b([if ADD or AND ?]) 
-- no --> d([if NOT ?]) -- no --> e([if BR ?])
-- no --> f([if LD or LDI orLEA ?])
-- no --> i([if LDR ?]) -- no --> j([if ST or STI ?])
-- no --> l([if STR ?]) -- no --> m([if JMP or RET ?])
-- no --> o([if JSR or JSRR ?])
-.->r[set CC]

b--yes-->A[load the value in R to oprand<br/>]
--ADD/AND-->b1[ADD:opr = opr1 + opr2<br/>AND:opr = opr1 && opr2<br/>R.addr = opr] --> r

d--yes-->A--NOT-->d1[opr = opr1 ^ 0xffff]-->r

e--yes-->A--BR-->e9[n=1 & CC<0:PC += opr<br/>z=1 & CC==0:PC:PC += opr<br/>p=1 & CC>0:PC += opr]-->r

f--yes-->A--LD/LDI/LDR/LEA-->f9[R.addr = the value in line]-->r

i--yes-->A--ST/STI/STR-->i9[line.x=the value in R<br/>ST:x=PC+opr+1<br/>x=opr-starting_addr+1<br/>x=opr1+opr2-starting_addr+1]-->r

j--yes-->A--JMP/RET/JSR/JSRR-->j9[PC=opr-starting_addr]

l--yes-->A

m--yes-->A

o--yes-->A


```



## Essential parts of codes

I used functions less in the body of the program, and a few functions such as whe(num),aver(num) and add(num1, num2), were used in the initialization phase.

```python
# function
def whe(num):
    ...
def aver(num):
    ...
def add(num1 ,num2):
    ...
```



whe(num) is used to judge the positive and negative of the number passed in (including whether it is a negative complement), aver(num) converts the negative number in the complement form to a negative number in decimal, and add(num1, num2) is used to realize the addition of two numbers (mainly used to avoid overflow).

In the main part of the program, I use multiple if-elif-else judgments to realize the reading and execution of instructions.

```python
# main program
for n in range(0x20000):
   
    IR = line[PC]
    # print('PC',PC,'IR',IR)

    if IR[0:16] == '1111000000100101':  # HALT
        break
    elif n == 0:
        starting_addr = int(IR,2)  # starting_address
    else:

        # ADD/AND
        if IR[0:4] == '0001' or IR[0:4] == '0101':
            ...
        # NOT
        elif IR[0:4] == '1001':
            ...
		...
        # set CC
        if IR[0:4]=='0001' or IR[0:4]=='0101' or IR[0:4]=='1001' or IR[0:4]=='0010' or IR[0:4]=='1010' or IR[0:4]=='0110':
            ...

```

## Unresolved bugs

To make it easier to read a few numbers in IR (currently read instructions), I chose to store the 0 and 1 instructions as binary strings in line (where all instructions are stored). Therefore, in the process of programming, there are some errors caused by improper processing of the positive and negative signs of data. After correcting these errors (line[], IR[] saves the unsigned binary number as a string, and R[] saves the decimal number, there is a conversion between the two), the program has a lot less problems.

However, unfortunately, the program still had unresolved bugs as of deadline, and as a result several points(exactly C1,C2 and C3) were not passed in the PTA. The preliminary speculation is that there is a lack of data processing, which may have overflow, array overstep and other problems.