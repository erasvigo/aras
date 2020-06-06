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
 * the engine module.
 */

#ifndef _ARAS_ENGINE_H
#define _ARAS_ENGINE_H

#include <glib.h>
#include <aras/config.h>
#include <aras/configuration.h>
#include <aras/schedule.h>
#include <aras/block.h>
#if (ARAS_CONFIG_MEDIA_LIBRARY == ARAS_CONFIG_MEDIA_LIBRARY_GSTREAMER)
#include <aras/player.h>
#elif (ARAS_CONFIG_MEDIA_LIBRARY == ARAS_CONFIG_MEDIA_LIBRARY_VLCLIB)
#include <aras/player_vlclib.h>
#endif

#define ARAS_ENGINE_STATE_NULL                  0
#define ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD 1
#define ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT 2
#define ARAS_ENGINE_STATE_MONITOR_TIME_SIGNAL   3
#define ARAS_ENGINE_STATE_PLAY_DEFAULT          4
#define ARAS_ENGINE_STATE_PLAY_PREVIOUS         5
#define ARAS_ENGINE_STATE_PLAY_NEXT             6
#define ARAS_ENGINE_STATE_PLAY_CURRENT          7
#define ARAS_ENGINE_STATE_CROSSFADE             8
#define ARAS_ENGINE_STATE_FADE_OUT              9

struct aras_engine {
        int state;
        long int state_time_elapsed;
        long int state_time_maximum;
        GList *playlist;
        GList *playlist_current_node;
};

int aras_engine_init(struct aras_engine *engine);
void aras_engine_schedule(struct aras_engine *engine, struct aras_player *player, struct aras_configuration *configuration, struct aras_schedule *schedule, struct aras_block *block);
void aras_engine_time_signal(struct aras_engine *engine, struct aras_player *player, struct aras_configuration *configuration, struct aras_block *block);
void aras_engine_set_state(struct aras_engine *engine, int state, long int state_time_maximum);

#endif  /* _ARAS_ENGINE_H */
