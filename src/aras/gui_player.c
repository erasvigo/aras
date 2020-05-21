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
 * Source file for the ARAS Radio Automation System. Functions for the GUI
 * player module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <gtk/gtk.h>
#include <aras/time.h>
#include <aras/configuration.h>
#include <aras/schedule.h>
#include <aras/block.h>
#include <aras/engine.h>
#include <aras/player.h>
#include <aras/gui_player.h>

/**
 * This function closes the application.
 */
void aras_gui_player_quit()
{
        gtk_main_quit();
}

/**
 * This function is the callback function for a window hide event.
 *
 * @param   widget  Pointer to the widget
 * @param   event   Pointer to the event
 * @param   gui     Pointer to the gui
 */
void aras_gui_player_callback_window_hide(GtkWidget *widget, GdkEvent *event, struct aras_gui_player *gui)
{
        gtk_widget_hide(gui->window);
}

/**
 * This function is the callback function for the about dialog.
 *
 * @param   widget  Pointer to the widget
 * @param   gui     Pointer to the gui
 */
void aras_gui_player_callback_about(GtkWidget *widget, struct aras_gui_player *gui)
{
        gchar *artists[] = {"Erasmo Alonso Iglesias",
                            NULL};
        gchar *authors[] = {"Erasmo Alonso Iglesias",
                            NULL};
        gchar *comments = ARAS_GUI_PLAYER_COMMENTS;
        gchar *copyright = ARAS_GUI_PLAYER_COPYRIGHT;
        gchar *documenters[] = {"Erasmo Alonso Iglesias",
                                "Henrik Enblom",
                                "Martin Zelaia",
                                NULL};
        gchar *license = ARAS_GUI_PLAYER_LICENSE;
        GtkImage *logo_image;
        GdkPixbuf *logo;
        gchar *program_name = ARAS_GUI_PLAYER_PROGRAM_NAME;
        gchar *version = ARAS_GUI_PLAYER_VERSION;
        gchar *website = ARAS_GUI_PLAYER_WEBSITE;
        gchar *website_label = ARAS_GUI_PLAYER_WEBSITE_LABEL;

        logo_image = (GtkImage*)gtk_image_new_from_file(ARAS_GUI_PLAYER_PATH_ICON);
        logo = gtk_image_get_pixbuf(logo_image);

        gtk_show_about_dialog((GtkWindow*)gui->window,
                              "artists", artists,
                              "authors", authors,
                              "comments", comments,
                              "copyright", copyright,
                              "documenters", documenters,
                              "license", license,
                              "logo", logo,
                              "program-name", program_name,
                              "version", version,
                              "website", website,
                              "website-label", website_label, NULL);
}

/**
 * This function is the callback function for the quit dialog.
 *
 * @param   widget  Pointer to the widget
 * @param   gui     Pointer to the gui
 */
void aras_gui_player_callback_quit_dialog(GtkWidget *widget, struct aras_gui_player *gui)
{
        gint result;

        if (gui->quit_dialog_count != 0)
                return;

        gui->quit_dialog_count++;
        gui->quit_dialog = gtk_message_dialog_new_with_markup((GtkWindow*)gui->window,
                                                                GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
                                                                GTK_BUTTONS_YES_NO,
                                                                ARAS_GUI_PLAYER_DIALOG_QUIT);
        result = gtk_dialog_run(GTK_DIALOG(gui->quit_dialog));
        switch (result) {
        case GTK_RESPONSE_YES:
                aras_gui_player_quit();
                break;
        case GTK_RESPONSE_NO:
                gui->quit_dialog_count = 0;
                break;
        default:
                break;
        }
        gtk_widget_destroy(gui->quit_dialog);
        return;
}

/**
 * This function is the callback function for the status icon switch.
 *
 * @param   status_icon     Pointer to the status icon
 * @param   gui             Pointer to the gui
 */
void aras_gui_player_callback_status_icon_switch(GtkStatusIcon *status_icon, struct aras_gui_player *gui)
{
        if (gtk_widget_get_visible(gui->window))
                gtk_widget_hide(gui->window);
        else
                gtk_widget_show_all(gui->window);
}

/**
 * This function is the callback function for the status icon menu.
 *
 * @param   status_icon     Pointer to the status icon
 * @param   button          Button identifier
 * @param   activate_time   Activate time value
 * @param   gui             Pointer to the gui
 */
