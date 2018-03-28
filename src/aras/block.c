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
 * Source file for the ARAS Radio Automation System. Functions for the block
 * module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <glib.h>
#include <aras/parse.h>
#include <aras/block.h>

/**
 * This function receives a block type string and returns its numerical value
 *
 * @param   type    Pointer to the block type string
 *
 * @return  The numerical value for the type
 */
int aras_block_convert_type(char *type)
{
        if (!strcasecmp(type, "file"))
                return ARAS_BLOCK_TYPE_FILE;
        else if (!strcasecmp(type, "playlist"))
                return ARAS_BLOCK_TYPE_PLAYLIST;
        else if (!strcasecmp(type, "random"))
                return ARAS_BLOCK_TYPE_RANDOM;
        else if (!strcasecmp(type, "randomfile"))
                return ARAS_BLOCK_TYPE_RANDOM_FILE;
        else if (!strcasecmp(type, "interleave"))
                return ARAS_BLOCK_TYPE_INTERLEAVE;
        else
                return -1;
}

/**
 * This function sets the name field in a block structure.
 *
 * @param   block   Pointer to the block structure
 * @param   name    Pointer to the block name string
 */
void aras_block_node_set_name(struct aras_block_node *node, char *name)
{
        snprintf(node->name, sizeof(node->name), "%s", name);
}

/**
 * This function sets the type field in a block structure.
 *
 * @param   block   Pointer to the block structure
 * @param   type    Pointer to the block type
 */
void aras_block_node_set_type(struct aras_block_node *node, int type)
{
        node->type = type;
}

/**
 * This function sets the data field in a block structure.
 *
 * @param   block   Pointer to the block structure
 * @param   data    Pointer to the block data string
 */
void aras_block_node_set_data(struct aras_block_node *node, char *data)
{
        snprintf(node->data, sizeof(node->data), "%s", data);
}

/**
 * This function loads in a block structure the data contained in a set of
 * strings.
 *
 * @param   block   Pointer to the block structure
 * @param   name    Pointer to the block name string
 * @param   type    Pointer to the block type string
 * @param   data    Pointer to the block data string
 *
 * @return  0 if success, -1 if error
 */
int aras_block_load_data(struct aras_block *block, char *name, char *type, char *data)
{
        int node_type;
        struct aras_block_node *node;

        /* Convert type */
        if ((node_type = aras_block_convert_type(type)) == -1)
                return -1;

        /* Create a node to be linked to the block list */
        if ((node = g_malloc(sizeof(struct aras_block_node))) == NULL)
                return -1;

        /* Fill the fields in the node */
        aras_block_node_set_name(node, name);
        aras_block_node_set_type(node, node_type);
        aras_block_node_set_data(node, data);

        /* Add the node to the schedule list */
        block->list = g_list_prepend(block->list, node);

        return 0;
}

/**
 * This function loads in a block structure the data contained in a line.
 *
 * @param   block   Pointer to the block structure
 * @param   line    Pointer to the line
 *
 * @return  0 if success, -1 if error
 */
int aras_block_load_line(struct aras_block *block, char *line)
{
        char name[ARAS_BLOCK_MAX_NAME];
        char type[ARAS_BLOCK_MAX_TYPE];
        char data[ARAS_BLOCK_MAX_DATA];

        /* Get name, type and data */
        if ((line = aras_parse_line_configuration(line, name, sizeof(name))) == NULL)
                return -1;

        if ((line = aras_parse_line_configuration(line, type, sizeof(type))) == NULL)
                return -1;

        if ((line = aras_parse_line_configuration(line, data, sizeof(data))) == NULL)
                return -1;

        aras_block_load_data(block, name, type, data);

        return 0;
}

/**
 * This function loads in a block structure the data contained in a file.
 *
 * @param   block   Pointer to the block structure
 * @param   file    Pointer to the file name string
 *
 * @return  0 if success, -1 if error
 */
int aras_block_load_file(struct aras_block *block, char *file)
{
        FILE *fp;
        char line[ARAS_BLOCK_MAX_LINE];

        /* Open block file */
        if ((fp = fopen(file, "r")) == NULL)
                return -1;
        /* Get lines from block file */
        while (fgets(line, ARAS_BLOCK_MAX_LINE, fp) != NULL)
                aras_block_load_line(block, line);
        /* Reverse the block list */
        block->list = g_list_reverse(block->list);
        /* Close block file */
        if (fclose(fp) != 0)
                return -1;

        return 0;
}

/**
 * This function initializes a block structure.
 *
 * @param   block   Pointer to the block structure
 *
 * @return  This function always returns 0
 */
int aras_block_init(struct aras_block *block)
{
        /* Clean structure */
        memset(block, 0, sizeof(*block));
        /* Block list initialization */
        block->list = NULL;
        return 0;
}

/**
 * This function receives a block structure and frees the block list.
 *
 * @param   block   Pointer to the block structure
 *
 * @return  This function always returns 0
 */
int aras_block_list_free(struct aras_block *block)
{
        g_list_free_full(block->list, g_free);
        block->list = NULL;

        return 0;
}

/**
 * This function receives a block structure and a block name and returns a
 * pointer to the first block node with this name.
 *
 * @param   block   Pointer to the block structure
 * @param   name    Pointer to the file name string to be sought
 *
 * @return  A pointer to the first block node with this name if node found
 *          A pointer to NULL if node not found
 */
struct aras_block_node *aras_block_seek_node_name(struct aras_block *block, char *name)
{
        GList *pointer;
        struct aras_block_node *node;

        if (block == NULL)
                return NULL;

        pointer = block->list;
        while (pointer != NULL) {
                node = pointer->data;
                if (!strcmp(node->name, name))
                        return node;
                pointer = pointer->next;
        }
        return NULL;
}

/**
 * This function receives a block structure and prints the block list.
 *
 * @param   block   Pointer to the block structure
 */
void aras_block_print(struct aras_block *block)
{
        GList *pointer;
        struct aras_block_node *node;

        printf("Block list\n");
        printf("----------\n");

        pointer = block->list;
        while (pointer != NULL) {
                node = pointer->data;
                printf("%s %d %s\n", node->name, node->type, node->data);
                pointer = pointer->next;
        }
        printf("\n");
        fflush(stdout);
}
