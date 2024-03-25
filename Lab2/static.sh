gcc -c avg.c -o avg.o
gcc -c std.c -o std.o
gcc -c fib.c -o fib.o
gcc -c factorial.c -o factorial.o
ar r lib_newstat.a avg.o std.o fib.o factorial.o
ar tv lib_newstat.a
gcc -c static.c -o static.o
gcc static.o lib_newstat.a -o static.out
./static.out
