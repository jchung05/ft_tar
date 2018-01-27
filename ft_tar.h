/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:15:55 by jchung            #+#    #+#             */
/*   Updated: 2018/01/27 09:10:11 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TAR_H
# define FT_TAR_H
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

# define BLOCK_SIZE 512

typedef struct		s_header {
	char			name[100];
	char			linkname[100];
	char			size[12];
	char			filler[300];
}					t_header;

void				ft_archive(int tarfd, char *filename, char *nextfile);

#endif
