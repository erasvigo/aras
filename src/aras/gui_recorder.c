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
 * Source file for the ARAS Radio Automation System. Functions for the GUI
 * recorder module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <gtk/gtk.h>
#include <aras/time.h>
#include <aras/configuration.h>
#include <aras/schedule.h>
#include <aras/block.h>
#include <aras/engine.h>
#include <aras/recorder.h>
#include <aras/gui_recorder.h>

/**
 * This function closes the application.
 */
void aras_gui_recorder_quit()
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
void aras_gui_recorder_callback_window_hide(GtkWidget *widget, GdkEvent *event, struct aras_gui_recorder *gui)
{
        gtk_widget_hide(gui->window);
}

/**
 * This function is the callback function for the about dialog.
 *
 * @param   widget  Pointer to the widget
 * @param   gui     Pointer to the gui
 */
void aras_gui_recorder_callback_about(GtkWidget *widget, struct aras_gui_recorder *gui)
{
        gchar *artists[] = {"Erasmo Alonso Iglesias",
                            NULL};
        gchar *authors[] = {"Erasmo Alonso Iglesias",
                            NULL};
        gchar *comments = ARAS_GUI_RECORDER_COMMENTS;
        gchar *copyright = ARAS_GUI_RECORDER_COPYRIGHT;
        gchar *documenters[] = {"Erasmo Alonso Iglesias",
                                "Henrik Enblom",
                                "Martin Zelaia",
                                NULL};
        gchar *license = ARAS_GUI_RECORDER_LICENSE;
        GtkImage *logo_image;
        GdkPixbuf *logo;
        gchar *program_name = ARAS_GUI_RECORDER_PROGRAM_NAME;
        gchar *version = ARAS_GUI_RECORDER_VERSION;
        gchar *website = ARAS_GUI_RECORDER_WEBSITE;
        gchar *website_label = ARAS_GUI_RECORDER_WEBSITE_LABEL;

        logo_image = (GtkImage*)gtk_image_new_from_file(ARAS_GUI_RECORDER_PATH_ICON);
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
void aras_gui_recorder_callback_quit_dialog(GtkWidget *widget, struct aras_gui_recorder *gui)
{
        gint result;

        if (gui->quit_dialog_count != 0)
                return;

        gui->quit_dialog_count++;
        gui->quit_dialog = gtk_message_dialog_new_with_markup((GtkWindow*)gui->window,
                                                                GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
                                                                GTK_BUTTONS_YES_NO,
                                                                ARAS_GUI_RECORDER_DIALOG_QUIT);
        result = gtk_dialog_run(GTK_DIALOG(gui->quit_dialog));
        switch (result) {
        case GTK_RESPONSE_YES:
                aras_gui_recorder_quit();
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
void aras_gui_recorder_callback_status_icon_switch(GtkStatusIcon *status_icon, struct aras_gui_recorder *gui)
{
        if (gtk_widget_get_visible(gui->window))
                gtk_widget_hide(gui->window);
        else
                gtk_widget_show(gui->window);
}

/**
 * This function is the callback function for the status icon menu.
 *
 * @param   status_icon     Pointer to the status icon
 * @param   button          Button identifier
 * @param   activate_time   Activate time value
 * @param   gui             Pointer to the gui
 */
void aras_gui_recorder_callback_status_icon_menu(GtkStatusIcon *status_icon, guint button, guint activate_time, struct aras_gui_recorder *gui)
{
        GtkWidget *menu;
        GtkWidget *item;

        menu = gtk_menu_new();

        if (gtk_widget_get_visible(gui->window))
                item = gtk_menu_item_new_with_label("Hide ARAS Recorder");
        else
                item = gtk_menu_item_new_with_label("Show ARAS Recorder");

        g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(aras_gui_recorder_callback_status_icon_switch), gui);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

        item = gtk_menu_item_new_with_label("About...");
        g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(aras_gui_recorder_callback_about), gui);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

        item = gtk_separator_menu_item_new();
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

        item = gtk_menu_item_new_with_label("Quit");
        g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(aras_gui_recorder_callback_quit_dialog), gui);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

        gtk_widget_show_all(menu);
        gtk_menu_popup((GtkMenu*)menu, NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}

/**
 * This function is the callback function for the record button. It gets a block
 * name from the combo box, checks the block type and retrieves its local path.
 * Then, it adds a timestamp to the file name, creates a symbolic link to it
 * with the original file name and starts recording.
 *
 * @param   widget  Pointer to the widget
 * @param   data    Pointer to the callback data
 */
void aras_gui_recorder_callback_button_record(GtkWidget *widget, struct aras_gui_recorder_callback *data)
{
        char *block_name_selected;
        struct aras_block_node *node;
        char *scheme;
        time_t t;
        struct tm tm;
        char *path;                                         /* Full path from block data */
        char *dir;                                          /* Directory from full path */
        char *file;                                         /* File from full path */
        char timestamp[ARAS_GUI_RECORDER_TIMESTAMP_MAX];    /* Timestamp buffer */
        char file_timestamp[ARAS_GUI_RECORDER_LABEL_MAX];   /* File with timestamp */
        char *path_timestamp;                               /* Full path with for file with timestamp */
        GtkWidget *dialog;

        /* Get block name from combo box */
        if ((block_name_selected = gtk_combo_box_text_get_active_text((GtkComboBoxText*)data->gui->combo_box)) == NULL) {
                /* Show the warning */
                dialog = gtk_message_dialog_new_with_markup((GtkWindow*)data->gui->window, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "<span size = \"large\" weight = \"bold\">Warning</span>\nPlease select a block before recording");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                return;
        }

        /* Get block node from block name */
        node = aras_block_seek_node_name(data->block, block_name_selected);
        g_free(block_name_selected);

        /* Check block type */
        if (node->type != ARAS_BLOCK_TYPE_FILE) {
                dialog = gtk_message_dialog_new_with_markup((GtkWindow*)data->gui->window, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size = \"large\" weight = \"bold\">Error</span>\nSelected block is not a file block");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                return;
        }

        /* Check if data is a local file or an URI */
        if ((scheme = g_uri_parse_scheme(node->data)) != NULL) {
                /* Get file from URI */
                g_free(scheme);
                if ((path = g_filename_from_uri(node->data, NULL, NULL)) == NULL) {
                        dialog = gtk_message_dialog_new_with_markup((GtkWindow*)data->gui->window, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size = \"large\" weight = \"bold\">Error</span>\nError while getting filename from URI");
                        gtk_dialog_run(GTK_DIALOG(dialog));
                        gtk_widget_destroy(dialog);
                        return;
                }
        } else {
                /* Get file from node data */
                if ((path = g_strdup(node->data)) == NULL) {
                        dialog = gtk_message_dialog_new_with_markup((GtkWindow*)data->gui->window, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size = \"large\" weight = \"bold\">Error</span>\nError while getting filename");
                        gtk_dialog_run(GTK_DIALOG(dialog));
                        gtk_widget_destroy(dialog);
                        return;
                }

        }

        /* Get directory and file from full path */
        dir = g_path_get_dirname(path);

        /* Check if directory is writable */
        if (access(dir, W_OK) != 0) {
                dialog = gtk_message_dialog_new_with_markup((GtkWindow*)data->gui->window, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size = \"large\" weight = \"bold\">Error</span>\nDirectory is not writable");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                g_free(path);
                g_free(dir);
                return;
        }

        /* Get file name */
        file = g_path_get_basename(path);

        /* Get timestamp */
        t = time(NULL);
        localtime_r(&t, &tm);
        strftime(timestamp, sizeof(timestamp), "%Y%m%dT%H%M%S", &tm);

        /* Write file name with timestamp */
        snprintf(file_timestamp, sizeof(file_timestamp), "%s %s", timestamp, file);

        /* Set the full path for file with timestamp */
        path_timestamp = g_build_filename(dir, file_timestamp, NULL);
        snprintf(data->gui->file_current, sizeof(data->gui->file_current), "%s", path_timestamp);

        /* Create symbolic link */
        remove(path);
        if (symlink(path_timestamp, path) != 0) {
                dialog = gtk_message_dialog_new_with_markup((GtkWindow*)data->gui->window, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size = \"large\" weight = \"bold\">Error</span>\nError while creating symbolic link");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                g_free(path);
                g_free(dir);
                g_free(file);
                g_free(path_timestamp);
                return;
        }

        /* Set the timestamp for the stopwatch */
        data->gui->stopwatch = aras_time_current();

        /* Free memory */
        g_free(path);
        g_free(dir);
        g_free(file);
        g_free(path_timestamp);

        /* Record */
        aras_recorder_set_state_null(data->recorder);
        aras_recorder_set_location(data->recorder, data->gui->file_current);
        aras_recorder_set_state_ready(data->recorder);
        aras_recorder_set_state_paused(data->recorder);
        aras_recorder_set_state_playing(data->recorder);

        /* Update widgets */
        gtk_widget_hide(data->gui->button_record);
        gtk_widget_show(data->gui->button_stop);
        gtk_widget_show(data->gui->file_stopwatch_label);
        gtk_widget_set_sensitive(data->gui->combo_box, FALSE);
}

/**
 * This function is the callback function for the stop button. It stops
 * recording, updates widgets, resets the current file name and resets the
 * timestamp.
 *
 * @param   widget  Pointer to the widget
 * @param   data    Pointer to the callback data
 */
void aras_gui_recorder_callback_button_stop(GtkWidget *widget, struct aras_gui_recorder_callback *data)
{
        /* Stop record */
        aras_recorder_set_state_paused(data->recorder);
        aras_recorder_set_state_ready(data->recorder);
        aras_recorder_set_state_null(data->recorder);

        /* Update widgets */
        gtk_widget_hide(data->gui->button_stop);
        gtk_widget_show(data->gui->button_record);
        gtk_widget_hide(data->gui->file_stopwatch_label);
        gtk_widget_set_sensitive(data->gui->combo_box, TRUE);

        /* Reset file label and stopwatch */
        snprintf((char*)data->gui->file_current, sizeof(data->gui->file_current), "%s", "");
        data->gui->stopwatch = aras_time_current();
}

/**
 * This function is called periodically and updates the time label in the time
 * area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_recorder_update_time(struct aras_gui_recorder *gui)
{
        struct timeval tv;
        struct timezone tz;
        struct tm tm;
        char buffer_day[ARAS_GUI_RECORDER_LABEL_MAX];
        char buffer_time[ARAS_GUI_RECORDER_LABEL_MAX];

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
void aras_gui_recorder_update_block_next_label(struct aras_gui_recorder *gui, struct aras_schedule *schedule)
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
void aras_gui_recorder_update_block_current_label(struct aras_gui_recorder *gui, struct aras_schedule *schedule)
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
void aras_gui_recorder_update_block_progress_bar(struct aras_gui_recorder *gui, struct aras_schedule *schedule)
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
 * This function is called periodically and updates the stopwatch.
 *
 * @param   gui             Pointer to the gui structure
 */
void aras_gui_recorder_update_file_stopwatch(struct aras_gui_recorder *gui)
{
        long int time;
        int hours;
        int minutes;
        int seconds;

        time = aras_time_difference(aras_time_current(), gui->stopwatch);
        aras_time_convert(time, &hours, &minutes, &seconds);

        snprintf(gui->file_stopwatch_str, sizeof(gui->file_stopwatch_str),
                 "Recording file\n%s\n<span size = \"large\" weight = \"bold\">%.2d:%.2d:%.2d</span> elapsed",
                 gui->file_current,
                 hours,
                 minutes,
                 seconds);

        gtk_label_set_markup(GTK_LABEL(gui->file_stopwatch_label), gui->file_stopwatch_str);
}

/**
 * This function is called periodically and updates all the widgets.
 *
 * @param   gui             Pointer to the gui structure
 * @param   configuration   Pointer to the configuration structure
 * @param   schedule        Pointer to the schedule structure
 */
void aras_gui_recorder_update(struct aras_gui_recorder *gui, struct aras_configuration *configuration, struct aras_schedule *schedule)
{
        aras_gui_recorder_update_time(gui);
        aras_gui_recorder_update_block_current_label(gui, schedule);
        aras_gui_recorder_update_block_next_label(gui, schedule);
        aras_gui_recorder_update_block_progress_bar(gui, schedule);
        aras_gui_recorder_update_file_stopwatch(gui);
}

/**
 * This function initializes the status icon.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_recorder_init_status_icon(struct aras_gui_recorder *gui)
{
        /* Status icon */
        gui->status_icon = (GtkWidget*)gtk_status_icon_new();
        gtk_status_icon_set_tooltip_text((GtkStatusIcon*)gui->status_icon, "ARAS Recorder");
        gtk_status_icon_set_from_file((GtkStatusIcon*)gui->status_icon, ARAS_GUI_RECORDER_PATH_ICON);

        /* Signal connections */
        g_signal_connect(G_OBJECT(gui->status_icon), "activate", G_CALLBACK(aras_gui_recorder_callback_status_icon_switch), gui);
        g_signal_connect(G_OBJECT(gui->status_icon), "popup-menu", G_CALLBACK(aras_gui_recorder_callback_status_icon_menu), gui);
}

