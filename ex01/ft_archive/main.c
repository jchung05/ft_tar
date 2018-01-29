/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:33:21 by jchung            #+#    #+#             */
/*   Updated: 2018/01/28 17:47:00 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

int					main(int argc, char **argv)
{
	FILE			*tar;
	
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

int					get_block_size(int size)
{
	return ((size / BLOCK_SIZE + (size % BLOCK_SIZE == 0 ? 0 : 1)) * BLOCK_SIZE);
}

char				*getName(uid_t uid)
{
	struct passwd	*pw;

	pw = getpwuid(uid);
	if (!pw)
		return ((char *)0);
	return (pw->pw_name);
}

char				*getGroup(gid_t gid)
{
	struct group	*gp;

	gp = getgrgid(gid);
	if (!gp)
		return ((char *)0);
	return (gp->gr_name);
}

t_header			*initheader(char *filename)
{
	t_header		*header;
	struct stat		st;

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
	(header->size)[sizeof(header->size) - 1] = ' ';
    sprintf(header->mtime, "%lo", st.st_mtime);
	sprintf(header->magic, "%s", "ustar");
	memset(header->version, '0', 2);
	sprintf(header->uname, "%.31s", getName(st.st_uid));
	sprintf(header->gname, "%.31s", getGroup(st.st_gid));
	sprintf(header->devmajor, "%06u ", major(st.st_rdev));
	sprintf(header->devminor, "%06u ", minor(st.st_rdev));
	memset(header->linkname, 0, sizeof(header->linkname));
	return (header);
}

/*
 * Pass the tar file object with "append" properties and the list of args from main
 * initialize file header with metadata using stat()
 * initialize a buffer with fopen and fread of the first arg in argv
 * buffer can be malloc'd according to the size of header->size rounded up to next block of 512
 * Set remainder of buffer to 
 * fwrite() header and body to tar file object
 * Recursively call ft_archive on next argument of argv
 */
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

void				write_block(FILE *tar, char **argv)
{
	t_header		*header;
	FILE			*f;
	char			*buf;
	int				size;
	int				bufsize;

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

void				write_end_block(FILE *tar, char **argv)
{
	char			*buf;
	int				size;

	size = BLOCK_SIZE * 2;
	buf = (char *)malloc(size);
	if (!buf)
		return ;
	memset(buf, 0, size);
	fwrite(buf, size, 1, tar);
	free(buf);
}

void				free_archive(char *buf, t_header *header)
{
	free(buf);
	free(header);
}
