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
 * Source file for the ARAS Radio Automation System. Functions for the engine
 * module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <aras/time.h>
#include <aras/parse.h>
#include <aras/log.h>
#include <aras/playlist.h>
#include <aras/configuration.h>
#include <aras/schedule.h>
#include <aras/block.h>
#include <aras/player.h>
#include <aras/engine.h>

/**
 * This function initializes an engine structure.
 *
 * @param   engine  Pointer to the engine structure
 *
 * @return  This function always returns 0
 */
int aras_engine_init(struct aras_engine *engine)
{
        engine->state = ARAS_ENGINE_STATE_NULL;
        engine->state_time_elapsed = 0;
        engine->state_time_maximum = 0;
        engine->playlist = NULL;
        engine->playlist_current_node = NULL;
        return 0;
}

/**
 * This function sets the state and its attribute state_time_maximum.
 *
 * @param   engine              Pointer to the engine structure containing the
 *                              state and the state attributes
 * @param   state               The next state
 * @param   state_time_maximum  The time limit for the state when applicable
 */
void aras_engine_set_state(struct aras_engine *engine, int state, long int state_time_maximum)
{
        if (engine == NULL)
                return;

        /* Set the required state */
        engine->state = state;

        /* Set the required time limit when applicable to the state */
        engine->state_time_elapsed = 0;
        engine->state_time_maximum = state_time_maximum;
}

/**
 * This function prints the playlist indicating the current playlist node.
 *
 * @param   engine  Pointer to the engine structure containing the
 *                  playlist and the current playlist node
 */
void aras_engine_playlist_print(struct aras_engine *engine)
{
        GList *pointer;
        char *node;

        printf("Playlist\n");
        printf("--------\n");

        pointer = engine->playlist;
        while (pointer != NULL) {
                node = pointer->data;
                printf("%s", node);
                if (pointer == engine->playlist_current_node)
                        printf(" (current)\n");
                else
                        printf("\n");
                pointer = pointer->next;
        }

        printf("\n");
        fflush(stdout);
}

/**
 * This function manages the state ARAS_ENGINE_STATE_FADE_OUT. It decreases the
 * volumes of both the current player unit and the idle player unit. Then, it
 * stops both player units. Finally, it sets the next state
 * ARAS_ENGINE_STATE_NULL.
 *
 * @param   engine  Pointer to the engine structure
 * @param   player  Pointer to the player structure with which the
 *                  engine works
 * @param   slope   The fade out slope
 * @param   period  The fade out execution period
 */
void aras_engine_fade_out(struct aras_engine *engine, struct aras_player *player, float slope, int period)
{
        /* Set volume */
        aras_player_set_volume_increment(player, player->current_unit, slope, 0);
        aras_player_set_volume_increment(player, (player->current_unit + 1) % 2, slope, 0);

        /* Check the end of the current state */
        if ((engine->state_time_elapsed += period) >= engine->state_time_maximum) {
                /* Stop player and update the current playlist node */
                aras_player_set_volume(player, player->current_unit, 0);
                aras_player_set_volume(player, (player->current_unit + 1) % 2, 0);
                /* Stop playback in current unit and idle unit */
                aras_player_set_state_ready(player, player->current_unit);
                aras_player_set_state_ready(player, (player->current_unit + 1) % 2);
                /* Next state */
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_NULL, 0);
        }
}

/**
 * This function manages the state ARAS_ENGINE_STATE_CROSSFADE. It increases the
 * volume of the current player unit and decreases the volume of the idle player
 * unit. Then, it stops the idle unit. Finally, it sets the next state
 * ARAS_ENGINE_STATE_NULL.
 *
 * @param   engine  Pointer to the engine structure
 * @param   player  Pointer to the player structure with which the
 *                  engine works
 * @param   volume  The final volume for the current unit
 * @param   slope   The fade out slope
 * @param   period  The fade out execution period
 */
