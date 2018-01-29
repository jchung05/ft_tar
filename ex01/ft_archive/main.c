/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:33:21 by jchung            #+#    #+#             */
/*   Updated: 2018/01/28 20:38:30 by jchung           ###   ########.fr       */
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
