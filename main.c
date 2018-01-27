/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:33:21 by jchung            #+#    #+#             */
/*   Updated: 2018/01/27 08:35:22 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

typedef struct		s_header {
	char			name[100];
	char			size[12];
	char			linkname[100];
	char			filler[300];
}					t_header;

int					main(void)
{
	int				i;
	int				fo_tar;
	
	if (argc > 2)
	{
		i = 1;
		
		fo_tar = open((const char *)argv[1], O_CREAT);//Assign fd
		
		//Prepare to "archive" files by writing to fd per file;
		while (argv[++i])
		{
			ft_archive(fo_tar, argv[i]);
		}
	}
	else
		printf("usage: ft_archive tar_name file1 ...\n");
	close(fo_tar);
	return (0);
}

t_block				*init(char *name)
{
	t_block			*newblock;
	
	newblock = (t_block *)malloc(sizeof(t_block));
	if (!newblock)
		return ((t_block *)0);
	newblock->size = 0;
	newblock->filename = name;
	newblock->next = (t_block *)0;
	return (newblock);
}

void				ft_archive(int tarfd, char *filename)
{
	t_block			*block;
	int				fo_file;
	
	block = init(filename);
	fo_file = open((const char *)filename, O_RDONLY);
	//Prepare to read, possibly with get_next_line
}
