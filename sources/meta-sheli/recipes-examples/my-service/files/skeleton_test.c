#include <unistd.h>

/* This demo does nothing except for testing /etc/init.d/skeleton */

volatile int* ptr;
int xyz = 10;

int main(int argc, char *argv[])
{
	daemon(0, 0);
	while (1) {
		if (ptr)
		{
			xyz = 99;
		}
		sleep(1);
	}
	return 0;
}
