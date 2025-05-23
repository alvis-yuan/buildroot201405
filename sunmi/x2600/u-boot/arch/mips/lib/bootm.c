/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>
#include <asm/addrspace.h>

DECLARE_GLOBAL_DATA_PTR;

#define	LINUX_MAX_ENVS		256
#define	LINUX_MAX_ARGS		256

static int linux_argc;
static char **linux_argv;
static char *argp;

static char **linux_env;
static char *linux_env_p;
static int linux_env_idx;

static void linux_params_init(ulong start, char *commandline);
static void linux_env_set(char *env_name, char *env_val);

static void boot_prep_linux(bootm_headers_t *images)
{
	char *commandline = getenv("bootargs");
	char env_buf[12];
	char *cp;

	if (IMAGE_ENABLE_OF_LIBFDT && images->ft_len) {
		if (image_setup_linux(images)) {
			printf("FDT Creating Failed:hanging ...\n");
		}
	}

	linux_params_init(UNCACHED_SDRAM(gd->bd->bi_boot_params), commandline);

#ifdef CONFIG_MEMSIZE_IN_BYTES
	sprintf(env_buf, "%lu", (ulong)gd->ram_size);
	debug("## Giving linux memsize in bytes, %lu\n", (ulong)gd->ram_size);
#else
	sprintf(env_buf, "%lu", (ulong)(gd->ram_size >> 20));
	debug("## Giving linux memsize in MB, %lu\n",
		(ulong)(gd->ram_size >> 20));
#endif /* CONFIG_MEMSIZE_IN_BYTES */

	linux_env_set("memsize", env_buf);

	sprintf(env_buf, "0x%08X", (uint) UNCACHED_SDRAM(images->rd_start));
	linux_env_set("initrd_start", env_buf);

	sprintf(env_buf, "0x%X", (uint) (images->rd_end - images->rd_start));
	linux_env_set("initrd_size", env_buf);

	sprintf(env_buf, "0x%08X", (uint) (gd->bd->bi_flashstart));
	linux_env_set("flash_start", env_buf);

	sprintf(env_buf, "0x%X", (uint) (gd->bd->bi_flashsize));
	linux_env_set("flash_size", env_buf);

	cp = getenv("ethaddr");
	if (cp)
		linux_env_set("ethaddr", cp);

	cp = getenv("eth1addr");
	if (cp)
		linux_env_set("eth1addr", cp);
}

static void linux_cmdline_set(const char *value, size_t len)
{
	if(linux_argc >= LINUX_MAX_ARGS) {
		printf("Too many linux_args: %d, max: %d\n", linux_argc, LINUX_MAX_ARGS);
		return;
	}

	linux_argv[linux_argc] = argp;
	memcpy(argp, value, len);
	argp[len] = 0;

	argp += len + 1;
	linux_argc++;
}


static void boot_jump_linux(bootm_headers_t *images)
{
	void (*theKernel) (int, char **, char **, int *);

	/* find kernel entry point */
	theKernel = (void (*)(int, char **, char **, int *))images->ep;

	debug("## Transferring control to Linux (at address %08lx) ...\n",
		(ulong) theKernel);

	bootstage_mark(BOOTSTAGE_ID_RUN_OS);

	/* we assume that the kernel is in place */
	printf("\nStarting kernel ...\n\n");
	if (IMAGE_ENABLE_OF_LIBFDT && images->ft_len)
		theKernel(-2, (ulong)images->ft_addr, 0, 0);
	else
		theKernel(linux_argc, linux_argv, linux_env, 0);
}

int do_bootm_linux(int flag, int argc, char * const argv[],
			bootm_headers_t *images)
{
	/* No need for those on MIPS */
	if (flag & BOOTM_STATE_OS_BD_T || flag & BOOTM_STATE_OS_CMDLINE)
		return -1;

	if (flag & BOOTM_STATE_OS_PREP) {
		boot_prep_linux(images);
		return 0;
	}

	if (flag & BOOTM_STATE_OS_GO) {
		boot_jump_linux(images);
		return 0;
	}

	boot_prep_linux(images);
	boot_jump_linux(images);

	/* does not return */
	return 1;
}

