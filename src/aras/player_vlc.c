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
 * Source file for the ARAS Radio Automation System. Functions for the player
 * module.
 */

#include <vlc/vlc.h>
#include <aras/configuration.h>
#include <aras/player_vlc.h>

/**
 * This function initializes a player structure with the block player
 * configuration.
 *
 * @param   player          Pointer to the player
 * @param   configuration   Pointer to the configuration
 *
 * @return  This function always return 0
 */
int aras_player_init_block_player(struct aras_player *player, struct aras_configuration *configuration)
{
        /* Initialize current unit, volume and buffer percent */
        player->current_unit = 0;
        player->volume_a = 0;
        player->volume_b = 0;
        player->buffer_percent_a = 0;
        player->buffer_percent_b = 0;

        XInitThreads();

        /* Create player */
        player->instance = libvlc_new(0, NULL);
        player->player_a = libvlc_media_player_new(player->instance);
        player->player_b = libvlc_media_player_new(player->instance);
        player->media_a = libvlc_media_new_location(player->instance, "file:///dev/null");
        player->media_b = libvlc_media_new_location(player->instance, "file:///dev/null");

        switch (configuration->block_player_audio_output) {
        case ARAS_CONFIGURATION_MODE_AUDIO_ALSA:
                libvlc_audio_output_set(player->player_a, "alsa");
                libvlc_audio_output_set(player->player_b, "alsa");
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_JACK:
                libvlc_audio_output_set(player->player_a, "jack");
                libvlc_audio_output_set(player->player_b, "jack");
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_PULSEAUDIO:
                libvlc_audio_output_set(player->player_a, "pulse");
                libvlc_audio_output_set(player->player_b, "pulse");
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_FILE:
                libvlc_audio_output_set(player->player_a, "afile");
                libvlc_audio_output_set(player->player_b, "afile");
                break;
        default:
                libvlc_audio_output_set(player->player_a, "alsa");
                libvlc_audio_output_set(player->player_b, "alsa");
                break;
        }

        if (configuration->block_player_audio_output != ARAS_CONFIGURATION_MODE_AUDIO_PULSEAUDIO) {
                libvlc_audio_output_device_set(player->player_a, NULL, configuration->block_player_audio_device);
                libvlc_audio_output_device_set(player->player_b, NULL, configuration->block_player_audio_device);
        }

        libvlc_audio_set_format(player->player_a,
                                "f32l",
                                configuration->block_player_sample_rate,
                                configuration->block_player_channels);
        libvlc_audio_set_format(player->player_b,
                                "f32l",
                                configuration->block_player_sample_rate,
                                configuration->block_player_channels);

        libvlc_audio_set_volume(player->player_a, (int)player->volume_a);
        libvlc_audio_set_volume(player->player_b, (int)player->volume_a);

        libvlc_video_set_format(player->player_a,
		                "RGBA",
                                configuration->block_player_display_resolution[0],
                                configuration->block_player_display_resolution[1],
		                4 * configuration->time_signal_player_display_resolution[0]);
        libvlc_video_set_format(player->player_b,
		                "RGBA",
                                configuration->block_player_display_resolution[0],
                                configuration->block_player_display_resolution[1],
		                4 * configuration->time_signal_player_display_resolution[0]);

        libvlc_set_fullscreen(player->player_a, true);
        libvlc_set_fullscreen(player->player_b, true);

        return 0;
}

/**
 * This function initializes a player structure with the time signal player
 * configuration.
 *
 * @param   player          Pointer to the player
 * @param   configuration   Pointer to the configuration
 *
 * @return  This function always return 0
 */
