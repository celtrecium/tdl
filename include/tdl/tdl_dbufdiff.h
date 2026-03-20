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

#ifndef TDL_DBUFDIFF_H
#define TDL_DBUFDIFF_H

#include "tdl/tdl_rowdiff.h"

typedef tdl_rowdiff_t *tdl_dbufdiff_t;

tdl_dbufdiff_t tdl_dbufdiff (void);
bool tdl_dbufdiff_set (tdl_dbufdiff_t *dbdiff, tdl_rowdiff_t rdiff);
bool tdl_dbufdiff_clear (tdl_dbufdiff_t dbdiff);
size_t tdl_dbufdiff_size (tdl_dbufdiff_t dbdiff);
bool tdl_dbufdiff_free (tdl_dbufdiff_t dbdiff);

#endif /* TDL_DBUFDIFF_H */
