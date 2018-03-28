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
 * the configuration module.
 */

#ifndef _ARAS_CONFIGURATION_H
#define _ARAS_CONFIGURATION_H

#define ARAS_CONFIGURATION_MAX_LINE                 2048
#define ARAS_CONFIGURATION_MAX_DIRECTIVE            32
#define ARAS_CONFIGURATION_MAX_ARGUMENT             1024

#define ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_OFF   0
#define ARAS_CONFIGURATION_MODE_DEFAULT_BLOCK_ON    1

#define ARAS_CONFIGURATION_MODE_SCHEDULE_HARD       0
#define ARAS_CONFIGURATION_MODE_SCHEDULE_SOFT       1

#define ARAS_CONFIGURATION_MODE_TIME_SIGNAL_OFF     0
#define ARAS_CONFIGURATION_MODE_TIME_SIGNAL_HALF    1
#define ARAS_CONFIGURATION_MODE_TIME_SIGNAL_HOUR    2

#define ARAS_CONFIGURATION_MODE_AUDIO_AUTO          0
#define ARAS_CONFIGURATION_MODE_AUDIO_PULSEAUDIO    1
#define ARAS_CONFIGURATION_MODE_AUDIO_ALSA          2
#define ARAS_CONFIGURATION_MODE_AUDIO_JACK          3
#define ARAS_CONFIGURATION_MODE_AUDIO_OSS           4
#define ARAS_CONFIGURATION_MODE_AUDIO_OSS4          5
#define ARAS_CONFIGURATION_MODE_AUDIO_OPENAL        6
#define ARAS_CONFIGURATION_MODE_AUDIO_FILE          7

#define ARAS_CONFIGURATION_MODE_VIDEO_AUTO          0
#define ARAS_CONFIGURATION_MODE_VIDEO_V4L2          1
#define ARAS_CONFIGURATION_MODE_VIDEO_X             2
#define ARAS_CONFIGURATION_MODE_VIDEO_XV            3
#define ARAS_CONFIGURATION_MODE_VIDEO_FB            4
#define ARAS_CONFIGURATION_MODE_VIDEO_GL            5
#define ARAS_CONFIGURATION_MODE_VIDEO_FILE          6

struct aras_configuration {

        /* Configuration files */
        int configuration_period;
        char schedule_file[ARAS_CONFIGURATION_MAX_ARGUMENT];
        char block_file[ARAS_CONFIGURATION_MAX_ARGUMENT];
        char log_file[ARAS_CONFIGURATION_MAX_ARGUMENT];

        /* Engine configuration */
        int engine_period;
        int schedule_mode;
        int default_block_mode;
        char default_block[ARAS_CONFIGURATION_MAX_ARGUMENT];
        int fade_out_time;
        float fade_out_slope;
        int time_signal_mode;
        int time_signal_advance;
        char time_signal_block[ARAS_CONFIGURATION_MAX_ARGUMENT];

        /* Block player configuration */
        char block_player_name[ARAS_CONFIGURATION_MAX_ARGUMENT];
        int block_player_audio_output;
        char block_player_audio_device[ARAS_CONFIGURATION_MAX_ARGUMENT];
        float block_player_volume;
        int block_player_sample_rate;
        int block_player_channels;
        int block_player_video_output;
        char block_player_video_device[ARAS_CONFIGURATION_MAX_ARGUMENT];
        char block_player_video_display[ARAS_CONFIGURATION_MAX_ARGUMENT];
        int block_player_display_resolution[2];

        /* Time signal player configuration */
        char time_signal_player_name[ARAS_CONFIGURATION_MAX_ARGUMENT];
        int time_signal_player_audio_output;
        char time_signal_player_audio_device[ARAS_CONFIGURATION_MAX_ARGUMENT];
        float time_signal_player_volume;
        int time_signal_player_sample_rate;
        int time_signal_player_channels;
        int time_signal_player_video_output;
        char time_signal_player_video_device[ARAS_CONFIGURATION_MAX_ARGUMENT];
        char time_signal_player_video_display[ARAS_CONFIGURATION_MAX_ARGUMENT];
        int time_signal_player_display_resolution[2];

        /* Recorder configuration */
        char recorder_name[ARAS_CONFIGURATION_MAX_ARGUMENT];
        int recorder_input;
        char recorder_device[ARAS_CONFIGURATION_MAX_ARGUMENT];
        int recorder_sample_rate;
        int recorder_channels;
        float recorder_quality;

        /* GUI configuration */
        int gui_period;
};

int aras_configuration_init(struct aras_configuration *configuration);
int aras_configuration_load_file(struct aras_configuration *configuration, char *file);

#endif  /* _ARAS_CONFIGURATION_H */
