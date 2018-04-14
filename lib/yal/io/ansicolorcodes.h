/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
 *
 *  This file is part of YAL.
 *
 *  YAL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  YAL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with YAL. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// Begin/end sections
#define YAL_CLRCD_RESET         "\033[0m"
#define YAL_CLRCD_BEGIN         "\033["
#define YAL_CLRCD_END           "m"
#define YAL_CLRCD_CONCAT        ";"

// Text formaters
#define YAL_CLRCD_BOLD          "1"
#define YAL_CLRCD_ITALIC        "2"
#define YAL_CLRCD_UNDERLINE     "4"

// Text colors
#define YAL_CLRCD_COLOR_BLACK   "30"
#define YAL_CLRCD_COLOR_RED     "31"
#define YAL_CLRCD_COLOR_GREEN   "32"
#define YAL_CLRCD_COLOR_YELLOW  "33"
#define YAL_CLRCD_COLOR_BLUE    "34"
#define YAL_CLRCD_COLOR_MAGENTA "35"
#define YAL_CLRCD_COLOR_CYAN    "36"
#define YAL_CLRCD_COLOR_WHITE   "37"

// Background colours
#define YAL_CLRCD_BG_BLACK      "40"
#define YAL_CLRCD_BG_RED        "41"
#define YAL_CLRCD_BG_GREEN      "42"
#define YAL_CLRCD_BG_YELLOW     "43"
#define YAL_CLRCD_BG_BLUE       "44"
#define YAL_CLRCD_BG_MAGENTA    "45"
#define YAL_CLRCD_BG_CYAN       "46"
#define YAL_CLRCD_BG_WHITE      "47"
