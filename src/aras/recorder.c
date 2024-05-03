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
 * Source file for the ARAS Radio Automation System. Functions for the recorder
 * module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>
#include <aras/configuration.h>
#include <aras/recorder.h>

/**
 * This function is the callback function for a recorder.
 *
 * @param   bus     Pointer to the bus
 * @param   msg     Pointer to the message
 * @param   data    Pointer to the callback data
 */
gboolean aras_recorder_callback(GstBus *bus, GstMessage *msg, gpointer data)
{
        struct aras_recorder *recorder;

        recorder = (struct aras_recorder*)data;

        switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
                gst_element_set_state(GST_ELEMENT(recorder->pipeline), GST_STATE_NULL);
                break;
        case GST_MESSAGE_ERROR:
                gst_element_set_state(GST_ELEMENT(recorder->pipeline), GST_STATE_NULL);
                break;
        default:
                break;
  }
  return TRUE;
}

/**
 * This function initializes an audio source in a recorder structure.
 *
 * @param   recorder    Pointer to the recorder
 * @param   name        Pointer to the name string
 * @param   input       Value for the audio input
 * @param   device      Pointer to the audio device string
 */
void aras_recorder_init_source(struct aras_recorder *recorder, char *name, int input, char *device)
{
        switch (input) {
        case ARAS_CONFIGURATION_MODE_AUDIO_AUTO:
                recorder->source = gst_element_factory_make("autoaudiosrc", name);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_PULSEAUDIO:
                recorder->source = gst_element_factory_make("pulsesrc", name);
                g_object_set(G_OBJECT(recorder->source), "device", device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_ALSA:
                recorder->source = gst_element_factory_make("alsasrc", name);
                g_object_set(G_OBJECT(recorder->source), "device", device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_JACK:
                recorder->source = gst_element_factory_make("jackaudiosrc", name);
                g_object_set(G_OBJECT(recorder->source), "server", device, NULL);
                g_object_set(G_OBJECT(recorder->source), "connect", 0, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_OSS:
                recorder->source = gst_element_factory_make("osssrc", name);
                g_object_set(G_OBJECT(recorder->source), "device", device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_OSS4:
                recorder->source = gst_element_factory_make("oss4src", name);
                g_object_set(G_OBJECT(recorder->source), "device", device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_OPENAL:
                recorder->source = gst_element_factory_make("openalsrc", name);
                g_object_set(G_OBJECT(recorder->source), "device", device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_FILE:
                recorder->source = gst_element_factory_make("filesrc", name);
                g_object_set(G_OBJECT(recorder->source), "location", device, NULL);
                g_object_set(G_OBJECT(recorder->source), "buffer-mode", 1, NULL);
                g_object_set(G_OBJECT(recorder->source), "buffer-size", 1000000, NULL);
                break;
        default:
                recorder->source = gst_element_factory_make("autoaudiosink", name);
                break;
        }
}

/**
 * This function initializes a recorder structure.
 *
 * @param   recorder        Pointer to the recorder
 * @param   configuration   Pointer to the configuration
 *
 * @return  This function always return 0
 */
int aras_recorder_init(struct aras_recorder *recorder, struct aras_configuration *configuration)
{
        g_get_current_time(&(recorder->timestamp));

        recorder->pipeline = gst_pipeline_new(configuration->recorder_name);
        aras_recorder_init_source(recorder, configuration->recorder_name, configuration->recorder_input, configuration->recorder_device);
        recorder->converter = gst_element_factory_make("audioconvert", "audio-converter");
        recorder->encoder = gst_element_factory_make("vorbisenc", "vorbis-encoder");
        recorder->muxer = gst_element_factory_make("oggmux", "ogg-muxer");
        recorder->filesink = gst_element_factory_make("filesink", "audio-sink");

        if (!recorder->pipeline || !recorder->source ||
            !recorder->converter || !recorder->encoder ||
            !recorder->muxer || !recorder->filesink)
                return -1;

        /* Setting the filename and the quality */
        g_object_set(G_OBJECT(recorder->filesink), "location", "output.ogg", NULL);
        g_object_set(G_OBJECT(recorder->encoder), "quality", configuration->recorder_quality, NULL);

        /* Setting bus */
        recorder->bus = gst_pipeline_get_bus(GST_PIPELINE(recorder->pipeline));
        gst_bus_add_watch(recorder->bus, aras_recorder_callback, (gpointer)recorder);
        gst_object_unref(recorder->bus);

        /* Adding */
        gst_bin_add_many(GST_BIN(recorder->pipeline), recorder->source, recorder->converter, recorder->encoder, recorder->muxer, recorder->filesink, NULL);

        /* Caps */
        recorder->caps = gst_caps_new_simple("audio/x-raw",
                                             "channels",
                                             G_TYPE_INT,
                                             configuration->recorder_channels,
                                             "rate",
                                             G_TYPE_INT,
                                             configuration->recorder_sample_rate,
                                             NULL);

        /* Linking */
        gst_element_link_filtered(recorder->source, recorder->converter, recorder->caps);
        gst_element_link_many(recorder->converter, recorder->encoder, recorder->muxer, recorder->filesink, NULL);

        /* Recording? No yet */
        gst_element_set_state(recorder->pipeline, GST_STATE_NULL);

        return 0;
}

/**
 * This function sets the URI in a recorder.
 *
 * @param   recorder    Pointer to the recorder
 * @param   uri         A pointer to the URI string
 */
void aras_recorder_set_location(struct aras_recorder *recorder, gchar *location)
{
        g_object_set(G_OBJECT(recorder->filesink), "location", location, NULL);
}

/**
 * This function sets the recorder state to GST_STATE_NULL.
 *
 * @param   recorder    Pointer to the recorder
 */
void aras_recorder_set_state_null(struct aras_recorder *recorder)
{
        gst_element_set_state(recorder->pipeline, GST_STATE_NULL);
}

/**
 * This function sets the recorder state to GST_STATE_READY.
 *
 * @param   recorder    Pointer to the recorder
 */
void aras_recorder_set_state_ready(struct aras_recorder *recorder)
{
        gst_element_set_state(recorder->pipeline, GST_STATE_READY);
}

/**
 * This function sets the recorder state to GST_STATE_PAUSED.
 *
 * @param   recorder    Pointer to the recorder
 */
void aras_recorder_set_state_paused(struct aras_recorder *recorder)
{
        gst_element_set_state(recorder->pipeline, GST_STATE_PAUSED);
}

/**
 * This function sets the recorder state to GST_STATE_PLAYING.
 *
 * @param   recorder    Pointer to the recorder
 */
void aras_recorder_set_state_playing(struct aras_recorder *recorder)
{
        gst_element_set_state(recorder->pipeline, GST_STATE_PLAYING);
}