/**
 * This function initializes the main window.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_recorder_init_window(struct aras_gui_recorder *gui)
{
        /* Window */
        gui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(gui->window), "ARAS Recorder");
        gtk_window_set_icon_from_file(GTK_WINDOW(gui->window), ARAS_GUI_RECORDER_PATH_ICON, NULL);
        gtk_window_set_default_size(GTK_WINDOW(gui->window), -1, -1);
        gtk_widget_set_size_request(gui->window, 640, 360);
        gtk_window_set_resizable(GTK_WINDOW(gui->window), FALSE);
        gtk_window_set_decorated(GTK_WINDOW(gui->window), TRUE);

        /* Signal connections */
        g_signal_connect(G_OBJECT(gui->window), "delete_event", G_CALLBACK(aras_gui_recorder_callback_window_hide), gui);
        g_signal_connect(G_OBJECT(gui->window), "destroy", G_CALLBACK(aras_gui_recorder_quit), gui);
}

/**
 * This function initializes the vertical box container in which all the widgets
 * will be added.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_recorder_init_vertical_box(struct aras_gui_recorder *gui)
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
void aras_gui_recorder_init_label_time(struct aras_gui_recorder *gui)
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
 * This function initializes the horizontal separator above the block area
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_recorder_init_block_separator(struct aras_gui_recorder *gui)
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
void aras_gui_recorder_init_block_next_label(struct aras_gui_recorder *gui)
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
void aras_gui_recorder_init_block_current_label(struct aras_gui_recorder *gui)
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
void aras_gui_recorder_init_block_progress_bar(struct aras_gui_recorder *gui)
{
        /* Block progress bar */
        gui->block_progress_bar = gtk_progress_bar_new();
        gtk_widget_set_size_request(gui->block_progress_bar, 640, 25);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->block_progress_bar);
}

