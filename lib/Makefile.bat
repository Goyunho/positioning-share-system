del libset.a
gcc -c aria.c
gcc -c sockcomm.c
ar rs libset.a aria.o sockcomm.o