void aras_gui_player_callback_status_icon_menu(GtkStatusIcon *status_icon, guint button, guint activate_time, struct aras_gui_player *gui)
{
        GtkWidget *menu;
        GtkWidget *item;

        menu = gtk_menu_new();

        if (gtk_widget_get_visible(gui->window))
                item = gtk_menu_item_new_with_label("Hide ARAS Player");
        else
                item = gtk_menu_item_new_with_label("Show ARAS Player");

        g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(aras_gui_player_callback_status_icon_switch), gui);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

        item = gtk_menu_item_new_with_label("About...");
        g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(aras_gui_player_callback_about), gui);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

        item = gtk_separator_menu_item_new();
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

        item = gtk_menu_item_new_with_label("Quit");
        g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(aras_gui_player_callback_quit_dialog), gui);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

        gtk_widget_show_all(menu);
        gtk_menu_popup((GtkMenu*)menu, NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}

/**
 * This function is the callback function for the backward button. It attempts
 * to set the state ARAS_ENGINE_STATE_BLOCK_PLAYER_PLAY_PREVIOUS with a time
 * limit according to the configuration fade_out_time field.
 *
 * @param   widget  Pointer to the widget
 * @param   event   Pointer to the event
 * @param   data    Pointer to the callback data
 */
void aras_gui_player_callback_button_backward(GtkWidget *widget, struct aras_gui_player_callback *data)
{
        if (data->engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD || data->engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT)
                aras_engine_set_state(data->engine, ARAS_ENGINE_STATE_PLAY_PREVIOUS, data->configuration->fade_out_time);
}

/**
 * This function is the callback function for the repeat button. It attempts to
 * set the state ARAS_ENGINE_STATE_BLOCK_PLAYER_PLAY_CURRENT with a time limit
 * according to the configuration fade_out_time field.
 *
 * @param   widget  Pointer to the widget
 * @param   event   Pointer to the event
 * @param   data    Pointer to the callback data
 */
void aras_gui_player_callback_button_repeat(GtkWidget *widget, struct aras_gui_player_callback *data)
{
        if (data->engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD || data->engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT)
                aras_engine_set_state(data->engine, ARAS_ENGINE_STATE_PLAY_CURRENT, data->configuration->fade_out_time);
}

/**
 * This function is the callback function for the forward button. It attempts to
 * set the state ARAS_ENGINE_STATE_BLOCK_PLAYER_PLAY_NEXT with a time limit
 * according to the configuration fade_out_time field.
 *
 * @param   widget  Pointer to the widget
 * @param   event   Pointer to the event
 * @param   data    Pointer to the callback data
 */
void aras_gui_player_callback_button_forward(GtkWidget *widget, struct aras_gui_player_callback *data)
{
        if (data->engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD || data->engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT)
                aras_engine_set_state(data->engine, ARAS_ENGINE_STATE_PLAY_NEXT, data->configuration->fade_out_time);
}

/**
 * This function is the callback function for the eject button. It attempts to
 * set the state ARAS_ENGINE_STATE_BLOCK_PLAYER_PLAY_DEFAULT with a time
 * limit according to the configuration fade_out_time field.
 *
 * @param   widget  Pointer to the widget
 * @param   event   Pointer to the event
 * @param   data    Pointer to the callback data
 */
void aras_gui_player_callback_button_eject(GtkWidget *widget, struct aras_gui_player_callback *data)
{
        if (data->engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD || data->engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT)
                aras_engine_set_state(data->engine, ARAS_ENGINE_STATE_PLAY_DEFAULT, data->configuration->fade_out_time);
}

/**
 * This function is called periodically and updates the time label in the time
 * area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_update_time(struct aras_gui_player *gui)
{
        struct timeval tv;
        struct timezone tz;
        struct tm tm;
        char buffer_day[ARAS_GUI_PLAYER_LABEL_MAX];
        char buffer_time[ARAS_GUI_PLAYER_LABEL_MAX];

        gettimeofday(&tv, &tz);
        localtime_r(&tv.tv_sec, &tm);

        strftime(buffer_day, sizeof(buffer_day), "%A %d %B %Y", &tm);
        strftime(buffer_time, sizeof(buffer_time), "%H:%M:%S", &tm);

        snprintf(gui->label_time_str, sizeof(gui->label_time_str),
                 "<span size = \"medium\" weight = \"normal\">%s\n</span><span size = \"xx-large\" weight = \"bold\">%s</span>",
                 buffer_day,
                 buffer_time);

        gtk_label_set_markup(GTK_LABEL(gui->label_time), gui->label_time_str);
}

/**
 * This function is called periodically and updates the next block label in the
 * block area.
 *
 * @param   gui         Pointer to the gui structure
 * @param   schedule    Pointer to the schedule structure
 */
