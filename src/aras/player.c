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

#include <gst/gst.h>
#include <vlc/vlc.h>
#include <aras/configuration.h>
#include <aras/player.h>

void aras_player_message_check(GstBus *bus)
{
        GstMessage *msg;

        while ((msg = gst_bus_pop(bus)) != NULL)
                gst_message_unref(msg);
}

/**
 * This function is the callback function for a player.
 *
 * @param   bus     Pointer to the bus
 * @param   msg     Pointer to the message
 * @param   data    Pointer to the callback data
 */
gboolean aras_player_callback_block_player_a(GstBus *bus, GstMessage *msg, gpointer data)
{
        struct aras_player *player;

        player = (struct aras_player*)data;

        switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
                gst_element_set_state(player->playbin_a, GST_STATE_NULL);
                break;
        case GST_MESSAGE_ERROR:
                gst_element_set_state(player->playbin_a, GST_STATE_NULL);
        break;
        case GST_MESSAGE_BUFFERING:
                gst_message_parse_buffering(msg, &player->buffer_percent_a);
                if (player->buffer_percent_a < 100)
                        gst_element_set_state(player->playbin_a, GST_STATE_PAUSED);
                else
                        gst_element_set_state(player->playbin_a, GST_STATE_PLAYING);
                break;
        default:
                break;
        }
        return TRUE;
}

/**
 * This function is the callback function for a player.
 *
 * @param   bus     Pointer to the bus
 * @param   msg     Pointer to the message
 * @param   data    Pointer to the callback data
 */
gboolean aras_player_callback_block_player_b(GstBus *bus, GstMessage *msg, gpointer data)
{
        struct aras_player *player;

        player = (struct aras_player*)data;

        switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
                gst_element_set_state(player->playbin_b, GST_STATE_NULL);
                break;
        case GST_MESSAGE_ERROR:
                gst_element_set_state(player->playbin_b, GST_STATE_NULL);
        break;
        case GST_MESSAGE_BUFFERING:
                gst_message_parse_buffering(msg, &player->buffer_percent_b);
                if (player->buffer_percent_b < 100)
                        gst_element_set_state(player->playbin_b, GST_STATE_PAUSED);
                else 
                        gst_element_set_state(player->playbin_b, GST_STATE_PLAYING);
                break;
        default:
                break;
        }
        return TRUE;
}

/**
 * This function is the callback function for a player.
 *
 * @param   bus     Pointer to the bus
 * @param   msg     Pointer to the message
 * @param   data    Pointer to the callback data
 */
gboolean aras_player_callback_time_signal_player_a(GstBus *bus, GstMessage *msg, gpointer data)
{
        struct aras_player *player;

        player = (struct aras_player*)data;

        switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
                gst_element_set_state(player->playbin_a, GST_STATE_NULL);
                break;
        case GST_MESSAGE_ERROR:
                gst_element_set_state(player->playbin_a, GST_STATE_NULL);
        break;
        case GST_MESSAGE_BUFFERING:
                gst_message_parse_buffering(msg, &player->buffer_percent_a);
                if (player->buffer_percent_a < 100)
                        gst_element_set_state(player->playbin_a, GST_STATE_PAUSED);
                else
                        gst_element_set_state(player->playbin_a, GST_STATE_PLAYING);
                break;
        default:
                break;
        }
        return TRUE;
}

/**
 * This function is the callback function for a player.
 *
 * @param   bus     Pointer to the bus
 * @param   msg     Pointer to the message
 * @param   data    Pointer to the callback data
 */
