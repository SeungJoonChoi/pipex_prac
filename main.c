#include "pipex.h"

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