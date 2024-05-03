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
 * Source file for the ARAS Radio Automation System. Functions for the
 * configuration module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <aras/parse.h>
#include <aras/configuration.h>

/**
 * This function sets the configuration_file_period field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_configuration_period(struct aras_configuration *configuration, char *argument)
{
        configuration->configuration_period = abs(atoi(argument));
}

/**
 * This function sets the schedule_file field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_schedule_file(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->schedule_file, sizeof(configuration->schedule_file), "%s", argument);
}

/**
 * This function sets the block_file field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_file(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->block_file, sizeof(configuration->block_file), "%s", argument);
}

/**
 * This function sets the log_file field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_log_file(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->log_file, sizeof(configuration->log_file), "%s", argument);
}

/**
 * This function sets the schedule_mode field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_schedule_mode(struct aras_configuration *configuration, char *argument)
{
        if (!strcasecmp(argument, "hard"))
                configuration->schedule_mode = ARAS_CONFIGURATION_MODE_SCHEDULE_HARD;
        else if (!strcasecmp(argument, "soft"))
                configuration->schedule_mode = ARAS_CONFIGURATION_MODE_SCHEDULE_SOFT;
        else
                configuration->schedule_mode = ARAS_CONFIGURATION_MODE_SCHEDULE_HARD;
}

/**
 * This function sets the default_block_mode field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_default_block_mode(struct aras_configuration *configuration, char *argument)
{
        if (!strcasecmp(argument, "off"))
                configuration->default_block_mode = ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_OFF;
        else if (!strcasecmp(argument, "on"))
                configuration->default_block_mode = ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_ON;
        else
                configuration->default_block_mode = ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_OFF;
}

/**
 * This function sets the default_block field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_default_block(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->default_block, sizeof(configuration->default_block), "%s", argument);
}

/**
 * This function sets the fade_out_time field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_fade_out_time(struct aras_configuration *configuration, char *argument)
{
        if (atoi(argument) < 0)
                configuration->fade_out_time = 0;
        else
                configuration->fade_out_time = atoi(argument);
}

/**
 * This function sets the fade_out_slope field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_fade_out_slope(struct aras_configuration *configuration, char *argument)
{
        if (atof(argument) < 0.0)
                configuration->fade_out_slope = 0.0;
        else if (atof(argument) > 1.0)
                configuration->fade_out_slope = 1.0;
        else
                configuration->fade_out_slope = atof(argument);
}

/**
 * This function sets the engine_period field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_engine_period(struct aras_configuration *configuration, char *argument)
{
        if (atoi(argument) < 0)
                configuration->engine_period = 0;
        else
                configuration->engine_period = atoi(argument);
}

/**
 * This function sets the time_signal_mode field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_mode(struct aras_configuration *configuration, char *argument)
{
        if (!strcasecmp(argument, "off"))
                configuration->time_signal_mode = ARAS_CONFIGURATION_MODE_TIME_SIGNAL_OFF;
        else if (!strcasecmp(argument, "hour"))
                configuration->time_signal_mode = ARAS_CONFIGURATION_MODE_TIME_SIGNAL_HOUR;
        else if (!strcasecmp(argument, "half"))
                configuration->time_signal_mode = ARAS_CONFIGURATION_MODE_TIME_SIGNAL_HALF;
        else
                configuration->time_signal_mode = ARAS_CONFIGURATION_MODE_TIME_SIGNAL_OFF;
}

/**
 * This function sets the time_signal_advance field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_advance(struct aras_configuration *configuration, char *argument)
{
        if (atoi(argument) < 0)
                configuration->time_signal_advance = 0;
        else
                configuration->time_signal_advance = atoi(argument);
}

/**
 * This function sets the time_signal_block field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_block(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->time_signal_block, sizeof(configuration->time_signal_block), "%s", argument);
}

/**
 * This function sets the block_player_name field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_player_name(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->block_player_name, sizeof(configuration->block_player_name), "%s", argument);
}

/**
 * This function sets the block_player_audio_output field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_player_audio_output(struct aras_configuration *configuration, char *argument)
{
        if (!strcasecmp(argument, "auto"))
                configuration->block_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_AUTO;
        else if (!strcasecmp(argument, "pulseaudio") || !strcasecmp(argument, "pulse"))
                configuration->block_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_PULSEAUDIO;
        else if (!strcasecmp(argument, "alsa"))
                configuration->block_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_ALSA;
        else if (!strcasecmp(argument, "jack"))
                configuration->block_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_JACK;
        else if (!strcasecmp(argument, "oss"))
                configuration->block_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_OSS;
        else if (!strcasecmp(argument, "oss4"))
                configuration->block_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_OSS4;
        else if (!strcasecmp(argument, "openal"))
                configuration->block_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_OPENAL;
        else if (!strcasecmp(argument, "file"))
                configuration->block_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_FILE;
        else
                configuration->block_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_AUTO;
}

/**
 * This function sets the block_player_audio_device field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_player_audio_device(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->block_player_audio_device, sizeof(configuration->block_player_audio_device), "%s", argument);
}

/**
 * This function sets the block_player_video_output field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_player_video_output(struct aras_configuration *configuration, char *argument)
{
        if (!strcasecmp(argument, "auto"))
                configuration->block_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_AUTO;
        else if (!strcasecmp(argument, "v4l2"))
                configuration->block_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_V4L2;
        else if (!strcasecmp(argument, "x"))
                configuration->block_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_X;
        else if (!strcasecmp(argument, "xv"))
                configuration->block_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_XV;
        else if (!strcasecmp(argument, "fb"))
                configuration->block_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_FB;
        else if (!strcasecmp(argument, "gl"))
                configuration->block_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_GL;
        else if (!strcasecmp(argument, "file"))
                configuration->block_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_FILE;
        else
                configuration->block_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_AUTO;
}

/**
 * This function sets the block_player_video_device field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_player_video_device(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->block_player_video_device, sizeof(configuration->block_player_video_device), "%s", argument);
}

/**
 * This function sets the block_player_video_display field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_player_video_display(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->block_player_video_display, sizeof(configuration->block_player_video_display), "%s", argument);
}

/**
 * This function sets the block_player_volume field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_player_volume(struct aras_configuration *configuration, char *argument)
{
        if (atof(argument) < 0.0)
                configuration->block_player_volume = 0.0;
        else if (atof(argument) > 1.0)
                configuration->block_player_volume = 1.0;
        else
                configuration->block_player_volume = atof(argument);
}

/**
 * This function sets the block_player_sample_rate field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_player_sample_rate(struct aras_configuration *configuration, char *argument)
{
        if (atoi(argument) < 0)
                configuration->block_player_sample_rate = 0;
        else
                configuration->block_player_sample_rate = atoi(argument);
}

/**
 * This function sets the block_player_channels field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_player_channels(struct aras_configuration *configuration, char *argument)
{
        if (atoi(argument) < 0)
                configuration->block_player_channels = 0;
        else
                configuration->block_player_channels = atoi(argument);
}

/**
 * This function sets the block_player_display_resolution field in a
 * configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_block_player_display_resolution(struct aras_configuration *configuration, char *argument)
{
        int width;
        int height;

        sscanf(argument, "%dx%d", &width, &height);
        configuration->block_player_display_resolution[0] = width;
        configuration->block_player_display_resolution[1] = height;
}

/**
 * This function sets the time_signal_player_name field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_player_name(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->time_signal_player_name, sizeof(configuration->time_signal_player_name), "%s", argument);
}

/**
 * This function sets the time_signal_player_audio_output field in a
 * configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_player_audio_output(struct aras_configuration *configuration, char *argument)
{
        if (!strcasecmp(argument, "auto"))
                configuration->time_signal_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_AUTO;
        else if (!strcasecmp(argument, "pulseaudio"))
                configuration->time_signal_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_PULSEAUDIO;
        else if (!strcasecmp(argument, "alsa"))
                configuration->time_signal_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_ALSA;
        else if (!strcasecmp(argument, "jack"))
                configuration->time_signal_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_JACK;
        else if (!strcasecmp(argument, "oss"))
                configuration->time_signal_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_OSS;
        else if (!strcasecmp(argument, "oss4"))
                configuration->time_signal_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_OSS4;
        else if (!strcasecmp(argument, "openal"))
                configuration->time_signal_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_OPENAL;
        else if (!strcasecmp(argument, "file"))
                configuration->time_signal_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_FILE;
        else
                configuration->time_signal_player_audio_output = ARAS_CONFIGURATION_MODE_AUDIO_AUTO;
}

/**
 * This function sets the time_signal_player_audio_device field in a
 * configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_player_audio_device(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->time_signal_player_audio_device, sizeof(configuration->time_signal_player_audio_device), "%s", argument);
}

/**
 * This function sets the time_signal_player_video_output field in a
 * configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_player_video_output(struct aras_configuration *configuration, char *argument)
{
        if (!strcasecmp(argument, "auto"))
                configuration->time_signal_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_AUTO;
        else if (!strcasecmp(argument, "v4l2"))
                configuration->time_signal_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_V4L2;
        else if (!strcasecmp(argument, "x"))
                configuration->time_signal_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_X;
        else if (!strcasecmp(argument, "xv"))
                configuration->time_signal_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_XV;
        else if (!strcasecmp(argument, "fb"))
                configuration->time_signal_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_FB;
        else if (!strcasecmp(argument, "gl"))
                configuration->time_signal_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_GL;
        else if (!strcasecmp(argument, "file"))
                configuration->time_signal_player_video_output = ARAS_CONFIGURATION_MODE_VIDEO_FILE;
        else
                configuration->time_signal_player_video_output = ARAS_CONFIGURATION_MODE_AUDIO_AUTO;
}

/**
 * This function sets the time_signal_player_video_device field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_player_video_device(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->time_signal_player_video_device, sizeof(configuration->time_signal_player_video_device), "%s", argument);
}

/**
 * This function sets the time_signal_player_video_display field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_player_video_display(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->time_signal_player_video_display, sizeof(configuration->time_signal_player_video_display), "%s", argument);
}

/**
 * This function sets the time_signal_player_volume field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_player_volume(struct aras_configuration *configuration, char *argument)
{
        if (atof(argument) < 0.0)
                configuration->time_signal_player_volume = 0.0;
        else if (atof(argument) > 1.0)
                configuration->time_signal_player_volume = 1.0;
        else
                configuration->time_signal_player_volume = atof(argument);
}

/**
 * This function sets the time_signal_player_sample_rate field in a
 * configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_player_sample_rate(struct aras_configuration *configuration, char *argument)
{
        if (atoi(argument) < 0)
                configuration->time_signal_player_sample_rate = 0;
        else
                configuration->time_signal_player_sample_rate = atoi(argument);
}

/**
 * This function sets the time_signal_player_channels field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_player_channels(struct aras_configuration *configuration, char *argument)
{
        if (atoi(argument) < 0)
                configuration->time_signal_player_channels = 0;
        else
                configuration->time_signal_player_channels = atoi(argument);
}

/**
 * This function sets the time_signal_player_display_resolution field in a
 * configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_time_signal_player_display_resolution(struct aras_configuration *configuration, char *argument)
{
        int width;
        int height;

        sscanf(argument, "%dx%d", &width, &height);
        configuration->time_signal_player_display_resolution[0] = width;
        configuration->time_signal_player_display_resolution[1] = height;
}

/**
 * This function sets the recoder_name field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_recorder_name(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->recorder_name, sizeof(configuration->recorder_name), "%s", argument);
}

/**
 * This function sets the recorder_input field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_recorder_input(struct aras_configuration *configuration, char *argument)
{
        if (!strcasecmp(argument, "auto"))
                configuration->recorder_input = ARAS_CONFIGURATION_MODE_AUDIO_AUTO;
        else if (!strcasecmp(argument, "pulseaudio"))
                configuration->recorder_input = ARAS_CONFIGURATION_MODE_AUDIO_PULSEAUDIO;
        else if (!strcasecmp(argument, "alsa"))
                configuration->recorder_input = ARAS_CONFIGURATION_MODE_AUDIO_ALSA;
        else if (!strcasecmp(argument, "jack"))
                configuration->recorder_input = ARAS_CONFIGURATION_MODE_AUDIO_JACK;
        else if (!strcasecmp(argument, "oss"))
                configuration->recorder_input = ARAS_CONFIGURATION_MODE_AUDIO_OSS;
        else if (!strcasecmp(argument, "oss4"))
                configuration->recorder_input = ARAS_CONFIGURATION_MODE_AUDIO_OSS4;
        else if (!strcasecmp(argument, "file"))
                configuration->recorder_input = ARAS_CONFIGURATION_MODE_AUDIO_FILE;
        else
                configuration->recorder_input = ARAS_CONFIGURATION_MODE_AUDIO_AUTO;
}

/**
 * This function sets the recorder_device field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_recorder_device(struct aras_configuration *configuration, char *argument)
{
        snprintf(configuration->recorder_device, sizeof(configuration->recorder_device), "%s", argument);
}

/**
 * This function sets the recorder_sample_rate field in a configuration
 * structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_recorder_sample_rate(struct aras_configuration *configuration, char *argument)
{
        if (atoi(argument) < 0)
                configuration->recorder_sample_rate = 0;
        else
                configuration->recorder_sample_rate = atoi(argument);
}

/**
 * This function sets the recorder_channels field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_recorder_channels(struct aras_configuration *configuration, char *argument)
{
        if (atoi(argument) < 0)
                configuration->recorder_channels = 0;
        else
                configuration->recorder_channels = atoi(argument);
}

/**
 * This function sets the recorder_quality field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_recorder_quality(struct aras_configuration *configuration, char *argument)
{
        if (atof(argument) < -0.1)
                configuration->recorder_quality = -0.1;
        else if (atof(argument) > 1.0)
                configuration->recorder_quality = 1.0;
        else
                configuration->recorder_quality = atof(argument);
}

/**
 * This function sets the gui_period field in a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   argument        Pointer to the configuration argument string
 */
