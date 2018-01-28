/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:15:55 by jchung            #+#    #+#             */
/*   Updated: 2018/01/27 16:27:06 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TAR_H
# define FT_TAR_H
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h> 

# define BLOCK_SIZE 512

typedef struct		s_header {
	char			name[100]; //Argument derived
	char			mode[8]; //x.st_mode
	char			uid[8]; //x.st_uid
	char			gid[8]; //x.st_gid
	char			size[12]; //x.st_size
	char			mtime[12]; //x.st_mtime
	char			checksum[8];
	char			link[1];
	char			linkname[100]; //Argument derived
	char			filler[255];
}					t_header;

t_header			*init(char *filename, char *nextfile);
void				ft_archive(FILE *tar, char **argv);

#endif
