export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:."
gcc -c -Wall -fPIC -D_GNU_SOURCE power.c
gcc -c -Wall -fPIC -D_GNU_SOURCE sqrt.c
gcc -c -Wall -fPIC -D_GNU_SOURCE cdiv.c
gcc -c -Wall -fPIC -D_GNU_SOURCE prime.c
gcc power.o sqrt.o cdiv.o prime.o -shared -o lib_newdyn.so
gcc -c dynamic.c -o dynamic.o
gcc dynamic.o -L. -l_newdyn -o dynamic.out
ldd dynamic.out
./dynamic.out
