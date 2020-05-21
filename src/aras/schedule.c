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
 * Source file for the ARAS Radio Automation System. Functions for the schedule
 * module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <glib.h>
#include <aras/parse.h>
#include <aras/time.h>
#include <aras/schedule.h>

/**
 * This function receives a string containing a week day and returns the number
 * of miliseconds from Sunday midnight to this week day midnight
 *
 * @param   day String containing the week day
 *
 * @return  The week time in miliseconds if success, -1 if error
 */
long int aras_schedule_convert_day(char *day)
{
        if (!strcasecmp(day, "sunday"))
                return ARAS_TIME_SUNDAY * ARAS_TIME_DAY;
        else if (!strcasecmp(day, "monday"))
                return ARAS_TIME_MONDAY * ARAS_TIME_DAY;
        else if (!strcasecmp(day, "tuesday"))
                return ARAS_TIME_TUESDAY * ARAS_TIME_DAY;
        else if (!strcasecmp(day, "wednesday"))
                return ARAS_TIME_WEDNESDAY * ARAS_TIME_DAY;
        else if (!strcasecmp(day, "thursday"))
                return ARAS_TIME_THURSDAY * ARAS_TIME_DAY;
        else if (!strcasecmp(day, "friday"))
                return ARAS_TIME_FRIDAY * ARAS_TIME_DAY;
        else if (!strcasecmp(day, "saturday"))
                return ARAS_TIME_SATURDAY * ARAS_TIME_DAY;
        else
                return -1;
}

/**
 * This function receives a string containing a time in format hh:mm:ss and
 * returns the number of miliseconds from midnight to this time
 *
 * @param   day String containing the week day
 *
 * @return  The day time in miliseconds if success, -1 if error
 */
long int aras_schedule_convert_time(char *time)
{
        int hours;
        int minutes;
        int seconds;

        /* Get hours, minutes and seconds */
        sscanf(time, "%d:%d:%d", &hours, &minutes, &seconds);

        /* Check time data */
        if (hours < 0 || hours > 23)
                return -1;

        if (minutes < 0 || minutes > 59)
                return -1;

        if (seconds < 0 || seconds > 59)
                return -1;

        /* Return time in miliseconds */
        return ARAS_TIME_HOUR * hours + ARAS_TIME_MINUTE * minutes + ARAS_TIME_SECOND * seconds;
}

/**
 * This function receives a string containing a week day and a string containing
 * a time, and returns the number of miliseconds from Sunday midnight to this
 * day and time.
 *
 * @param   day     String containing the week day
 * @param   time    String containing the day time
 *
 * @return  The week time in miliseconds if success, -1 if error
 */
long int aras_schedule_convert_day_time(char *day, char *time)
{
        long int miliseconds_day;
        long int miliseconds_time;

        if ((miliseconds_day = aras_schedule_convert_day(day)) == -1)
                return -1;

        if ((miliseconds_time = aras_schedule_convert_time(time)) == -1)
                return -1;

        /* Return time in seconds */
        return miliseconds_day + miliseconds_time;
}

/**
 * This function sets the time field in a schedule structure.
 *
 * @param   schedule    Pointer to the schedule structure
 * @param   time        Time value
 */
void aras_schedule_node_set_time(struct aras_schedule_node *node, long int time)
{
        node->time = time;
}

/**
 * This function sets the block_name field in a schedule structure.
 *
 * @param   schedule    Pointer to the schedule structure
 * @param   block_name  Pointer to the block name string
 */
void aras_schedule_node_set_block_name(struct aras_schedule_node *node, char *block_name)
{
        snprintf(node->block_name, sizeof(node->block_name), "%s", block_name);
}

/**
 * This function loads in a schedule structure the data contained in a set of
 * strings.
 *
 * @param   schedule    Pointer to the schedule structure
 * @param   day         Pointer to the day name string
 * @param   time        Pointer to the type string
 * @param   block_name  Pointer to the block name string
 *
 * @return  0 if success, -1 if error
 */
int aras_schedule_load_data(struct aras_schedule *schedule, char *day, char *time, char *block_name)
{
        long int node_time;
        struct aras_schedule_node *node;

        /* Convert time */
        if ((node_time = aras_schedule_convert_day_time(day, time)) == -1)
                return -1;

        /* Create a node to be linked to the schedule list */
        if ((node = g_malloc(sizeof(struct aras_schedule_node))) == NULL)
                return -1;

        /* Fill the fields in the node */
        aras_schedule_node_set_time(node, node_time);
        aras_schedule_node_set_block_name(node, block_name);

        /* Add the node to the schedule list */
        schedule->list = g_list_prepend(schedule->list, node);

        return 0;
}

/**
 * This function loads in a schedule structure the data contained in a line.
 *
 * @param   schedule    Pointer to the schedule structure
 * @param   line        Pointer to the line
 *
 * @return  0 if success, -1 if error
 */
