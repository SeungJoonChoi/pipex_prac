/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <seungjoon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 13:34:51 by seunchoi          #+#    #+#             */
/*   Updated: 2022/07/04 20:55:32 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>

# define BUFFER_SIZE 4096

char		*get_ret(char *backup);
char		*read_line(char *backup, int fd);
char		*new_backup(char **backup, size_t len);
char		*get_next_line(int fd);
size_t		ft_strlen(const char *s);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
char		*ft_dupjoin(char const *s1, char const *s2);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *s1);

#endif
