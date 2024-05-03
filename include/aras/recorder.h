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
 * the recorder module.
 */

#ifndef _ARAS_RECORDER_H
#define _ARAS_RECORDER_H

#include <gst/gst.h>
#include <aras/configuration.h>

#define ARAS_RECORDER_MAX_NAME      1024
#define ARAS_RECORDER_MAX_URI       1024
#define ARAS_RECORDER_MAX_DEVICE    1024

struct aras_recorder {
        char uri[ARAS_RECORDER_MAX_URI];
        GTimeVal timestamp;
        char name[ARAS_RECORDER_MAX_NAME];
        int input;
        char device[ARAS_RECORDER_MAX_DEVICE];
        GstElement *pipeline;
        GstBus *bus;
        GstElement *source;
        GstElement *converter;
        GstElement *encoder;
        GstElement *muxer;
        GstElement *filesink;
        GstCaps *caps;
};

int aras_recorder_init(struct aras_recorder *recorder, struct aras_configuration *configuration);
void aras_recorder_set_location(struct aras_recorder *player, gchar *uri);
void aras_recorder_set_state_null(struct aras_recorder *recorder);
void aras_recorder_set_state_ready(struct aras_recorder *recorder);
void aras_recorder_set_state_paused(struct aras_recorder *recorder);
void aras_recorder_set_state_playing(struct aras_recorder *recorder);

#endif  /* _ARAS_RECORDER_H */
