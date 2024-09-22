/*
 * This file is part of Text Drawing Library.
 *
 * Text Drawing Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Text Drawing Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Text Drawing Library. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef TDL_VERSION_H
#define TDL_VERSION_H

#include "tdl/tdl_symbols_export.h"
#include <stdint.h>

/*
 * The byte order in the version number:
 *   - Empty byte
 *   - Major version
 *   - Minor version
 *   - Patch number
 */

#define TDL_VERSION 0x00010102

TDL_API int32_t tdl_get_version (void);

#endif /* TDL_VERSION_H */
