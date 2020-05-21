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
 * Main source file for ARAS Recorder.
 */

#include <stdio.h>
#include <stdlib.h>
#include <aras/configuration.h>
#include <aras/recorder.h>
#include <aras/gui_recorder.h>
#include <aras/main_recorder.h>

/**
 * This function checks the command line syntax
 *
 * @param   argc    The number of command line parameters
 * @param   argv    The pointer to the command line parameters
 *
 * @return  0 if the syntax is correct, -1 if the syntax is not correct
 */
int aras_main_recorder_syntax_check(int argc, char **argv)
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
 * @param   main_recorder   Pointer to the ARAS recorder main structure
 *
 * @return  This function always returns TRUE
 */
int aras_main_recorder_callback_configuration(struct aras_main_recorder *main_recorder)
{
        if (main_recorder == NULL)
                return FALSE;

        if (main_recorder->configuration_file == NULL)
                return FALSE;

        /* Update data from configuration file */
        aras_configuration_load_file(&main_recorder->configuration, main_recorder->configuration_file);

        /* Update data from schedule file */
        aras_schedule_list_free(&main_recorder->schedule);
        aras_schedule_init(&main_recorder->schedule);
        aras_schedule_load_file(&main_recorder->schedule, main_recorder->configuration.schedule_file);

        /* Update data from block file */
        aras_block_list_free(&main_recorder->block);
        aras_block_init(&main_recorder->block);
        aras_block_load_file(&main_recorder->block, main_recorder->configuration.block_file);

        return TRUE;
}

/**
 * This function is the callback function for GUI. It is called periodically and
 * it calls the function managing the GUI.
 *
 * @param   main_recorder   Pointer to the ARAS recorder main structure
 *
 * @return  This function always returns TRUE
 */
int aras_main_recorder_callback_gui(struct aras_main_recorder *main_recorder)
{
        aras_gui_recorder_update(&main_recorder->gui,
                                &main_recorder->configuration,
                                &main_recorder->schedule);
        return TRUE;
}

/**
 * This function initializes a main_recorder structure.
 *
 * @param   main_recorder   Pointer to the ARAS recorder main structure
 *
 * @return  0 if success, -1 if error
 */
int aras_main_recorder_init(struct aras_main_recorder *main_recorder)
{
        /* Initialize and load configuration */
        aras_configuration_init(&main_recorder->configuration);
        if (aras_configuration_load_file(&main_recorder->configuration, main_recorder->configuration_file) == -1) {
                fprintf(stderr, "aras: unable to open configuration file ""%s""\n", main_recorder->configuration_file);
                return -1;
        }

        /* Initialize and load schedule */
        aras_schedule_init(&main_recorder->schedule);
        if (aras_schedule_load_file(&main_recorder->schedule, main_recorder->configuration.schedule_file) == -1) {
                fprintf(stderr, "aras: unable to open schedule file ""%s""\n", main_recorder->configuration.schedule_file);
                return -1;
        }

        /* Initialize and load block */
        aras_block_init(&main_recorder->block);
        if (aras_block_load_file(&main_recorder->block, main_recorder->configuration.block_file) == -1) {
                fprintf(stderr, "aras: unable to open block file ""%s""\n", main_recorder->configuration.block_file);
                return -1;
        }

        /* Initialize GStreamer and recorder */
        gst_init(NULL, NULL);
        aras_recorder_init(&main_recorder->recorder, &main_recorder->configuration);

        /* Initialize GTK and GUI */
        gtk_disable_setlocale();
        gtk_init(NULL, NULL);
        aras_gui_recorder_init(&main_recorder->gui, &main_recorder->recorder, &main_recorder->configuration, &main_recorder->block);

        return 0;
}

/**
 * The main function for the ARAS Radio Automation System Recorder
 *
 * @param   argc    The number of command line parameters
 * @param   argv    The pointer to the command line parameters
 */
int main(int argc, char **argv)
{
        /* The main data structure */
        struct aras_main_recorder main_recorder;

        /* The main loop */
        //GMainLoop *main_loop;

        /* Check syntax */
        if (aras_main_recorder_syntax_check(argc, argv) == -1) {
                fprintf(stderr, "aras: Incorrect syntax\n");
                exit(-1);
        }

        /* Define the configuration file */
        main_recorder.configuration_file = argv[1];

        /* Main initialization */
        if (aras_main_recorder_init(&main_recorder) == -1) {
                fprintf(stderr, "aras: Initialization error\n");
                exit(-1);
        }

        /* Set the callback functions */
        g_timeout_add(main_recorder.configuration.configuration_period, (GSourceFunc)aras_main_recorder_callback_configuration, &main_recorder);
        g_timeout_add(main_recorder.configuration.gui_period, (GSourceFunc)aras_main_recorder_callback_gui, &main_recorder);

        /* Run the main loop */
        gtk_main();

        exit(0);
}
