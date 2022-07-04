#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>

# define INFILE 0
# define OUTFILE 1
# define HEREIN 2
# define HEREOUT 3

typedef struct s_pipex
{
    int heredoc;
    int infile;
    int outfile;
    int idx_cmd;
    int last_cmd;
} t_pipex;

// void openfile(t_pipex *pipex, int argc, char **argv);
// void piping(char *cmd, char *envp[]);
// void do_cmd(char *cmd, char **envp);

#endif