void aras_gui_player_update_block_next_label(struct aras_gui_player *gui, struct aras_schedule *schedule)
{
        struct aras_schedule_node *next_schedule_node;
        long int time;
        int hours;
        int minutes;
        int seconds;

        if ((next_schedule_node = aras_schedule_seek_node_next(schedule, aras_time_current())) == NULL)
                return;

        time = aras_time_difference(next_schedule_node->time, aras_time_current());
        aras_time_convert(time, &hours, &minutes, &seconds);

        snprintf(gui->block_next_str, sizeof(gui->block_next_str),
                 "Next block\n<span size = \"large\" weight = \"bold\">%s</span>\n<span size = \"large\" weight = \"bold\">%.2d:%.2d:%.2d</span> to start",
                 next_schedule_node->block_name,
                 hours,
                 minutes,
                 seconds);

        gtk_label_set_markup(GTK_LABEL(gui->block_next_label), gui->block_next_str);
}

/**
 * This function is called periodically and updates the current block label in
 * the block area.
 *
 * @param   gui         Pointer to the gui structure
 * @param   schedule    Pointer to the schedule structure
 */
void aras_gui_player_update_block_current_label(struct aras_gui_player *gui, struct aras_schedule *schedule)
{
        struct aras_schedule_node *current_schedule_node;
        long int time;
        int hours;
        int minutes;
        int seconds;

        if ((current_schedule_node = aras_schedule_seek_node_current(schedule, aras_time_current())) == NULL)
                return;

        time = aras_time_difference(aras_time_current(), current_schedule_node->time);
        aras_time_convert(time, &hours, &minutes, &seconds);

        snprintf(gui->block_current_str,
                 sizeof(gui->block_current_str),
                 "Current block\n<span size = \"large\" weight = \"bold\">%s</span>\n<span size = \"large\" weight = \"bold\">%.2d:%.2d:%.2d</span> elapsed",
                 current_schedule_node->block_name,
                 hours,
                 minutes,
                 seconds);

        gtk_label_set_markup(GTK_LABEL(gui->block_current_label), gui->block_current_str);
}

/**
 * This function is called periodically and updates the progress bar in the
 * block area.
 *
 * @param   gui         Pointer to the gui structure
 * @param   schedule    Pointer to the schedule structure
 */
void aras_gui_player_update_block_progress_bar(struct aras_gui_player *gui, struct aras_schedule *schedule)
{
        struct aras_schedule_node *current_schedule_node;
        struct aras_schedule_node *next_schedule_node;
        long int duration;
        long int position;
        int duration_hours;
        int duration_minutes;
        int duration_seconds;
        int position_hours;
        int position_minutes;
        int position_seconds;
        float fraction;

        if ((current_schedule_node = aras_schedule_seek_node_current(schedule, aras_time_current())) == NULL)
                return;

        if ((next_schedule_node = aras_schedule_seek_node_next(schedule, aras_time_current())) == NULL)
                return;

        /* Get duration and position and compute fraction */
        duration = aras_time_difference(next_schedule_node->time, current_schedule_node->time);
        position = aras_time_difference(aras_time_current(), current_schedule_node->time);
        fraction = (float)position/(float)duration;

        /* Convert duration and position to readable values */
        aras_time_convert(duration, &duration_hours, &duration_minutes, &duration_seconds);
        aras_time_convert(position, &position_hours, &position_minutes, &position_seconds);

        /* Write string to be shown in the progress bar */
        snprintf(gui->block_progress_bar_str,
                 sizeof(gui->block_progress_bar_str),
                 "%.2d:%.2d:%.2d / %.2d:%.2d:%.2d",
                 position_hours,
                 position_minutes,
                 position_seconds,
                 duration_hours,
                 duration_minutes,
                 duration_seconds);

        /* Show the message and the fraction in the progress bar */
        g_object_set(gui->block_progress_bar, "show-text", TRUE, NULL);
        gtk_progress_bar_set_text((GtkProgressBar*)gui->block_progress_bar, gui->block_progress_bar_str);
        gtk_progress_bar_set_fraction((GtkProgressBar*)gui->block_progress_bar, fraction);
}

/**
 * This function is called periodically and updates the current file label in
 * the file area.
 *
 * @param   gui     Pointer to the gui structure
 * @param   engine  Pointer to the engine structure
 * @param   player  Pointer to the player structure
 */
