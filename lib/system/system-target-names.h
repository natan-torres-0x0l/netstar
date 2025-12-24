#ifndef _SYSTEM_SYSTEM_NAMES_H
#define _SYSTEM_SYSTEM_NAMES_H

#if defined _AIX || defined __TOS_AIX__
  #define SYSTEM_TARGET_NAME "AIX"

#elif defined __ANDROID__
  #define SYSTEM_TARGET_NAME "Android"

#elif defined UTS
  #define SYSTEM_TARGET_NAME "Amdahl UTS"

#elif defined AMIGA || defined __amigaos__
  #define SYSTEM_TARGET_NAME "AmigaOS"

#elif defined aegis
  #define SYSTEM_TARGET_NAME "Apollo AEGIS"

#elif defined apollo
  #define SYSTEM_TARGET_NAME "Apollo Domain/OS"

#elif defined __BEOS__
  #define SYSTEM_TARGET_NAME "BeOS"

#elif defined __bg__ || defined __THW_BLUEGENE__
  #define SYSTEM_TARGET_NAME "Blue Gene"

#elif defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__ || defined __bsdi__ || defined __DragonFly__ || defined _SYSTYPE_BSD
  #define SYSTEM_TARGET_NAME "FreeBSD"

#elif defined __bsdi__
  #define SYSTEM_TARGET_NAME "BSD/OS"

#elif defined __convex__
  #define SYSTEM_TARGET_NAME "ConvexOS"

#elif defined __CYGWIN__
  #defien _SYSTEM_S "Cygwin Environment"

#elif defined DGUX || defined __DGUX__ || defined __dgux__
  #define SYSTEM_TARGET_NAME "DG/UX"

#elif defined __DragonFly__
  #define SYSTEM_TARGET_NAME "DragonFly"

#elif defined _SEQUENT_ || defined sequent
  #define SYSTEM_TARGET_NAME "DYNIX/ptx"

#elif defined __ECOS
  #define SYSTEM_TARGET_NAME "eCos"

#elif defined __EMX__
  #define SYSTEM_TARGET_NAME "EMX Environment"

#elif defined __FreeBSD__ || defined __FreeBSD_kernel__
  #define SYSTEM_TARGET_NAME "FreeBSD"

#elif defined __sun
  #define SYSTEM_TARGET_NAME "Solaris"

#elif defined __GNU__ || defined __gnu_hurd__
  #define SYSTEM_TARGET_NAME "GNU aka GNU/Hurd"

#elif defined __FreeBSD_kernel__ && __GLIBC__
  #define SYSTEM_TARGET_NAME "GNU/kFreeBSD"

#elif defined __gnu_linux__
  #define SYSTEM_TARGET_NAME "GNU/Linux"

#elif defined __hiuxmpp
  #define SYSTEM_TARGET_NAME "HI-UX MPP"

#elif defined _hpux || defined hpux || defined __hpux
  #define SYSTEM_TARGET_NAME "HP-UX"

#elif defined __OS400__
  #define SYSTEM_TARGET_NAME "IBM OS/400"

#elif defined __INTEGRITY
  #define SYSTEM_TARGET_NAME "INTEGRITY"

#elif defined __INTERIX
  #define SYSTEM_TARGET_NAME "Interix Environment"

#elif defined sgi || defined __sgi
  #define SYSTEM_TARGET_NAME "IRIX"

#elif defined __linux__ || defined __linux
  #define SYSTEM_TARGET_NAME "Linux"

#elif defined __Lynx__
  #define SYSTEM_TARGET_NAME "LynxOS"

#elif defined __APPLE__ && defined __MACH__
  #define SYSTEM_TARGET_NAME "Apple"

#elif defined __OS9000 || defined _OSK
  #define SYSTEM_TARGET_NAME "Microware OS-9"

#elif defined __minix
  #define SYSTEM_TARGET_NAME "MINIX"

#elif defined __MORPHOS__
  #define SYSTEM_TARGET_NAME "MorphOS"

#elif defined mpeix || defined __mpexl
  #define SYSTEM_TARGET_NAME "MPE/iX"

#elif defined MSDOS || defined __MSDOS__ || defined _MSDOS || defined __DOS__
  #define SYSTEM_TARGET_NAME "MSDOS"

#elif defined __NetBSD__
  #define SYSTEM_TARGET_NAME "NetBSD"

#elif defined __TANDEM
  #define SYSTEM_TARGET_NAME "NonStop"

#elif defined __nucleus__
  #define SYSTEM_TARGET_NAME "Nucleus RTOS"

#elif defined __OpenBSD__
  #define SYSTEM_TARGET_NAME "OpenBSD"

#elif defined OS2 || defined _OS2 || defined __OS2__ || defined __TOS_OS2__
  #define SYSTEM_TARGET_NAME "OS/2"

#elif defined __palmos__
  #define SYSTEM_TARGET_NAME "Palm OS"

#elif defined EPLAN9
  #define SYSTEM_TARGET_NAME "Plan 9"

#elif defined pyr
  #define SYSTEM_TARGET_NAME "Pyramid DC/OSx"

#elif defined __QNX__ || defined __QNXNTO__
  #define SYSTEM_TARGET_NAME "QNX"

#elif defined sinux
  #define SYSTEM_TARGET_NAME "Reliant UNIX"

#elif defined M_I386 || defined M_XENIX || defined _SCO_DS
  #define SYSTEM_TARGET_NAME "SCO OpenServer"

#elif defined sun || defined __sun
  #define SYSTEM_TARGET_NAME "Solaris"

#elif defined __VOS__
  #define SYSTEM_TARGET_NAME "Stratus VOS"

#elif defined __sysv__ || defined __SVR4 || defined __svr4__ || defined _SYSTYPE_SVR4
  #define SYSTEM_TARGET_NAME "SVR4 Environment"

#elif defined __SYLLABLE__
  #define SYSTEM_TARGET_NAME "Syllable"

#elif defined __SYMBIAN32__
  #define SYSTEM_TARGET_NAME "Symbian OS"

#elif defined __osf__ || defined __osf
  #define SYSTEM_TARGET_NAME "Tru64 (OSF/1)"

#elif defined ultrix || defined __ultrix || defined __ultrix__ || defined unix && defined vax
  #define SYSTEM_TARGET_NAME "Ultrix"

#elif defined _UNICOS
  #define SYSTEM_TARGET_NAME "UNICOS"

#elif defined _CRAY
  #define SYSTEM_TARGET_NAME "UNICOS/mp"

#elif defined __unix || defined __unix__
  #define SYSTEM_TARGET_NAME "Unix"

#elif defined _UNIXWARE7 || defined sco
  #define SYSTEM_TARGET_NAME "UnixWare"

#elif defined _UWIN
  #define SYSTEM_TARGET_NAME "U/Win Environment"

#elif defined VMS || defined __VMS
  #define SYSTEM_TARGET_NAME "VMS"

#elif defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #define SYSTEM_TARGET_NAME "Windows"

#elif defined _WIN32_WCE
  #define SYSTEM_TARGET_NAME "Windows CE"

#elif defined _WINDU_SOURCE
  #define SYSTEM_TARGET_NAME "Wind/U Environment"

#elif defined __MVS__ || defined __HOS_MVS__ || defined __TOS_MVS__
  #define SYSTEM_TARGET_NAME "z/OS"

#else
  #define SYSTEM_TARGET_NAME "Unknown"

#endif

#endif
