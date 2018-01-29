/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:15:55 by jchung            #+#    #+#             */
/*   Updated: 2018/01/28 17:46:18 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TAR_H
# define FT_TAR_H
# include <fcntl.h>
# include <grp.h>
# include <pwd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h> 
# include <sys/types.h>

# define BLOCK_SIZE 512

typedef struct		s_header {
	char			name[100]; //Argument derived
	char			mode[8]; //x.st_mode
	char			uid[8]; //x.st_uid
	char			gid[8]; //x.st_gid
	char			size[12]; //x.st_size
	char			mtime[12]; //x.st_mtime
	char			checksum[8];
	char			typeflag[1];
	char			linkname[100]; //Argument derived
	char			magic[6];
	char			version[2];
	char			uname[32];
	char			gname[32];
	char			devmajor[8];
	char			devminor[8];
	char			prefix[155];
	char			pad[12];
}					t_header;

t_header			*initheader(char *filename);
void				ft_archive(FILE *tar, char **argv);
void				free_archive(char *buf, t_header *header);

void				write_block(FILE *tar, char **argv);
void				write_end_block(FILE *tar, char **argv);
int					get_block_size(int size);

char				*getName(uid_t uid);
char				*getGroup(gid_t gid);

#endif