void aras_configuration_set_gui_period(struct aras_configuration *configuration, char *argument)
{
        if (atoi(argument) < 0)
                configuration->gui_period = 0;
        else
                configuration->gui_period = atoi(argument);
}

/**
 * This function loads in a configuration structure the data contained in a set
 * of strings.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   directive       Pointer to the directive string
 * @param   argument        Pointer to the argument string
 *
 * @return  0 if success, -1 if error
 */
int aras_configuration_load_data(struct aras_configuration *configuration, char *directive, char *argument)
{
        if (!strcasecmp(directive, "ConfigurationPeriod"))
                aras_configuration_set_configuration_period(configuration, argument);
        else if (!strcasecmp(directive, "ScheduleFile"))
                aras_configuration_set_schedule_file(configuration, argument);
        else if (!strcasecmp(directive, "BlockFile"))
                aras_configuration_set_block_file(configuration, argument);
        else if (!strcasecmp(directive, "LogFile"))
                aras_configuration_set_log_file(configuration, argument);
        else if (!strcasecmp(directive, "EnginePeriod"))
                aras_configuration_set_engine_period(configuration, argument);
        else if (!strcasecmp(directive, "ScheduleMode"))
                aras_configuration_set_schedule_mode(configuration, argument);
        else if (!strcasecmp(directive, "DefaultBlockMode"))
                aras_configuration_set_default_block_mode(configuration, argument);
        else if (!strcasecmp(directive, "DefaultBlock"))
                aras_configuration_set_default_block(configuration, argument);
        else if (!strcasecmp(directive, "FadeOutTime"))
                aras_configuration_set_fade_out_time(configuration, argument);
        else if (!strcasecmp(directive, "FadeOutSlope"))
                aras_configuration_set_fade_out_slope(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalMode"))
                aras_configuration_set_time_signal_mode(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalAdvance"))
                aras_configuration_set_time_signal_advance(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalBlock"))
                aras_configuration_set_time_signal_block(configuration, argument);
        else if (!strcasecmp(directive, "BlockPlayerName"))
                aras_configuration_set_block_player_name(configuration, argument);
        else if (!strcasecmp(directive, "BlockPlayerAudioOutput"))
                aras_configuration_set_block_player_audio_output(configuration, argument);
        else if (!strcasecmp(directive, "BlockPlayerAudioDevice"))
                aras_configuration_set_block_player_audio_device(configuration, argument);
        else if (!strcasecmp(directive, "BlockPlayerVolume"))
                aras_configuration_set_block_player_volume(configuration, argument);
        else if (!strcasecmp(directive, "BlockPlayerSampleRate"))
                aras_configuration_set_block_player_sample_rate(configuration, argument);
        else if (!strcasecmp(directive, "BlockPlayerChannels"))
                aras_configuration_set_block_player_channels(configuration, argument);
        else if (!strcasecmp(directive, "BlockPlayerVideoOutput"))
                aras_configuration_set_block_player_video_output(configuration, argument);
        else if (!strcasecmp(directive, "BlockPlayerVideoDevice"))
                aras_configuration_set_block_player_video_device(configuration, argument);
        else if (!strcasecmp(directive, "BlockPlayerVideoDisplay"))
                aras_configuration_set_block_player_video_display(configuration, argument);
        else if (!strcasecmp(directive, "BlockPlayerDisplayResolution"))
                aras_configuration_set_block_player_display_resolution(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalPlayerName"))
                aras_configuration_set_time_signal_player_name(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalPlayerAudioOutput"))
                aras_configuration_set_time_signal_player_audio_output(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalPlayerAudioDevice"))
                aras_configuration_set_time_signal_player_audio_device(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalPlayerVolume"))
                aras_configuration_set_time_signal_player_volume(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalPlayerSampleRate"))
                aras_configuration_set_time_signal_player_sample_rate(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalPlayerChannels"))
                aras_configuration_set_time_signal_player_channels(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalPlayerVideoOutput"))
                aras_configuration_set_time_signal_player_video_output(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalPlayerVideoDevice"))
                aras_configuration_set_time_signal_player_video_device(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalPlayerVideoDisplay"))
                aras_configuration_set_time_signal_player_video_display(configuration, argument);
        else if (!strcasecmp(directive, "TimeSignalDisplayResolution"))
                aras_configuration_set_time_signal_player_display_resolution(configuration, argument);
        else if (!strcasecmp(directive, "RecorderName"))
                aras_configuration_set_recorder_name(configuration, argument);
        else if (!strcasecmp(directive, "RecorderInput"))
                aras_configuration_set_recorder_input(configuration, argument);
        else if (!strcasecmp(directive, "RecorderDevice"))
                aras_configuration_set_recorder_device(configuration, argument);
        else if (!strcasecmp(directive, "RecorderSampleRate"))
                aras_configuration_set_recorder_sample_rate(configuration, argument);
        else if (!strcasecmp(directive, "RecorderChannels"))
                aras_configuration_set_recorder_channels(configuration, argument);
        else if (!strcasecmp(directive, "RecorderQuality"))
                aras_configuration_set_recorder_quality(configuration, argument);
        else if (!strcasecmp(directive, "GUIPeriod"))
                aras_configuration_set_gui_period(configuration, argument);
        else
                return -1;

        return 0;
}

/**
 * This function loads in a configuration structure the data contained in a
 * line.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   line            Pointer to the line
 *
 * @return  0 if success, -1 if error
 */
int aras_configuration_load_line(struct aras_configuration *configuration, char *line)
{
        char directive[ARAS_CONFIGURATION_MAX_DIRECTIVE];
        char argument[ARAS_CONFIGURATION_MAX_ARGUMENT];

        /* Get directive and argument */
        if ((line = aras_parse_line_configuration(line, directive, sizeof(directive))) == NULL)
                return -1;

        if ((line = aras_parse_line_configuration(line, argument, sizeof(argument))) == NULL)
                return -1;

        aras_configuration_load_data(configuration, directive, argument);

        return 0;
}

/**
 * This function loads in a configuration structure the data contained in a
 * file.
 *
 * @param   configuration   Pointer to the configuration structure
 * @param   file            Pointer to the file name string
 *
 * @return  0 if success, -1 if error
 */
int aras_configuration_load_file(struct aras_configuration *configuration, char *file)
{
        FILE *fp;
        char line[ARAS_CONFIGURATION_MAX_LINE];

        /* Open configuration file */
        if ((fp = fopen(file, "r")) == NULL)
                return -1;

        /* Get lines from configuration file */
        while (fgets(line, ARAS_CONFIGURATION_MAX_LINE, fp) != NULL)
                aras_configuration_load_line(configuration, line);

        /* Close configuration file */
        if (fclose(fp) != 0)
                return -1;

        return 0;
}

/**
 * This function initializes a configuration structure.
 *
 * @param   configuration   Pointer to the configuration structure
 *
 * @return  This function always returns 0
 */
int aras_configuration_init(struct aras_configuration *configuration)
{
        /* Clean structure */
        memset(configuration, 0, sizeof(*configuration));

        /* Configuration period */
        aras_configuration_set_configuration_period(configuration, "10000");

        /* Files */
        aras_configuration_set_schedule_file(configuration, "/etc/aras/aras.schedule");
        aras_configuration_set_block_file(configuration, "/etc/aras/aras.block");
        aras_configuration_set_log_file(configuration, "/var/log/aras/aras.log");

        /* Engine configuration */
        aras_configuration_set_engine_period(configuration, "100");
        aras_configuration_set_schedule_mode(configuration, "hard");
        aras_configuration_set_default_block_mode(configuration, "on");
        aras_configuration_set_default_block(configuration, "default");
        aras_configuration_set_fade_out_time(configuration, "2000");
        aras_configuration_set_fade_out_slope(configuration, "0.1");
        aras_configuration_set_time_signal_mode(configuration, "off");
        aras_configuration_set_time_signal_advance(configuration, "4000");
        aras_configuration_set_time_signal_block(configuration, "time_signal");

        /* Block player configuration */
        aras_configuration_set_block_player_name(configuration, "block_player");
        aras_configuration_set_block_player_audio_output(configuration, "auto");
        aras_configuration_set_block_player_audio_device(configuration, "default");
        aras_configuration_set_block_player_volume(configuration, "1.0");
        aras_configuration_set_block_player_sample_rate(configuration, "48000");
        aras_configuration_set_block_player_channels(configuration, "2");
        aras_configuration_set_block_player_video_output(configuration, "auto");
        aras_configuration_set_block_player_video_device(configuration, "default");
        aras_configuration_set_block_player_video_display(configuration, ":0");
        aras_configuration_set_block_player_display_resolution(configuration, "1920x1080");

        /* Time signal player configuration */
        aras_configuration_set_time_signal_player_name(configuration, "time_signal_player");
        aras_configuration_set_time_signal_player_audio_output(configuration, "auto");
        aras_configuration_set_time_signal_player_audio_device(configuration, "default");
        aras_configuration_set_time_signal_player_volume(configuration, "1.0");
        aras_configuration_set_time_signal_player_sample_rate(configuration, "48000");
        aras_configuration_set_time_signal_player_channels(configuration, "2");
        aras_configuration_set_time_signal_player_video_output(configuration, "auto");
        aras_configuration_set_time_signal_player_video_device(configuration, "default");
        aras_configuration_set_time_signal_player_video_display(configuration, ":0");
        aras_configuration_set_time_signal_player_display_resolution(configuration, "1920x1080");

        /* Recorder configuration */
        aras_configuration_set_recorder_name(configuration, "recorder");
        aras_configuration_set_recorder_sample_rate(configuration, "48000");
        aras_configuration_set_recorder_channels(configuration, "2");
        aras_configuration_set_recorder_input(configuration, "auto");
        aras_configuration_set_recorder_device(configuration, "default");
        aras_configuration_set_recorder_quality(configuration, "0.5");

        /* GUI configuration */
        aras_configuration_set_gui_period(configuration, "50");

        return 0;
}
