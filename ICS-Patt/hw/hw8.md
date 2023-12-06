# hw8 (ddl 7.23)

![image-20230721173704399](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20230721173704399.png)

9.16
a. 256. Because the trap vector is 8-bit wide.
b. The RTI instruction returns control to the calling program(pops the top two values on the system stack into the PC and PSR). While BR instruction doesn’t restore PC.
c. A TRAP instruction can make only one access to memory.

* `Get the address from the Trap Vector Table. Push the PC into the system stack. Push the PSR
  into the system stack. So the number of accesses is 3. (-1)`

9.17
a. Only some external mechanism can restart the clock after it’s halted, because the original program has stopped.
b. STI R0, MCR (Because it sets the RUN latch to ‘0’.)
c. LD R1, SaveR1(Because it’s the first instruction after HALT.)
d. To the next address of the HALT instruction, in the program which executed the HALT instruction.