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
 * the time module.
 */

#ifndef _ARAS_TIME_H
#define _ARAS_TIME_H

#define ARAS_TIME_SECOND    1000
#define ARAS_TIME_MINUTE    60000
#define ARAS_TIME_HOUR      3600000
#define ARAS_TIME_DAY       86400000
#define ARAS_TIME_WEEK      604800000

#define ARAS_TIME_SUNDAY    0
#define ARAS_TIME_MONDAY    1
#define ARAS_TIME_TUESDAY   2
#define ARAS_TIME_WEDNESDAY 3
#define ARAS_TIME_THURSDAY  4
#define ARAS_TIME_FRIDAY    5
#define ARAS_TIME_SATURDAY  6

long int aras_time_current(void);
long int aras_time_addition(long int time1, long int time0);
long int aras_time_difference(long int time1, long int time0);
int aras_time_reached(long int time1, long int time0, long int window);
void aras_time_convert(long int time0, int *hours, int *minutes, int *seconds);

#endif  /* _ARAS_TIME_H */
