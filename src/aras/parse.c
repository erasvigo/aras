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
 * Source file for the ARAS Radio Automation System. Functions for the parse
 * module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <aras/parse.h>

/**
 * This function receives a string, a buffer and a delimiter and it copies in
 * the buffer the subsequent string characters until the delimiter.
 *
 * @param   str     The pointer to the input string
 * @param   buf     The pointer to the buffer where the field is copied
 * @param   size    The buffer size
 *
 * @return  A pointer to the inmediately subsequent character in the string if
 *          delimiter found
 *          A pointer to NULL if delimiter not found
 */
char *aras_parse_copy_to_delimiter(char *str, char *buf, int size, char *delimiter)
{
        char *marker;
        int length;

        if ((marker = strpbrk(str, delimiter)) != NULL) {
                length = marker - str + 1;
                length = (length <= size) ? length : size;
                snprintf(buf, length, "%s", str);
                return marker + 1;
        } else {
                snprintf(buf, size, "%s", str);
                return str + strlen(str);
        }
}

/**
 * This function receives a pointer to a string and a pointer to a buffer, it
 * looks for the next field in the string, if found, it copies the field to the
 * buffer and returns a pointer to the inmediately subsequent character in the
 * string, if not found, it returns a null pointer.
 *
 * @param   str     The pointer to the input string
 * @param   buf     The pointer to the buffer where the field is copied
 * @param   size    The buffer size
 *
 * @return  If a field is found, it returns a pointer to the inmediately
 *          subsequent character in the string
 *          If a field is not found, it returns a null pointer
 */
char *aras_parse_line_configuration(char *str, char *buf, int size)
{
        /* Return if an input argument is NULL */
        if ((str == NULL) || (buf == NULL))
                return NULL;

        /* Jump over leading blank characters */
        while (isblank(*str))
                str++;

        /* Copy the field in the buffer */
        switch (*str) {
        case '#':
                snprintf(buf, size, "%s","");
                return NULL;
                break;
        case '\n':
                snprintf(buf, size, "%s","");
                return NULL;
                break;
        case '\0':
                snprintf(buf, size, "%s","");
                return NULL;
                break;
        case '"':
                return aras_parse_copy_to_delimiter(str + 1, buf, size, "\"");
                break;
        case '\'':
                return aras_parse_copy_to_delimiter(str + 1, buf, size, "\'");
                break;
        case '(':
                return aras_parse_copy_to_delimiter(str + 1, buf, size, ")");
                break;
        default:
                return aras_parse_copy_to_delimiter(str, buf, size, "\n \t");
                break;
        }
}

/**
 * This function receives a pointer to a string and a pointer to a buffer, it
 * looks for the only field in the string, if found, it copies the field to the
 * buffer and returns a pointer to the buffer.
 *
 * @param   str     The pointer to the input string
 * @param   buf     The pointer to the buffer where the field is copied
 * @param   size    The buffer size
 *
 * @return  If a field is found, it returns a pointer to the inmediately
 *          subsequent character in the string
 *          If a field is not found, it returns a null pointer
 */
char *aras_parse_line_m3u(char *str, char *buf, int size)
{
        /* Return if an input argument is NULL */
        if ((str == NULL) || (buf == NULL))
                return NULL;

        /* Jump over leading blank characters */
        while (isblank(*str))
                str++;

        /* Copy the field in the buffer */
        switch (*str) {
        case '#':
                snprintf(buf, size, "%s","");
                return NULL;
                break;
        case '\n':
                snprintf(buf, size, "%s","");
                return NULL;
                break;
        case '\0':
                snprintf(buf, size, "%s","");
                return NULL;
                break;
        case '"':
                return aras_parse_copy_to_delimiter(str + 1, buf, size, "\"");
                break;
        case '\'':
                return aras_parse_copy_to_delimiter(str + 1, buf, size, "\'");
                break;
        default:
                return aras_parse_copy_to_delimiter(str, buf, size, "\n");
                break;
        }
}
