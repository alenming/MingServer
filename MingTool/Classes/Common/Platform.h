#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#define MING_PLATFORM_UNKNOWN 0  
#define MING_PLATFORM_WIN32 1
#define MING_PLATFORM_LINUX 2
#define MING_PLATFORM_MAC 3

#define MING_TARGET_PLATFORM  MING_PLATFORM_UNKNOWN

// mac
#if defined(__APPLE__)
#undef  MING_TARGET_PLATFORM
#define MING_TARGET_PLATFORM KX_PLATFORM_MAC
#endif

// win32
#if defined(_WIN32)
	#undef  MING_TARGET_PLATFORM
	#define MING_TARGET_PLATFORM        MING_PLATFORM_WIN32
#endif

// linux
#if defined(linux)
	#undef  MING_TARGET_PLATFORM
	#define MING_TARGET_PLATFORM         MING_PLATFORM_LINUX
#endif

#endif
