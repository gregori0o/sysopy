#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>


int main ()
{
	char pipe [] = "potok";
	char size [] = "8500";
	mkfifo (pipe, 0666);
	pid_t pids [10];

	printf("%d\n", PIPE_BUF);


	pids[0] = fork ();
	if (pids[0] == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/customer", "customer", pipe, "res.txt", size, NULL);
	}
/*	pids[1] = fork ();
	if (pids[1] == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/customer", "customer", pipe, "res.txt", size, NULL);
	}
	pids[2] = fork ();
	if (pids[2] == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/customer", "customer", pipe, "res.txt", size, NULL);
	}
	pids[3] = fork ();
	if (pids[3] == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/customer", "customer", pipe, "res.txt", size, NULL);
	}
*/
	pid_t pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "1", "f1.txt", size, NULL);
	}
	pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "6", "f6.txt", size, NULL);
	}
	pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "3", "f3.txt", size, NULL);
	}
	pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "2", "f2.txt", size, NULL);
	}
	
	
 	waitpid (pids[0]);
// 	waitpid (pids[1]);
// 	waitpid (pids[2]);
// 	waitpid (pids[3]);

	return 0;
}