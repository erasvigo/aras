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
 * Main source file for ARAS Player.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <glib.h>
#include <aras/configuration.h>
#include <aras/schedule.h>
#include <aras/block.h>
#include <aras/engine.h>
#include <aras/player.h>
#include <aras/gui_player.h>
#include <aras/main_player.h>

/**
 * This function checks the command line syntax
 *
 * @param   argc    The number of command line parameters
 * @param   argv    The pointer to the command line parameters
 *
 * @return  0 if the syntax is correct, -1 if the syntax is not correct
 */
int aras_main_player_syntax_check(int argc, char **argv)
{
        if (argc == 2)
                return 0;
        else
                return -1;
}

/**
 * This function is the callback function for configuration. It is called
 * periodically and it calls the functions responsible for updating the
 * configuration.
 *
 * @param   main_player Pointer to the ARAS Player main structure
 *
 * @return  This function always returns TRUE
 */
int aras_main_player_callback_configuration(struct aras_main_player *main_player)
{
        if (main_player == NULL)
                return FALSE;

        if (main_player->configuration_file == NULL)
                return FALSE;

        /* Update data from configuration file */
        aras_configuration_load_file(&main_player->configuration, main_player->configuration_file);

        /* Update data from schedule file */
        aras_schedule_list_free(&main_player->schedule);
        aras_schedule_init(&main_player->schedule);
        aras_schedule_load_file(&main_player->schedule, main_player->configuration.schedule_file);

        /* Update data from block file */
        aras_block_list_free(&main_player->block);
        aras_block_init(&main_player->block);
        aras_block_load_file(&main_player->block, main_player->configuration.block_file);

        return TRUE;
}

/**
 * This function is the callback function for engines. It is called periodically
 * and it calls the functions managing the engines.
 *
 * @param   main_player Pointer to the ARAS Player main structure
 *
 * @return  This function always returns TRUE
 */
int aras_main_player_callback_engine(struct aras_main_player *main_player)
{
        aras_engine_schedule(&main_player->engine_block_player, &main_player->block_player, &main_player->configuration, &main_player->schedule, &main_player->block);
        aras_engine_time_signal(&main_player->engine_time_signal_player, &main_player->time_signal_player, &main_player->configuration, &main_player->block);
        return TRUE;
}

/**
 * This function is the callback function for GUI. It is called periodically and
 * it calls the function managing the GUI.
 *
 * @param   main_player Pointer to the ARAS Player main structure
 *
 * @return  This function always returns TRUE
 */
int aras_main_player_callback_gui(struct aras_main_player *main_player)
{
        aras_gui_player_update(&main_player->gui, &main_player->engine_block_player, &main_player->block_player, &main_player->configuration, &main_player->schedule);
        return TRUE;
}

/**
 * This function initializes a main player structure.
 *
 * @param   main_player Pointer to the main player structure
 *
 * @return  0 if success, -1 if error
 */
int aras_main_player_init(struct aras_main_player *main_player)
{
        struct timeval time;

        /* Initialize the pseudorandom number generator */
        gettimeofday(&time, NULL);
        srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

        /* Initialize and load configuration */
        aras_configuration_init(&main_player->configuration);
        if (aras_configuration_load_file(&main_player->configuration, main_player->configuration_file) == -1) {
                fprintf(stderr, "aras: unable to open configuration file ""%s""\n", main_player->configuration_file);
                return -1;
        }

        /* Initialize and load schedule */
        aras_schedule_init(&main_player->schedule);
        if (aras_schedule_load_file(&main_player->schedule, main_player->configuration.schedule_file) == -1) {
                fprintf(stderr, "aras: unable to open schedule file ""%s""\n", main_player->configuration.schedule_file);
                return -1;
        }

        /* Initialize and load block */
        aras_block_init(&main_player->block);
        if (aras_block_load_file(&main_player->block, main_player->configuration.block_file) == -1) {
                fprintf(stderr, "aras: unable to open block file ""%s""\n", main_player->configuration.block_file);
                return -1;
        }

        /* Initialize GStreamer and players */
        gst_init(NULL, NULL);
        aras_player_init_block_player(&main_player->block_player, &main_player->configuration);
        aras_player_init_time_signal_player(&main_player->time_signal_player, &main_player->configuration);

        /* Initialize engines */
        aras_engine_init(&main_player->engine_block_player);
        aras_engine_init(&main_player->engine_time_signal_player);

        /* Initialize GTK and GUI */
        gtk_disable_setlocale();
        gtk_init(NULL, NULL);
        aras_gui_player_init(&main_player->gui, &main_player->engine_block_player, &main_player->configuration);

        return 0;
}

/**
 * The main function for the ARAS Radio Automation System Player
 *
 * @param   argc    The number of command line parameters
 * @param   argv    The pointer to the command line parameters
 */
int main(int argc, char **argv)
{
        /* The main data structure */
        struct aras_main_player main_player;

        /* Check syntax */
        if (aras_main_player_syntax_check(argc, argv) == -1) {
                fprintf(stderr, "aras: Incorrect syntax\n");
                exit(-1);
        }

        /* Define the configuration file */
        main_player.configuration_file = argv[1];

        /* Main initialization */
        if (aras_main_player_init(&main_player) == -1) {
                fprintf(stderr, "aras: Initialization error\n");
                exit(-1);
        }

        /* Set the callback functions */
        g_timeout_add(main_player.configuration.configuration_period, (GSourceFunc)aras_main_player_callback_configuration, &main_player);
        g_timeout_add(main_player.configuration.engine_period, (GSourceFunc)aras_main_player_callback_engine, &main_player);
        g_timeout_add(main_player.configuration.gui_period, (GSourceFunc)aras_main_player_callback_gui, &main_player);

        /* Run the main loop */
        gtk_main();

        exit(0);
}
