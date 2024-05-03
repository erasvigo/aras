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
 * Header file for the ARAS Radio Automation System. Types and definitions for
 * the schedule module.
 */

#ifndef _ARAS_SCHEDULE_H
#define _ARAS_SCHEDULE_H

#include <glib.h>

#define ARAS_SCHEDULE_MAX_LINE          2048
#define ARAS_SCHEDULE_MAX_DAY           16
#define ARAS_SCHEDULE_MAX_TIME          16
#define ARAS_SCHEDULE_MAX_BLOCK_NAME    1024

struct aras_schedule_node {
        long int time;
        char block_name[ARAS_SCHEDULE_MAX_BLOCK_NAME];
};

struct aras_schedule {
        GList *list;
};

int aras_schedule_init(struct aras_schedule *schedule);
int aras_schedule_load_file(struct aras_schedule *schedule, char *file);
int aras_schedule_list_free(struct aras_schedule *schedule);
struct aras_schedule_node *aras_schedule_seek_node_current(struct aras_schedule *schedule, long int time);
struct aras_schedule_node *aras_schedule_seek_node_next(struct aras_schedule *schedule, long int time);
void aras_schedule_print(struct aras_schedule *schedule);

#endif  /* _ARAS_SCHEDULE_H */
