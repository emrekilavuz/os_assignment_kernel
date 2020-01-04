#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main(int argc,char* argv[]){
if(argc<2){
printf("usage error, missing pid\n");
}
char* argument = argv[1];
long value = syscall(436,argument);
printf("system call returned %ld\n",value);
return 0;
}
