
#include "cursor.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <math.h>
#include <stdlib.h>

#include <data_structures.h>
#include <graph.h>

void read_cursor_file_header(char *path, struct cursor_file_header *header)
{
	int fd = open (path, O_RDONLY);
	if (fd == -1)
	{
		perror ("open");
		exit (-1);
	}
	read (fd, header->cfReserve, 2);
	read (fd, &header->cfType, 2);
	read (fd, &header->cfNum, 2);

	close (fd);
}

void print_cursor_file_header(struct cursor_file_header *header)
{
	printf ("cfReserve: %s\n", header->cfReserve);
	printf ("cfType: %d\n", header->cfType);
	printf ("cfNum: %d\n", header->cfNum);
}

void read_cursor_info_header(char *path, struct cursor_info_header *header, int order)
{
	int fd = open (path, O_RDONLY);
	if (fd == -1)
	{
		perror ("open");
		exit (-1);
	}

	lseek (fd, 6+16*(order-1), SEEK_SET);
	read (fd, &header->ciWidth, 1);
	read (fd, &header->ciHeight, 1);
	read (fd, &header->ciColorCount, 1);
	read (fd, &header->ciReserve1, 1);
	read (fd, header->ciReserve2, 4);
	read (fd, &header->ciDataSize, 4);
	read (fd, &header->ciDataOffset, 4);

	close (fd);
}

void print_cursor_info_header(struct cursor_info_header *header)
{
	printf ("ciWidth: %d\n", header->ciWidth);
	printf ("ciHeight: %d\n", header->ciHeight);
	printf ("ciColorCount: %d\n", header->ciColorCount);
	printf ("ciReserve1: %c\n", header->ciReserve1);
	printf ("ciReserve2: %s\n", header->ciReserve2);
	printf ("ciDataSize: %ld\n", header->ciDataSize);
	printf ("ciDataOffset: %ld\n", header->ciDataOffset);
}

void read_bmp_info_header(char *path, struct bmp_info_header *header, int order)
{
	int fd = open (path, O_RDONLY);
	if (fd == -1)
	{
		perror ("open");
		exit (-1);
	}

	struct cursor_info_header ciHeader;
	memset (&ciHeader, 0, sizeof (struct cursor_info_header));
	read_cursor_info_header(path, &ciHeader, order);

	lseek (fd, ciHeader.ciDataOffset, SEEK_SET);
	read (fd, &header->biSize, 4);
	read (fd, &header->biWidth, 4);
	read (fd, &header->biHeight, 4);
	read (fd, &header->biNum, 2);
	read (fd, &header->biBitCount, 2);
	read (fd, &header->biCompressType, 4);
	read (fd, &header->biDataSize, 4);
	read (fd, header->biReserve, 16);

	close (fd);
}

void print_bmp_info_header(struct bmp_info_header *header)
{
	printf ("biSize: %ld\n", header->biSize);
	printf ("biWidth: %ld\n", header->biWidth);
	printf ("biHeight: %ld\n", header->biHeight);
	printf ("biNum: %d\n", header->biNum);
	printf ("biBitCount: %d\n", header->biBitCount);
	printf ("biCompressType: %ld\n", header->biCompressType);
	printf ("biDataSize: %ld\n", header->biDataSize);
	printf ("biReserve: %s\n", header->biReserve);
}

void read_palette(char *path, struct color *pale, int bits, int order)
{
	struct cursor_info_header ciHeader;
	memset (&ciHeader, 0, sizeof (struct cursor_info_header));
	read_cursor_info_header(path, &ciHeader, order);

	int fd = open (path, O_RDONLY);
	if (fd == -1)
	{
		perror ("open");
		exit (-1);
	}

	lseek (fd, ciHeader.ciDataOffset + 40, SEEK_SET);

	int i;
	for (i = 0; i < bits; i++)
	{
		read (fd, &pale[i].r, 1);
		read (fd, &pale[i].g, 1);
		read (fd, &pale[i].b, 1);
		read (fd, &pale[i].a, 1);
	}

	close (fd);
}

void print_palette(struct color *pale, int bits)
{
	int i;
	for (i = 0; i < bits; i++)
	{
/*		printf ("%0x\t%0x %0x %0x %0x\n", i, pale[i].r, pale[i].g, pale[i].b);	*/
	}
}

void read_and_xor_data(char *path, struct cursor* cur)
{
	cur->xorData = (char**) malloc (sizeof (char*) * cur->cfHeader.cfNum);
	cur->andData = (char**) malloc (sizeof (char*) * cur->cfHeader.cfNum);
	int i;
	int fd = open (path, O_RDONLY);
	if (fd == -1)
	{
		perror ("open");
		exit (-1);
	}


	for ( i=0; i<cur->cfHeader.cfNum; i++)
	{
		int xorSize = 0;
		int andSize = 0;
		/* 24位和32位光标文件的xor和and位图的数据大小*/
		if (cur->biHeader[i].biBitCount == 24 || cur->biHeader[i].biBitCount == 32)
		{
			xorSize = cur->ciHeader[i].ciWidth * cur->ciHeader[i].ciHeight * cur->biHeader[i].biBitCount / 8;

			if (cur->ciHeader[i].ciWidth % 32 == 0)
			{
				 andSize = cur->ciHeader[i].ciWidth * cur->ciHeader[i].ciHeight / 8;
			}
			else
			{
				andSize = (cur->ciHeader[i].ciWidth / 32 + 1) * 32 * cur->ciHeader[i].ciHeight / 8;
			}
		}

		/* 1 4 8位光标文件的xor和and位图的数据大小*/
		if(cur->biHeader[i].biBitCount == 8 ||
			cur->biHeader[i].biBitCount == 4 ||
			cur->biHeader[i].biBitCount == 1)
		{
			xorSize = cur->ciHeader[i].ciWidth * cur->ciHeader[i].ciWidth * cur->biHeader[i].biBitCount / 8;
			if(cur->biHeader[i].biBitCount != 1)
			xorSize += (1 << cur->biHeader[i].biBitCount) * 4;
			if(cur->ciHeader[i].ciWidth == 16)
				andSize = 64;
			if(cur->ciHeader[i].ciWidth == 24)
				andSize = 96;
			if(cur->ciHeader[i].ciWidth == 32)
				andSize = 128;
			if(cur->ciHeader[i].ciWidth == 48)
				andSize = 384;
		}

		cur->xorData[i] = (char*) malloc (sizeof (char) * xorSize);
		lseek (fd, cur->ciHeader[i].ciDataOffset + 40, SEEK_SET);
		read (fd, cur->xorData[i], xorSize);

		cur->andData[i] = (char*) malloc (sizeof (char) * andSize);
		read (fd, cur->andData[i], andSize);
	}

	close (fd);
}


void init_cursor(char *path, struct cursor *cur)
{
	read_cursor_file_header(path, &cur->cfHeader);

	cur->ciHeader = calloc (cur->cfHeader.cfNum, sizeof (struct cursor_info_header));
	cur->biHeader = calloc (cur->cfHeader.cfNum, sizeof (struct bmp_info_header));

	int i;
	for ( i=0; i<cur->cfHeader.cfNum; i++)
	{
		read_cursor_info_header (path, &cur->ciHeader[i], i+1);
		read_bmp_info_header(path, &cur->biHeader[i], i+1);
		read_and_xor_data (path, cur);
	}
}