/**
 * This function initializes the horizontal separator above the combo box area
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_recorder_init_combo_box_separator(struct aras_gui_recorder *gui)
{
        /* Combo box separator */
        gui->combo_box_separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->combo_box_separator);
}

/**
 * This function initializes the combo box in the combo box area.
 *
 * @param   gui     Pointer to the gui structure
 * @param   block   Pointer to the block structure
 */
void aras_gui_recorder_init_combo_box(struct aras_gui_recorder *gui, struct aras_block *block)
{
        GList *pointer;
        struct aras_block_node *node;
        char *scheme;

        gui->combo_box = gtk_combo_box_text_new();

        pointer = block->list;
        while (pointer != NULL) {
                node = pointer->data;
                if (node->type == ARAS_BLOCK_TYPE_FILE) {
                        if ((scheme = g_uri_parse_scheme(node->data)) != NULL) {
                                /* Data is a URI */
                                if (!strcmp(scheme, "file"))
                                        gtk_combo_box_text_append((GtkComboBoxText*)gui->combo_box, NULL, node->name);
                                g_free(scheme);
                        } else {
                                /* Try local file */
                                gtk_combo_box_text_append((GtkComboBoxText*)gui->combo_box, NULL, node->name);
                        }
                }
                pointer = pointer->next;
        }
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->combo_box);
}

