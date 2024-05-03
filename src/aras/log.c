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
 * Source file for the ARAS Radio Automation System. Functions for the log
 * module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <aras/log.h>

/**
 * This function writes an arbitrary message to the log file.
 *
 * @param   file    Pointer to the file name string
 * @param   msg     Pointer to the message to be appended to the file
 *
 * @return  0 if success, -1 if error
 */
int aras_log_write(char *file, char *msg)
{
        FILE *fp;
        time_t t;
        struct tm tm;
        char timestamp[ARAS_LOG_TIMESTAMP_MAX];

        /* Get time */
        t = time(NULL);
        localtime_r(&t, &tm);

        /* Write timestamp */
        strftime(timestamp, ARAS_LOG_TIMESTAMP_MAX, "%Y-%m-%d %H:%M:%S", &tm);

        /* Open log file */
        if ((fp = fopen(file, "a")) == NULL)
                return -1;

        /* Write timestamp and message in log file */
        fprintf(fp, "%s %s", timestamp, msg);

        /* Close configuration file */
        if (fclose(fp) != 0)
                return -1;

        return 0;
}
