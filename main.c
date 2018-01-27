#include <fcntl.h>

typedef struct		s_header {
	
}					t_header;

typedef struct		s_block {
	int				size;
	char			*filename;
	struct s_block	*next;
}					t_block;

int					main(void)
{
	int				i;
	int				fo_tar;
	
	if (argc > 2)
	{
		i = 1;
		
		fo_tar = open((const char *)argv[1], O_CREAT);
		
		while (argv[++i])
		{
			//Prepare to "archive" files
			ft_archive(fo_tar, argv[i]);
		}
	}
	write(1, "\n", 1);
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