int aras_player_init_time_signal_player(struct aras_player *player, struct aras_configuration *configuration)
{
        /* Initialize current unit, volume and buffer percent */
        player->current_unit = 0;
        player->volume_a = 0;
        player->volume_b = 0;
        player->buffer_percent_a = 0;
        player->buffer_percent_b = 0;

        XInitThreads();

        /* Create player */
        player->instance = libvlc_new(0, NULL);
        player->player_a = libvlc_media_player_new(player->instance);
        player->player_b = libvlc_media_player_new(player->instance);
        player->media_a = libvlc_media_new_location(player->instance, "file:///dev/null");
        player->media_b = libvlc_media_new_location(player->instance, "file:///dev/null");

        switch (configuration->time_signal_player_audio_output) {
        case ARAS_CONFIGURATION_MODE_AUDIO_ALSA:
                libvlc_audio_output_set(player->player_a, "alsa");
                libvlc_audio_output_set(player->player_b, "alsa");
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_JACK:
                libvlc_audio_output_set(player->player_a, "jack");
                libvlc_audio_output_set(player->player_b, "jack");
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_PULSEAUDIO:
                libvlc_audio_output_set(player->player_a, "pulse");
                libvlc_audio_output_set(player->player_b, "pulse");
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_FILE:
                libvlc_audio_output_set(player->player_a, "afile");
                libvlc_audio_output_set(player->player_b, "afile");
                break;
        default:
                libvlc_audio_output_set(player->player_a, "alsa");
                libvlc_audio_output_set(player->player_b, "alsa");
                break;
        }

        if (configuration->time_signal_player_audio_output != ARAS_CONFIGURATION_MODE_AUDIO_PULSEAUDIO) {
                libvlc_audio_output_device_set(player->player_a, NULL, configuration->time_signal_player_audio_device);
                libvlc_audio_output_device_set(player->player_b, NULL, configuration->time_signal_player_audio_device);
        }

        libvlc_audio_set_format(player->player_a,
                                "f32l",
                                configuration->time_signal_player_sample_rate,
                                configuration->time_signal_player_channels);
        libvlc_audio_set_format(player->player_b,
                                "f32l",
                                configuration->time_signal_player_sample_rate,
                                configuration->time_signal_player_channels);

        libvlc_audio_set_volume(player->player_a, (int)player->volume_a);
        libvlc_audio_set_volume(player->player_b, (int)player->volume_b);

        libvlc_video_set_format(player->player_a,
		                "RGBA",
                                configuration->time_signal_player_display_resolution[0],
                                configuration->time_signal_player_display_resolution[1],
		                4 * configuration->time_signal_player_display_resolution[0]);
        libvlc_video_set_format(player->player_b,
		                "RGBA",
                                configuration->time_signal_player_display_resolution[0],
                                configuration->time_signal_player_display_resolution[1],
		                4 * configuration->time_signal_player_display_resolution[0]);

        libvlc_set_fullscreen(player->player_a, true);
        libvlc_set_fullscreen(player->player_b, true);

        return 0;
}

/**
 * This function sets the volume in a player.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 * @param   volume  The volume value
 */
void aras_player_set_volume(struct aras_player *player, int unit, float volume)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                player->volume_a = volume;
                libvlc_audio_set_volume(player->player_a, (int)(1e2 * volume));
                break;
        case ARAS_PLAYER_UNIT_B:
                player->volume_b = volume;
                libvlc_audio_set_volume(player->player_b, (int)(1e2 * volume));
                break;
        default:
                break;
        }
}

/**
 * This function sets a given volume increment in a player.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 * @param   slope   The slope of the volume curve
 * @param   limit   The asymptotic limit for the volume
 */
void aras_player_set_volume_increment(struct aras_player *player, int unit, float slope, float limit)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                player->volume_a += slope * (limit - player->volume_a);
                libvlc_audio_set_volume(player->player_a, (int)(1e2 * player->volume_a));
                break;
        case ARAS_PLAYER_UNIT_B:
                player->volume_b += slope * (limit - player->volume_b);
                libvlc_audio_set_volume(player->player_b, (int)(1e2 * player->volume_b));
                break;
        default:
                break;
        }
}

/**
 * This function sets the URI in a player.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 * @param   uri     A pointer to the URI string
 */
void aras_player_set_uri(struct aras_player *player, int unit, char *uri)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                libvlc_media_release(player->media_a);
                player->media_a = libvlc_media_new_location(player->instance, uri);
                libvlc_media_player_set_media(player->player_a, player->media_a);
                break;
        case ARAS_PLAYER_UNIT_B:
                libvlc_media_release(player->media_b);
                player->media_b = libvlc_media_new_location(player->instance, uri);
                libvlc_media_player_set_media(player->player_b, player->media_b);
                break;
        default:
                break;
        }
}

/**
 * This function sets the player state to stop state.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 */
void aras_player_set_state_null(struct aras_player *player, int unit)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                libvlc_media_player_stop(player->player_a);
                break;
        case ARAS_PLAYER_UNIT_B:
                libvlc_media_player_stop(player->player_b);
                break;
        default:
                break;
        }
}