void aras_gui_player_update_file_current_label(struct aras_gui_player *gui, struct aras_engine *engine, struct aras_player *player)
{
        long int position;
        int position_hours;
        int position_minutes;
        int position_seconds;

        if (engine == NULL)
                return;

        if (engine->playlist == NULL) {
                snprintf(gui->file_current_str,
                         sizeof(gui->file_current_str),
                         "Waiting for playlist");
                gtk_label_set_markup(GTK_LABEL(gui->file_current_label), gui->file_current_str);
                return;
        }

        if (engine->playlist_current_node == NULL) {
                snprintf(gui->file_current_str,
                         sizeof(gui->file_current_str),
                         "Waiting for playback");
                gtk_label_set_markup(GTK_LABEL(gui->file_current_label), gui->file_current_str);
                return;
        }

        position = aras_player_get_position(player, player->current_unit);
        aras_time_convert(position, &position_hours, &position_minutes, &position_seconds);
        snprintf(gui->file_current_str,
                 sizeof(gui->file_current_str),
                 "Current file\n%s\n<span size = \"large\" weight = \"bold\">%.2d:%.2d:%.2d</span> elapsed",
                 (char*)engine->playlist_current_node->data,
                 position_hours,
                 position_minutes,
                 position_seconds);
        gtk_label_set_markup(GTK_LABEL(gui->file_current_label), gui->file_current_str);
}

/**
 * This function is called periodically and updates the next file label in the
 * file area.
 *
 * @param   gui     Pointer to the gui structure
 * @param   engine  Pointer to the engine structure
 * @param   player  Pointer to the player structure
 */
void aras_gui_player_update_file_next_label(struct aras_gui_player *gui, struct aras_engine *engine, struct aras_player *player)
{
        long int position;
        long int duration;
        long int difference;
        int difference_hours;
        int difference_minutes;
        int difference_seconds;

        if (engine == NULL)
                return;

        if (engine->playlist == NULL) {
                snprintf(gui->file_next_str,
                         sizeof(gui->file_next_str),
                         "Waiting for playlist");
                gtk_label_set_markup(GTK_LABEL(gui->file_next_label), gui->file_next_str);
                return;
        }

        if (engine->playlist_current_node == NULL) {
                snprintf(gui->file_next_str,
                         sizeof(gui->file_next_str),
                         "Waiting for playback");
                gtk_label_set_markup(GTK_LABEL(gui->file_next_label), gui->file_next_str);
                return;
        }

        if (engine->playlist_current_node->next == NULL) {
                snprintf(gui->file_next_str,
                         sizeof(gui->file_next_str),
                         "No more files in this block");
                gtk_label_set_markup(GTK_LABEL(gui->file_next_label), gui->file_next_str);
                return;
        }

        if ((duration = aras_player_get_duration(player, player->current_unit)) > 0) {
                position = aras_player_get_position(player, player->current_unit);
                difference = aras_time_difference(duration, position);
                aras_time_convert(difference, &difference_hours, &difference_minutes, &difference_seconds);
                snprintf(gui->file_next_str,
                         sizeof(gui->file_next_str),
                         "Next file\n%s\n<span size = \"large\" weight = \"bold\">%.2d:%.2d:%.2d</span> to start",
                         (char*)engine->playlist_current_node->next->data,
                         difference_hours,
                         difference_minutes,
                         difference_seconds);
        } else {
                snprintf(gui->file_next_str,
                         sizeof(gui->file_next_str),
                         "Next file\n%s\n<span size = \"large\" weight = \"bold\">Undefined time</span> to start",
                         (char*)engine->playlist_current_node->next->data);
        }
        gtk_label_set_markup(GTK_LABEL(gui->file_next_label), gui->file_next_str);
}

/**
 * This function is called periodically and updates the progress bar in the file
 * area.
 *
 * @param   gui     Pointer to the gui structure
 * @param   player  Pointer to the player structure
 */
