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
 * Source file for the ARAS Radio Automation System. Functions for the time
 * module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <aras/time.h>

/**
 * This function returns the current week time in miliseconds.
 *
 * @return  The current week time in miliseconds
 */
long int aras_time_current(void)
{
        struct timeval tv;
        struct timezone tz;
        struct tm tm;

        gettimeofday(&tv, &tz);
        localtime_r(&tv.tv_sec, &tm);
        return ARAS_TIME_DAY * tm.tm_wday + ARAS_TIME_HOUR * tm.tm_hour + ARAS_TIME_MINUTE * tm.tm_min + ARAS_TIME_SECOND * tm.tm_sec + ldiv(tv.tv_usec, 1000).quot;
}

/**
 * This function returns the addition of two week times in a weekly cyclic way.
 *
 * @param   time1   Arbitrary time
 * @param   time0   Base time
 *
 * @return  The addition of two week times in a weekly cyclic way
 */
long int aras_time_addition(long int time1, long int time0)
{
        return (time1 + time0) % ARAS_TIME_WEEK;
}

/**
 * This function returns the difference of two week times in a weekly cyclic
 * way.
 *
 * @param   time1   Arbitrary time
 * @param   time0   Base time
 *
 * @return  The difference of two week times in a weekly cyclic way
 */
long int aras_time_difference(long int time1, long int time0)
{
        if (time1 >= time0)
                return (time1 - time0) % ARAS_TIME_WEEK;
        else
                return ARAS_TIME_WEEK + (time1 - time0) % ARAS_TIME_WEEK;
}

/**
 * This function returns a boolean value indicating if an arbitrary time belongs
 * to a given time window.
 *
 * @param   time1   Arbitrary time
 * @param   time0   Base time
 * @param   window  Length of a time window starting in time0
 *
 * @return  1 if time1 belongs to the window, 0 if time1 does not belong to the
 *          window
 */
int aras_time_reached(long int time1, long int time0, long int window)
{
        if (aras_time_difference(time1, time0) <= window)
                return 1;
        else
                return 0;
}

void aras_time_convert(long int time0, int *hours, int *minutes, int *seconds)
{
        ldiv_t division_hours;
        ldiv_t division_minutes;
        ldiv_t division_seconds;

        division_hours = ldiv(time0, ARAS_TIME_HOUR);
        division_minutes = ldiv(division_hours.rem, ARAS_TIME_MINUTE);
        division_seconds = ldiv(division_minutes.rem, ARAS_TIME_SECOND);

        *hours = division_hours.quot;
        *minutes = division_minutes.quot;
        *seconds = division_seconds.quot;
}
