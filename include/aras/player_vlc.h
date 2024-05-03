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
 * the player module.
 */

#ifndef _ARAS_PLAYER_VLCLIB_H
#define _ARAS_PLAYER_VLCLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vlc/vlc.h>

#define ARAS_PLAYER_MAX_NAME            1024
#define ARAS_PLAYER_MAX_URI             1024
#define ARAS_PLAYER_MAX_DEVICE          1024

#define ARAS_PLAYER_UNIT_A              0
#define ARAS_PLAYER_UNIT_B              1

#define ARAS_PLAYER_STATE_ERROR         0
#define ARAS_PLAYER_STATE_BUFFERING     1
#define ARAS_PLAYER_STATE_STOP          2
#define ARAS_PLAYER_STATE_PLAYING       3
#define ARAS_PLAYER_STATE_OTHER         4

struct aras_player {
        int current_unit;
        float volume_a;
        float volume_b;
        int buffer_percent_a;
        int buffer_percent_b;
        libvlc_instance_t *instance;
        libvlc_media_player_t *player_a;
        libvlc_media_player_t *player_b;
        libvlc_media_t *media_a;
        libvlc_media_t *media_b;
};

int aras_player_init(struct aras_player *player,
                     char *name,
                     int audio_output,
                     char *audio_device,
                     int audio_sample_rate,
                     int audio_channels,
                     int video_output,
                     char *video_device,
                     char *video_display,
                     int video_resolution[]);
int aras_player_init_block_player(struct aras_player *player, struct aras_configuration *configuration);
int aras_player_init_time_signal_player(struct aras_player *player, struct aras_configuration *configuration);
void aras_player_set_volume(struct aras_player *player, int unit, float volume);
void aras_player_set_volume_increment(struct aras_player *player, int unit, float slope, float limit);
void aras_player_set_uri(struct aras_player *player, int unit, char *uri);
void aras_player_set_state_null(struct aras_player *player, int unit);
void aras_player_set_state_ready(struct aras_player *player, int unit);
void aras_player_set_state_paused(struct aras_player *player, int unit);
void aras_player_set_state_playing(struct aras_player *player, int unit);
void aras_player_set_current_unit(struct aras_player *player, int unit);
void aras_player_swap_current_unit(struct aras_player *player);
float aras_player_get_volume(struct aras_player *player, int unit);
void aras_player_get_state(struct aras_player *player, int unit, int *state);
int aras_player_get_buffer_percent(struct aras_player *player, int unit);
int aras_player_get_current_unit(struct aras_player *player);
long int aras_player_get_duration(struct aras_player *player, int unit);
long int aras_player_get_position(struct aras_player *player, int unit);

#endif  /* _ARAS_PLAYER_VLCLIB_H */
