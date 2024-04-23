/* EINA - EFL data type library
 * Copyright (C) 2021 Vincent Torri
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

/*
 * LICENSE: MIT, see COPYRIGHT file in the directory
 */

/*
 * An implementation of what I call the "Sea of Stars" algorithm for
 * POSIX fnmatch(). The basic idea is that we factor the pattern into
 * a head component (which we match first and can reject without ever
 * measuring the length of the string), an optional tail component
 * (which only exists if the pattern contains at least one star), and
 * an optional "sea of stars", a set of star-separated components
 * between the head and tail. After the head and tail matches have
 * been removed from the input string, the components in the "sea of
 * stars" are matched sequentially by searching for their first
 * occurrence past the end of the previous match.
 *
 * - Rich Felker, April 2012
 */

#ifndef	EINA_FNMATCH_H
#define	EINA_FNMATCH_H

typedef enum
{
   EINA_FNMATCH_PATHNAME    = (1 << 0),
   EINA_FNMATCH_NOESCAPE    = (1 << 1),
   EINA_FNMATCH_PERIOD      = (1 << 2),
   EINA_FNMATCH_LEADING_DIR = (1 << 3),
   EINA_FNMATCH_CASEFOLD    = (1 << 4),
   EINA_FNMATCH_FILE_NAME   = EINA_FNMATCH_PATHNAME,
   EINA_FNMATCH_NOMATCH = 1,
   EINA_FNMATCH_NOSYS   = -1,
} Eina_Fnmatch_Flags;

EINA_API Efl_Bool eina_fnmatch(const char *glob, const char *string, Eina_Fnmatch_Flags flags);

#endif