static void linux_params_init(ulong start, char *line)
{
	char *next, *quote/*, *argp*/;

	linux_argc = 1;
	linux_argv = (char **) start;
	linux_argv[0] = 0;
	argp = (char *) (linux_argv + LINUX_MAX_ARGS);

	next = line;

	while (line && *line && linux_argc < LINUX_MAX_ARGS) {
		quote = strchr(line, '"');
		next = strchr(line, ' ');

		while (next && quote && quote < next) {
			/* we found a left quote before the next blank
			 * now we have to find the matching right quote
			 */
			next = strchr(quote + 1, '"');
			if (next) {
				quote = strchr(next + 1, '"');
				next = strchr(next + 1, ' ');
			}
		}

		if (!next)
			next = line + strlen(line);

		linux_argv[linux_argc] = argp;
		memcpy(argp, line, next - line);
		argp[next - line] = 0;

		argp += next - line + 1;
		linux_argc++;

		if (*next)
			next++;

		line = next;
	}

	int found_mem = 0;
	{
		int i = 0;
		for(i = 0; i < linux_argc; i++) {
			char *s = linux_argv[i];

			if(!s) {
				continue;
			}
			/*found mem= str*/
			if(s[0] == 'm' && s[1] == 'e' && s[2] == 'm' && s[3] == '=') {
				found_mem = 1;
			}
		}
	}


#if (CONFIG_BOOTARGS_AUTO_MODIFY == 1)

	/*modify argp before linux_set_env.*/
	/*default environment*/
#include <env_default.h>
	unsigned long ram_size = (ulong)gd->ram_size >> 20;

	if(!found_mem) {
		const char *mem_str = NULL;
		int mem_strlen = 0;
		/* 64M size ddr*/
		if(ram_size == 64) {
#ifdef CONFIG_BOOTARGS_MEM_64M
			mem_str = CONFIG_BOOTARGS_MEM_64M;
			mem_strlen = strlen(CONFIG_BOOTARGS_MEM_64M);

#endif
		} else if(ram_size == 128) {
#ifdef CONFIG_BOOTARGS_MEM_128M
			mem_str = CONFIG_BOOTARGS_MEM_128M;
			mem_strlen = strlen(CONFIG_BOOTARGS_MEM_128M);
#endif
		} else if(ram_size == 256) {
#ifdef CONFIG_BOOTARGS_MEM_256M
			mem_str = CONFIG_BOOTARGS_MEM_256M;
			mem_strlen = strlen(CONFIG_BOOTARGS_MEM_256M);
#endif
		} else if(ram_size == 512) {
#ifdef CONFIG_BOOTARGS_MEM_512M
			mem_str = CONFIG_BOOTARGS_MEM_512M;
			mem_strlen = strlen(CONFIG_BOOTARGS_MEM_512M);
#endif
		} else if(ram_size == 32) {
#ifdef CONFIG_BOOTARGS_MEM_32M
			mem_str = CONFIG_BOOTARGS_MEM_32M;
			mem_strlen = strlen(CONFIG_BOOTARGS_MEM_32M);
#endif
		} else if(ram_size == 16) {
#ifdef CONFIG_BOOTARGS_MEM_16M
			mem_str = CONFIG_BOOTARGS_MEM_16M;
			mem_strlen = strlen(CONFIG_BOOTARGS_MEM_16M);
#endif
		} else {
			printf("Warining ... bootargs for ram_size (%d)M not defined!\n");
		}
		linux_cmdline_set(mem_str, mem_strlen);
		printf("bootargs for mem adjust to : %s\n", mem_str);
	}

#else
	if(!found_mem) {
		printf("Warining, mem= in bootargs is not defined, Kernel may hungup!\n");
	}
#endif

	linux_env = (char **) (((ulong) argp + 15) & ~15);
	linux_env[0] = 0;
	linux_env_p = (char *) (linux_env + LINUX_MAX_ENVS);
	linux_env_idx = 0;
}

static void linux_env_set(char *env_name, char *env_val)
{
	if (linux_env_idx < LINUX_MAX_ENVS - 1) {
		linux_env[linux_env_idx] = linux_env_p;

		strcpy(linux_env_p, env_name);
		linux_env_p += strlen(env_name);

		strcpy(linux_env_p, "=");
		linux_env_p += 1;

		strcpy(linux_env_p, env_val);
		linux_env_p += strlen(env_val);

		linux_env_p++;
		linux_env[++linux_env_idx] = 0;
	}
}
