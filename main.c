#include "pipex.h"

int openfile(char *file_or_limiter, int mode)
{
    if (mode == INFILE)
    {
        if (access(file_or_limiter, F_OK))
        {
            exit_with_err();
        }
        else
            return (open(file_or_limiter, O_RDONLY));
    }
    else if (mode == OUTFILE)
        return (open(file_or_limiter, O_WRONLY | O_CREAT | O_TRUNC, 0644));
    else if (mode == HEREIN)
        return (here_doc(file_or_limiter));
    else if (mode == HEREOUT)
        return (open(file_or_limiter, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

void piping(char *cmd, char *envp[])
{
    pid_t pid;
    int fd[2];

    pipe(fd);
    pid = fork();
    if (!pid)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        do_cmd(cmd, envp);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        waitpid(pid, NULL, 0);
    }
}

int main(int argc, char *argv[], char *envp[])
{
    int infile;
    int outfile;
    int cmd_idx;

    if (!ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")))
    {
        cmd_idx = 3;
        infile = openfile(argv[2], HEREIN);
        outfile = openfile(argv[argc - 1], HEREOUT);
    }
    else
    {
        cmd_idx = 2;
        infile = openfile(argv[1], INFILE);
        outfile = openfile(argv[argc - 1], OUTFILE);
    }
    dup2(infile, STDIN_FILENO);
    dup2(outfile, STDOUT_FILENO);
    while (cmd_idx < argc - 2)
    {
        piping(argv[cmd_idx], envp);
        cmd_idx++;
    }
    do_cmd(argv[cmd_idx], envp);
}