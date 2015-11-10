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
#include <errno.h>
#include <liblightnvm.h>

#include "dflash.h"
#include "ioctl.h"

static struct dflash_guid guid = {
	.guid = 0,
};

static struct dflash_file *dfiles = NULL;

/* TODO: Map lnvm targets */
int nvm_create(int tgt, int flags)
{
	struct dflash_file *f;

	f = (struct dflash_file*)malloc(sizeof(struct dflash_file));
	if (!f)
		return -ENOMEM;

	atomic_assign_id(&guid, &f->gid);
	HASH_ADD_INT(dfiles, gid, f);
	LNVM_DEBUG("Created dflash file. Target: %d\n", tgt);

	return f->gid;
}

void nvm_delete(int fd, int flags)
{
	struct dflash_file *f;

	HASH_FIND_INT(dfiles, &fd, f);
	HASH_DEL(dfiles, f);
	free(f);
}

// Is this necessary??
int nvm_open(int fd, int flags)
{
	struct dflash_file *f;

	HASH_FIND_INT(dfiles, &fd, f);
	if (!f) {
		LNVM_DEBUG("File descriptor %d does not exist\n", fd);
		return -EINVAL;
	}

	return 0;
}

void nvm_close(int fd, int flags)
{
}

int nvm_append(int fd, const void *buf, size_t count)
{
	return 0;
}

int nvm_read(int fd, void *buf, size_t count, off_t offset, int flags)
{
	return 0;
}


