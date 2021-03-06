#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main()
{

void *library_handler;
float (*f1)(float a,float b);
float (*f2)(float a,float b);
 
library_handler = dlopen("/home/alex/libs/libfsdyn.so",RTLD_LAZY); 
if (!library_handler){ 
	fprintf(stderr,"dlopen() error: %s\n", dlerror());
	exit(1); 
} 

f1=dlsym(library_handler,"f1");
f2=dlsym(library_handler,"f2");

printf("5.1+4.3=%.1f\n",(*f1)(5.1,4.3));
printf("5.1-4.4=%.1f\n",(*f2)(5.1,4.3));

dlclose(library_handler);

    return 0;
}