/**
 * This function initializes the label for the stopwatch in the file area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_recorder_init_file_stopwatch(struct aras_gui_recorder *gui)
{
        gui->file_stopwatch_label = gtk_label_new("");
        gtk_widget_set_halign(gui->file_stopwatch_label, GTK_ALIGN_START);
        gtk_widget_set_margin_start(gui->file_stopwatch_label, 12);
        gtk_widget_set_margin_top(gui->file_stopwatch_label, 6);
        gtk_label_set_justify((GtkLabel*)gui->file_stopwatch_label, GTK_JUSTIFY_LEFT);
        gtk_label_set_max_width_chars((GtkLabel*)gui->file_stopwatch_label, 80);
        gtk_label_set_ellipsize((GtkLabel*)gui->file_stopwatch_label, PANGO_ELLIPSIZE_END);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->file_stopwatch_label);
        snprintf(gui->file_stopwatch_str, sizeof(gui->file_stopwatch_str), "%s", "");
        gtk_label_set_markup(GTK_LABEL(gui->file_stopwatch_label), gui->file_stopwatch_str);
}

/**
 * This function initializes the horizontal separator above the buttons area.
 *
 * @param   gui Pointer to the gui structure
 */
void aras_gui_recorder_init_buttons_separator(struct aras_gui_recorder *gui)
{
        /* Combo box separator */
        gui->buttons_separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->buttons_separator);
}

