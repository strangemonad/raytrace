/*
 * defs.hpp
 * A4
 *
 * Global defines for things like logging etc.
 *
 * Created by Shawn Morel on 6/24/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */


#ifndef DEFS_HPP
#define DEFS_HPP

#include <iostream>

#include "stats.hpp"

// Uncomment the following line to enable debugging messages
// #define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#  define DEBUG(x) do { std::cerr << x << std::endl; } while (0)
#else
#  define DEBUG(x) do { } while (0)
#endif


// Uncomment the following line to enable error messages
#define ENABLE_WARN
#ifdef ENABLE_WARN
#  define WARN(x) do { std::cerr << x << std::endl; } while (0)
#else
#  define WARN(x) do { } while (0)
#endif


#define ENABLE_PANIC
#ifdef ENABLE_PANIC
#define PANIC(x) do { std::cerr << x << std::endl              \
                      << "Terminating execution" << std::endl; \
                      exit(-1); } while (0)
#else
#define PANIC(x) do { } while(0)
#endif


#define ENABLE_ASSERT
#ifdef ENABLE_ASSERT

#define ASSERT(condition)                                       \
{                                                               \
   if(!(condition))                                             \
   {                                                            \
      Stats::sharedStats().exit();                              \
      std::cerr << "ASSERT reached in function: "               \
                << __FUNCTION__ << std::endl                    \
                << __FILE__ << " : " << __LINE__ << std::endl;  \
      exit(-1);                                                 \
   }                                                            \
}

#define ASSERT_NOT_REACHED                                      \
{                                                               \
   Stats::sharedStats().exit();                                 \
   std::cerr << "ASSERT reached in function: "                  \
   << __FUNCTION__ << std::endl                                 \
   << __FILE__ << " : " << __LINE__ << std::endl;               \
   exit(-1);                                                    \
}

#else
#define ASSERT(condition) do { } while (0)
#define ASSERT_NOT_REACHED do { } while (0)
#endif
                         
#endif // DEFS_HPP
