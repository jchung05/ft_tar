/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:33:21 by jchung            #+#    #+#             */
/*   Updated: 2018/01/27 16:52:56 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

int				main(int argc, char **argv)
{
	FILE		*tar;
	
	if (argc > 2)
	{
		tar = fopen((const char *)argv[1], "a");
		ft_archive(tar, &argv[2]);
	}
	else
		printf("usage: ft_archive tar_name file1 [file2 ... fileN]\n");
	fclose(tar);
	return (0);
}

int				get_block_size(int size)
{
	return ((size / BLOCK_SIZE + (size % BLOCK_SIZE == 0 ? 0 : 1)) * BLOCK_SIZE);
}

t_header		*init(char *filename, char *nextfile)
{
	t_header	*header;
	struct stat	st;

	header = (t_header *){filename, 0, 0, 0, 0, 0, 0, 0, nextfile, 0};
	if (!stat(filename, &st))
		return ((t_header *)0);
	/*
	header->mode = st.st_mode;
	header->uid = st.st_uid;
	header->gid = st.st_gid;
	header->size = st.st_size;
	header->mtime = st.st_mtime;
	*/
	strcpy((mode_t)header->mode, st.st_mode);
	strcpy((uid_t)header->uid, st.st_uid);
	strcpy((gid_t)header->gid, st.st_gid);
	strcpy((off_t)header->size, st.st_size);
	strcpy((time_t)header->mtime, st.st_mtime);
	return (header);
}
/*
 * Pass the tar file object with "append" properties and the list of args from main
 * Initialize file header with metadata using stat()
 * Initialize a buffer with fopen and fread of the first arg in argv
 * buffer can be malloc'd according to the size of header->size rounded up to next block of 512
 * Set remainder of buffer to \0
 * fwrite() header and body to tar file object
 * Recursively call ft_archive on next argument of argv
 */

void			ft_archive(FILE *tar, char **argv)
{
	t_header	*header;
	FILE		*f;
	char		*buf;
	int			size;
	int			bufsize;

	if (argv[0])
	{
		header = init(argv[0], (argv[1] != 0 ? argv[1] : 0));
		f = fopen(argv[0], "r");
		size = atoi(header->size);
		bufsize = get_block_size(size);
		buf = (char *)malloc(bufsize);
		if (!buf)
			return ;
		fread(buf, (size_t)size, 1, f);
		memset(&buf[size], 0, bufsize - size);
	}
	else
	{
		header = (t_header *){0};
		buf = (char *)malloc(BLOCK_SIZE);
		if (!buf)
			return ;
		memset(buf, 0, BLOCK_SIZE);
	}
	fwrite(header, sizeof(t_header), 1, tar);
	fwrite(buf, sizeof(buf), 1, tar);
	fclose(f);
	free(buf);
	if (argv[0])
		ft_archive(tar, &argv[1]);
}