void aras_gui_player_update_file_progress_bar(struct aras_gui_player *gui, struct aras_player *player)
{
        GstState state;
        long int duration;
        long int position;
        float fraction;
        int duration_hours;
        int duration_minutes;
        int duration_seconds;
        int position_hours;
        int position_minutes;
        int position_seconds;

        /* Check if duration is greater then zero */
        if ((duration = aras_player_get_duration(player, player->current_unit)) > 0) {
                /* Get position and compute fraction */
                position = aras_player_get_position(player, player->current_unit);
                fraction = (float)position/(float)duration;

                /* Convert duration and position to readable values */
                aras_time_convert(duration, &duration_hours, &duration_minutes, &duration_seconds);
                aras_time_convert(position, &position_hours, &position_minutes, &position_seconds);

                /* Write string to be shown in the progress bar */
                snprintf(gui->file_progress_bar_str,
                         sizeof(gui->file_progress_bar_str),
                         "%.2d:%.2d:%.2d / %.2d:%.2d:%.2d",
                         position_hours,
                         position_minutes,
                         position_seconds,
                         duration_hours,
                         duration_minutes,
                         duration_seconds);
        } else {
                /* If duration is zero and player state is GST_STATE_PLAYING then notify streaming */
                aras_player_get_state(player, player->current_unit, &state);
                if (state == GST_STATE_PLAYING) {
                        /* Write string to be shown in the progress bar */
                        position = aras_player_get_position(player, player->current_unit);
                        aras_time_convert(position, &position_hours, &position_minutes, &position_seconds);
                        fraction = 1;
                        snprintf(gui->file_progress_bar_str,
                                 sizeof(gui->file_progress_bar_str),
                                 "Connected to streaming %.2d:%.2d:%.2d", position_hours, position_minutes, position_seconds);
                } else {
                        /* Check if buffering */
                        if ((player->buffer_percent_a > 0 && player->buffer_percent_a < 100) &&
                            (player->buffer_percent_b == 0)) {

                                fraction = (float)(player->buffer_percent_a)/100;
                                snprintf(gui->file_progress_bar_str,
                                         sizeof(gui->file_progress_bar_str),
                                         "Buffering... %d%%", player->buffer_percent_a);

                        } else if ((player->buffer_percent_b > 0 && player->buffer_percent_b < 100) &&
                                (player->buffer_percent_a == 0)) {

                                fraction = (float)(player->buffer_percent_b)/100;
                                snprintf(gui->file_progress_bar_str,
                                         sizeof(gui->file_progress_bar_str),
                                         "Buffering... %d%%", player->buffer_percent_b);

                        } else if ((player->buffer_percent_a > 0 && player->buffer_percent_a < 100) &&
                                   (player->buffer_percent_b > 0 && player->buffer_percent_b < 100)) {

                                if (player->buffer_percent_a < player->buffer_percent_b) {
                                        fraction = (float)(player->buffer_percent_a)/100;
                                        snprintf(gui->file_progress_bar_str,
                                                 sizeof(gui->file_progress_bar_str),
                                                 "Buffering... %d%%", player->buffer_percent_a);
                                } else {
                                        fraction = (float)(player->buffer_percent_b)/100;
                                        snprintf(gui->file_progress_bar_str,
                                                 sizeof(gui->file_progress_bar_str),
                                                 "Buffering... %d%%", player->buffer_percent_b);
                                }
                        } else {
                                fraction = 0;
                                snprintf(gui->file_progress_bar_str,
                                        sizeof(gui->file_progress_bar_str),
                                        "Waiting for playback");
                        }
                }
        }

        /* Show the message and the fraction in the progress bar */
        g_object_set(gui->file_progress_bar, "show-text", TRUE, NULL);
        gtk_progress_bar_set_text((GtkProgressBar*)gui->file_progress_bar, gui->file_progress_bar_str);
        gtk_progress_bar_set_fraction((GtkProgressBar*)gui->file_progress_bar, fraction);
}

/**
 * This function is called periodically and updates the buttons.
 *
 * @param   gui             Pointer to the gui structure
 * @param   engine          Pointer to the engine structure
 */
void aras_gui_player_update_buttons(struct aras_gui_player *gui, struct aras_engine *engine)
{
        if (engine->playlist_current_node != NULL) {
                if ((engine->playlist_current_node->prev != NULL) && ((engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD) || (engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT)))
                        gtk_widget_set_sensitive(gui->button_backward, TRUE);
                else
                        gtk_widget_set_sensitive(gui->button_backward, FALSE);

                if ((engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD) || (engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT))
                        gtk_widget_set_sensitive(gui->button_repeat, TRUE);
                else
                        gtk_widget_set_sensitive(gui->button_repeat, FALSE);

                if ((engine->playlist_current_node->next != NULL) && ((engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD) || (engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT)))
                        gtk_widget_set_sensitive(gui->button_forward, TRUE);
                else
                        gtk_widget_set_sensitive(gui->button_forward, FALSE);

                if ((engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_HARD) || (engine->state == ARAS_ENGINE_STATE_MONITOR_SCHEDULE_SOFT))
                        gtk_widget_set_sensitive(gui->button_eject, TRUE);
                else
                        gtk_widget_set_sensitive(gui->button_eject, FALSE);

        } else {
                gtk_widget_set_sensitive(gui->button_backward, FALSE);
                gtk_widget_set_sensitive(gui->button_repeat, FALSE);
                gtk_widget_set_sensitive(gui->button_forward, FALSE);
                gtk_widget_set_sensitive(gui->button_eject, FALSE);
        }
}

