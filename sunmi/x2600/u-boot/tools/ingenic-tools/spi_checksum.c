/*
 * SPI SPL check tool.
 *
 * Copyright (C) 2013 Ingenic Semiconductor Co.,Ltd
 * Based on: u-boot-1.1.6/tools/spi_checksum/spi_checksum.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <config.h>

#ifdef CONFIG_M200
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_JZ4780
#define SKIP_SIZE 16
#endif
#ifdef CONFIG_JZ4775
#define SKIP_SIZE 16
#endif
#ifdef CONFIG_M150
#define SKIP_SIZE 16
#endif
#ifdef CONFIG_X1000
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X2000
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X1630
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X1830
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X1520
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X1021
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X1521
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X1800
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X1600
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X2000_V12
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_M300
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X2100
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X2500
#define SKIP_SIZE 2048
#endif

#ifdef CONFIG_X2580
#define SKIP_SIZE 2048
#endif
#ifdef CONFIG_X2600
#define SKIP_SIZE 2048
#endif

#ifdef CONFIG_AD100
#define SKIP_SIZE 2048
#endif

#define le(a) (((a & 0xff)<<24) | ((a>>8 & 0xff)<< 16) | ((a>>16 & 0xff)<< 8) | ((a>>24 & 0xff)))

/*
 * NAND FLASH
 */
#if defined(CONFIG_SPL_SFC_SUPPORT) || defined(CONFIG_SPL_SPI_NAND)

#if (defined(CONFIG_X2000_V12) || defined(CONFIG_M300) || defined(CONFIG_X2100) || \
		defined(CONFIG_X1600) || defined(CONFIG_X2600) || defined(CONFIG_AD100))
#define BUFFER_SIZE 256
#else
#define BUFFER_SIZE 4
#endif
#define CRC_POSITION	9		/* 9th bytes */
#define SPL_LENGTH_POSITION	12	/* 11th */

typedef unsigned char u8;
const u8 crc7_syndrome_table[256] = {
	0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f,
	0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77,
	0x19, 0x10, 0x0b, 0x02, 0x3d, 0x34, 0x2f, 0x26,
	0x51, 0x58, 0x43, 0x4a, 0x75, 0x7c, 0x67, 0x6e,
	0x32, 0x3b, 0x20, 0x29, 0x16, 0x1f, 0x04, 0x0d,
	0x7a, 0x73, 0x68, 0x61, 0x5e, 0x57, 0x4c, 0x45,
	0x2b, 0x22, 0x39, 0x30, 0x0f, 0x06, 0x1d, 0x14,
	0x63, 0x6a, 0x71, 0x78, 0x47, 0x4e, 0x55, 0x5c,
	0x64, 0x6d, 0x76, 0x7f, 0x40, 0x49, 0x52, 0x5b,
	0x2c, 0x25, 0x3e, 0x37, 0x08, 0x01, 0x1a, 0x13,
	0x7d, 0x74, 0x6f, 0x66, 0x59, 0x50, 0x4b, 0x42,
	0x35, 0x3c, 0x27, 0x2e, 0x11, 0x18, 0x03, 0x0a,
	0x56, 0x5f, 0x44, 0x4d, 0x72, 0x7b, 0x60, 0x69,
	0x1e, 0x17, 0x0c, 0x05, 0x3a, 0x33, 0x28, 0x21,
	0x4f, 0x46, 0x5d, 0x54, 0x6b, 0x62, 0x79, 0x70,
	0x07, 0x0e, 0x15, 0x1c, 0x23, 0x2a, 0x31, 0x38,
	0x41, 0x48, 0x53, 0x5a, 0x65, 0x6c, 0x77, 0x7e,
	0x09, 0x00, 0x1b, 0x12, 0x2d, 0x24, 0x3f, 0x36,
	0x58, 0x51, 0x4a, 0x43, 0x7c, 0x75, 0x6e, 0x67,
	0x10, 0x19, 0x02, 0x0b, 0x34, 0x3d, 0x26, 0x2f,
	0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
	0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04,
	0x6a, 0x63, 0x78, 0x71, 0x4e, 0x47, 0x5c, 0x55,
	0x22, 0x2b, 0x30, 0x39, 0x06, 0x0f, 0x14, 0x1d,
	0x25, 0x2c, 0x37, 0x3e, 0x01, 0x08, 0x13, 0x1a,
	0x6d, 0x64, 0x7f, 0x76, 0x49, 0x40, 0x5b, 0x52,
	0x3c, 0x35, 0x2e, 0x27, 0x18, 0x11, 0x0a, 0x03,
	0x74, 0x7d, 0x66, 0x6f, 0x50, 0x59, 0x42, 0x4b,
	0x17, 0x1e, 0x05, 0x0c, 0x33, 0x3a, 0x21, 0x28,
	0x5f, 0x56, 0x4d, 0x44, 0x7b, 0x72, 0x69, 0x60,
	0x0e, 0x07, 0x1c, 0x15, 0x2a, 0x23, 0x38, 0x31,
	0x46, 0x4f, 0x54, 0x5d, 0x62, 0x6b, 0x70, 0x79
};

