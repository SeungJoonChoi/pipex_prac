#include "pipex.h"
#include "get_next_line.h"
#include <stdio.h>
#define BUFFER_SIZE 4096
///////////////////////

void exit_with_err(void)
{
	return ;
}
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n > 0 && *s1 != '\0' && *s2 != '\0')
	{
		if (*s1 != *s2)
			break ;
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	else
		return ((unsigned char)*s1 - (unsigned char)*s2);
}

char	*ft_dupjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*ret;

	if (s1)
		str = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) \
				* sizeof(char));
	else
		str = (char *)malloc((ft_strlen(s2) + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	ret = str;
	while (s1 && *s1 != '\0')
	{
		*str = *s1;
		str++;
		s1++;
	}
	while (*s2 != '\0')
	{
		*str = *s2;
		str++;
		s2++;
	}
	*str = '\0';
	return (ret);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	i;

	src_len = ft_strlen(src);
	i = 0;
	while ((i + 1) < dstsize && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize > 0)
		dst[i] = '\0';
	return (src_len);
}

char	*ft_strchr(const char *s, int c)
{
	int		len;

	len = ft_strlen(s);
	while (0 < len + 1)
	{
		if (*s == (char)c)
			return ((char *)s);
		len--;
		s++;
	}
	return (NULL);
}


char	*get_next_line(int fd)
{
	static char	*backup;
	char		*ret;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	backup = read_line(backup, fd);
	if (backup == NULL || *backup == '\0')
	{
		free(backup);
		backup = NULL;
		return (NULL);
	}
	ret = get_ret(backup);
	if (ret == NULL)
	{
		free(backup);
		backup = NULL;
		return (NULL);
	}
	backup = new_backup(&backup, ft_strlen(ret));
	if (backup == NULL)
		return (NULL);
	return (ret);
}

char	*get_ret(char *backup)
{
	size_t	len;
	char	*ret;

	if (ft_strchr(backup, '\n'))
	{
		len = ft_strchr(backup, '\n') - backup + 1;
	}
	else
	{
		len = ft_strlen(backup);
	}
	ret = (char *)malloc((len + 1) * sizeof(char));
	if (ret == NULL)
		return (NULL);
	ft_strlcpy(ret, backup, len + 1);
	return (ret);
}

char	*read_line(char *backup, int fd)
{
	ssize_t	n_read;
	char	*buf;
	char	*new;
	char	*temp;

	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (buf == NULL)
		return (NULL);
	new = backup;
	while (new == NULL || !ft_strchr(new, '\n'))
	{
		n_read = read(fd, buf, BUFFER_SIZE);
		if (n_read <= 0)
			break ;
		buf[n_read] = '\0';
		temp = new;
		new = ft_dupjoin(temp, buf);
		free(temp);
	}
	free(buf);
	buf = NULL;
	if (n_read >= 0)
		return (new);
	free(new);
	return (NULL);
}

char	*new_backup(char **backup, size_t len)
{
	size_t	new_len;
	char	*ret;

	new_len = ft_strlen(*backup) - len;
	ret = (char *)malloc((new_len + 1) * sizeof(char));
	if (ret == NULL)
		return (NULL);
	ft_strlcpy(ret, *backup + len, new_len + 1);
	free(*backup);
	*backup = NULL;
	return (ret);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}
/////////libft////////////////////////

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = 0;
		i++;
	}
}

static int	malloc_count(char *str, char c)
{
	char	*flag;
	int		argc;

	argc = 0;
	flag = str;
	while (*str != '\0')
	{
		while (*flag != c && *flag != '\0')
			flag++;
		if ((flag - str) > 0)
			argc++;
		str = flag;
		while (*str == c && *str != '\0')
			str++;
		flag = str;
	}
	return (argc);
}

static void	malloc_free(char **strs, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

static int	str_alloc(char **strs, int n, char *str, char *flag)
{
	char	*set;
	long	len;
	int		i;

	len = flag - str;
	set = (char *)malloc((len + 1) * sizeof(char));
	if (set == NULL)
	{
		malloc_free(strs, n);
		return (0);
	}
	i = 0;
	while (i < len)
	{
		set[i] = str[i];
		i++;
	}
	set[i] = '\0';
	strs[n] = set;
	return (1);
}

static int	set_strs(char **strs, char *str, char c)
{
	char	*flag;
	int		i;

	flag = str;
	i = 0;
	while (*str != '\0')
	{
		while (*flag != c && *flag != '\0')
			flag++;
		if ((flag - str) > 0)
		{
			if (!str_alloc(strs, i, str, flag))
				return (0);
			i++;
		}
		str = flag;
		while (*str == c && *str != '\0')
			str++;
		flag = str;
	}
	strs[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	char	*str;

	str = (char *)s;
	strs = (char **)malloc((malloc_count(str, c) + 1) * sizeof(char *));
	if (strs == NULL)
		return (NULL);
	if (!set_strs(strs, str, c))
		return (NULL);
	return (strs);
}

// char	*ft_strchr(const char *s, int c)
// {
// 	int		len;
// 	char	*str;
// 	char	character;

// 	str = (char *)s;
// 	character = (char)c;
// 	len = 0;
// 	while (s[len])
// 		len++;
// 	while (0 < len + 1)
// 	{
// 		if (*str == character)
// 			return (str);
// 		len--;
// 		str++;
// 	}
// 	return (NULL);
// }

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*ret;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	str = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	ret = str;
	while (*s1 != '\0')
	{
		*str = *s1;
		str++;
		s1++;
	}
	while (*s2 != '\0')
	{
		*str = *s2;
		str++;
		s2++;
	}
	*str = '\0';
	return (ret);
}

// size_t	ft_strlen(const char *s)
// {
// 	size_t	len;

// 	len = 0;
// 	while (s[len])
// 		len++;
// 	return (len);
// }

// int	ft_strncmp(const char *s1, const char *s2, size_t n)
// {
// 	while (n > 0 && *s1 != '\0' && *s2 != '\0')
// 	{
// 		if (*s1 != *s2)
// 			break ;
// 		s1++;
// 		s2++;
// 		n--;
// 	}
// 	if (n == 0)
// 		return (0);
// 	else
// 		return ((unsigned char)*s1 - (unsigned char)*s2);
// }

char	*ft_strdup(const char *s1)
{
	int		i;
	int		srclen;
	char	*dup;

	srclen = 0;
	while (s1[srclen])
		srclen++;
	dup = (char *)malloc((srclen + 1) * sizeof(char));
	if (dup == NULL)
		return (0);
	i = 0;
	while (i < srclen)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[srclen] = '\0';
	return (dup);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	char	*str;
	size_t	s_len;
	size_t	i;

	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if ((s_len - start) < len)
		len = s_len - start;
	str = (char *)s + start;
	sub = (char *)malloc((len + 1) * sizeof(char));
	if (sub == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = str[i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

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
        if (!ft_strncmp(cmp, buf, ft_strlen(buf) + 1))
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
        dir = ft_substr(path, 0, ft_strchr(path, ':') - path);
        bin = path_join(dir, cmd);
        free(dir);
        if (access(bin, F_OK) == 0)
            return (bin);
        free(bin);
        path = ft_strchr(path, ':') + 1;
    }
    // return (cmd);
}

int main(int argc, char *argv[], char *envp[])
{
    char *path;
    char *cmd = "grep";

    path = pathfinder(cmd, envp);
    printf("%s\n", path);

    return(0);
}