gboolean aras_player_callback_time_signal_player_b(GstBus *bus, GstMessage *msg, gpointer data)
{
        struct aras_player *player;

        player = (struct aras_player*)data;

        switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
                gst_element_set_state(player->playbin_b, GST_STATE_NULL);
                break;
        case GST_MESSAGE_ERROR:
                gst_element_set_state(player->playbin_b, GST_STATE_NULL);
        break;
        case GST_MESSAGE_BUFFERING:
                gst_message_parse_buffering(msg, &player->buffer_percent_b);
                if (player->buffer_percent_b < 100)
                        gst_element_set_state(player->playbin_b, GST_STATE_PAUSED);
                else
                        gst_element_set_state(player->playbin_b, GST_STATE_PLAYING);
                break;
        default:
                break;
        }
        return TRUE;
}

/**
 * This function initializes a video sink in a player structure.
 *
 * @param   name            Pointer to the name string
 * @param   video_output    Value for the video output
 * @param   video_device    Pointer to the video device string
 * @param   video_display   Value for the video display string
 * @param   resolution      Array containing the screen resolution
 * @return  The video sink
 */
GstElement *aras_player_init_video_sink(char *name, int video_output, char *video_device, char *video_display, int resolution[])
{
        GstElement *sink;

        switch (video_output) {
        case ARAS_CONFIGURATION_MODE_VIDEO_AUTO:
                sink = gst_element_factory_make("autovideosink", name);
                break;
        case ARAS_CONFIGURATION_MODE_VIDEO_V4L2:
                sink = gst_element_factory_make("v4l2sink", name);
                g_object_set(G_OBJECT(sink), "device", video_device, NULL);
                g_object_set(G_OBJECT(sink), "overlay-width", resolution[0], NULL);
                g_object_set(G_OBJECT(sink), "overlay-height", resolution[1], NULL);
                break;
        case ARAS_CONFIGURATION_MODE_VIDEO_X:
                sink = gst_element_factory_make("ximagesink", name);
                g_object_set(G_OBJECT(sink), "display", video_display, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_VIDEO_XV:
                sink = gst_element_factory_make("xvimagesink", name);
                g_object_set(G_OBJECT(sink), "device", video_device, NULL);
                g_object_set(G_OBJECT(sink), "display", video_display, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_VIDEO_FB:
                sink = gst_element_factory_make("fbdevsink", name);
                g_object_set(G_OBJECT(sink), "device", video_device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_VIDEO_GL:
                sink = gst_element_factory_make("glimagesink", name);
                break;
        case ARAS_CONFIGURATION_MODE_VIDEO_FILE:
                sink = gst_element_factory_make("filesink", name);
                g_object_set(G_OBJECT(sink), "location", video_device, NULL);
                g_object_set(G_OBJECT(sink), "buffer-mode", 1, NULL);
                g_object_set(G_OBJECT(sink), "buffer-size", 1000000, NULL);
                break;
        default:
                sink = gst_element_factory_make("autovideosink", name);
                break;
        }
        return sink;
}

/**
 * This function initializes a video sink bin to be used as a sink with a
 * playbin element.
 *
 * @param   name            Pointer to the name string
 * @param   video_output    Value for the video output
 * @param   video_device    Pointer to the video device string
 * @param   video_display   Pointer to the video display string
 * @param   resolution      Array containing the screen resolution
 * @return  The video bin
 */
void aras_player_init_video_sink_bin(struct aras_player_sink *sink, char *name, int video_output, char *video_device, char *video_display, int resolution[])
{
        /* Create bin */
        sink->bin = gst_bin_new("video_sink_bin");

        /* Create the elements to be added to the bin */
        sink->convert = gst_element_factory_make("videoconvert", "videoconvert");
        sink->sink = aras_player_init_video_sink(name, video_output, video_device, video_display, resolution);

        /* Add the elements to the bin */
        gst_bin_add_many(GST_BIN(sink->bin), sink->convert, sink->sink, NULL);

        /* Create the capabilities and link elements */
        sink->caps = gst_caps_new_simple("video/x-raw", "width", G_TYPE_INT, resolution[0], "height", G_TYPE_INT, resolution[1], NULL);

        gst_element_link_filtered(sink->convert, sink->sink, sink->caps);
        //gst_caps_unref(sink->caps);

        /* Create the pad in the bin */
        sink->pad = gst_element_get_static_pad(sink->convert, "sink");
        sink->ghost_pad = gst_ghost_pad_new("sink", sink->pad);
        gst_pad_set_active(sink->ghost_pad, TRUE);
        gst_element_add_pad(sink->bin, sink->ghost_pad);
        //gst_object_unref(sink->pad);
}

/**
 * This function returns an audio sink.
 *
 * @param   name            Pointer to the name string
 * @param   audio_output    Value for the audio output
 * @param   audio_device    Pointer to the audio device string
 * @return  The audio sink
 */
GstElement *aras_player_init_audio_sink(char *name, int audio_output, char *audio_device)
{
        GstElement *sink;

        switch (audio_output) {
        case ARAS_CONFIGURATION_MODE_AUDIO_AUTO:
                sink = gst_element_factory_make("autoaudiosink", name);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_PULSEAUDIO:
                sink = gst_element_factory_make("pulsesink", name);
                g_object_set(G_OBJECT(sink), "device", audio_device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_ALSA:
                sink = gst_element_factory_make("alsasink", name);
                g_object_set(G_OBJECT(sink), "device", audio_device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_JACK:
                sink = gst_element_factory_make("jackaudiosink", name);
                g_object_set(G_OBJECT(sink), "server", audio_device, NULL);
                g_object_set(G_OBJECT(sink), "connect", 0, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_OSS:
                sink = gst_element_factory_make("osssink", name);
                g_object_set(G_OBJECT(sink), "device", audio_device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_OSS4:
                sink = gst_element_factory_make("oss4sink", name);
                g_object_set(G_OBJECT(sink), "device", audio_device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_OPENAL:
                sink = gst_element_factory_make("openalsink", name);
                g_object_set(G_OBJECT(sink), "device", audio_device, NULL);
                break;
        case ARAS_CONFIGURATION_MODE_AUDIO_FILE:
                sink = gst_element_factory_make("filesink", name);
                g_object_set(G_OBJECT(sink), "location", audio_device, NULL);
                g_object_set(G_OBJECT(sink), "buffer-mode", 1, NULL);
                g_object_set(G_OBJECT(sink), "buffer-size", 1000000, NULL);
                break;
        default:
                sink = gst_element_factory_make("autoaudiosink", name);
                break;
        }
        return sink;
}

/**
 * This function returns an audio sink bin to be used as an audio sink with a
 * playbin element.
 *
 * @param   sink            Pointer to an aras_player_sink structure
 * @param   name            Pointer to the name string
 * @param   audio_output    Value for the audio output
 * @param   audio_device    Pointer to the audio device string
 * @param   sample_rate     Value for the sample rate
 * @param   channels        Value for the number of channels
 */
void aras_player_init_audio_sink_bin(struct aras_player_sink *sink, char *name, int audio_output, char *audio_device, int sample_rate, int channels)
{
        /* Create bin */
        sink->bin = gst_bin_new("audio_sink_bin");

        /* Create the elements */
        sink->convert = gst_element_factory_make("audioconvert", "audioconvert");
        sink->sink = aras_player_init_audio_sink(name, audio_output, audio_device);

        /* Add the elements to the bin */
        gst_bin_add_many(GST_BIN(sink->bin), sink->convert, sink->sink, NULL);

        /* Create the capabilities and link elements */
        sink->caps = gst_caps_new_simple("audio/x-raw",
                                                 "channels",
                                                 G_TYPE_INT,
                                                 channels,
                                                 "rate",
                                                 G_TYPE_INT,
                                                 sample_rate,
                                                 NULL);
        gst_element_link_filtered(sink->convert, sink->sink, sink->caps);
        //gst_caps_unref(sink->caps);

        /* Create the pad in the bin */
        sink->pad = gst_element_get_static_pad(sink->convert, "sink");
        sink->ghost_pad = gst_ghost_pad_new("sink", sink->pad);
        gst_pad_set_active(sink->ghost_pad, TRUE);
        gst_element_add_pad(sink->bin, sink->ghost_pad);
        //gst_object_unref(sink->pad);
}

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

        /* Create player */
        player->instance = libvlc_new(0, NULL);
        player->player_a = libvlc_media_player_new(player->instance);
        player->player_b = libvlc_media_player_new(player->instance);

        libvlc_audio_output_set(player->player_a, "jack");
        libvlc_audio_output_set(player->player_b, "jack");

        /* Create playbin_a and playbin_b */
        //player->playbin_a = gst_element_factory_make("playbin", "deck_a");
        //player->playbin_b = gst_element_factory_make("playbin", "deck_b");

        /* Create the audio sink bin and the video sink bin */
        // aras_player_init_audio_sink_bin(&player->audio_sink_a,
        //                                 configuration->block_player_name,
        //                                 configuration->block_player_audio_output,
        //                                 configuration->block_player_audio_device,
        //                                 configuration->block_player_sample_rate,
        //                                 configuration->block_player_channels);
        // aras_player_init_video_sink_bin(&player->video_sink_a,
        //                                 configuration->block_player_name,
        //                                 configuration->block_player_video_output,
        //                                 configuration->block_player_video_device,
        //                                 configuration->block_player_video_display,
        //                                 configuration->block_player_display_resolution);
        // aras_player_init_audio_sink_bin(&player->audio_sink_b,
        //                                 configuration->block_player_name,
        //                                 configuration->block_player_audio_output,
        //                                 configuration->block_player_audio_device,
        //                                 configuration->block_player_sample_rate,
        //                                 configuration->block_player_channels);
        // aras_player_init_video_sink_bin(&player->video_sink_b,
        //                                 configuration->block_player_name,
        //                                 configuration->block_player_video_output,
        //                                 configuration->block_player_video_device,
        //                                 configuration->block_player_video_display,
        //                                 configuration->block_player_display_resolution);

        // g_object_set(player->playbin_a, "audio-sink", player->audio_sink_a.bin, NULL);
        // g_object_set(player->playbin_a, "video-sink", player->video_sink_a.bin, NULL);
        // g_object_set(player->playbin_b, "audio-sink", player->audio_sink_b.bin, NULL);
        // g_object_set(player->playbin_b, "video-sink", player->video_sink_b.bin, NULL);

        // /* Enable property async-handling */
        // g_object_set(player->playbin_a, "async-handling", TRUE, NULL);
        // g_object_set(player->playbin_b, "async-handling", TRUE, NULL);

        // /* Create the buses */
        // player->bus_a = gst_pipeline_get_bus(GST_PIPELINE(player->playbin_a));
        // player->bus_b = gst_pipeline_get_bus(GST_PIPELINE(player->playbin_b));
        // gst_bus_add_watch(player->bus_a, aras_player_callback_block_player_a, player);
        // gst_bus_add_watch(player->bus_b, aras_player_callback_block_player_b, player);
        // //gst_object_unref(player->bus_a);
        // //gst_object_unref(player->bus_b);

        /* Set the volume */
        //g_object_set(player->playbin_a, "volume", player->volume_a, NULL);
        //g_object_set(player->playbin_b, "volume", player->volume_b, NULL);

        /* Set state to GST_STATE_NULL */
        //gst_element_set_state(player->playbin_a, GST_STATE_NULL);
        //gst_element_set_state(player->playbin_b, GST_STATE_NULL);
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

        /* Create playbin_a and playbin_b */
        player->playbin_a = gst_element_factory_make("playbin", "deck_a");
        player->playbin_b = gst_element_factory_make("playbin", "deck_b");

        /* Create the audio sink bin and the video sink bin */
        aras_player_init_audio_sink_bin(&player->audio_sink_a, configuration->time_signal_player_name,
                                        configuration->time_signal_player_audio_output,
                                        configuration->time_signal_player_audio_device,
                                        configuration->time_signal_player_sample_rate,
                                        configuration->time_signal_player_channels);
        aras_player_init_video_sink_bin(&player->video_sink_a, configuration->time_signal_player_name,
                                        configuration->time_signal_player_video_output,
                                        configuration->time_signal_player_video_device,
                                        configuration->time_signal_player_video_display,
                                        configuration->time_signal_player_display_resolution);
        aras_player_init_audio_sink_bin(&player->audio_sink_b, configuration->time_signal_player_name,
                                        configuration->time_signal_player_audio_output,
                                        configuration->time_signal_player_audio_device,
                                        configuration->time_signal_player_sample_rate,
                                        configuration->time_signal_player_channels);
        aras_player_init_video_sink_bin(&player->video_sink_b, configuration->time_signal_player_name,
                                        configuration->time_signal_player_video_output,
                                        configuration->time_signal_player_video_device,
                                        configuration->time_signal_player_video_display,
                                        configuration->time_signal_player_display_resolution);

        /* Bind the audio sink bin and the video sink bin to the playbin */
        g_object_set(player->playbin_a, "audio-sink", player->audio_sink_a.bin, NULL);
        g_object_set(player->playbin_a, "video-sink", player->video_sink_a.bin, NULL);
        g_object_set(player->playbin_b, "audio-sink", player->audio_sink_b.bin, NULL);
        g_object_set(player->playbin_b, "video-sink", player->video_sink_b.bin, NULL);
        
        /* Enable property async-handling */
        g_object_set(player->playbin_a, "async-handling", TRUE, NULL);
        g_object_set(player->playbin_b, "async-handling", TRUE, NULL);

        /* Create the buses */
        player->bus_a = gst_pipeline_get_bus(GST_PIPELINE(player->playbin_a));
        player->bus_b = gst_pipeline_get_bus(GST_PIPELINE(player->playbin_b));
        gst_bus_add_watch(player->bus_a, aras_player_callback_time_signal_player_a, player);
        gst_bus_add_watch(player->bus_b, aras_player_callback_time_signal_player_b, player);
        //gst_object_unref(player->bus_a);
        //gst_object_unref(player->bus_b);

        /* Set the volume */
        g_object_set(player->playbin_a, "volume", player->volume_a, NULL);
        g_object_set(player->playbin_b, "volume", player->volume_b, NULL);

        /* Set state to GST_STATE_NULL */
        gst_element_set_state(player->playbin_a, GST_STATE_NULL);
        gst_element_set_state(player->playbin_b, GST_STATE_NULL);

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
                libvlc_audio_set_volume(player->player_a, 1e2 * volume);
                //g_object_set(player->playbin_a, "volume", player->volume_a, NULL);
                break;
        case ARAS_PLAYER_UNIT_B:
                player->volume_b = volume;
                libvlc_audio_set_volume(player->player_b, 1e2 * volume);
                //g_object_set(player->playbin_b, "volume", player->volume_b, NULL);
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
                libvlc_audio_set_volume(player->player_a, 1e2 * player->volume_a);
                //g_object_set(player->playbin_a, "volume", player->volume_a, NULL);
                break;
        case ARAS_PLAYER_UNIT_B:
                player->volume_b += slope * (limit - player->volume_b);
                libvlc_audio_set_volume(player->player_b, 1e2 * player->volume_b);
                //g_object_set(player->playbin_b, "volume", player->volume_b, NULL);
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
void aras_player_set_uri(struct aras_player *player, int unit, gchar *uri)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                libvlc_media_release(player->media_a);
                player->media_a = libvlc_media_new_location(player->instance, uri);
                libvlc_media_player_set_media(player->player_a, player->media_a);
                //g_object_set(player->playbin_a, "uri", uri, NULL);
                break;
        case ARAS_PLAYER_UNIT_B:
                libvlc_media_release(player->media_b);
                player->media_b = libvlc_media_new_location(player->instance, uri);
                libvlc_media_player_set_media(player->player_b, player->media_b);
                //g_object_set(player->playbin_b, "uri", uri, NULL);
                break;
        default:
                break;
        }
}

/**
 * This function sets the player state to GST_STATE_NULL.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 */
void aras_player_set_state_null(struct aras_player *player, int unit)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                libvlc_media_player_stop(player->player_a);
                //gst_element_set_state(player->playbin_a, GST_STATE_NULL);
                break;
        case ARAS_PLAYER_UNIT_B:
                libvlc_media_player_stop(player->player_a);
                //gst_element_set_state(player->playbin_b, GST_STATE_NULL);
                break;
        default:
                break;
        }
}

/**
 * This function sets the player state to GST_STATE_READY.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 */
void aras_player_set_state_ready(struct aras_player *player, int unit)
{
        GstState state;
        GstState pending;
        GstMessage *msg;

        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                libvlc_media_player_stop(player->player_a);
                /*do {
                        gst_element_get_state(player->playbin_a, &state, &pending, GST_CLOCK_TIME_NONE);
                        g_usleep(1000);
                } while (pending != GST_STATE_VOID_PENDING);
                gst_element_set_state(player->playbin_a, GST_STATE_READY);
                switch (gst_element_get_state(player->playbin_a, &state, &pending, GST_CLOCK_TIME_NONE)) {
                case GST_STATE_CHANGE_SUCCESS:
                        break;
                case GST_STATE_CHANGE_ASYNC:
                        fprintf(stderr, "ARAS: ASYNC: State change to GST_STATE_READY: %d %d ", state, pending);
                        msg = gst_bus_timed_pop_filtered(player->bus_a, GST_CLOCK_TIME_NONE, GST_MESSAGE_STATE_CHANGED);
                        gst_element_get_state(player->playbin_a, &state, &pending, GST_CLOCK_TIME_NONE);
                        fprintf(stderr, "END: %d %d\n", state, pending);
                        gst_message_unref(msg);
                        break;
                case GST_STATE_CHANGE_FAILURE:
                        fprintf(stderr, "ARAS: FAILURE: State change to GST_STATE_READY: %d %d ", state, pending);
                        do {
                                gst_element_set_state(player->playbin_a, GST_STATE_NULL);
                                g_usleep(100000);
                                gst_element_get_state(player->playbin_a, &state, &pending, GST_CLOCK_TIME_NONE);
                        } while (state != GST_STATE_NULL);
                        fprintf(stderr, "END: %d %d\n", state, pending);
                        break;
                default:
                        gst_element_set_state(player->playbin_a, GST_STATE_NULL);
                        break;
                }
                aras_player_message_check(player->bus_a);*/
                break;
        case ARAS_PLAYER_UNIT_B:
                libvlc_media_player_stop(player->player_b);
                /*do {
                        gst_element_get_state(player->playbin_b, &state, &pending, GST_CLOCK_TIME_NONE);
                        g_usleep(1000);
                } while (pending != GST_STATE_VOID_PENDING);
                gst_element_set_state(player->playbin_b, GST_STATE_READY);
                switch (gst_element_get_state(player->playbin_b, &state, &pending, GST_CLOCK_TIME_NONE)) {
                case GST_STATE_CHANGE_SUCCESS:
                        break;
                case GST_STATE_CHANGE_ASYNC:
                        fprintf(stderr, "ARAS: ASYNC: State change to GST_STATE_READY: %d %d ", state, pending);
                        msg = gst_bus_timed_pop_filtered(player->bus_b, GST_CLOCK_TIME_NONE, GST_MESSAGE_STATE_CHANGED);
                        gst_element_get_state(player->playbin_b, &state, &pending, GST_CLOCK_TIME_NONE);
                        fprintf(stderr, "END: %d %d\n", state, pending);
                        gst_message_unref(msg);
                        break;
                case GST_STATE_CHANGE_FAILURE:
                        fprintf(stderr, "ARAS: FAILURE: State change to GST_STATE_READY: %d %d ", state, pending);
                        do {
                                gst_element_set_state(player->playbin_b, GST_STATE_NULL);
                                g_usleep(100000);
                                gst_element_get_state(player->playbin_b, &state, &pending, GST_CLOCK_TIME_NONE);
                        } while (state != GST_STATE_NULL);
                        fprintf(stderr, "END: %d %d\n", state, pending);
                        break;
                default:
                        gst_element_set_state(player->playbin_b, GST_STATE_NULL);
                        break;
                }
                aras_player_message_check(player->bus_b);*/
                break;
        default:
                break;
        }
}

/**
 * This function sets the player state to GST_STATE_PAUSED.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 */
void aras_player_set_state_paused(struct aras_player *player, int unit)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                gst_element_set_state(player->playbin_a, GST_STATE_PAUSED);
                break;
        case ARAS_PLAYER_UNIT_B:
                gst_element_set_state(player->playbin_b, GST_STATE_PAUSED);
                break;
        default:
                break;
        }
}

/**
 * This function sets the player state to GST_STATE_PLAYING.
 *
 * @param   player  Pointer to the player
 * @param   unit    The identifier of the player unit
 */
void aras_player_set_state_playing(struct aras_player *player, int unit)
{
        GstState state;
        GstState pending;
        GstMessage *msg;

        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                     libvlc_media_player_play(player->player_a);
                /*
                do {
                        gst_element_get_state(player->playbin_a, &state, &pending, GST_CLOCK_TIME_NONE);
                        g_usleep(1000);
                } while (pending != GST_STATE_VOID_PENDING);
                gst_element_set_state(player->playbin_a, GST_STATE_PLAYING);
                switch (gst_element_get_state(player->playbin_a, &state, &pending, GST_CLOCK_TIME_NONE)) {
                case GST_STATE_CHANGE_SUCCESS:
                        break;
                case GST_STATE_CHANGE_ASYNC:
                        fprintf(stderr, "ARAS: ASYNC: State change to GST_STATE_PLAYING: %d %d ", state, pending);
                        msg = gst_bus_timed_pop_filtered(player->bus_a, GST_CLOCK_TIME_NONE, GST_MESSAGE_STATE_CHANGED);
                        gst_element_get_state(player->playbin_a, &state, &pending, GST_CLOCK_TIME_NONE);
                        fprintf(stderr, "END: %d %d\n", state, pending);
                        gst_message_unref(msg);
                        break;
                case GST_STATE_CHANGE_FAILURE:
                        fprintf(stderr, "ARAS: FAILURE: State change to GST_STATE_PLAYING: %d %d ", state, pending);
                        do {
                                gst_element_set_state(player->playbin_a, GST_STATE_NULL);
                                g_usleep(100000);
                                gst_element_get_state(player->playbin_a, &state, &pending, GST_CLOCK_TIME_NONE);
                        } while (state != GST_STATE_NULL);
                        fprintf(stderr, "END: %d %d\n", state, pending);
                        break;
                default:
                        gst_element_set_state(player->playbin_a, GST_STATE_NULL);
                        break;
                }
                aras_player_message_check(player->bus_a);*/
                break;
        case ARAS_PLAYER_UNIT_B:
                libvlc_media_player_play(player->player_b);
                /*
                do {
                        gst_element_get_state(player->playbin_b, &state, &pending, GST_CLOCK_TIME_NONE);
                        g_usleep(1000);
                } while (pending != GST_STATE_VOID_PENDING);
                gst_element_set_state(player->playbin_b, GST_STATE_PLAYING);
                switch (gst_element_get_state(player->playbin_b, &state, &pending, GST_CLOCK_TIME_NONE)) {
                case GST_STATE_CHANGE_SUCCESS:
                        break;
                case GST_STATE_CHANGE_ASYNC:
                        fprintf(stderr, "ARAS: ASYNC: State change to GST_STATE_PLAYING: %d %d ", state, pending);
                        msg = gst_bus_timed_pop_filtered(player->bus_b, GST_CLOCK_TIME_NONE, GST_MESSAGE_STATE_CHANGED);
                        gst_element_get_state(player->playbin_b, &state, &pending, GST_CLOCK_TIME_NONE);
                        fprintf(stderr, "END: %d %d\n", state, pending);
                        gst_message_unref(msg);
                        break;
                case GST_STATE_CHANGE_FAILURE:
                        fprintf(stderr, "ARAS: FAILURE: State change to GST_STATE_PLAYING: %d %d ", state, pending);
                        do {
                                gst_element_set_state(player->playbin_b, GST_STATE_NULL);
                                g_usleep(100000);
                                gst_element_get_state(player->playbin_b, &state, &pending, GST_CLOCK_TIME_NONE);
                        } while (state != GST_STATE_NULL);
                        fprintf(stderr, "END: %d %d\n", state, pending);
                        break;
                default:
                        gst_element_set_state(player->playbin_b, GST_STATE_NULL);
                        break;
                }
                aras_player_message_check(player->bus_b);*/
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
void aras_player_get_state(struct aras_player *player, int unit, libvlc_state_t *state)
{
        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                *state = libvlc_media_player_get_state(player->player_a);
                //gst_element_get_state(player->playbin_a, state, NULL, GST_CLOCK_TIME_NONE);
                break;
        case ARAS_PLAYER_UNIT_B:
                *state = libvlc_media_player_get_state(player->player_b);
                //gst_element_get_state(player->playbin_b, state, NULL, GST_CLOCK_TIME_NONE);
                break;
        default:
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
        GstState state;
        gint64 duration;

        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                duration = libvlc_media_player_get_length(player->player_a);
                /*gst_element_get_state(player->playbin_a, &state, NULL, GST_CLOCK_TIME_NONE);
                if (state == GST_STATE_PLAYING)
                        gst_element_query_duration(player->playbin_a, GST_FORMAT_TIME, &duration);
                else
                        duration = 0;*/
                break;
        case ARAS_PLAYER_UNIT_B:
                duration = libvlc_media_player_get_length(player->player_b);
                /*gst_element_get_state(player->playbin_b, &state, NULL, GST_CLOCK_TIME_NONE);
                if (state == GST_STATE_PLAYING)
                        gst_element_query_duration(player->playbin_b, GST_FORMAT_TIME, &duration);
                else
                        duration = 0;*/
                break;
        default:
                        duration = 0;
                break;
        }

        //return (long int)(lldiv(duration, 1000000).quot);
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
        GstState state;
        gint64 position;

        switch (unit) {
        case ARAS_PLAYER_UNIT_A:
                position = libvlc_media_player_get_time(player->player_a);
                /*gst_element_get_state(player->playbin_a, &state, NULL, GST_CLOCK_TIME_NONE);
                if (state == GST_STATE_PLAYING)
                        gst_element_query_position(player->playbin_a, GST_FORMAT_TIME, &position);
                else
                        position = 0;*/
                break;
        case ARAS_PLAYER_UNIT_B:
                position = libvlc_media_player_get_time(player->player_b);
                /*gst_element_get_state(player->playbin_b, &state, NULL, GST_CLOCK_TIME_NONE);
                if (state == GST_STATE_PLAYING)
                        gst_element_query_position(player->playbin_b, GST_FORMAT_TIME, &position);
                else
                        position = 0;*/
                break;
        default:
                        position = 0;
                break;
        }

        //return (long int)(lldiv(position, 1000000).quot);
        return position;
}