void aras_engine_crossfade(struct aras_engine *engine, struct aras_player *player, float volume, float slope, int period)
{
        /* Set volume */
        aras_player_set_volume_increment(player, player->current_unit, slope, volume);
        aras_player_set_volume_increment(player, (player->current_unit + 1) % 2, slope, 0);

        /* Check the end of the current state */
        if ((engine->state_time_elapsed += period) >= engine->state_time_maximum) {
                /* Stop player and update the current playlist node */
                aras_player_set_volume(player, player->current_unit, volume);
                aras_player_set_volume(player, (player->current_unit + 1) % 2, 0);
                /* Stop playback in idle unit */
                aras_player_set_state_ready(player, (player->current_unit + 1) % 2);
                /* Next state */
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_NULL, 0);
        }
}

/**
 * This function manages the state ARAS_ENGINE_STATE_PLAY_CURRENT. It swaps the
 * current player unit and plays the current playlist node in it. Finally, it
 * sets the next state ARAS_ENGINE_STATE_CROSSFADE.
 *
 * @param   engine          Pointer to the engine structure
 * @param   player          Pointer to the player structure with which the
 *                          engine works
 * @param   fade_out_time   The fade out time in miliseconds
 * @param   log_file        The name of the log file
 */
void aras_engine_play_current(struct aras_engine *engine, struct aras_player *player, int fade_out_time, char *log_file)
{
        char msg[ARAS_LOG_MESSAGE_MAX];

        if (engine->playlist_current_node == NULL) {
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_NULL, 0);
                return;
        }

        /* Swap unit and play current node */
        aras_player_swap_current_unit(player);
        aras_player_set_volume(player, player->current_unit, 0);
        aras_player_set_uri(player, player->current_unit, engine->playlist_current_node->data);
        aras_player_set_state_playing(player, player->current_unit);

        /* Append message to log file */
        snprintf(msg, sizeof(msg),"URI: %s\n", (char*)engine->playlist_current_node->data);
        aras_log_write(log_file, msg);

        /* Perform crossfade */
        aras_engine_set_state(engine, ARAS_ENGINE_STATE_CROSSFADE, fade_out_time);
}

/**
 * This function manages the state ARAS_ENGINE_STATE_PLAY_PREVIOUS. If previous
 * playlist node exists, it updates the pointer to the current playlist node and
 * sets the state ARAS_ENGINE_STATE_PLAY_CURRENT; if previous playlist node does
 * not exist, it frees the playlist, loads the playlist for the default block if
 * necessary and sets the next state ARAS_ENGINE_STATE_PLAY_CURRENT or
 * ARAS_ENGINE_STATE_FADE_OUT.
 *
 * @param   engine              Pointer to the engine structure
 * @param   player              Pointer to the player structure with which the
 *                              engine works
 * @param   default_block_mode  The default block mode, it may take one of these
 *                              values:
 *                              (1) ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_OFF
 *                              (2) ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_ON
 * @param   default_block_mode  The name of the default block
 * @param   block               Pointer to the block structure
 * @param   fade_out_time       The fade out time in miliseconds
 * @param   log_file            The name of the block file
 */
void aras_engine_play_previous(struct aras_engine *engine, struct aras_player *player, int default_block_mode, char *default_block, struct aras_block *block, int fade_out_time, char *log_file)
{
        char msg[ARAS_LOG_MESSAGE_MAX];

        if (engine->playlist_current_node == NULL) {
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_NULL, 0);
                return;
        }

        if ((engine->playlist_current_node = engine->playlist_current_node->prev) == NULL) {
                /* Free playlist */
                engine->playlist = aras_playlist_free(engine->playlist);
                /* Check is default block is enabled */
                if (default_block_mode == ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_ON) {
                        /* Load default block and write log entry */
                        engine->playlist = aras_playlist_load(engine->playlist, default_block, block, 0);
                        snprintf(msg, sizeof(msg),"Default block: \"%s\"\n", default_block);
                        aras_log_write(log_file, msg);
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
                } else {
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_FADE_OUT, fade_out_time);
                }
                engine->playlist_current_node = engine->playlist;
        } else {
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
        }
}

/**
 * This function manages the state ARAS_ENGINE_STATE_PLAY_NEXT. If next playlist
 * node exists, it updates the pointer to the current playlist node and sets the
 * state ARAS_ENGINE_STATE_PLAY_CURRENT; if next playlist node does not exist,
 * it frees the playlist, loads the playlist for the default block if necessary
 * and sets the next state ARAS_ENGINE_STATE_PLAY_CURRENT or
 * ARAS_ENGINE_STATE_FADE_OUT.
 *
 * @param   engine              Pointer to the engine structure
 * @param   player              Pointer to the player structure with which the
 *                              engine works
 * @param   default_block_mode  The default block mode, it may take one of these
 *                              values:
 *                              (1) ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_OFF
 *                              (2) ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_ON
 * @param   default_block_mode  The name of the default block
 * @param   block               Pointer to the block structure
 * @param   fade_out_time       The fade out time in miliseconds
 * @param   log_file            The name of the block file
 */
