/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:15:55 by jchung            #+#    #+#             */
/*   Updated: 2018/01/27 08:36:34 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TAR_H
# define FT_TAR_H
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct		s_header {
	char			name[100];
	char			size[12];
	char			filler[400];
}					t_header;

#endif
