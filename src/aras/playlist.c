/**
 * @file
 * @author  Erasmo Alonso Iglesias <erasmo1982@users.sourceforge.net>
 * @version 4.7
 *
 * @section LICENSE
 *
 * The ARAS Radio Automation System
 * Copyright (C) 2024  Erasmo Alonso Iglesias
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
 * Source file for the ARAS Radio Automation System. Functions for the playlist
 * module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <aras/parse.h>
#include <aras/block.h>
#include <aras/playlist.h>

/**
 * This function prints a playlist
 *
 * @param   playlist    Pointer to the playlist
 */
void aras_playlist_print(GList *playlist)
{
        char *node;

        printf("Playlist\n");
        printf("--------\n");

        while (playlist != NULL) {
                node = playlist->data;
                printf("%s\n", node);
                playlist = playlist->next;
        }

        printf("\n");
        fflush(stdout);
}

/**
 * This function frees a playlist
 *
 * @param   playlist    Pointer to the playlist
 *
 * @return  This function always returns NULL
 */
GList *aras_playlist_free(GList *playlist)
{
        if (playlist != NULL)
                g_list_free_full(playlist, g_free);

        return NULL;
}

/**
 * This function performs a random permutation of the playlist according to the
 * Fisher-Yates algorithm.
 *
 * @param   playlist    Pointer to the playlist
 *
 * @return  The playlist itself
 */
GList *aras_playlist_shuffle(GList *playlist)
{
        int n;
        int i;
        int j;
        GList *pointer_1;
        GList *pointer_2;
        char *aux;

        if (playlist == NULL)
                return playlist;

        n = g_list_length(playlist);
        for (i = n - 1; i > 0; i--) {
                j = rand() % (i + 1);
                pointer_1 = g_list_nth(playlist, i);
                pointer_2 = g_list_nth(playlist, j);
                if ((pointer_1 != NULL) && (pointer_2 != NULL)) {
                        aux = pointer_1->data;
                        pointer_1->data = pointer_2->data;
                        pointer_2->data = aux;
                }
        }
        return playlist;
}

/**
 * This function adds a single URI to the playlist head from a single URI or
 * local file.
 *
 * @param   playlist    Pointer to the playlist
 * @param   data        Pointer to a single URI or local file string
 *
 * @return  The augmented playlist if success, NULL if error
 */
GList *aras_playlist_load_file(GList *playlist, char *data)
{
        char *node;
        char *scheme;
        char *reserved_chars_allowed = "!*'();:@&=+$,/?#[]%";

        if (data == NULL)
                return playlist;

        /* Check if data is a local file or an URI */
        if ((scheme = g_uri_parse_scheme(data)) != NULL) {
                /* Data is an URI */
                g_free(scheme);
                if ((node = g_uri_escape_string(data, reserved_chars_allowed, TRUE)) != NULL)
                        playlist = g_list_prepend(playlist, node);
        } else {
                /* Try local file */
                if (g_file_test(data, G_FILE_TEST_IS_REGULAR) == TRUE)
                        if ((node = g_filename_to_uri(data, NULL, NULL)) != NULL)
                                playlist = g_list_prepend(playlist, node);
        }

        return playlist;
}

/**
 * This function adds a set of URIs to the playlist from a local m3u playlist.
 *
 * @param   playlist    Pointer to the playlist
 * @param   data        Pointer to the file name of a local m3u playlist
 *
 * @return  The augmented playlist if success, NULL if error
 */
GList *aras_playlist_load_m3u(GList *playlist, char *data)
{
        FILE *fp;
        char line[ARAS_PLAYLIST_MAX_LINE];
        char buffer[ARAS_PLAYLIST_MAX_LINE];

        if (data == NULL)
                return playlist;

        /* Reverse the list */
        playlist = g_list_reverse(playlist);

        /* Open playlist file */
        if ((fp = fopen(data, "r")) == NULL)
                return playlist;

        /* Get lines from playlist file */
        while (fgets(line, ARAS_PLAYLIST_MAX_LINE, fp) != NULL)
                if (aras_parse_line_m3u(line, buffer, sizeof(buffer)) != NULL)
                        playlist = aras_playlist_load_file(playlist, buffer);

        /* Reverse the list */
        playlist = g_list_reverse(playlist);

        /* Close playlist file */
        fclose(fp);

        return playlist;
}

