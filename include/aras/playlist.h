/**
 * @file
 * @author  Erasmo Alonso Iglesias <erasmo1982@users.sourceforge.net>
 * @version 4.6
 *
 * @section LICENSE
 *
 * The ARAS Radio Automation System
 * Copyright (C) 2020  Erasmo Alonso Iglesias
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
 * the playlist module.
 */

#ifndef _ARAS_PLAYLIST_H
#define _ARAS_PLAYLIST_H

#include <glib.h>
#include <aras/block.h>

#define ARAS_PLAYLIST_MAX_LINE              2048
#define ARAS_PLAYLIST_MAX_RECURSION_DEPTH   16

GList *aras_playlist_free(GList *playlist);
GList *aras_playlist_load(GList *playlist, char *block_name, struct aras_block *block, int recursion);

#endif  /* _ARAS_PLAYLIST_H */
