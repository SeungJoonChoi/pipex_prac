// #include <stdio.h>
// #include <unistd.h>

// int main(int argc, char *argv[], char *envp[])
// {
// 	// for (int i = 0; envp[i]; i++)
// 	// 	printf("%s\n", envp[i]);
// 	// printf("\n");
// 	char *cmd[] = {"ls", "-al", NULL};


// 	execve("ls", cmd, envp);

// 	return (0);
// }

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *envp[] = {"USER=abcd", "PATH=/bin", NULL};
	char *arg[] = {"ls", "-la", NULL};
	printf("execute ls\n");
	execve("ls", arg, envp);
	perror("execl is failed\n");
	exit (1);
}