void aras_engine_play_next(struct aras_engine *engine, struct aras_player *player, int default_block_mode, char *default_block, struct aras_block *block, int fade_out_time, char *log_file)
{
        char msg[ARAS_LOG_MESSAGE_MAX];

        if (engine->playlist_current_node == NULL) {
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_NULL, 0);
                return;
        }

        if ((engine->playlist_current_node = engine->playlist_current_node->next) == NULL) {
                /* Free playlist */
                engine->playlist = aras_playlist_free(engine->playlist);
                /* Check is default block is enabled */
                if (default_block_mode == ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_ON) {
                        /* Load default block and write log entry */
                        engine->playlist = aras_playlist_load(engine->playlist, default_block, block, 0);
                        snprintf(msg, sizeof(msg),"Default block: \"%s\"\n", default_block);
                        aras_log_write(log_file, msg);
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
                } else {
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_FADE_OUT, fade_out_time);
                }
                engine->playlist_current_node = engine->playlist;
        } else {
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
        }
}

/**
 * This function manages the state ARAS_ENGINE_STATE_PLAY_DEFAULT. It frees the
 * playlist, loads the playlist for the default block if necessary and sets the
 * next state ARAS_ENGINE_STATE_PLAY_CURRENT or ARAS_ENGINE_STATE_FADE_OUT.
 *
 * @param   engine              Pointer to the engine structure
 * @param   player              Pointer to the player structure with which the
 *                              engine works
 * @param   default_block_mode  The default block mode, it may take one of these
 *                              values:
 *                              (1) ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_OFF
 *                              (2) ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_ON
 * @param   default_block       The name of the default block
 * @param   block               Pointer to the block structure
 * @param   fade_out_time       The fade out time in miliseconds
 * @param   log_file            The name of the log file
 */
void aras_engine_play_default(struct aras_engine *engine, struct aras_player *player, int default_block_mode, char *default_block, struct aras_block *block, int fade_out_time, char *log_file)
{
        char msg[ARAS_LOG_MESSAGE_MAX];

        /* Free playlist */
        engine->playlist = aras_playlist_free(engine->playlist);
        /* Check is default block is enabled */
        if (default_block_mode == ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_ON) {
                /* Load default block and write log entry */
                engine->playlist = aras_playlist_load(engine->playlist, default_block, block, 0);
                snprintf(msg, sizeof(msg),"Default block: \"%s\"\n", default_block);
                aras_log_write(log_file, msg);
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
        } else {
                /* Free playlist */
                engine->playlist = aras_playlist_free(engine->playlist);
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_FADE_OUT, fade_out_time);
        }
        engine->playlist_current_node = engine->playlist;
}

/**
 * This function manages the state ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT. It
 * updates the playlist according to the schedule and monitors the playback.
 *
 * @param   engine          Pointer to the engine structure
 * @param   player          Pointer to the player structure with which the
 *                          engine works
 * @param   configuration   Pointer to the configuration structure
 * @param   schedule        Pointer to the schedule structure
 * @param   block           Pointer to the block structure
 */
