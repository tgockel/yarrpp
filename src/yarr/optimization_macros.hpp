/** \file
 *  Various macros that are useful for optimizing code.
**/
#pragma once

#include <yarr/config.hpp>

#ifndef YARR_ALWAYS_INLINE
#   define YARR_ALWAYS_INLINE __attribute__((always_inline))
#endif
