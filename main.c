/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:33:21 by jchung            #+#    #+#             */
/*   Updated: 2018/01/27 10:20:47 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

int				main(void)
{
	FILE		*tar;
	int			i;
	int			fd_tar;
	
	if (argc > 2)
	{
		i = 1;
		
		fd_tar = open((const char *)argv[1], O_CREAT);//Assign fd
//		tar = fopen((const char *)argv[1], "a");
		
		//Prepare to "archive" files by writing to fd per file;
		while (argv[++i])
		{
			argv[i + 1] != 0 ?
				ft_archive(fd_tar, argv[i], argv[i + 1]) :
				ft_archive(fd_tar, argv[i], NULL);
		}
	}
	else
		printf("usage: ft_archive tar_name file1 [file2 ... fileN]\n");
	close(fd_tar);
	return (0);
}

t_block			*init(char *name)
{
	t_block		*newblock;
	
	newblock = (t_block *)malloc(sizeof(t_block));
	if (!newblock)
		return ((t_block *)0);
	newblock->size = 0;
	newblock->filename = name;
	newblock->next = (t_block *)0;
	return (newblock);
}

void			ft_archive(int fd_tar, char *filename, char *nextfile)
{
	t_header	*header;
	char		*buf;
	int			fd_file;
	int			write_size;
	
	//Init the header of file
	header = (t_header *){filename, nextfile, 0, 0};

	//Open the file being tar'd
	fd_file = open((const char *)filename, O_RDONLY);
	//Read all of buffer into file with a bookmark
	write_size = write_buf(header, fd_file, &buf);
	//Close file
	close(fd_file);

	//Write to the tarball fd the size and buffer assigned from write_buf
	write(fd_tar, buf, write_size);
	//Free the buffer
	realloc(buf, 0);

}

/*
 * Return the total block size stored to buffer, AKA bufsize
 * Write directly to buf from parent function
 */
int				write_buf(t_header *header, int fd, char **buf)
{
	char		*itr;
	int			bufsize;
	int			size;
	int			rdbk;
	
	itr = *buf;
	bufsize = BLOCK_SIZE;
	size = 0;
	realloc(*buf, bufsize);

	while ((rdbk = read(fd, itr, BLOCK_SIZE)))
	{
		size += rdbk;
		itr += rdbk;
		if (size == arrsize)
			realloc(*buf, (bufsize += BLOCK_SIZE));
	}
	header->size = size;
	memset(itr, 0, arrsize - itr);
	return (bufsize);
}