static inline u8 crc7_byte(u8 crc, u8 data)
{
	return crc7_syndrome_table[(crc << 1) ^ data];
}

u8 crc7(u8 crc, u8 *buffer, int len)
{
	while (len--)
		crc = crc7_byte(crc, *buffer++);
	return crc;
}

int main(int argc, char *argv[])
{
	int count;
	FILE * fd;
	int bytes_read;
	u8 buffer[BUFFER_SIZE];
	volatile int t = 0;
	u8 crc = 0;

	if (argc != 2) {
		printf("Usage: %s fromfile tofile\n\a",argv[0]);
		return 1;
	}

	fd = fopen(argv[1], "rb+");
	if (fd == NULL) {
		printf("Open %s Error\n", argv[1]);
		return 1;
	}

	count = 0;

	while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fd)) > 0) {
		if (t >= SKIP_SIZE) {
			crc = crc7(crc, buffer, bytes_read);
		} else {
			t += BUFFER_SIZE;
		}
		count += bytes_read;
	}

	printf("spi spl count = 0x%08x \n", count);
	printf("spi spl crc7 = 0x%x \n", crc);

	/*set crc*/
	fseek( fd, CRC_POSITION, SEEK_SET);

	if ((t = fwrite(&crc, 1, 1, fd)) != 1) {
		printf("Write %s Error\n",argv[1]);
		return 1;
	}

	/*set spl len*/
	fseek( fd, SPL_LENGTH_POSITION, SEEK_SET);
#if (defined(CONFIG_X2000_V12) || defined(CONFIG_M300) || defined(CONFIG_X2100) || defined(CONFIG_X1600) || defined(CONFIG_X2600) || defined(CONFIG_AD100))
	if ((t = fwrite(&count, 2, 1, fd)) != 1) {
#else
	if ((t = fwrite(&count, 4, 1, fd)) != 1) {
#endif
		printf("Check: Write %s Error\n",argv[1]);
		return 1;
	}

#if (defined(CONFIG_X2000_V12) || defined(CONFIG_M300) || defined(CONFIG_X2100) || defined(CONFIG_X1600) || defined(CONFIG_X2600) || defined(CONFIG_AD100))
	/* set env crc */
	fseek(fd, 0x100, SEEK_SET);

	memset(buffer, 0, BUFFER_SIZE);
	if ((t = fread(buffer, 1, 256, fd)) < 0) {
		printf("read %d \n",t);
	}

	crc = crc7(0, buffer, 256);

	fseek(fd, 0xe, SEEK_SET);

	if ((t = fwrite(&crc, 1, 1, fd)) != 1) {
		printf("crc: Write %s Error\n",argv[1]);
		return 1;
	}


	/* set spl head crc */
	fseek(fd, 0, SEEK_SET);

	memset(buffer, 0, BUFFER_SIZE);
	if ((t = fread(buffer, 15, 1, fd)) < 0) {
		printf("read %d \n",t);
	}

	crc = crc7(0, buffer, 15);

	fseek(fd, 0xf, SEEK_SET);

	if ((t = fwrite(&crc, 1, 1, fd)) != 1) {
		printf("crc: Write %s Error\n",argv[1]);
		return 1;
	}
#endif

	fclose(fd);

	return 0;
}
#else

/*
 * NOR FLASH
 */
int main(int argc, char *argv[])
{
	int count;
	FILE * fd;
	int bytes_read;
	char buffer[BUFFER_SIZE];
	unsigned int check = 0;
	volatile int t = 0;

	if (argc != 2) {
		printf("Usage: %s fromfile tofile\n\a",argv[0]);
		return 1;
	}

	fd = fopen(argv[1], "rb+");
	if (fd < 0) {
		printf("Open %s Error\n", argv[1]);
		return 1;
	}

	count = 0;

	while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fd)) > 0) {
		if (t >= SKIP_SIZE)
			check += *((unsigned int *)buffer);
		else
			t += BUFFER_SIZE;
		count += bytes_read;
	}

	printf("spi spl count = %d \n", count);
	printf("spi spl check = %#x \n", check);

	fseek( fd, 8, SEEK_SET);

	if ((t = fwrite(&count, 4, 1, fd)) != 1) {
		printf("Write %s Error\n",argv[1]);
		return 1;
	}

	check = 0 - check;
	if ((t = fwrite(&check, 4, 1, fd)) != 1) {
		printf("Check: Write %s Error\n",argv[1]);
		return 1;}

	fclose(fd);

	return 0;
}
#endif
