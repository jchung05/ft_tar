/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 08:33:21 by jchung            #+#    #+#             */
/*   Updated: 2018/01/27 15:44:48 by jchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

int				main(void)
{
//	FILE		*tar;
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
		//ft_archive can be done recursively here
//		ft_archive(tar, &argv[2]);
	}
	else
		printf("usage: ft_archive tar_name file1 [file2 ... fileN]\n");
	close(fd_tar);
//	fclose(tar);
	return (0);
}

/*
 * Pass the tar file object with "append" properties and the list of args from main
 * Initialize file header with metadata using stat()
 * Initialize a buffer with fopen and fread of the first arg in argv
 * buffer can be malloc'd according to the size of header->size rounded up to next block of 512
 * Set remainder of buffer to \0
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

	//Check that any argument exists
	if (argv[0])
	{
		//Determine if there is a trailing file to write to linkname, else write nothing
		header = init(argv[0], (argv[1] != 0 ? argv[1] : 0));

		//Open file argument, guaranteed to exist. Read only
		f = fopen(argv[0], "o");
		//Convert string of header->size into number size
		size = atoi(header->size);
		//Find the bufsize to malloc
		bufsize = get_block_size(size);
		//Malloc the buffer
		buf = (char *)malloc(bufsize);
		if (!buf)
			return ;
		//Read from the opened file f into buf
		fread(buf, (size_t)size, 1, f);
		//Set remainder of buffer to 0
		memset(&buf[size], 0, bufsize - size);
	}
	else
	{
		header = {};
		buf = (char *)malloc(BLOCK_SIZE);
		if (!buf)
			return ;
		memset(buf, 0, BLOCK_SIZE);
	}
	fwrite(header, sizeof(t_header), 1, tar);
	fwrite(buf, sizeof(buf), 1, tar);
	fclose(f);
	realloc(buf, 0);
	if (argv[0])
		ft_archive(tar, &argv[1]);
}
	/*
		//Write to the tar file object both the header and the buffer content
		fwrite(header, sizeof(t_header), 1, f);
		fwrite(buf, sizeof(buf), 1, f);
		//Close the read file out
		fclose(f);
		//Free the buffer
		realloc(buf, 0);
		//Recursively call ft_archive until all arguments are written to tar object file
		ft_archive(tar, &argv[1]);
	}
	else //Base case where no more arguments are available
	{
		//Init an empty header block
		header = {};
		//Malloc an empty buffer
		buf = (char *)malloc(BLOCK_SIZE);
		if (!buf)
			return ;
		//Set buffer to all null
		memset(buf, 0, BLOCK_SIZE);
		//Write both empty header blocks
		fwrite(header, sizeof(t_header), 1, f);
		fwrite(buf, sizeof(buf), 1, f);
		//Close the read file out
		fclose(f);
		//Free the buffer
		realloc(buf, 0);
	}
}
*/

int				get_block_size(int size);
{
	return ((size / BLOCK_SIZE + (size % BLOCK_SIZE == 0 ? 0 : 1)) * BLOCK_SIZE);
}

/*
void			ft_archive(int fd_tar, char *filename, char *nextfile)
{
	t_header	*header;
	char		*buf;
	int			fd_file;
	int			write_size;
	
	//Init the header of file
	//header = (t_header *){filename, 0, 0, 0, 0, 0, 0, 0, nextfile, 0};
	header = init(filename, nextfile);

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
*/

t_header		*init(char *filename, char *nextfile)
{
	t_header	*header;
	struct stat	st;

	header = (t_header *){filename, 0, 0, 0, 0, 0, 0, 0, nextfile, 0};
	if (!stat(filename, &st))
		return ((t_header *)0);
	header->mode = st.st_mode;
	header->uid = st.st_uid;
	header->gid = st.st_gid;
	header->size = st.st_size;
	header->mtime = st.st_mtime;
	return (header);
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
