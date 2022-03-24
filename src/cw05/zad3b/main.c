#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main ()
{
	char pipe [] = "potok";
	char size [] = "10";
	mkfifo (pipe, 0666);


	pid_t pid_c = fork ();
	if (pid_c == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/customer", "customer", pipe, "res.txt", size, NULL);
	}

	pid_t pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "1", "f1.txt", size, NULL);
	}
	pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "8", "f8.txt", size, NULL);
	}
	pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "3", "f3.txt", size, NULL);
	}
	pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "5", "f5.txt", size, NULL);
	}
	pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "2", "f2.txt", size, NULL);
	}
	pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "7", "f7.txt", size, NULL);
	}
	pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "6", "f6.txt", size, NULL);
	}
	pid = fork ();
	if (pid == 0)
	{
		execl("/home/glegeza/Programming/sysopy/lab5/zad3b/producent", "producent", pipe, "4", "f4.txt", size, NULL);
	}
	
 	waitpid (pid_c);

	return 0;
}