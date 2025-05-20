#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/file.h>
#include <fcntl.h>

#define HWINFO_FILE_MAX_SIZE	(1 * 1024 * 1024) /*1MB*/

enum HWINFO_SEEK {
	HWINFO_MASK1_SEEK				= 0,
	HWINFO_SIZE_SEEK				= 10,
	HWINFO_CONTENT_SEEK				= 18,
	HWINFO_MASK2_SEEK				= 118,
	HWINFO_MAX_SEEK					= 127
};

enum HWINFO_LENS {
	HWINFO_MASK1_LENS				= 10,
	HWINFO_SIZE_LENS				= 8,
	HWINFO_CONTENT_LENS				= 100,
	HWINFO_MASK2_LENS				= 10,
	HWINFO_BLOCK_LENS				= 128
};

enum HWINFO_BLOCK {
    HWINFO_BLOCK_PROJECT 			= 0,
	HWINFO_BLOCK_CRC				= 255,
    HWINFO_BLOCK_MAX 				= 256
};

int s_file_size(char *name)
{
    struct stat finfo;

    if(stat(name,&finfo)<0)return errno;

    return (int)finfo.st_size;
}

int s_file_exist(char *name)
{
    struct stat finfo;

    if(stat(name,&finfo)<0)return errno;

    return 0;
}

int hwinfo_block_fill(char *file_path, char *mask, char *size, char *content, enum HWINFO_BLOCK idx)
{
	int ret;
	int fd;
	int file_size = 0;
	int content_size = 0;
	char *file_buf = NULL;

	if((file_path == NULL) || (mask == NULL) || (size == NULL) || (content == NULL)) return -2000;
	if((strlen(file_path) == 0) || (strlen(mask) == 0) || (strlen(size) == 0) || (strlen(content) == 0)) return -2001;

	content_size = atoi(size);
	if(content_size > HWINFO_CONTENT_LENS) {
		printf("content size error:size=%d\n", content_size);
		return -2002;
	}
	if(strlen(content) != content_size) {
		printf("content len=%d, don't match the gived size=%d\n", (int)strlen(content), content_size);
		return -20020;
	}

	if((strlen(mask) >= HWINFO_MASK1_LENS) || (strlen(size) >= HWINFO_SIZE_LENS)) {
		printf("param length is error\n");
		return -20021;
	}

	if((idx < HWINFO_BLOCK_PROJECT) || (idx >= HWINFO_BLOCK_MAX)) {
		printf("block index error\n");
		return -2003;
	}
	if(s_file_exist(file_path)) {
		printf("file don't exist\n");
		return -2004;
	}

	file_size = s_file_size(file_path);
	if((file_size < ((idx + 1) * HWINFO_BLOCK_LENS)) || (file_size > HWINFO_FILE_MAX_SIZE)) {
		printf("file size error, size=%d\n", file_size);
		return -2005;
	}

    do {
        fd = open(file_path, O_RDONLY | O_CLOEXEC);
    } while(fd < 0 && errno == EINTR);
    if(fd < 0) return errno;

	file_buf = malloc(file_size);
	if(file_buf == NULL) {
		printf("malloc failed\n");
	}

    do {
        ret = read(fd, file_buf, file_size);
    }while(ret<0 && errno==EINTR);
    if((ret<0) || (ret < file_size)) {
		printf("file read error, file_size=%d, ret=%d\n", file_size, ret);
		ret = errno;
		goto EXIT;
    }
	close(fd);

	strcpy(&file_buf[idx * HWINFO_BLOCK_LENS + HWINFO_MASK1_SEEK], mask);
	strcpy(&file_buf[idx * HWINFO_BLOCK_LENS + HWINFO_SIZE_SEEK], size);
	strncpy(&file_buf[idx * HWINFO_BLOCK_LENS + HWINFO_CONTENT_SEEK], content, content_size);
	strcpy(&file_buf[idx * HWINFO_BLOCK_LENS + HWINFO_MASK2_SEEK], mask);

    do {
        fd = open(file_path, O_WRONLY | O_TRUNC | O_CLOEXEC);
    } while(fd < 0 && errno == EINTR);
    if(fd < 0) {
		printf("file write open failed\n");
		ret = errno;
		goto EXIT;
    }

    do
    {
        ret = write(fd, file_buf, file_size);
    }while(ret<0 && errno==EINTR);
    if((ret<0) || (ret < file_size)) {
		printf("file write error, file_size=%d, ret=%d\n", file_size, ret);
		ret = errno;
		goto EXIT;
    }

	ret = 0;
EXIT:
	if(file_buf != NULL) {
		free(file_buf);
	}

	if(fd >= 0) {
    	close(fd);
	}
	return ret;
}

/*
 *param:
 *1: file_path
 *2: mask
 *3: size
 *4: content
 *5: hwinfo_block_index
 */
int main(int argc, char **argv)
{
	int hwinfo_block_index = -1;
	char file_path[256];
	char mask[HWINFO_MASK1_LENS];
	char size[HWINFO_SIZE_LENS];
	char content[HWINFO_CONTENT_LENS];

	if(argc != 6) {
		printf("param error\n");
		printf("help:\n");
		printf("param1: file_path\n");
		printf("param2: mask\n");
		printf("param3: size\n");
		printf("param4: content\n");
		printf("param5: hwinfo_block_index\n");
		return -1000;
	}

	if((strlen(argv[1]) == 0) ||(strlen(argv[2]) == 0)
		||(strlen(argv[3]) == 0) ||(strlen(argv[4]) == 0)
		||(strlen(argv[5]) == 0)) {
		printf("err: arg string is empty\n");
		return -1000;
	}
	hwinfo_block_index = atoi(argv[5]);
	if((hwinfo_block_index < HWINFO_BLOCK_PROJECT)
		|| (hwinfo_block_index >= HWINFO_BLOCK_MAX)) {
		printf("hwinfo_block_index error\n");
		return -900;
	}

	memset(file_path, 0, sizeof(file_path));
	memset(mask, 0, sizeof(mask));
	memset(size, 0, sizeof(size));
	memset(content, 0, sizeof(content));
	if(strlen(argv[1]) >= sizeof(file_path)) {
		printf("error: file path is too long\n");
		return -801;
	} else {
		strcpy(file_path, argv[1]);
	}
	if(strlen(argv[2]) > sizeof(mask)) {
		printf("error: mask is too long\n");
		return -802;
	} else {
		strcpy(mask, argv[2]);
	}
	if(strlen(argv[3]) > sizeof(size)) {
		printf("error: size is too long\n");
		return -803;
	} else {
		strcpy(size, argv[3]);
	}
	if(strlen(argv[4]) > sizeof(content)) {
		printf("error: content is too long\n");
		return -804;
	} else {
		strcpy(content, argv[4]);
	}

	return hwinfo_block_fill(file_path, mask, size, content, hwinfo_block_index);
}
