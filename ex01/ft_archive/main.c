/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:33:21 by jchung            #+#    #+#             */
/*   Updated: 2018/01/28 11:33:56 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

int				main(int argc, char **argv)
{
	FILE		*tar;
	
	tar = (FILE *)0;
	if (argc > 2)
	{
		tar = fopen((const char *)argv[1], "w+");
		fclose(tar);
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
	if (lstat(filename, &st) < 0)
		return ((t_header *)0);
    sprintf(header->mode, "%06o ", st.st_mode);
    
    sprintf(header->uid, "%06o ", st.st_uid);
	
    sprintf(header->gid, "%06o ", st.st_gid);
	
    sprintf(header->size, "%011llo", st.st_size);
	
    sprintf(header->mtime, "%lo", st.st_mtime);

	memset(header->linkname, 0, sizeof(header->linkname));
	return (header);
}

/*
 * Pass the tar file object with "append" properties and the list of args from main
 * initheaderialize file header with metadata using stat()
 * initheaderialize a buffer with fopen and fread of the first arg in argv
 * buffer can be malloc'd according to the size of header->size rounded up to next block of 512
 * Set remainder of buffer to 
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
		size = strtol(header->size, 0, 8);
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
	fwrite(header, sizeof(t_header), 1, tar);
	printf("%zu\n", size);
	fwrite(buf, size, 1, tar);
	fclose(f);
	free_archive(buf, header);
//	if (argv[0])
//		ft_archive(tar, &argv[1]);
}

void			free_archive(char *buf, t_header *header)
{
	free(buf);
	free(header);
}
