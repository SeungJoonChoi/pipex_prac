#include "pipex.h"

void make_temp(char *limiter)
{
    int temp_file;
    char *cmp;
    char *buf;

    cmp = ft_strjoin(limiter, "\n");
    temp_file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0000644);
    while(1)
    {
        write(1, ">", 1);
        buf = get_next_line(0);
        if (!ft_strncmp(cmp, buf, ft_strlen(buf) + 1));
            break ;
        write(temp_file, buf, ft_strlen(buf));
        free(buf);
    }
    free(buf);
    free(cmp);
    close(temp_file);
}

void here_doc(t_pipex *pipex, int argc, char **argv)
{
    pipex->heredoc++;
    pipex->idx_cmd = 3;
    pipex->last_cmd = argc - 2;
    make_temp(argv[2]);
    pipex->infile = open(".heredoc_tmp", O_RDONLY);
    pipex->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
}

void openfile(t_pipex *pipex, int argc, char **argv)
{
    pipex->idx_cmd = 2;
    pipex->last_cmd = argc - 2;
    if (access(argv[1], F_OK))
    {
        exit_with_err();
    }
    else
        pipex->infile = open(argv[1], O_RDONLY);
    pipex->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

// int openfile(char *file_or_limiter, int mode)
// {
//     if (mode == INFILE)
//     {
//         if (access(file_or_limiter, F_OK))
//         {
//             exit_with_err();
//         }
//         else
//             return (open(file_or_limiter, O_RDONLY));
//     }
//     else if (mode == OUTFILE)
//         return (open(file_or_limiter, O_WRONLY | O_CREAT | O_TRUNC, 0644));
//     else if (mode == HEREIN)
//         return (here_doc(file_or_limiter));
//     else if (mode == HEREOUT)
//         return (open(file_or_limiter, O_WRONLY | O_CREAT | O_APPEND, 0644));
// }

void piping(char *cmd, char *envp[])
{
    pid_t pid;
    int fd[2];

    pipe(fd);
    pid = fork();
    if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        do_cmd(cmd, envp);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        waitpid(pid, NULL, 0);
    }
}

char *path_join(char const *dir, char const *cmd)
{
	char	*str;
	char	*ret;

	str = (char *)malloc((ft_strlen(dir) + ft_strlen(cmd) + 2) * sizeof(char));
	if (str == NULL)
		return (NULL);
	ret = str;
	while (*dir != '\0')
	{
		*str = *dir;
		str++;
		dir++;
	}
    *str = '/';
    str++;
	while (*cmd != '\0')
	{
		*str = *cmd;
		str++;
		cmd++;
	}
	*str = '\0';
	return (ret);
}

static char *pathfinder(char *cmd, char **envp)
{
    char *path;
    char *dir;
    char *bin;
    int i;

    i = 0;
    while (envp[i] && ft_strncmp(envp[i], "PATH=", ft_strlen("PATH=")))
        i++;
    if (envp[i] == NULL)
        return (cmd);
    path = envp[i] + ft_strlen("PATH=");
    while (path && ft_strchr(path, ':') != NULL)
    {
        dir = ft_substr(path, 0, path - ft_strchr(path, ':'));
        bin = path_join(dir, cmd);
        free(dir);
        if (access(bin, F_OK) == 0)
            return (bin);
        free(bin);
        path = ft_strchr(path, ':') + 1;
    }
    return (cmd);
}

void do_cmd(char *cmd, char **envp)
{
    char **argv;
    char *path;

    argv = ft_split(cmd, ' ');
    if (ft_strchr(argv[0], '/') == NULL)
        path = pathfinder(argv[0], envp);
    else
        path = argv[0];
    execve(path, argv, envp);
    exit_with_err();
}