/**
 * This function is called periodically and updates all the widgets.
 *
 * @param   gui             Pointer to the gui structure
 * @param   engine          Pointer to the engine structure
 * @param   player          Pointer to the player structure
 * @param   configuration   Pointer to the configuration structure
 * @param   schedule        Pointer to the schedule structure
 */
void aras_gui_player_update(struct aras_gui_player *gui, struct aras_engine *engine, struct aras_player *player, struct aras_configuration *configuration, struct aras_schedule *schedule)
{
        aras_gui_player_update_time(gui);
        aras_gui_player_update_block_current_label(gui, schedule);
        aras_gui_player_update_block_next_label(gui, schedule);
        aras_gui_player_update_block_progress_bar(gui, schedule);
        aras_gui_player_update_file_current_label(gui, engine, player);
        aras_gui_player_update_file_next_label(gui, engine, player);
        aras_gui_player_update_file_progress_bar(gui, player);
        aras_gui_player_update_buttons(gui, engine);
}

/**
 * This function initializes the status icon.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_status_icon(struct aras_gui_player *gui)
{
        /* Status icon */
        gui->status_icon = (GtkWidget*)gtk_status_icon_new();
        gtk_status_icon_set_tooltip_text((GtkStatusIcon*)gui->status_icon, "ARAS Player");
        gtk_status_icon_set_from_file((GtkStatusIcon*)gui->status_icon, ARAS_GUI_PLAYER_PATH_ICON);

        /* Signal connections */
        g_signal_connect(G_OBJECT(gui->status_icon), "activate", G_CALLBACK(aras_gui_player_callback_status_icon_switch), gui);
        g_signal_connect(G_OBJECT(gui->status_icon), "popup-menu", G_CALLBACK(aras_gui_player_callback_status_icon_menu), gui);
}

/**
 * This function initializes the main window.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_window(struct aras_gui_player *gui)
{
        /* Window */
        gui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(gui->window), "ARAS Player");
        gtk_window_set_icon_from_file(GTK_WINDOW(gui->window), ARAS_GUI_PLAYER_PATH_ICON, NULL);
        gtk_window_set_default_size(GTK_WINDOW(gui->window), -1, -1);
        gtk_widget_set_size_request(gui->window, 640, 360);
        gtk_window_set_resizable(GTK_WINDOW(gui->window), FALSE);
        gtk_window_set_decorated(GTK_WINDOW(gui->window), TRUE);

        /* Signal connections */
        g_signal_connect(G_OBJECT(gui->window), "delete_event", G_CALLBACK(aras_gui_player_callback_window_hide), gui);
        g_signal_connect(G_OBJECT(gui->window), "destroy", G_CALLBACK(aras_gui_player_quit), gui);
}

/**
 * This function initializes the vertical box container in which all the widgets
 * will be added.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_vertical_box(struct aras_gui_player *gui)
{
        /* Vertical box in window */
        gui->vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_container_add(GTK_CONTAINER(gui->window), gui->vertical_box);
}

/**
 * This function initializes the label for the time in the time area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_label_time(struct aras_gui_player *gui)
{
        /* Hour label in the vertical box */
        gui->label_time = gtk_label_new("");
        gtk_widget_set_halign(gui->label_time, GTK_ALIGN_START);
        gtk_widget_set_margin_start(gui->label_time, 12);
        gtk_widget_set_margin_top(gui->label_time, 6);
        gtk_label_set_justify((GtkLabel*)gui->label_time, GTK_JUSTIFY_LEFT);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->label_time);
}

/**
 * This function initializes the horizontal separator above the block area.
 *
 * @param   gui     Pointer to the gui structure
 */
void aras_gui_player_init_block_separator(struct aras_gui_player *gui)
{
        /* Block separator */
        gui->block_separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->block_separator);
}

/**
 * This function initializes the label for the next block in the block area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_block_next_label(struct aras_gui_player *gui)
{
        /* Next block label */
        gui->block_next_label = gtk_label_new("");
        gtk_widget_set_halign(gui->block_next_label, GTK_ALIGN_START);
        gtk_widget_set_margin_start(gui->block_next_label, 12);
        gtk_widget_set_margin_top(gui->block_next_label, 6);
        gtk_label_set_justify((GtkLabel*)gui->block_next_label, GTK_JUSTIFY_LEFT);
        gtk_label_set_max_width_chars((GtkLabel*)gui->block_next_label, 80);
        gtk_label_set_ellipsize((GtkLabel*)gui->block_next_label, PANGO_ELLIPSIZE_END);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->block_next_label);
}

