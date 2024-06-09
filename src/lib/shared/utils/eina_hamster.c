/* EINA - EFL data type library
 * Copyright (C) 2008 Cedric Bail
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

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <string.h>

#include "shared_config.h"
#include "eina_types.h"
#include "eina_hamster.h"

/*============================================================================*
*                                  Local                                     *
*============================================================================*/

/**
 * @cond LOCAL
 */

static const int _eina_hamster = (EFL_VERSION_MAJOR * 100 * 100 * 100) +
                                 (EFL_VERSION_MINOR * 100 * 100) +
                                 (EFL_VERSION_MICRO * 100);

/**
 * @endcond
 */

/*============================================================================*
*                                 Global                                     *
*============================================================================*/

/*============================================================================*
*                                   API                                      *
*============================================================================*/

EINA_API int
eina_hamster_count(void)
{
    return _eina_hamster;
}

/**
 * @}
 */
