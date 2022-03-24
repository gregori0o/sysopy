#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

void search_in_dir (char* absolute_path, char* key, int depth, char* relative_path)
{
	char* path = calloc (200, sizeof(char));

	strcpy (path, absolute_path);
	strcat (path, relative_path);

	//printf("******\n");
	//printf("Przeglądam folder: %s\n", path);
	//printf("W procesie %d\n", (int)getpid());
	//printf("******\n");

	struct dirent *file;
	DIR * directory;
	directory = opendir(path);
	if (directory == NULL)
	{
		printf("%s\n", "Błąd odczytania folderu!");
		free (path);
		free (relative_path);
		return;
	}
	char* buffer = calloc (256, sizeof(char));
	while (file = readdir (directory))
	{
		if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
			continue;
		if (file->d_type == DT_DIR && depth > 0)
		{
			char* r_path = calloc (200, sizeof(char));
			sprintf (r_path, "%s/%s", relative_path, file->d_name);
			pid_t child = fork ();
			if (child == 0)
			{
				search_in_dir (absolute_path, key, depth - 1, r_path);
				break;
			}
		}
		else if (file->d_type == DT_REG)
		{
			sprintf (buffer, "%s/%s/%s", absolute_path, relative_path, file->d_name);
			FILE * fp;
			fp = fopen (buffer, "r");
			while (fgets(buffer, 256, fp) != NULL)
			{
				if (strstr (buffer, key) != NULL)
				{
					printf("Odnaleziono słowo: %s\n", key);
					printf("Ścieżka względna pasującego pliku: %s/%s\n", relative_path, file->d_name);
					printf("Odnaleziono w procesie o PID równym: %d\n", (int)getpid());
					printf("\n");
					break;
				}
			}
			fclose (fp);
		}
	}
	free (buffer);
	closedir(directory);
	free (relative_path);
	free (path);

}

int main (int argc, char **argv)
{
	if (argc < 4)
	{
		printf("%s\n", "Nieprawidłowe argumenty!");
		exit (1);
	}
	int depth;
	char* relative_path = calloc (200, sizeof(char));
	char* absolute_path;
	char* key;
	if (argc >= 4)
	{
		absolute_path = argv[1];
		key = argv[2];
		sscanf (argv[3], "%d", &depth);
	}

	search_in_dir (absolute_path, key, depth, relative_path);

	return 0;
}