/**
 * This function initializes the label for the current block in the block area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_block_current_label(struct aras_gui_player *gui)
{
        /* Current block label */
        gui->block_current_label = gtk_label_new("");
        gtk_widget_set_halign(gui->block_current_label, GTK_ALIGN_START);
        gtk_widget_set_margin_start(gui->block_current_label, 12);
        gtk_widget_set_margin_top(gui->block_current_label, 6);
        gtk_label_set_justify((GtkLabel*)gui->block_current_label, GTK_JUSTIFY_LEFT);
        gtk_label_set_max_width_chars((GtkLabel*)gui->block_current_label, 80);
        gtk_label_set_ellipsize((GtkLabel*)gui->block_current_label, PANGO_ELLIPSIZE_END);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->block_current_label);
}

/**
 * This function initializes the progess bar in the block area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_block_progress_bar(struct aras_gui_player *gui)
{
        /* Block progress bar */
        gui->block_progress_bar = gtk_progress_bar_new();
        gtk_widget_set_size_request(gui->block_progress_bar, 640, 25);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->block_progress_bar);
}

/**
 * This function initializes the horizontal separator above the file area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_file_separator(struct aras_gui_player *gui)
{
        /* File separator */
        gui->file_separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->file_separator);
}

/**
 * This function initializes the label for the next file in the file area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_file_next_label(struct aras_gui_player *gui)
{
        /* Next file label */
        gui->file_next_label = gtk_label_new("");
        gtk_widget_set_halign(gui->file_next_label, GTK_ALIGN_START);
        gtk_widget_set_margin_start(gui->file_next_label, 12);
        gtk_widget_set_margin_top(gui->file_next_label, 6);
        gtk_label_set_justify((GtkLabel*)gui->file_next_label, GTK_JUSTIFY_LEFT);
        gtk_label_set_max_width_chars((GtkLabel*)gui->file_next_label, 80);
        gtk_label_set_ellipsize((GtkLabel*)gui->file_next_label, PANGO_ELLIPSIZE_END);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->file_next_label);
}

/**
 * This function initializes the label for the current file in the file area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_file_current_label(struct aras_gui_player *gui)
{
        /* Current file label */
        gui->file_current_label = gtk_label_new("");
        gtk_widget_set_halign(gui->file_current_label, GTK_ALIGN_START);
        gtk_widget_set_margin_start(gui->file_current_label, 12);
        gtk_widget_set_margin_top(gui->file_current_label, 6);
        gtk_label_set_justify((GtkLabel*)gui->file_current_label, GTK_JUSTIFY_LEFT);
        gtk_label_set_max_width_chars((GtkLabel*)gui->file_current_label, 80);
        gtk_label_set_ellipsize((GtkLabel*)gui->file_current_label, PANGO_ELLIPSIZE_END);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->file_current_label);
}

/**
 * This function initializes the progess bar in the file area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_file_progress_bar(struct aras_gui_player *gui)
{
        /* File progress bar */
        gui->file_progress_bar = gtk_progress_bar_new();
        gtk_widget_set_size_request(gui->file_progress_bar, 640, 25);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->file_progress_bar);
}

/**
 * This function initializes the horizontal separator above the buttons area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_buttons_separator(struct aras_gui_player *gui)
{
        /* File separator */
        gui->buttons_separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->buttons_separator);
}

/**
 * This function initializes the horizontal box container in which the buttons
 * will be added.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_player_init_horizontal_box(struct aras_gui_player *gui)
{
        /* Horizontal box in vertical box */
        gui->horizontal_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        gtk_box_set_homogeneous((GtkBox*)gui->horizontal_box, TRUE);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->horizontal_box);
}

/**
 * This function initializes the backward button in the buttons area.
 *
 * @param   gui     Pointer to the gui structure
 * @param   data    Pointer to the data used for callback
 */
void aras_gui_player_init_button_backward(struct aras_gui_player *gui, struct aras_gui_player_callback *data)
{
        /* Previous file button in the horizontal box */
        gui->button_backward = gtk_button_new_from_icon_name("media-skip-backward", GTK_ICON_SIZE_BUTTON);
        gtk_widget_set_size_request((GtkWidget*)gui->button_backward, 120, 40);
        gtk_container_add(GTK_CONTAINER(gui->horizontal_box), gui->button_backward);
        g_signal_connect(G_OBJECT(gui->button_backward), "clicked", G_CALLBACK(aras_gui_player_callback_button_backward), data);
        gtk_widget_set_tooltip_text(gui->button_backward, "Previous file");
}

