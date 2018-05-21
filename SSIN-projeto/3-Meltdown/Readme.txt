$ make 
$ sudo insmod CreateKernelModule.ko 
$ dmesg | grep 'secret data address' 
secret data address: 0xfc514000 (example output)

$ gcc -march=native -o MeltdownAttack MeltdownAttack.c
$ ./MeltdownAttack