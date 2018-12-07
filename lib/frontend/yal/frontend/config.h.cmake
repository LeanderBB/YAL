/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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

#cmakedefine YAL_FRONTEND__VERSION_MAJOR @YAL_FRONTEND__VERSION_MAJOR@

#if !defined(YAL_FRONTEND__VERSION_MAJOR)
#define YAL_FRONTEND__VERSION_MAJOR 0
#endif

#cmakedefine YAL_FRONTEND__VERSION_MINOR @YAL_FRONTEND__VERSION_MINOR@

#if !defined(YAL_FRONTEND__VERSION_MINOR)
#define YAL_FRONTEND__VERSION_MINOR 0
#endif

#cmakedefine YAL_FRONTEND__VERSION_PATCH @YAL_FRONTEND__VERSION_PATCH@

#if !defined(YAL_FRONTEND__VERSION_PATCH)
#define YAL_FRONTEND__VERSION_PATCH 0
#endif

#define YAL_FRONTEND__VERSION_STR @YAL_FRONTEND__VERSION_STR@