/**
 * This function initializes the repeat button in the buttons area.
 *
 * @param   gui     Pointer to the gui structure
 * @param   data    Pointer to the data used for callback
 */
void aras_gui_player_init_button_repeat(struct aras_gui_player *gui, struct aras_gui_player_callback *data)
{
        /* Current file button in the horizontal box */
        gui->button_repeat = gtk_button_new_from_icon_name("media-playlist-repeat", GTK_ICON_SIZE_BUTTON);
        gtk_widget_set_size_request((GtkWidget*)gui->button_repeat, 120, 40);
        gtk_container_add(GTK_CONTAINER(gui->horizontal_box), gui->button_repeat);
        g_signal_connect(G_OBJECT(gui->button_repeat), "clicked", G_CALLBACK(aras_gui_player_callback_button_repeat), data);
        gtk_widget_set_tooltip_text(gui->button_repeat, "Repeat file");
}

/**
 * This function initializes the forward button in the buttons area.
 *
 * @param   gui     Pointer to the gui structure
 * @param   data    Pointer to the data used for callback
 */
void aras_gui_player_init_button_forward(struct aras_gui_player *gui, struct aras_gui_player_callback *data)
{
        /* Next file button in the horizontal box */
        gui->button_forward = gtk_button_new_from_icon_name("media-skip-forward", GTK_ICON_SIZE_BUTTON);
        gtk_widget_set_size_request((GtkWidget*)gui->button_forward, 120, 40);
        gtk_container_add(GTK_CONTAINER(gui->horizontal_box), gui->button_forward);
        g_signal_connect(G_OBJECT(gui->button_forward), "clicked", G_CALLBACK(aras_gui_player_callback_button_forward), data);
        gtk_widget_set_tooltip_text(gui->button_forward, "Next file");
}

/**
 * This function initializes the eject button in the buttons area.
 *
 * @param   gui     Pointer to the gui structure
 * @param   data    Pointer to the data used for callback
 */
void aras_gui_player_init_button_eject(struct aras_gui_player *gui, struct aras_gui_player_callback *data)
{
        /* Eject button in the horizontal box */
        gui->button_eject = gtk_button_new_from_icon_name("media-eject", GTK_ICON_SIZE_BUTTON);
        gtk_widget_set_size_request((GtkWidget*)gui->button_eject, 120, 40);
        gtk_container_add(GTK_CONTAINER(gui->horizontal_box), gui->button_eject);
        g_signal_connect(G_OBJECT(gui->button_eject), "clicked", G_CALLBACK(aras_gui_player_callback_button_eject), data);
        gtk_widget_set_tooltip_text(gui->button_eject, "Eject playlist");
}

/**
 * This function initializes the ARAS Player GUI and stores data for future
 * calls to callback functions.
 *
 * @param   gui             Pointer to the gui structure
 * @param   engine          Pointer to the engine
 * @param   configuration   Pointer to the configuration structure
 */
void aras_gui_player_init(struct aras_gui_player *gui, struct aras_engine *engine, struct aras_configuration *configuration)
{
        static struct aras_gui_player_callback data;

        /* Pointers to arguments will remain after the function finishes */
        data.engine = engine;
        data.configuration = configuration;

        /* Quit dialog counter */
        gui->quit_dialog_count = 0;

        /* Widgets */
        aras_gui_player_init_status_icon(gui);
        aras_gui_player_init_window(gui);
        aras_gui_player_init_vertical_box(gui);
        aras_gui_player_init_label_time(gui);

        aras_gui_player_init_block_separator(gui);
        aras_gui_player_init_block_next_label(gui);
        aras_gui_player_init_block_current_label(gui);
        aras_gui_player_init_block_progress_bar(gui);

        aras_gui_player_init_file_separator(gui);
        aras_gui_player_init_file_next_label(gui);
        aras_gui_player_init_file_current_label(gui);
        aras_gui_player_init_file_progress_bar(gui);

        aras_gui_player_init_buttons_separator(gui);
        aras_gui_player_init_horizontal_box(gui);
        aras_gui_player_init_button_backward(gui, &data);
        aras_gui_player_init_button_repeat(gui, &data);
        aras_gui_player_init_button_forward(gui, &data);
        aras_gui_player_init_button_eject(gui, &data);

        /* Display */
        gtk_widget_show_all(gui->window);
}
