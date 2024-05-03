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
 * Header file for the ARAS Radio Automation System. Types and definitions for
 * the main daemon module.
 */

#ifndef _ARAS_MAIN_DAEMON_H
#define _ARAS_MAIN_DAEMON_H

#include <aras/configuration.h>
#include <aras/schedule.h>
#include <aras/block.h>
#include <aras/engine.h>

struct aras_main_daemon {
        char *configuration_file;
        struct aras_configuration configuration;
        struct aras_schedule schedule;
        struct aras_block block;
        struct aras_engine engine_block_player;
        struct aras_engine engine_time_signal_player;
        struct aras_player block_player;
        struct aras_player time_signal_player;
};

#endif  /* _ARAS_MAIN_DAEMON_H */