void aras_engine_monitor_schedule_soft(struct aras_engine *engine, struct aras_player *player, struct aras_configuration *configuration, struct aras_schedule *schedule, struct aras_block *block)
{
        char msg[ARAS_LOG_MESSAGE_MAX];
        libvlc_state_t state;
        struct aras_schedule_node *current_schedule_node;
        struct aras_schedule_node *next_schedule_node;
        static int pending_playlist = 0;
        long duration;
        long position;

        /* If playlist not present, load playlist for the default block and notify pending playlist */
        if (engine->playlist == NULL) {
                if (configuration->default_block_mode == ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_ON) {
                        /* Load default block and write log entry */
                        engine->playlist = aras_playlist_load(engine->playlist, configuration->default_block, block, 0);
                        engine->playlist_current_node = engine->playlist;
                        snprintf(msg, sizeof(msg),"Default block: \"%s\"\n", configuration->default_block);
                        aras_log_write(configuration->log_file, msg);
                        pending_playlist = 1;
                        return;
                }
        }

        /* If next schedule node not present, do nothing */
        if ((next_schedule_node = aras_schedule_seek_node_next(schedule, aras_time_current())) == NULL)
                return;

        /* If current schedule node not present, do nothing */
        if ((current_schedule_node = aras_schedule_seek_node_current(schedule, aras_time_current())) == NULL)
                return;

        /* If next schedule node is inminent, do nothing */
        if (aras_time_difference(next_schedule_node->time, aras_time_current()) < (configuration->fade_out_time + 6 * configuration->engine_period))
                return;

        /* If a new schedule node is reached, load the appropriate playlist and notify pending playlist */
        if (aras_time_reached(aras_time_current(), current_schedule_node->time, configuration->engine_period)) {
                /* Load playlist for the new schedule node and write log entry */
                engine->playlist = aras_playlist_free(engine->playlist);
                engine->playlist = aras_playlist_load(engine->playlist, current_schedule_node->block_name, block, 0);
                engine->playlist_current_node = engine->playlist;
                snprintf(msg, sizeof(msg),"Regular block: \"%s\"\n", current_schedule_node->block_name);
                aras_log_write(configuration->log_file, msg);
                pending_playlist = 1;
                return;
        }

        /* If no more files to play, do nothing */
        if (engine->playlist_current_node == NULL)
                return;

        /* If the current unit is not playing and next schedule node does not interfere with the crossfade, play the next playlist node */
        aras_player_get_state(player, player->current_unit, &state);
        switch (state) {
        case libvlc_Error:
                aras_player_set_state_ready(player, player->current_unit);
                if (pending_playlist == 1) {
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
                        pending_playlist = 0;
                } else {
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_NEXT, 0);
                }
                break;
        case libvlc_Ended:
                if (pending_playlist == 1) {
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
                        pending_playlist = 0;
                } else {
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_NEXT, 0);
                }
                break;
        case libvlc_Playing:
                /* If not streaming play the next playlist node */
                if ((duration = aras_player_get_duration(player, player->current_unit)) != 0) {
                        position = aras_player_get_position(player, player->current_unit);
                        if (duration - position <= configuration->fade_out_time) {
                                if (pending_playlist == 1) {
                                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
                                        pending_playlist = 0;
                                } else {
                                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_NEXT, 0);
                                }
                        }
                }
                break;
        default:
                break;
        }
}

/**
 * This function manages the state ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD. It
 * updates the playlist according to the schedule and monitors the playback.
 *
 * @param   engine          Pointer to the engine structure
 * @param   player          Pointer to the player structure with which the
 *                          engine works
 * @param   configuration   Pointer to the configuration structure
 * @param   schedule        Pointer to the schedule structure
 * @param   block           Pointer to the block structure
 */
