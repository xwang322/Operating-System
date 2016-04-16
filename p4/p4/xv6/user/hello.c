#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

//void myfunc(int arg) {
void myfunc(void *arg) {
	printf(1, "hello from func %d\n", arg);
	// int x = getpid();
	// printf(1, "hello from func (pid %d)\n", x);
}

int main (int argc, char *argv[]) {
	printf(1, "hello from main\n");
	int pid1 = getpid();
	printf(1, "parent pid:%d\n", pid1);
	//myfunc(0);
	int pid2 = thread_create(myfunc, NULL);
	printf(1, "created child pid:%d. Parent ends!\n", pid2);
	exit();
}
