# hw7 (ddl 7.22)

9.2
Cause synchronous I/O is used, the processor knows when the data should be loaded and printed in the screen(the time of input and output). It just does input/output at regular intervals and the synchronization guarantees the correctness.

9.6
It may read the same character(just read last time) again.

9.10
It can not write to DDR.

* `it may cause data loss`

9.14
It depends on the address control logic. That is, load from the KBDR if the address is xFE02.

9.26
Not storing R~7~ before jump to ‘B’.