void aras_engine_monitor_schedule_hard(struct aras_engine *engine, struct aras_player *player, struct aras_configuration *configuration, struct aras_schedule *schedule, struct aras_block *block)
{
        char msg[ARAS_LOG_MESSAGE_MAX];
        libvlc_state_t state;
        struct aras_schedule_node *current_schedule_node;
        struct aras_schedule_node *next_schedule_node;
        long duration;
        long position;

        /* If playlist not present, load playlist for the default block */
        if (engine->playlist == NULL) {
                if (configuration->default_block_mode == ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_ON) {
                        /* Load default block and write log entry */
                        engine->playlist = aras_playlist_load(engine->playlist, configuration->default_block, block, 0);
                        engine->playlist_current_node = engine->playlist;
                        snprintf(msg, sizeof(msg),"Default block: \"%s\"\n", configuration->default_block);
                        aras_log_write(configuration->log_file, msg);
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
                        return;
                }
        }

        /* If next schedule node not present, do nothing */
        if ((next_schedule_node = aras_schedule_seek_node_next(schedule, aras_time_current())) == NULL)
                return;

        /* If current schedule node not present, do nothing */
        if ((current_schedule_node = aras_schedule_seek_node_current(schedule, aras_time_current())) == NULL)
                return;

        /* If next schedule node is inminent, do nothing */
        if (aras_time_difference(next_schedule_node->time, aras_time_current()) < (configuration->fade_out_time + 6 * configuration->engine_period))
                return;

        /* If a new schedule node is reached, load the appropriate playlist and play the first playlist node */
        if (aras_time_reached(aras_time_current(), current_schedule_node->time, configuration->engine_period)) {
                /* Load playlist for the new schedule node and write log entry */
                engine->playlist = aras_playlist_free(engine->playlist);
                engine->playlist = aras_playlist_load(engine->playlist, current_schedule_node->block_name, block, 0);
                engine->playlist_current_node = engine->playlist;
                snprintf(msg, sizeof(msg),"Regular block: \"%s\"\n", current_schedule_node->block_name);
                aras_log_write(configuration->log_file, msg);
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
                return;
        }

        /* If no more files to play, do nothing */
        if (engine->playlist_current_node == NULL)
                return;

        /* If the current unit is not playing and next schedule node does not interfere with the crossfade, play the next playlist node */
        aras_player_get_state(player, player->current_unit, &state);
        switch (state) {
        case libvlc_Error:
                aras_player_set_state_ready(player, player->current_unit);
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_NEXT, 0);
                break;
        case libvlc_Ended:
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_NEXT, 0);
                break;
        case libvlc_Playing:
                /* If not streaming play the next playlist node */
                if ((duration = aras_player_get_duration(player, player->current_unit)) != 0) {
                        position = aras_player_get_position(player, player->current_unit);
                        if (duration - position <= configuration->fade_out_time) 
                                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_NEXT, 0);
                }
                break;
        default:
                break;
        }
}

/**
 * This function manages a block player engine.
 *
 * @param   engine          Pointer to the engine structure
 * @param   player          Pointer to the player structure with which the
 *                          engine works
 * @param   configuration   Pointer to the configuration structure
 * @param   schedule        Pointer to the schedule structure
 * @param   block           Pointer to the block structure
 */
void aras_engine_schedule(struct aras_engine *engine, struct aras_player *player, struct aras_configuration *configuration, struct aras_schedule *schedule, struct aras_block *block)
{
        char msg[ARAS_LOG_MESSAGE_MAX];

        switch (engine->state) {
        case ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD:
                aras_engine_monitor_schedule_hard(engine, player, configuration, schedule, block);
                break;
        case ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT:
                aras_engine_monitor_schedule_soft(engine, player, configuration, schedule, block);
                break;
        case ARAS_ENGINE_STATE_PLAY_PREVIOUS:
                aras_engine_play_previous(engine, player, configuration->default_block_mode, configuration->default_block, block, configuration->fade_out_time, configuration->log_file);
                break;
        case ARAS_ENGINE_STATE_PLAY_NEXT:
                aras_engine_play_next(engine, player, configuration->default_block_mode, configuration->default_block, block, configuration->fade_out_time, configuration->log_file);
                break;
        case ARAS_ENGINE_STATE_PLAY_CURRENT:
                aras_engine_play_current(engine, player, configuration->fade_out_time, configuration->log_file);
                break;
        case ARAS_ENGINE_STATE_PLAY_DEFAULT:
                aras_engine_play_default(engine, player, configuration->default_block_mode, configuration->default_block, block, configuration->fade_out_time, configuration->log_file);
                break;
        case ARAS_ENGINE_STATE_CROSSFADE:
                aras_engine_crossfade(engine, player, configuration->block_player_volume, configuration->fade_out_slope, configuration->engine_period);
                break;
        case ARAS_ENGINE_STATE_FADE_OUT:
                aras_engine_fade_out(engine, player, configuration->fade_out_slope, configuration->engine_period);
                break;
        default:
                if (configuration->schedule_mode == ARAS_CONFIGURATION_MODE_SCHEDULE_HARD)
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD, 0);
                else
                        aras_engine_set_state(engine, ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT, 0);
                break;
        }
}

/**
 * This function manages the state ARAS_ENGINE_STATE_MONITOR_TIME_SIGNAL.
 * It computes the time for the next time signal and sets the time limit
 * according to the configuration. Then, it sets the next state
 * ARAS_ENGINE_STATE_MONITOR_WAIT.
 *
 * @param   engine          Pointer to the engine structure
 * @param   player          Pointer to the player structure with which the
 *                          engine works
 * @param   configuration   Pointer to the configuration structure
 * @param   schedule        Pointer to the schedule structure
 * @param   block           Pointer to the block structure
 */
