/*
 * Copyright (c) International Business Machines Corp., 2006
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Author: Artem Bityutskiy (Битюцкий Артём)
 */

/* Here we keep miscellaneous functions which are used all over the UBI code */

#include <ubi_uboot.h>
#include "ubi.h"

/**
 * calc_data_len - calculate how much real data is stored in a buffer.
 * @ubi: UBI device description object
 * @buf: a buffer with the contents of the physical eraseblock
 * @length: the buffer length
 *
 * This function calculates how much "real data" is stored in @buf and returnes
 * the length. Continuous 0xFF bytes at the end of the buffer are not
 * considered as "real data".
 */
int ubi_calc_data_len(const struct ubi_device *ubi, const void *buf,
		      int length)
{
	int i;

	ubi_assert(!(length & (ubi->min_io_size - 1)));

	for (i = length - 1; i >= 0; i--)
		if (((const uint8_t *)buf)[i] != 0xFF)
			break;

	/* The resulting length must be aligned to the minimum flash I/O size */
	length = ALIGN(i + 1, ubi->min_io_size);
	return length;
}

/**
 * ubi_check_volume - check the contents of a static volume.
 * @ubi: UBI device description object
 * @vol_id: ID of the volume to check
 *
 * This function checks if static volume @vol_id is corrupted by fully reading
 * it and checking data CRC. This function returns %0 if the volume is not
 * corrupted, %1 if it is corrupted and a negative error code in case of
 * failure. Dynamic volumes are not checked and zero is returned immediately.
 */
int ubi_check_volume(struct ubi_device *ubi, int vol_id)
{
	void *buf;
	int err = 0, i;
	struct ubi_volume *vol = ubi->volumes[vol_id];

	if (vol->vol_type != UBI_STATIC_VOLUME)
		return 0;

	buf = vmalloc(vol->usable_leb_size);
	if (!buf)
		return -ENOMEM;

	for (i = 0; i < vol->used_ebs; i++) {
		int size;

		if (i == vol->used_ebs - 1)
			size = vol->last_eb_bytes;
		else
			size = vol->usable_leb_size;

		err = ubi_eba_read_leb(ubi, vol, i, buf, 0, size, 1);
		if (err) {
			if (mtd_is_eccerr(err))
				err = 1;
			break;
		}
	}

	vfree(buf);
	return err;
}

/**
 * ubi_calculate_rsvd_pool - calculate how many PEBs must be reserved for bad
 * eraseblock handling.
 * @ubi: UBI device description object
 */
void ubi_calculate_reserved(struct ubi_device *ubi)
{
#ifdef CONFIG_MTD_UBI_BEB_LIMIT

/*
 * Multiplies an integer by a fraction, while avoiding unnecessary
 * overflow or loss of precision.
 */
#define mult_frac(x, numer, denom)(                     \
{                                                       \
        typeof(x) quot = (x) / (denom);                 \
        typeof(x) rem  = (x) % (denom);                 \
        (quot * (numer)) + ((rem * (numer)) / (denom)); \
}                                                       \
)

	int max_beb_per1024 = CONFIG_MTD_UBI_BEB_LIMIT;
	int limit, device_pebs;
	uint64_t device_size;

	device_size = mtd_get_device_size(ubi->mtd);
	device_pebs = mtd_div_by_eb(device_size, ubi->mtd);

	limit = mult_frac(device_pebs, max_beb_per1024, 1024);

	/* Round it up */
	if (mult_frac(limit, 1024, max_beb_per1024) < device_pebs)
		limit += 1;

	ubi->beb_rsvd_level = limit;

#else
	ubi->beb_rsvd_level = ubi->good_peb_count/100;
	ubi->beb_rsvd_level *= CONFIG_MTD_UBI_BEB_RESERVE;
	if (ubi->beb_rsvd_level < MIN_RESEVED_PEBS)
		ubi->beb_rsvd_level = MIN_RESEVED_PEBS;
#endif
}
