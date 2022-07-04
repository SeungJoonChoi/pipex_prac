/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seunchoi <seunchoi@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 13:34:05 by seunchoi          #+#    #+#             */
/*   Updated: 2022/02/10 13:34:15 by seunchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
