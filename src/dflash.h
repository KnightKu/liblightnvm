/*
 * dflash - user-space append-only file system for flash memories
 *
 * Copyright (C) 2015 Javier Gonzalez <javier@cnexlabs.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 */
#ifndef __DFLASH_H
#define __DFLASH_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "provisioning.h"
#include "../util/uthash.h"
#include "../util/debug.h"

/*
 * TODO:
 *	- Dynamic block list
 */

#define MAX_BLOCKS 5

struct dflash_guid {
	uint64_t guid;
	pthread_spinlock_t lock;
};

/* TODO: Allocate dynamic number of blocks */
/* TODO: Store the lnvm target the file belongs to */
struct dflash_file {
	uint64_t gid;				/* internal global identifier */
	struct vblock vblocks[MAX_BLOCKS];	/* vblocks forming the file */
	uint8_t nvblocks;			/* number of vblocks */
	unsigned long bytes;			/* valid bytes */
	struct timespec atime;			/* last access time */
	struct timespec mtime;			/* last modify time */
	struct timespec ctime;			/* last change time */
	UT_hash_handle hh;			/* hash handle for uthash */
};

static inline void atomic_assign_id(struct dflash_guid *cnt, uint64_t *id)
{
	pthread_spin_lock(&cnt->lock);
	cnt->guid++;
	*id = cnt->guid;
	pthread_spin_unlock(&cnt->lock);
}

#endif
