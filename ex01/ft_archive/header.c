/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 18:11:29 by jchung            #+#    #+#             */
/*   Updated: 2018/01/28 18:11:36 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

char                *getName(uid_t uid)
{
    struct passwd    *pw;
    
    pw = getpwuid(uid);
    if (!pw)
        return ((char *)0);
    return (pw->pw_name);
}

char                *getGroup(gid_t gid)
{
    struct group    *gp;
    
    gp = getgrgid(gid);
    if (!gp)
        return ((char *)0);
    return (gp->gr_name);
}

t_header            *initheader(char *filename)
{
    t_header        *header;
    struct stat        st;
    
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