/**
 * This function sets the player state to stop state.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 */
void aras_player_set_state_ready(struct aras_player *player, int unit)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                libvlc_media_player_stop(player->player_a);
                break;
        case ARAS_PLAYER_UNIT_B:
                libvlc_media_player_stop(player->player_b);
                break;
        default:
                break;
        }
}

/**
 * This function sets the player state to pause state.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 */
void aras_player_set_state_paused(struct aras_player *player, int unit)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                libvlc_media_player_pause(player->player_a);
                break;
        case ARAS_PLAYER_UNIT_B:
                libvlc_media_player_pause(player->player_b);
                break;
        default:
                break;
        }
}

/**
 * This function sets the player state to play state.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 */
void aras_player_set_state_playing(struct aras_player *player, int unit)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                libvlc_media_player_play(player->player_a);
                break;
        case ARAS_PLAYER_UNIT_B:
                libvlc_media_player_play(player->player_b);
                break;
        default:
                break;
        }
}

/**
 * This function sets the player current unit.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 */
void aras_player_set_current_unit(struct aras_player *player, int unit)
{
        player->current_unit = unit;
}

/**
 * This function sets the player current unit.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 */
void aras_player_swap_current_unit(struct aras_player *player)
{
        player->current_unit = (player->current_unit + 1) % 2;
}

/**
 * This function gets the volume in a player.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 * @return  volume  The volume value
 */
float aras_player_get_volume(struct aras_player *player, int unit)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                return(player->volume_a);
        case ARAS_PLAYER_UNIT_B:
                return(player->volume_b);
        default:
                return 0;
        }
}

/**
 * This function gets the player state.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 * @param   state   Pointer to the buffer where the state is written
 */
void aras_player_get_state(struct aras_player *player, int unit, int *state)
{
        libvlc_state_t vlc_state;

        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                vlc_state = libvlc_media_player_get_state(player->player_a);
                break;
        case ARAS_PLAYER_UNIT_B:
                vlc_state = libvlc_media_player_get_state(player->player_b);
                break;
        default:
                vlc_state = libvlc_media_player_get_state(player->player_a);
                break;
        }

        switch (vlc_state) {
        case libvlc_Error:
                *state = ARAS_PLAYER_STATE_ERROR;
                break;
        case libvlc_Ended:
                *state = ARAS_PLAYER_STATE_STOP;
                break;
        case libvlc_Playing:
                *state = ARAS_PLAYER_STATE_PLAYING;
                break;
        case libvlc_Buffering:
                *state = ARAS_PLAYER_STATE_BUFFERING;
                break;
        default:
                *state = ARAS_PLAYER_STATE_ERROR;
                break;
        }
}

/**
 * This function returns the buffer percent field in a player structure.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 * @return  The current buffer percent value
 */
int aras_player_get_buffer_percent(struct aras_player *player, int unit)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                return player->buffer_percent_a;
        case ARAS_PLAYER_UNIT_B:
                return player->buffer_percent_b;
        default:
                return 0;
        }
}

/**
 * This function gets the player current unit.
 *
 * @param   player  Pointer to the player
 * @return  The current unit
 */
int aras_player_get_current_unit(struct aras_player *player)
{
        return player->current_unit;
}

/**
 * This function returns the duration in miliseconds of the file being played in
 * a player structure.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 * @return  The duration of the file being played
 */
long int aras_player_get_duration(struct aras_player *player, int unit)
{
        int64_t duration;

        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                duration = libvlc_media_player_get_length(player->player_a);
                break;
        case ARAS_PLAYER_UNIT_B:
                duration = libvlc_media_player_get_length(player->player_b);
                break;
        default:
                duration = 0;
                break;
        }
        return duration;
}

/**
 * This function returns the position in miliseconds of the file being played in
 * a player structure.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 * @return  The position of the file being played
 */
long int aras_player_get_position(struct aras_player *player, int unit)
{
        int64_t position;

        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                position = libvlc_media_player_get_time(player->player_a);
                break;
        case ARAS_PLAYER_UNIT_B:
                position = libvlc_media_player_get_time(player->player_b);
                break;
        default:
                        position = 0;
                break;
        }
        return position;
}
