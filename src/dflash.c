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

static struct dflash_guid guid = {
	.guid = 0,
};

static struct dflash_file *dfiles = NULL;

/* TODO: Map lnvm targets */
/*
 * This function allows an application to choose the LightNVM target from which
 * the new dflash file will obtain flash blocks. If this function is not called
 * before opening a dflash file, a default LightNVM target will be used
 */
uint64_t nvm_create(int tgt, uint32_t stream_id, int flags)
{
	struct dflash_file *f;

	f = (struct dflash_file*)malloc(sizeof(struct dflash_file));
	if (!f)
		return -ENOMEM;

	f->stream_id = stream_id;
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

/* TODO: Assign different file descriptors to same dflash file. For now access
 * dflash files by file id */
int nvm_open(uint64_t dflash_id, int flags)
{
	struct dflash_file *f;
	size_t w_buf_size;

	HASH_FIND_INT(dfiles, &dflash_id, f);
	if (!f) {
		LNVM_DEBUG("File descriptor %d does not exist\n", fd);
		return -EINVAL;
	}

	/* Write buffer for small writes */
	w_buf_size = calculate_wbuf_size(f);
	f->w_buffer->buf = memalign(PAGE_SIZE, w_buf_size);
	if (!f->w_buffer)
		return -ENOMEM;

	f->w_buffer->cursize = 0;
	f->w_buffer->curflush =0;
	f->w_buffer->mem = f->w_buffer->buf;
	f->w_buffer->flush = f->w_buffer->buf;

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


