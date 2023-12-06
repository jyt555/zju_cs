import sys

# initialization
line = []   # str
CC = 0  # condition code
PC = 0  # program counter
R = []  # int
for n in range(8):
    R.append(0x7777)

# put in from stdin
line = sys.stdin.readlines()
line.append(' ')
while len(line) < 0x20000:
    line.append('0111011101110111')        # fill

# function
def whe(num):
    if num>=0 & num <= 0x7FFF:
        return 0
    else:
        return 1
def aver(num):
    if num <= 0x7FFF:
        return num
    else:
        return (~num)+1
def add(num1, num2):
    if whe(num1) == 0:
        if whe(num2) == 1:
            return num1 + num2
        else:
            return num1 + aver(num2)
    else:
        if whe(num2) == 0:
            return num1 + num2
        else:
            return aver(num1) + num2

# main program
for n in range(0x20000):
   
    IR = line[PC]
    print('PC',PC,'IR',IR)

    if IR == ' ':           # end now
        break
    elif IR[0:16] == '1111000000100101':  # HALT
        break
    elif n == 0:
        starting_addr = int(IR,2)  # starting_address
    else:

        # ADD/AND
        if IR[0:4] == '0001' or IR[0:4] == '0101':
            addr1 = int(IR[7:10],2)
            opr1 = R[addr1] # decimal

            if IR[10] == '0':
                addr2 = int(IR[13:],2)
                opr2 = R[addr2]
            elif IR[11] == '0':
                opr2 = int(IR[12:],2)
            else:
                opr2 = int(IR[12:],2)^15
                opr2 = -(opr2 + 1)
            
            addr = int(IR[4:7],2)
            if IR[0:4] == '0001':
                print(opr1,opr2)
                opr = add(opr1,opr2)       # ADD
            else:
                opr = opr1 & opr2       # AND
            R[addr] = opr
           
        # NOT
        elif IR[0:4] == '1001':
            addr1 = int(IR[7:10],2)
            opr1 = R[addr1] # decimal
            opr = opr1 ^ 0xffff
            addr = int(IR[4:7],2)
            R[addr] = opr

        # BR
        elif IR[0:4] == '0000':
            if IR[7] == '0':
                opr = int(IR[8:],2)
            else:
                opr = int(IR[8:],2)^255
                opr = -(opr + 1)
            if IR[4] == '1' and CC < 0:
                PC = add(PC, opr)
            elif IR[5] == '1' and CC == 0:
                PC = add(PC, opr)
            elif IR[6] == '1' and CC > 0:
                PC = add(PC, opr)

        # LD/LDI/LEA
        elif IR[0:4] == '0010' or IR[0:4] == '1010' or IR[0:4] == '1110':
            addr = int(IR[4:7],2)
            if IR[7] == '0':
                opr1 = int(IR[8:],2)
            else:
                opr1 = int(IR[8:],2)^255
                opr1 = -(opr1 + 1)

            if IR[0:4] == '0010':
                R[addr] = opr = int(line[add(PC, opr1) + 1],2)     # LD
            elif IR[0:4] == '1010':
                opr = int(line[add(opr1,PC)+1],2)    # LDI   
                R[addr] = int(line[opr - starting_addr + 1],2)
            elif IR[0:4] == '1110':
                opr = add(PC, opr1) + starting_addr     # LEA
                R[addr] = opr

        # LDR
        elif IR[0:4] == '0110':
            addr = int(IR[4:7],2)
            addr1 = int(IR[7:10],2)
            opr1 = R[addr1] # decimal

            if IR[10] == '0':
                opr2 = int(IR[11:],2)
            else:
                opr2 = int(IR[11:],2)^31
                opr2 = -(opr2 + 1)
            R[addr] = opr = int(line[add(opr1, opr2)-starting_addr+1],2)

        # ST/STI
        elif IR[0:4] == '0011' or IR[0:4] == '1011':
            addr = int(IR[4:7],2)
            if IR[7] == '0':
                opr1 = int(IR[8:],2)
            else:
                opr1 = int(IR[8:],2)^255
                opr1 = -(opr1 + 1)

            if IR[0:4] == '0011':       # ST
                line[add(PC,opr1)+1] = '{0:16b}'.format(R[addr])
            elif IR[0:4] == '1011':     # STI
                opr = int(line[add(opr1,PC)+1],2)
                line[opr - starting_addr + 1] = '{0:16b}'.format(R[addr])

        # STR
        elif IR[0:4] == '0111':
            addr = int(IR[4:7],2)
            addr1 = int(IR[7:10],2)
            opr1 = R[addr1] # decimal

            if IR[10] == '0':
                opr2 = int(IR[11:],2)
            else:
                opr2 = int(IR[11:],2)^31
                opr2 = -(opr2 + 1)
            line[add(opr2, opr1)-starting_addr+1] = '{0:16b}'.format(R[addr])

        # JMP/RET
        elif IR[0:4] == '1100':
            addr = int(IR[7:10],2)
            opr = R[addr]
            if addr == 7:
                PC = opr - starting_addr      # RET
            else:
                PC = opr - starting_addr      # JMP

        # JSR/JSRR
        elif IR[0:4] == '0100':
            if IR[4] == '1':        # JSR
                if IR[5] == '0':
                    opr = int(IR[6:],2)
                else:
                    opr = int(IR[6:],2)^1023
                    opr = -(opr + 1)
                R[7] = PC + starting_addr
                PC = add(PC, opr)
            else:                   # JSRR
                addr = int(IR[7:10],2)
                R[7] = PC + starting_addr
                PC = R[addr] - starting_addr

        # set CC
        if IR[0:4]=='0001' or IR[0:4]=='0101' or IR[0:4]=='1001' or IR[0:4]=='0010' or IR[0:4]=='1010' or IR[0:4]=='0110':
            if opr > 0:
                CC = 1
            elif opr == 0:
                CC = 0
            else:
                CC = -1
    PC += 1
    print(R)

# put out R0 ~ R7
for n in range(8):
    temp = format(int(R[n]),'04X')
    print('R{0} = x{1}'.format(n, temp))
exit()

# it does not pass some point :(