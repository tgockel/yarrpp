#pragma once

/** \def YARR_USER_CONFIG
 *  \brief A user-defined configuration file to be included before all other YarrPP content.
**/
#ifdef YARR_USER_CONFIG
#   include YARR_USER_CONFIG
#endif

#define YARR_VERSION_MAJOR 0
#define YARR_VERSION_MINOR 1
#define YARR_VERSION_PATCH 0

#ifndef YARR_DEBUG
#   define YARR_DEBUG 0
#endif
