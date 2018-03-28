/**
 * @file
 * @author  Erasmo Alonso Iglesias <erasmo1982@users.sourceforge.net>
 * @version 4.5
 *
 * @section LICENSE
 *
 * The ARAS Radio Automation System
 * Copyright (C) 2018  Erasmo Alonso Iglesias
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * Header file for the ARAS Radio Automation System. Types and definitions for
 * the block module.
 */

#ifndef _ARAS_BLOCK_H
#define _ARAS_BLOCK_H

#include <glib.h>

#define ARAS_BLOCK_MAX_LINE             2048
#define ARAS_BLOCK_MAX_NAME             256
#define ARAS_BLOCK_MAX_TYPE             32
#define ARAS_BLOCK_MAX_DATA             1024

#define ARAS_BLOCK_TYPE_FILE            0
#define ARAS_BLOCK_TYPE_PLAYLIST        1
#define ARAS_BLOCK_TYPE_RANDOM          2
#define ARAS_BLOCK_TYPE_RANDOM_FILE     3
#define ARAS_BLOCK_TYPE_INTERLEAVE      4

struct aras_block_node {
        char name[ARAS_BLOCK_MAX_NAME];
        int type;
        char data[ARAS_BLOCK_MAX_DATA];
};

struct aras_block {
        GList *list;
};

int aras_block_init(struct aras_block *block);
int aras_block_load_file(struct aras_block *block, char *file);
int aras_block_list_free(struct aras_block *block);
struct aras_block_node *aras_block_seek_node_name(struct aras_block *block, char *name);
void aras_block_print(struct aras_block *block);

#endif  /* _ARAS_BLOCK_H */
