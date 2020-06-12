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
 * the GUI player module.
 */

#ifndef _ARAS_GUI_PLAYER_H
#define _ARAS_GUI_PLAYER_H

#include <gtk/gtk.h>
#include <aras/configuration.h>
#include <aras/schedule.h>
#include <aras/block.h>
#include <aras/engine.h>

#define ARAS_GUI_PLAYER_PATH_ICON       "/usr/share/aras/icons/aras-player-icon.png"
#define ARAS_GUI_PLAYER_COMMENTS        "The ARAS Radio Automation System"
#define ARAS_GUI_PLAYER_COPYRIGHT       "Copyright © 2020 Erasmo Alonso Iglesias"
#define ARAS_GUI_PLAYER_PROGRAM_NAME    "ARAS Player"
#define ARAS_GUI_PLAYER_VERSION         "4.6"
#define ARAS_GUI_PLAYER_WEBSITE         "http://aras.sourceforge.net"
#define ARAS_GUI_PLAYER_WEBSITE_LABEL   "ARAS website"

#define ARAS_GUI_PLAYER_LICENSE \
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

#define ARAS_GUI_PLAYER_DIALOG_QUIT \
"<span size = \"large\" weight = \"bold\">Warning</span>\n\
Are you sure you want to quit ARAS Player?"

#define ARAS_GUI_PLAYER_LABEL_MAX       1024

/* Structure for widgets */
struct aras_gui_player {
        /* System tray */
        GtkWidget *status_icon;
        GtkWidget *menu;

        /* Main window */
        GtkWidget *window;
        GtkWidget *vertical_box;
        GtkWidget *horizontal_box;

        /* Widgets for time information */
        GtkWidget *label_time;
        char label_time_str[ARAS_GUI_PLAYER_LABEL_MAX];

        /* Widgets for block information */
        GtkWidget *block_separator;
        GtkWidget *block_current_label;
        char block_current_str[ARAS_GUI_PLAYER_LABEL_MAX];
        GtkWidget *block_next_label;
        char block_next_str[ARAS_GUI_PLAYER_LABEL_MAX];
        GtkWidget *block_progress_bar;
        char block_progress_bar_str[ARAS_GUI_PLAYER_LABEL_MAX];

        /* Widgets for file information */
        GtkWidget *file_separator;
        GtkWidget *file_current_label;
        char file_current_str[ARAS_GUI_PLAYER_LABEL_MAX];
        GtkWidget *file_next_label;
        char file_next_str[ARAS_GUI_PLAYER_LABEL_MAX];
        GtkWidget *file_progress_bar;
        char file_progress_bar_str[ARAS_GUI_PLAYER_LABEL_MAX];

        /* Buttons */
        GtkWidget *buttons_separator;
        GtkWidget *button_backward;
        GtkWidget *button_repeat;
        GtkWidget *button_forward;
        GtkWidget *button_eject;

        /* Quit dialog */
        GtkWidget *quit_dialog;
        int quit_dialog_count;
};

/* Structure for callback data */
struct aras_gui_player_callback {
        struct aras_engine *engine;
        struct aras_configuration *configuration;
};

void aras_gui_player_update(struct aras_gui_player *gui, struct aras_engine *engine, struct aras_player *player, struct aras_configuration *configuration, struct aras_schedule *schedule);
void aras_gui_player_init(struct aras_gui_player *gui, struct aras_engine *engine, struct aras_configuration *configuration);

#endif  /* _ARAS_GUI_PLAYER_H */
