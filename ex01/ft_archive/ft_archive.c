/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_archive.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 20:31:43 by jchung            #+#    #+#             */
/*   Updated: 2018/01/28 20:35:25 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

void			ft_archive(FILE *tar, char **argv)
{
	if (argv[0])
	{
		write_block(tar, argv);
	}
	else
		write_end_block(tar, argv);
	if (argv[0])
		ft_archive(tar, &argv[1]);
}

void			write_block(FILE *tar, char **argv)
{
	t_header	*header;
	FILE		*f;
	char		*buf;
	int			size;
	int			bufsize;

	header = initheader(argv[0]);
	f = fopen(argv[0], "r");
	size = strtol(header->size, 0, 8);
	bufsize = get_block_size(size);
	buf = (char *)malloc(bufsize);
	if (!buf)
		return ;
	fread(buf, (size_t)size, 1, f);
	memset(&buf[size], 0, bufsize - size);
	fwrite(header, sizeof(t_header), 1, tar);
	fwrite(buf, bufsize, 1, tar);
	fclose(f);
	free_archive(buf, header);
}

void			write_end_block(FILE *tar, char **argv)
{
	char		*buf;
	int			size;

	size = BLOCK_SIZE * 2;
	buf = (char *)malloc(size);
	if (!buf)
		return ;
	memset(buf, 0, size);
	fwrite(buf, size, 1, tar);
	free(buf);
}

int				get_block_size(int size)
{
	if (size % BLOCK_SIZE == 0)
		return ((size / BLOCK_SIZE) * BLOCK_SIZE);
	return ((size / BLOCK_SIZE + 1) * BLOCK_SIZE);
}

void			free_archive(char *buf, t_header *header)
{
	free(buf);
	free(header);
}