/**
 * This function initializes the record button in the buttons area.
 *
 * @param   gui     Pointer to the gui structure
 * @param   data    Pointer to the data used for callback
 */
void aras_gui_recorder_init_button_record(struct aras_gui_recorder *gui, struct aras_gui_recorder_callback *data)
{
        /* Recorder record button */
        gui->button_record = gtk_button_new_from_icon_name("media-record", GTK_ICON_SIZE_BUTTON);
        gtk_widget_set_size_request((GtkWidget*)gui->button_record, 120, 40);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->button_record);
        g_signal_connect(G_OBJECT(gui->button_record), "clicked", G_CALLBACK(aras_gui_recorder_callback_button_record), data);
        gtk_widget_set_tooltip_text(gui->button_record, "Record");
        gui->stopwatch = 0;
        snprintf(gui->file_current, sizeof(gui->file_current), "%s", "");
}

/**
 * This function initializes the stop button in the buttons area.
 *
 * @param   gui     Pointer to the gui structure
 * @param   data    Pointer to the data used for callback
 */
void aras_gui_recorder_init_button_stop(struct aras_gui_recorder *gui, struct aras_gui_recorder_callback *data)
{
        /* Recorder stop button */
        gui->button_stop = gtk_button_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_BUTTON);
        gtk_widget_set_size_request((GtkWidget*)gui->button_stop, 120, 40);
        gtk_container_add(GTK_CONTAINER(gui->vertical_box), gui->button_stop);
        g_signal_connect(G_OBJECT(gui->button_stop), "clicked", G_CALLBACK(aras_gui_recorder_callback_button_stop), data);
        gtk_widget_set_tooltip_text(gui->button_stop, "Stop");
}

/**
 * This function initializes the ARAS Recorder GUI and stores data for future
 * calls to callback functions.
 *
 * @param   gui             Pointer to the gui structure
 * @param   recorder        Pointer to the recorder structure
 * @param   configuration   Pointer to the configuration structure
 * @param   block           Pointer to the block structure
 */
void aras_gui_recorder_init(struct aras_gui_recorder *gui, struct aras_recorder *recorder, struct aras_configuration *configuration, struct aras_block *block)
{
        static struct aras_gui_recorder_callback data;

        /* Pointers to arguments will remain after the function finishes */
        data.gui = gui;
        data.recorder = recorder;
        data.configuration = configuration;
        data.block = block;

        /* Quit dialog counter */
        gui->quit_dialog_count = 0;

        /* Widgets */
        aras_gui_recorder_init_status_icon(gui);
        aras_gui_recorder_init_window(gui);
        aras_gui_recorder_init_vertical_box(gui);
        aras_gui_recorder_init_label_time(gui);

        aras_gui_recorder_init_block_separator(gui);
        aras_gui_recorder_init_block_next_label(gui);
        aras_gui_recorder_init_block_current_label(gui);
        aras_gui_recorder_init_block_progress_bar(gui);

        aras_gui_recorder_init_combo_box_separator(gui);
        aras_gui_recorder_init_combo_box(gui, block);
        aras_gui_recorder_init_file_stopwatch(gui);

        aras_gui_recorder_init_buttons_separator(gui);
        aras_gui_recorder_init_button_record(gui, &data);
        aras_gui_recorder_init_button_stop(gui, &data);

        /* Display */
        gtk_widget_show_all(gui->vertical_box);
        gtk_widget_hide(gui->file_stopwatch_label);
        gtk_widget_hide(gui->button_stop);
        gtk_widget_show(gui->window);
}
