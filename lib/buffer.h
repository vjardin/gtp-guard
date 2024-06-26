/* SPDX-License-Identifier: AGPL-3.0-or-later */
/*
 * Soft:        The main goal of gtp-guard is to provide robust and secure
 *              extensions to GTP protocol (GPRS Tunneling Procol). GTP is
 *              widely used for data-plane in mobile core-network. gtp-guard
 *              implements a set of 3 main frameworks:
 *              A Proxy feature for data-plane tweaking, a Routing facility
 *              to inter-connect and a Firewall feature for filtering,
 *              rewriting and redirecting.
 *
 * Authors:     Alexandre Cassen, <acassen@gmail.com>
 *
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU Affero General Public
 *              License Version 3.0 as published by the Free Software Foundation;
 *              either version 3.0 of the License, or (at your option) any later
 *              version.
 *
 * Copyright (C) 2023-2024 Alexandre Cassen, <acassen@gmail.com>
 */

#ifndef _BUFFER_H
#define _BUFFER_H

/* buffer definition */
typedef struct _buffer_data {
	struct _buffer_data	*next;

	size_t			cp;	/* Location to add new data. */
	size_t			sp;	/* Pointer to data not yet flushed. */
	unsigned char		data[];	/* Actual data stream (variable length).
					 * real dimension is buffer->size.
					 */
} buffer_data_t;

typedef struct _buffer {
	buffer_data_t		*head;
	buffer_data_t		*tail;

	size_t			size;	/* Size of each buffer_data chunk. */
} buffer_t;

typedef enum _buffer_status {
	BUFFER_ERROR = -1,		/* An I/O error occurred.
					 * The buffer should be destroyed and the
					 * file descriptor should be closed.
					 */
	BUFFER_EMPTY = 0,		/* The data was written successfully,
					 * and the buffer is now empty (there is
					 * no pending data waiting to be flushed).
					 */
	BUFFER_PENDING = 1		/* There is pending data in the buffer
					 * waiting to be flushed. Please try
					 * flushing the buffer when select
					 * indicates that the file descriptor
					 * is writeable.
					 */
} buffer_status_t;

/* Some defines */
#define BUFFER_SIZE_DEFAULT	4096

/* Some usefull macros */
#define ERRNO_IO_RETRY(EN) \
	(((EN) == EAGAIN) || ((EN) == EWOULDBLOCK) || ((EN) == EINTR))

/* Prototypes */
extern buffer_t *buffer_new(size_t);
extern void buffer_reset(buffer_t *);
extern void buffer_free(buffer_t *);
extern void buffer_put(buffer_t *, const void *, size_t);
extern void buffer_putc(buffer_t *, uint8_t);
extern void buffer_putstr(buffer_t *, const char *);
extern char *buffer_getstr(buffer_t *);
extern int buffer_empty(buffer_t *);
extern buffer_status_t buffer_write(buffer_t *, int fd,
                                    const void *, size_t);
extern buffer_status_t buffer_flush_available(buffer_t *, int fd);
extern buffer_status_t buffer_flush_all(buffer_t *, int fd);
extern buffer_status_t buffer_flush_window(buffer_t *, int fd, int width,
                                           int height, int erase, int no_more);

#endif