/**
 * This function adds a set of URIs to the playlist from a local directory.
 *
 * @param   playlist    Pointer to the playlist
 * @param   data        Pointer to the path string of a local directory
 * @param   recursion   Recursion counter used to limit recursions
 *
 * @return  The augmented playlist if success, NULL if error
 */
GList *aras_playlist_load_directory(GList *playlist, char *data, int recursion)
{
        GDir *dir;
        const char *entry;
        char *path;
        char *node = NULL;

        /* If recursion is too deep, return the playlist itself */
        if (recursion >= ARAS_PLAYLIST_MAX_RECURSION_DEPTH) {
                fprintf(stderr, "aras: maximum number of recursions reached\n");
                return playlist;
        }

        if (data == NULL)
                return playlist;

        /* Reverse the list */
        playlist = g_list_reverse(playlist);

        /* Open directory */
        if ((dir = g_dir_open(data, 0, NULL)) == NULL)
                return playlist;

        /* Read directory entries */
        while ((entry = g_dir_read_name(dir)) != NULL) {
                /* Get the full path for entries */
                path = g_build_filename(data, entry, NULL);
                /* Check if the entry is a regular file or a subdirectory */
                if (g_file_test(path, G_FILE_TEST_IS_REGULAR) == TRUE) {
                        /* Add regular file to the playlist */
                        if ((node = g_filename_to_uri(path, NULL, NULL)) != NULL)
                                playlist = g_list_prepend(playlist, node);
                } else if (g_file_test(path, G_FILE_TEST_IS_DIR) == TRUE) {
                        /* If a directory, recursive call */
                        playlist = aras_playlist_load_directory(playlist, path, recursion + 1);
                }
                g_free(path);
        }

        /* Reverse the list */
        playlist = g_list_reverse(playlist);

        /* Close directory */
        g_dir_close(dir);

        return playlist;
}

/**
 * This function adds a set of URIs to the playlist from a local directory and
 * performs a random permutation of the acquired playlist.
 *
 * @param   playlist    Pointer to the playlist
 * @param   data        Pointer to the path string of a local directory
 *
 * @return  The augmented playlist if success, NULL if error
 */
GList *aras_playlist_load_random_directory(GList *playlist, char *data)
{
        if (data == NULL)
                return playlist;

        playlist = aras_playlist_load_directory(playlist, data, 0);
        playlist = aras_playlist_shuffle(playlist);

        return playlist;
}

/**
 * This function adds a random URI to the playlist from a local directory.
 *
 * @param   playlist    Pointer to the playlist
 * @param   data        Pointer to the path string of a local directory
 *
 * @return  The playlist if success, NULL if error
 */
GList *aras_playlist_load_random_file(GList *playlist, char *data)
{
        if (data == NULL)
                return playlist;

        if ((playlist = aras_playlist_load_random_directory(playlist, data)) != NULL)
                if (playlist->next != NULL)
                        playlist->next = aras_playlist_free(playlist->next);

        return playlist;
}

/**
 * This function adds an interleave playlist to the playlist from data
 * containing block names and multiplicities.
 *
 * @param   playlist    Pointer to the playlist
 * @param   block       Pointer to a block structure
 * @param   data        Pointer to the data string containing block names and
 *                      multiplicities
 * @param   recursion   Recursion counter used to limit recursions
 *
 * @return  The playlist if success, NULL if error
 */