int aras_schedule_load_line(struct aras_schedule *schedule, char *line)
{
        char day[ARAS_SCHEDULE_MAX_DAY];
        char time[ARAS_SCHEDULE_MAX_TIME];
        char block_name[ARAS_SCHEDULE_MAX_BLOCK_NAME];

        /* Get day, hour and block */
        if ((line = aras_parse_line_configuration(line, day, sizeof(day))) == NULL)
                return -1;

        if ((line = aras_parse_line_configuration(line, time, sizeof(time))) == NULL)
                return -1;

        if ((line = aras_parse_line_configuration(line, block_name, sizeof(block_name))) == NULL)
                return -1;

        aras_schedule_load_data(schedule, day, time, block_name);

        return 0;
}

/**
 * This function loads in a schedule structure the data contained in a file.
 *
 * @param   schedule    Pointer to the schedule structure
 * @param   file        Pointer to the file name string
 *
 * @return  0 if success, -1 if error
 */
int aras_schedule_load_file(struct aras_schedule *schedule, char *file)
{
        FILE *fp;
        char line[ARAS_SCHEDULE_MAX_LINE];

        /* Open schedule file */
        if ((fp = fopen(file, "r")) == NULL)
                return -1;

        /* Get lines from schedule file */
        while (fgets(line, ARAS_SCHEDULE_MAX_LINE, fp) != NULL)
                aras_schedule_load_line(schedule, line);

        /* Reverse the schedule list */
        schedule->list = g_list_reverse(schedule->list);

        /* Close schedule file */
        if (fclose(fp) != 0)
                return -1;

        return 0;
}

/**
 * This function initializes a schedule structure.
 *
 * @param   block   Pointer to the schedule structure
 *
 * @return  This function always returns 0
 */
int aras_schedule_init(struct aras_schedule *schedule)
{
        /* Clean structure */
        memset(schedule, 0, sizeof(*schedule));

        /* Schedule list initialization */
        schedule->list = NULL;
        return 0;
}


/**
 * This function receives a schedule structure and frees the schedule list.
 *
 * @param   schedule    Pointer to the schedule structure
 *
 * @return  This function always returns 0
 */
int aras_schedule_list_free(struct aras_schedule *schedule)
{
        g_list_free_full(schedule->list, g_free);
        schedule->list = NULL;

        return 0;
}

/**
 * This function receives a schedule structure and a time and returns a pointer
 * to the current node in the schedule in relation to the time
 *
 * @param   schedule    Pointer to the schedule structure
 * @param   time        Time in relation to which the next node is sought
 *
 * @return  A pointer to the current node in the schedule in relation to the
 *          time
 */
struct aras_schedule_node *aras_schedule_seek_node_current(struct aras_schedule *schedule, long int time)
{
        GList *pointer;
        struct aras_schedule_node *node;
        struct aras_schedule_node *next_node;

        if (schedule == NULL)
                return NULL;

        pointer = schedule->list;
        next_node = pointer->data;

        while (pointer != NULL) {
                node = pointer->data;
                if (aras_time_difference(time, node->time) < aras_time_difference(time, next_node->time))
                        next_node = node;
                pointer = pointer->next;
        }

        return (struct aras_schedule_node *)next_node;
}

/**
 * This function receives a schedule structure and a time and returns a pointer
 * to the next node in the schedule in relation to the time
 *
 * @param   schedule    Pointer to the schedule structure
 * @param   time        Time in relation to which the next node is sought
 *
 * @return  A pointer to the next node if next node found
 *          A pointer to NULL if next node not found
 */
struct aras_schedule_node *aras_schedule_seek_node_next(struct aras_schedule *schedule, long int time)
{
        GList *pointer;
        struct aras_schedule_node *node;
        struct aras_schedule_node *next_node;
        long int time_difference;

        if (schedule == NULL)
                return NULL;

        pointer = schedule->list;
        next_node = pointer->data;

        while (pointer != NULL) {
                node = pointer->data;
                time_difference = aras_time_difference(node->time, time);
                if (time_difference < aras_time_difference(next_node->time, time) &&
                    time_difference > 0)
                                next_node = node;
                pointer = pointer->next;
        }

        return (struct aras_schedule_node *)next_node;
}

/**
 * This function receives a schedule structure and prints the schedule list.
 *
 * @param   schedule    Pointer to the schedule structure
 */
void aras_schedule_print(struct aras_schedule *schedule)
{
        GList *pointer;
        struct aras_schedule_node *node;

        printf("Schedule list\n");
        printf("-------------\n");

        pointer = schedule->list;
        while (pointer != NULL) {
                node = pointer->data;
                printf("%s %ld\n", node->block_name, node->time);
                pointer = pointer->next;
        }

        printf("\n");
        fflush(stdout);
}
