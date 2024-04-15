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

#ifndef _RT_TABLE_H
#define _RT_TABLE_H

/* system includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

/* Each routing entry */
struct _rt_table;
typedef struct _rt_node {
	/* Actual prefix of this radix */
	prefix_t		p;

	/* Tree link */
	struct _rt_table	*table;
	struct _rt_node		*parent;
	struct _rt_node		*link[2];
#define l_left link[0]
#define l_right link[1]

	/* Lock of this radix */
	unsigned int		lock;

	/* Each node of route */
	void			*info;
} rt_node_t;

/* Routing table top structure */
typedef struct _rt_table {
	rt_node_t	*top;
	int		(*free_info) (void *);
	int		(*dump_info) (void *);
} rt_table_t;


/*
 *	Prototypes
 */
extern rt_table_t *rt_table_init(int (*free) (void *), int (*dump) (void *));
extern int rt_table_free(rt_table_t *);
extern int rt_table_dump(rt_table_t *);
extern rt_node_t *rt_node_match(const rt_table_t *, const prefix_t *);
extern rt_node_t *rt_node_match_ipv4(const rt_table_t *, const struct in_addr *);
extern rt_node_t *rt_node_match_ipv6(const rt_table_t *, const struct in6_addr *);
extern rt_node_t *rt_node_lookup(rt_table_t *, prefix_t *);
extern rt_node_t *rt_node_lookup_lpm(rt_table_t *, prefix_t *);
extern rt_node_t *rt_node_get(rt_table_t *, prefix_t *);
extern int rt_node_delete(rt_node_t *);
extern rt_node_t *rt_next(rt_node_t *);
extern rt_node_t *rt_next_until(rt_node_t *, rt_node_t *);

#endif
