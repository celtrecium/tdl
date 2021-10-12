
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


#ifndef TDL_SYMBOLS_EXPORT_H
#define TDL_SYMBOLS_EXPORT_H

#if defined(_WIN32) || defined(__CYGWIN__)
# if defined(_MSC_VER)
#  define TDL_API __declspec(dllexport)
# elif (defined(__GNUC__) && (__GNUC__ >= 4))
#  define TDL_API __attribute__((dllexport))
# else
#  define TDL_API
# endif /* _MSC_VER */
#elif defined(__unix__)
# if (defined(__GNUC__) && (__GNUC__ >= 4))
#  define TDL_API __attribute__((visibility("default")))
# else
#  define TDL_API
# endif
#else
# define TDL_API
#endif /* _WIN32 || __CYGWIN__ */

#endif  /* TDL_SYMBOLS_EXPORT_H */
