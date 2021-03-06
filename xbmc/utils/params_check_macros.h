/*
 *  Copyright (C) 2014-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// macros for gcc, clang & others
#ifndef PARAM1_PRINTF_FORMAT
#ifdef __GNUC__
// for use in functions that take printf format string as first parameter and additional printf parameters as second parameter
// for example: int myprintf(const char* format, ...) PARAM1_PRINTF_FORMAT;
#define PARAM1_PRINTF_FORMAT __attribute__((format(printf,1,2)))

// for use in functions that take printf format string as second parameter and additional printf parameters as third parameter
// for example: bool log_string(int logLevel, const char* format, ...) PARAM2_PRINTF_FORMAT;
// note: all non-static class member functions take pointer to class object as hidden first parameter
#define PARAM2_PRINTF_FORMAT __attribute__((format(printf,2,3)))

// for use in functions that take printf format string as third parameter and additional printf parameters as fourth parameter
// note: all non-static class member functions take pointer to class object as hidden first parameter
// for example: class A { bool log_string(int logLevel, const char* functionName, const char* format, ...) PARAM3_PRINTF_FORMAT; }
#define PARAM3_PRINTF_FORMAT __attribute__((format(printf,3,4)))

// for use in functions that take printf format string as fourth parameter and additional printf parameters as fifth parameter
// note: all non-static class member functions take pointer to class object as hidden first parameter
// for example: class A { bool log_string(int logLevel, const char* functionName, int component, const char* format, ...) PARAM4_PRINTF_FORMAT; }
#define PARAM4_PRINTF_FORMAT __attribute__((format(printf,4,5)))
#else  // ! __GNUC__
#define PARAM1_PRINTF_FORMAT
#define PARAM2_PRINTF_FORMAT
#define PARAM3_PRINTF_FORMAT
#define PARAM4_PRINTF_FORMAT
#endif // ! __GNUC__
#endif // PARAM1_PRINTF_FORMAT

// macros for VC
// VC check parameters only when "Code Analysis" is called
#ifndef PRINTF_FORMAT_STRING
#ifdef _MSC_VER
#include <sal.h>

// for use in any function that take printf format string and parameters
// for example: bool log_string(int logLevel, PRINTF_FORMAT_STRING const char* format, ...);
#define PRINTF_FORMAT_STRING _In_z_ _Printf_format_string_

// specify that parameter must be zero-terminated string
// for example: void SetName(IN_STRING const char* newName);
#define IN_STRING _In_z_

// specify that parameter must be zero-terminated string or NULL
// for example: bool SetAdditionalName(IN_OPT_STRING const char* addName);
#define IN_OPT_STRING _In_opt_z_
#else  // ! _MSC_VER
#define PRINTF_FORMAT_STRING
#define IN_STRING
#define IN_OPT_STRING
#endif // ! _MSC_VER
#endif // PRINTF_FORMAT_STRING