GList *aras_playlist_load_interleave(GList *playlist, struct aras_block *block, char *data, int recursion)
{
        int k;
        char arguments[4][ARAS_BLOCK_MAX_NAME];
        GList *playlist_1;
        GList *playlist_2;
        int n_1;
        int n_2;
        GList *pointer_1;
        GList *pointer_2;

        /* If recursion is too deep, return the playlist itself */
        if (recursion >= ARAS_PLAYLIST_MAX_RECURSION_DEPTH) {
                fprintf(stderr, "aras: maximum number of recursions reached\n");
                return playlist;
        }

        if (block == NULL || data == NULL)
                return playlist;

        /* Read arguments */
        for (k = 0; k < 4; k++) {
                if ((data = aras_parse_line_configuration(data, &arguments[k][0], ARAS_BLOCK_MAX_NAME)) == NULL)
                        return playlist;
        }

        /* Prevent multiplicities lesser than one */
        if ((n_1 = atoi(&arguments[2][0])) < 1)
                n_1 = 1;
        if ((n_2 = atoi(&arguments[3][0])) < 1)
                n_2 = 1;

        /* Initialize two new playists */
        playlist_1 = playlist_2 = NULL;

        /* If any playlist does not grow up, free both playlists and return the input playlist */
        if (((playlist_1 = aras_playlist_load(playlist_1, &arguments[0][0], block, recursion + 1)) == NULL) ||
            ((playlist_2 = aras_playlist_load(playlist_2, &arguments[1][0], block, recursion + 1)) == NULL)) {
                playlist_1 = aras_playlist_free(playlist_1);
                playlist_2 = aras_playlist_free(playlist_2);
                return playlist;
        }

        /* Reverse the list */
        playlist = g_list_reverse(playlist);

        /* Interleave playlists */
        pointer_1 = playlist_1;
        pointer_2 = playlist_2;
        while ((pointer_1 != NULL) || (pointer_2 != NULL)) {
                /* Add elements from playlist_1 */
                for (k = 0; k < n_1; k++) {
                        if (pointer_1 != NULL) {
                                playlist = g_list_prepend(playlist, pointer_1->data);
                                pointer_1 = pointer_1->next;
                        }
                }
                /* Add elements from playlist_2 */
                for (k = 0; k < n_2; k++) {
                        if (pointer_2 != NULL) {
                                playlist = g_list_prepend(playlist, pointer_2->data);
                                pointer_2 = pointer_2->next;
                        }
                }
        }

        /* Free playlist_1 and playlist_2 */
        g_list_free(playlist_1);
        g_list_free(playlist_2);

        /* Reverse the list */
        playlist = g_list_reverse(playlist);

        return playlist;
}

/**
 * This function receives a playlist, a block name, a block structure and a
 * recursion counter, it looks for the block node and adds the appropriate
 * playlist to the playlist list.
 *
 * @param   playlist    A pointer to the playlist
 * @param   block_name  A pointer to the block name string
 * @param   block       A pointer to a block structure
 * @param   recursion   Recursion counter used to limit recursions
 *
 * @return  The playlist if success, NULL if error
 */
GList *aras_playlist_load(GList *playlist, char *block_name, struct aras_block *block, int recursion)
{
        struct aras_block_node *block_node;

        /* Check if block name is NULL */
        if (block == NULL || block_name == NULL)
                return playlist;

        /* Look for the block node in block list */
        if ((block_node = aras_block_seek_node_name(block, block_name)) == NULL)
                return playlist;

        /* Load playlist according to the block type */
        switch (block_node->type) {
        case ARAS_BLOCK_TYPE_FILE:
                return aras_playlist_load_file(playlist, block_node->data);
                break;
        case ARAS_BLOCK_TYPE_PLAYLIST:
                return aras_playlist_load_m3u(playlist, block_node->data);
                break;
        case ARAS_BLOCK_TYPE_RANDOM:
                return aras_playlist_load_random_directory(playlist, block_node->data);
                break;
        case ARAS_BLOCK_TYPE_RANDOM_FILE:
                return aras_playlist_load_random_file(playlist, block_node->data);
                break;
        case ARAS_BLOCK_TYPE_INTERLEAVE:
                return aras_playlist_load_interleave(playlist, block, block_node->data, recursion);
                break;
        default:
                break;
        }
        return playlist;
}
