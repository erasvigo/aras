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
 * Header file for the ARAS Radio Automation System. Types and definitions for
 * the GUI recorder module.
 */

#ifndef _ARAS_GUI_RECORDER_H
#define _ARAS_GUI_RECORDER_H

#include <gtk/gtk.h>
#include <aras/configuration.h>
#include <aras/schedule.h>
#include <aras/block.h>
#include <aras/recorder.h>

#define ARAS_GUI_RECORDER_PATH_ICON         "/usr/share/aras/icons/aras-recorder-icon.png"
#define ARAS_GUI_RECORDER_COMMENTS          "The ARAS Radio Automation System"
#define ARAS_GUI_RECORDER_COPYRIGHT         "Copyright © 2020 Erasmo Alonso Iglesias"
#define ARAS_GUI_RECORDER_PROGRAM_NAME      "ARAS Recorder"
#define ARAS_GUI_RECORDER_VERSION           "4.6"
#define ARAS_GUI_RECORDER_WEBSITE           "http://aras.sourceforge.net"
#define ARAS_GUI_RECORDER_WEBSITE_LABEL     "ARAS website"

#define ARAS_GUI_RECORDER_LICENSE \
"The ARAS Radio Automation System\n\
Copyright (C) 2020  Erasmo Alonso Iglesias\n\
\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 3 of the License, or\n\
(at your option) any later version.\n\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License\n\
along with this program.  If not, see <http://www.gnu.org/licenses/>."

#define ARAS_GUI_RECORDER_DIALOG_QUIT \
"<span size = \"large\" weight = \"bold\">Warning</span>\n\
Are you sure you want to quit ARAS Recorder?"

#define ARAS_GUI_RECORDER_LABEL_MAX       1024
#define ARAS_GUI_RECORDER_TIMESTAMP_MAX   24

struct aras_gui_recorder {
        /* System tray */
        GtkWidget *status_icon;
        GtkWidget *menu;

        /* Main window */
        GtkWidget *window;
        GtkWidget *vertical_box;

        /* Widgets for time information */
        GtkWidget *label_time;
        char label_time_str[ARAS_GUI_RECORDER_LABEL_MAX];

        /* Widgets for block information */
        GtkWidget *block_separator;
        GtkWidget *block_current_label;
        char block_current_str[ARAS_GUI_RECORDER_LABEL_MAX];
        GtkWidget *block_next_label;
        char block_next_str[ARAS_GUI_RECORDER_LABEL_MAX];
        GtkWidget *block_progress_bar;
        char block_progress_bar_str[ARAS_GUI_RECORDER_LABEL_MAX];

        /* Widgets for file information */
        GtkWidget *combo_box_separator;
        GtkWidget *combo_box;
        char file_current[ARAS_GUI_RECORDER_LABEL_MAX];
        long int stopwatch;
        GtkWidget *file_stopwatch_label;
        char file_stopwatch_str[ARAS_GUI_RECORDER_LABEL_MAX];

        /* Buttons */
        GtkWidget *buttons_separator;
        GtkWidget *button_record;
        GtkWidget *button_stop;

        /* Quit dialog */
        GtkWidget *quit_dialog;
        int quit_dialog_count;
};

/* Structure for callback data */
struct aras_gui_recorder_callback {
        struct aras_gui_recorder *gui;
        struct aras_recorder *recorder;
        struct aras_configuration *configuration;
        struct aras_block *block;
};

void aras_gui_recorder_update(struct aras_gui_recorder *gui, struct aras_configuration *configuration, struct aras_schedule *schedule);
void aras_gui_recorder_init(struct aras_gui_recorder *gui, struct aras_recorder *recorder, struct aras_configuration *configuration, struct aras_block *block);

#endif  /* _ARAS_GUI_RECORDER_H */
