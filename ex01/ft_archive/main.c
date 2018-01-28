/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:33:21 by jchung            #+#    #+#             */
/*   Updated: 2018/01/27 22:30:58 by jchung           ###   ########.fr       */
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

t_header		*initheader(char *filename)
{
	t_header	*header;
	struct stat	st;

	header = (t_header *)malloc(sizeof(t_header));
	memset(header, 0, sizeof(t_header));
	if (!header)
		return ((t_header *)0);
	filename == 0 ? 0 : strcpy(header->name, filename);
	if (stat(filename, &st) < 0)
		return ((t_header *)0);
	memcpy(header->mode, &st.st_mode, sizeof(mode_t));
	printf("size of mode_t %zu\n", sizeof(mode_t));
	printf("%o\n\n", st.st_mode);
	printf("%o\n\n", header->mode);

	memcpy(header->uid, &st.st_uid, sizeof(uid_t));
	printf("size of uid_t %zu\n", sizeof(uid_t));
	printf("%o\n\n", st.st_uid);
	
	memcpy(header->gid, &st.st_gid, sizeof(gid_t));
	printf("size of gid_t %zu\n", sizeof(gid_t));
	printf("%o\n\n", st.st_gid);
	
	memcpy(header->size, &st.st_size, sizeof(off_t));
	printf("size of off_t %zu\n", sizeof(off_t));
	printf("%llo\n\n", st.st_size);
	printf("%lld\n\n", (off_t)strtol(header->size, 0, 8));
	
	memcpy(header->mtime, &st.st_mtime, sizeof(time_t));
	printf("size of time_t %zu\n", sizeof(time_t));
	printf("%lo\n\n", st.st_mtime);
	
	memset(header->linkname, 0, sizeof(header->linkname));
	return (header);
}

/*
 * Pass the tar file object with "append" properties and the list of args from main
 * initheaderialize file header with metadata using stat()
 * initheaderialize a buffer with fopen and fread of the first arg in argv
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

	printf("\nIn ft_archive call\n");
	if (argv[0])
	{
		printf("%s\n", argv[0]);
		header = initheader(argv[0]);
		f = fopen(argv[0], "r");
		size = (int)header->size;
		printf("THIS ONE    %lo\n", strtol(header->size, 0, 8));
		bufsize = get_block_size(size);
		printf("Before malloc\n");
		buf = (char *)malloc(bufsize);
		if (!buf)
			return ;
		fread(buf, (size_t)size, 1, f);
		printf("%d    %d    %d\n", size, bufsize, bufsize - size);
		memset(&buf[size], 0, bufsize - size);
	}
	else
	{
		header = initheader(0);
		f = (FILE *)0;
		printf("No header before malloc\n");
		buf = (char *)malloc(BLOCK_SIZE);
		if (!buf)
			return ;
		memset(buf, 0, BLOCK_SIZE);
	}
	printf("Am I finally out?\n");
	fwrite(header, sizeof(t_header), 1, tar);
	printf("Meow\n");
	fwrite(buf, sizeof(buf), 1, tar);
	fclose(f);
	free_archive(buf, header);
	if (argv[0])
		ft_archive(tar, &argv[1]);
}

void			free_archive(char *buf, t_header *header)
{
	free(buf);
	free(header);
}
