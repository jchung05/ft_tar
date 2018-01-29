/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 20:36:05 by jchung            #+#    #+#             */
/*   Updated: 2018/01/28 20:59:29 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

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
	settypeflag_mode(header, st.st_mode);
	sprintf(header->uid, "%06o ", st.st_uid);
	sprintf(header->gid, "%06o ", st.st_gid);
	sprintf(header->size, "%011llo", st.st_size);
	(header->size)[sizeof(header->size) - 1] = ' ';
	sprintf(header->mtime, "%lo", st.st_mtime);
	(header->mtime)[sizeof(header->mtime) - 1] = ' ';
	sprintf(header->magic, "%s", "ustar");
	memset(header->version, '0', 2);
	sprintf(header->uname, "%.31s", getname(st.st_uid));
	sprintf(header->gname, "%.31s", getgroup(st.st_gid));
	sprintf(header->devmajor, "%06u ", major(st.st_rdev));
	sprintf(header->devminor, "%06u ", minor(st.st_rdev));
	setchecksum(header);
	return (header);
}

char				*getname(uid_t uid)
{
	struct passwd	*pw;

	pw = getpwuid(uid);
	if (!pw)
		return ((char *)0);
	return (pw->pw_name);
}

char				*getgroup(gid_t gid)
{
	struct group	*gp;

	gp = getgrgid(gid);
	if (!gp)
		return ((char *)0);
	return (gp->gr_name);
}

void				setchecksum(t_header *header)
{
	size_t			checksum;
	int				i;
	unsigned char	*headarr;

	checksum = 0;
	i = -1;
	headarr = (unsigned char *)(header);
	memset(header->checksum, 32, 8);
	while (++i < sizeof(t_header))
		checksum += headarr[i];
	printf("%zu\n", checksum);
	sprintf(header->checksum, "%06zo", checksum);
}

void				settypeflag_mode(t_header *header, mode_t mode)
{
	if (S_ISREG(mode))
		memset(header->typeflag, '0', 1);
	else if (S_ISLNK(mode))
		memset(header->typeflag, '2', 1);
	else if (S_ISCHR(mode))
		memset(header->typeflag, '3', 1);
	else if (S_ISBLK(mode))
		memset(header->typeflag, '4', 1);
	else if (S_ISDIR(mode))
		memset(header->typeflag, '5', 1);
	else if (S_ISFIFO(mode))
		memset(header->typeflag, '6', 1);
	sprintf(header->mode, "%06o ", mode);
	memset(header->mode, '0', 3);
}
