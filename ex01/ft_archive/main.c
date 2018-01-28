/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:33:21 by jchung            #+#    #+#             */
/*   Updated: 2018/01/27 17:36:45 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

int				main(int argc, char **argv)
{
	FILE		*tar;
	
	tar = (FILE *)0;
	if (argc > 2)
	{
		tar = fopen((const char *)argv[1], "a");
		ft_archive(tar, &argv[2]);
		fclose(tar);
	}
	else
		printf("usage: ft_archive tar_name file1 [file2 ... fileN]\n");
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

	header = (t_header *)malloc(sizeof(t_header));
	if (!header)
		return ((t_header *)0);
	strcpy(header->name, filename);
	strcpy(header->linkname, nextfile);
	if (!stat(filename, &st))
		return ((t_header *)0);
	memcpy(header->mode, &st.st_mode, sizeof(mode_t));
	memcpy(header->uid, &st.st_uid, sizeof(uid_t));
	memcpy(header->gid, &st.st_gid, sizeof(gid_t));
	memcpy(header->size, &st.st_size, sizeof(off_t));
	memcpy(header->mtime, &st.st_mtime, sizeof(time_t));
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
		header = init(0, 0);
		f = (FILE *)0;
		buf = (char *)malloc(BLOCK_SIZE);
		if (!buf)
			return ;
		memset(buf, 0, BLOCK_SIZE);
	}
	fwrite(header, sizeof(t_header), 1, tar);
	fwrite(buf, sizeof(buf), 1, tar);
	fclose(f);
	free(buf);
	free(header);
	if (argv[0])
		ft_archive(tar, &argv[1]);
}
