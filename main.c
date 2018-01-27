/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:33:21 by jchung            #+#    #+#             */
/*   Updated: 2018/01/27 09:56:45 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

int				main(void)
{

	int			i;
	int			fd_tar;
	
	if (argc > 2)
	{
		i = 1;
		
		fd_tar = open((const char *)argv[1], O_CREAT);//Assign fd
		
		//Prepare to "archive" files by writing to fd per file;
		while (argv[++i])
		{
			argv[i + 1] != 0 ?
				ft_archive(fd_tar, argv[i], argv[i + 1]) :
				ft_archive(fd_tar, argv[i], NULL);
		}
	}
	else
		printf("usage: ft_archive tar_name file1 ...\n");
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

void			ft_archive(int tarfd, char *filename, char *nextfile)
{
	t_header	*header;
	char		*arr;
	char		*itr;
	int			fd_file;
	int			arrsize;
	int			size; //Size of file, convert to string later in header
	int			rdbk;
	
	//Init the header of file
	header = (t_header *){filename, nextfile, 0, 0};

	fd_file = open((const char *)filename, O_RDONLY);
	//Prepare to read, possibly with get_next_line
	size = 0;
	arrsize = BLOCK_SIZE;
	itr = arr;
	realloc(arr, arrsize); //Defaults to malloc

	while ((rdbk = read(fd_file, itr, BLOCK_SIZE)))
	{
		size += rdbk;
		if (size == arrsize)
			realloc(arr, (arrsize += BLOCK_SIZE));
		itr += rdbk;
	}
	header->size = size;
	memset(itr, 0, arrsize - itr);
	close(fd_file);
}

char			*write_buf(t_header *header, int fd)
{
	char		*buf;
	char		*itr;
	int			bufsize;
	int			size;
	int			rdbk;
	
	itr = buf;
	bufsize = BLOCK_SIZE;
	size = 0;
	realloc(buf, bufsize);

	while ((rdbk = read(fd, itr, BLOCK_SIZE)))
	{
		size += rdbk;
		if (size == arrsize)
			realloc(buf, (bufsize += BLOCK_SIZE));
		itr += rdbk;
	}
	header->size = size;
	memset(itr, 0, arrsize - itr);
	return (buf);
}