void aras_engine_monitor_time_signal(struct aras_engine *engine, struct aras_player *player, struct aras_configuration *configuration, struct aras_block *block)
{
        long int next_time_signal;
        char msg[ARAS_LOG_MESSAGE_MAX];
        libvlc_state_t state;
        long duration;
        long position;

        /* Get the time for the next time signal */
        switch (configuration->time_signal_mode) {
        case ARAS_CONFIGURATION_MODE_TIME_SIGNAL_HALF:
                next_time_signal = (ldiv(aras_time_current(), ARAS_TIME_HOUR / 2).quot + 1) * ARAS_TIME_HOUR / 2;
                break;
        case ARAS_CONFIGURATION_MODE_TIME_SIGNAL_HOUR:
                next_time_signal = (ldiv(aras_time_current(), ARAS_TIME_HOUR).quot + 1) * ARAS_TIME_HOUR;
                break;
        default:       
                return;
        }

        /* If a new time signal is reached, load the appropriate playlist and play the first playlist node */
        if (aras_time_reached(aras_time_current(),
                              aras_time_difference(next_time_signal, configuration->time_signal_advance),
                              configuration->engine_period)) {
                /* Load playlist for the new schedule node and write log entry */
                engine->playlist = aras_playlist_free(engine->playlist);
                engine->playlist = aras_playlist_load(engine->playlist, configuration->time_signal_block, block, 0);
                engine->playlist_current_node = engine->playlist;
                snprintf(msg, sizeof(msg),"Time signal block: \"%s\"\n", configuration->time_signal_block);
                aras_log_write(configuration->log_file, msg);
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_CURRENT, 0);
                return;
        }

        /* If no more files to play, do nothing */
        if (engine->playlist_current_node == NULL)
                return;

        /* Play the next playlist node */
        aras_player_get_state(player, player->current_unit, &state);
        switch (state) {
        case libvlc_Error:
                aras_player_set_state_ready(player, player->current_unit);
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_NEXT, 0);
                break;
        case libvlc_Ended:
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_NEXT, 0);
                break;
        case libvlc_Playing:
                /* If not streaming play the next playlist node */
                if ((duration = aras_player_get_duration(player, player->current_unit)) != 0) {
                        position = aras_player_get_position(player, player->current_unit);
                        if (duration - position <= configuration->fade_out_time)
                                aras_engine_set_state(engine, ARAS_ENGINE_STATE_PLAY_NEXT, 0);
                }
                break;
        default:
                break;
        }
}

/**
 * This function manages a time signal engine.
 *
 * @param   engine          The engine structure
 * @param   player          Pointer to the player structure with which the
 *                          engine works
 * @param   configuration   Pointer to the configuration structure
 * @param   block           Pointer to the block structure
 */
void aras_engine_time_signal(struct aras_engine *engine, struct aras_player *player, struct aras_configuration *configuration, struct aras_block *block)
{

        switch (engine->state) {
        case ARAS_ENGINE_STATE_MONITOR_TIME_SIGNAL:
                aras_engine_monitor_time_signal(engine, player, configuration, block);
                break;
        case ARAS_ENGINE_STATE_PLAY_NEXT:
                aras_engine_play_next(engine, player, ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_OFF, configuration->default_block, block, configuration->fade_out_time, configuration->log_file);
                break;
        case ARAS_ENGINE_STATE_PLAY_CURRENT:
                aras_engine_play_current(engine, player, configuration->fade_out_time, configuration->log_file);
                break;
        case ARAS_ENGINE_STATE_CROSSFADE:
                aras_engine_crossfade(engine, player, configuration->time_signal_player_volume, configuration->fade_out_slope, configuration->engine_period);
                break;
        case ARAS_ENGINE_STATE_FADE_OUT:
                aras_engine_fade_out(engine, player, configuration->fade_out_slope, configuration->engine_period);
                break;
        default:
                aras_engine_set_state(engine, ARAS_ENGINE_STATE_MONITOR_TIME_SIGNAL, 0);
                break;
        }
}
