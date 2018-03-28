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
 * Main source file for ARAS Daemon.
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
#include <aras/main_daemon.h>

/**
 * This function checks the command line syntax
 *
 * @param   argc    The number of command line parameters
 * @param   argv    The pointer to the command line parameters
 *
 * @return  0 if the syntax is correct, -1 if the syntax is not correct
 */
int aras_main_daemon_syntax_check(int argc, char **argv)
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
 * @param   main_daemon Pointer to the main daemon structure
 *
 * @return  This function always returns TRUE
 */
int aras_main_daemon_callback_configuration(struct aras_main_daemon *main_daemon)
{
        if (main_daemon == NULL)
                return FALSE;

        if (main_daemon->configuration_file == NULL)
                return FALSE;

        /* Update data from configuration file */
        aras_configuration_load_file(&main_daemon->configuration, main_daemon->configuration_file);

        /* Update data from schedule file */
        aras_schedule_list_free(&main_daemon->schedule);
        aras_schedule_init(&main_daemon->schedule);
        aras_schedule_load_file(&main_daemon->schedule, main_daemon->configuration.schedule_file);

        /* Update data from block file */
        aras_block_list_free(&main_daemon->block);
        aras_block_init(&main_daemon->block);
        aras_block_load_file(&main_daemon->block, main_daemon->configuration.block_file);

        return TRUE;
}

/**
 * This function is the callback function for engines. It is called
 * periodically and it calls the functions managing the engines.
 *
 * @param   main_daemon Pointer to the main daemon structure
 *
 * @return  This function always returns TRUE
 */
int aras_main_daemon_callback_engine(struct aras_main_daemon *main_daemon)
{
        aras_engine_schedule(&main_daemon->engine_block_player, &main_daemon->block_player, &main_daemon->configuration, &main_daemon->schedule, &main_daemon->block);
        aras_engine_time_signal(&main_daemon->engine_time_signal_player, &main_daemon->time_signal_player, &main_daemon->configuration, &main_daemon->block);

        return TRUE;
}

/**
 * This function initializes a main daemon structure.
 *
 * @param   main_daemon Pointer to the main daemon structure
 *
 * @return  0 if success, -1 if error
 */
int aras_main_daemon_init(struct aras_main_daemon *main_daemon)
{
        struct timeval time;

        /* Initialize the pseudorandom number generator */
        gettimeofday(&time, NULL);
        srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

        /* Initialize and load configuration */
        aras_configuration_init(&main_daemon->configuration);
        if (aras_configuration_load_file(&main_daemon->configuration, main_daemon->configuration_file) == -1) {
                fprintf(stderr, "aras: unable to open configuration file ""%s""\n", main_daemon->configuration_file);
                return -1;
        }

        /* Initialize and load schedule */
        aras_schedule_init(&main_daemon->schedule);
        if (aras_schedule_load_file(&main_daemon->schedule, main_daemon->configuration.schedule_file) == -1) {
                fprintf(stderr, "aras: unable to open schedule file ""%s""\n", main_daemon->configuration.schedule_file);
                return -1;
        }

        /* Initialize and load block */
        aras_block_init(&main_daemon->block);
        if (aras_block_load_file(&main_daemon->block, main_daemon->configuration.block_file) == -1) {
                fprintf(stderr, "aras: unable to open block file ""%s""\n", main_daemon->configuration.block_file);
                return -1;
        }

        /* Initialize GStreamer and players */
        gst_init(NULL, NULL);
        aras_player_init_block_player(&main_daemon->block_player, &main_daemon->configuration);
        aras_player_init_time_signal_player(&main_daemon->time_signal_player, &main_daemon->configuration);

        /* Initialize engines */
        aras_engine_init(&main_daemon->engine_block_player);
        aras_engine_init(&main_daemon->engine_time_signal_player);

        return 0;
}

/**
 * The main function for the ARAS Radio Automation System Daemon
 *
 * @param   argc    The number of command line parameters
 * @param   argv    The pointer to the command line parameters
 */
int main(int argc, char **argv)
{
        /* The main data structure */
        struct aras_main_daemon main_daemon;

        /* The main loop */
        GMainLoop *main_loop;

        /* Check syntax */
        if (aras_main_daemon_syntax_check(argc, argv) == -1) {
                fprintf(stderr, "aras: Incorrect syntax\n");
                exit(-1);
        }

        /* Define the configuration file */
        main_daemon.configuration_file = argv[1];

        /* Main initialization */
        if (aras_main_daemon_init(&main_daemon) == -1) {
                fprintf(stderr, "aras: Initialization error\n");
                exit(-1);
        }

        /* Create the main loop */
        main_loop = g_main_loop_new(NULL, FALSE);

        /* Set the callback functions */
        g_timeout_add(main_daemon.configuration.configuration_period, (GSourceFunc)aras_main_daemon_callback_configuration, &main_daemon);
        g_timeout_add(main_daemon.configuration.engine_period, (GSourceFunc)aras_main_daemon_callback_engine, &main_daemon);

        /* Run the main loop */
        g_main_loop_run(main_loop);

        /* Unref the main loop */
        g_main_loop_unref(main_loop);

        exit(0);
}
