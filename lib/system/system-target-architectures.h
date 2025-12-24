#ifndef _SYSTEM_SYSTEM_ARCHITECTURE_H
#define _SYSTEM_SYSTEM_ARCHITECTURE_H

#if defined __alpha__ || defined __alpha || defined _M_ALPHA
  #if defined __alpha_ev4__
    #define SYSTEM_TARGET_ARCHITECTURE "Alpha EV4"

  #elif defined __alpha_ev5__
    #define SYSTEM_TARGET_ARCHITECTURE "Alpha EV5"

  #elif defined __alpha_ev6__
    #define SYSTEM_TARGET_ARCHITECTURE "Alpha EV6"

  #else
    #define SYSTEM_TARGET_ARCHITECTURE "Alpha"

  #endif

#elif defined __amd64__ || defined __amd64 || defined __x86_64__ || defined __x86_64 || defined _M_AMD64
  #define SYSTEM_TARGET_ARCHITECTURE "AMD64"

#elif defined __arm__ || defined __thumb__ || defined __TARGET_ARCH_ARM || defined __TARGET_ARCH_THUMB || defined _ARM || defined _M_ARM || defined _M_ARMT || defined __arm
  #if defined __ARM_ARCH_2__
    #define SYSTEM_TARGET_ARCHITECTURE "ARM 2"

  #elif defined __ARM_ARCH_3__ || defined __ARM_ARCH_3M__
    #define SYSTEM_TARGET_ARCHITECTURE "ARM 3"
	
  #elif defined __ARM_ARCH_4T__ || defined __TARGET_ARM_4T
    #define SYSTEM_TARGET_ARCHITECTURE "ARM 4T"
	
  #elif defined __ARM_ARCH_5__ || defined __ARM_ARCH_5E__
    #define SYSTEM_TARGET_ARCHITECTURE "ARM 5"

  #elif defined __ARM_ARCH_5T__ || defined __ARM_ARCH_5TE__ || defined __ARM_ARCH_5TEJ__
    #define SYSTEM_TARGET_ARCHITECTURE "ARM 5T"

  #elif defined __ARM_ARCH_6__ || defined __ARM_ARCH_6J__ || defined __ARM_ARCH_6K__ || defined __ARM_ARCH_6Z__ || defined __ARM_ARCH_6ZK__
    #define SYSTEM_TARGET_ARCHITECTURE "ARM 6"

  #elif defined __ARM_ARCH_6T2__
    #define SYSTEM_TARGET_ARCHITECTURE "ARM 6T2"

  #elif defined __ARM_ARCH_7__ || defined __ARM_ARCH_7A__ || defined __ARM_ARCH_7R__ || defined __ARM_ARCH_7M__ || defined __ARM_ARCH_7S__
    #define SYSTEM_TARGET_ARCHITECTURE "ARM 7"

  #else
    #define SYSTEM_TARGET_ARCHITECTURE "ARM"

  #endif

#elif defined __aarch64__
  #define SYSTEM_TARGET_ARCHITECTURE "ARM64"

#elif defined __bfin || defined __BFIN__
  #define SYSTEM_TARGET_ARCHITECTURE "Blackfin"

#elif defined __convex__
  #if defined __convex_c1__
    #define SYSTEM_TARGET_ARCHITECTURE "Convex C1"

  #elif defined __convex_c2__
    #define SYSTEM_TARGET_ARCHITECTURE "Convex C2"

  #elif defined __convex_c32__
    #define SYSTEM_TARGET_ARCHITECTURE "Convex C32" // C32XX series

  #elif defined __convex_c34__
    #define SYSTEM_TARGET_ARCHITECTURE "Convex C34" // C34XX series

  #elif defined __convex_c38__
    #define SYSTEM_TARGET_ARCHITECTURE "Convex C38" // C38XX series

  #else
    #define SYSTEM_TARGET_ARCHITECTURE "Convex"

  #endif

#elif defined __epiphany__
  #define SYSTEM_TARGET_ARCHITECTURE "Epiphany"

#elif defined __hppa__ || defined __HPPA__ || defined __hppa
  #if defined _PA_RISC1_0
    #define SYSTEM_TARGET_ARCHITECTURE "HP/PA RISC 1.0"

  #elif defined _PA_RISC1_1 || defined __HPPA11__ || defined __PA7100__
    #define SYSTEM_TARGET_ARCHITECTURE "HP/PA RISC 1.1"

  #elif defined _PA_RISC2_0 || defined __RISC2_0__ || defined __HPPA20__ || defined __PA8000__
    #define SYSTEM_TARGET_ARCHITECTURE "HP/PA RISC 2.0"

  #else
    #define SYSTEM_TARGET_ARCHITECTURE "HP/PA RISC"

  #endif

#elif defined i386 || defined __i386 || defined __i386__ || defined __i386 || defined __i386 || defined __IA32__ || defined _M_I86 || defined _M_IX86 || defined __X86__ || defined _X86_ || defined __THW_INTEL__ || defined __I86__ || defined __INTEL__ || defined __386
  #define SYSTEM_TARGET_ARCHITECTURE "Intel x86"

#elif defined __ia64__ || defined _IA64 || defined __IA64__ || defined __ia64 || defined _M_IA64 || defined _M_IA64 || defined __itanium__
  #define SYSTEM_TARGET_ARCHITECTURE "Intel Itanium (IA-64)"

#elif defined __m68k__ || defined M68000 || defined __MC68K__
  #if defined __mc68000__ || defined __MC68000__
    #define SYSTEM_TARGET_ARCHITECTURE "Motorola 68000"

  #elif defined __mc68010__
    #define SYSTEM_TARGET_ARCHITECTURE "Motorola 68010"

  #elif defined __mc68020__ || defined __MC68020__
    #define SYSTEM_TARGET_ARCHITECTURE "Motorola 68020"

  #elif defined __mc68030__ || defined __MC68030__
    #define SYSTEM_TARGET_ARCHITECTURE "Motorola 68030"

  #elif defined __mc68040__
    #define SYSTEM_TARGET_ARCHITECTURE "Motorola 68040"

  #elif defined __mc68060__
    #define SYSTEM_TARGET_ARCHITECTURE "Motorola 68060"

  #else
    #define SYSTEM_TARGET_ARCHITECTURE "Motorola 68K"

  #endif

#elif defined __mips__ || defined mips || defined __mips || defined __MIPS__
  #if defined _MIPS_ISA_MIPS1
    #define SYSTEM_TARGET_ARCHITECTURE "MIPS R2000"

  #elif defined _MIPS_ISA_MIPS1 || defined _R3000
    #define SYSTEM_TARGET_ARCHITECTURE "MIPS R3000"

  #elif defined _MIPS_ISA_MIPS2 || defined __MIPS_ISA2__
    #define SYSTEM_TARGET_ARCHITECTURE "MIPS R6000"

  #elif defined _R4000
    #define SYSTEM_TARGET_ARCHITECTURE "MIPS R4000"

  #elif defined _MIPS_ISA_MIPS3 || defined __MIPS_ISA3__
    #define SYSTEM_TARGET_ARCHITECTURE "MIPS R4400"

  #elif defined _MIPS_ISA_MIPS4 || defined __MIPS_ISA4__
    #define SYSTEM_TARGET_ARCHITECTURE "MIPS R8000"

  #elif defined _MIPS_ISA_MIPS4 || defined __MIPS_ISA4__
    #define SYSTEM_TARGET_ARCHITECTURE "MIPS R10000"

  #else
    #define SYSTEM_TARGET_ARCHITECTURE "MIPS"

  #endif

#elif defined __powerpc || defined __powerpc__ || defined __powerpc64__ || defined __POWERPC__ || defined __ppc__ || defined __ppc64__ || defined __PPC__ || defined __PPC64__ || defined _ARCH_PPC || defined _ARCH_PPC64 || defined _M_PPC || defined _ARCH_PPC || defined _ARCH_PPC64 || defined __ppc
  #if defined _ARCH_440
    #define SYSTEM_TARGET_ARCHITECTURE "PowerPC 440"

  #elif defined _ARCH_450
    #define SYSTEM_TARGET_ARCHITECTURE "PowerPC 450"

  #elif defined __ppc601__ || defined _ARCH_601
    #define SYSTEM_TARGET_ARCHITECTURE "PowerPC 601"

  #elif defined __ppc603__ || defined _ARCH_603
    #define SYSTEM_TARGET_ARCHITECTURE "PowerPC 603"

  #elif defined __ppc604__ || defined _ARCH_604
    #define SYSTEM_TARGET_ARCHITECTURE "PowerPC 604"

  #else
    #define SYSTEM_TARGET_ARCHITECTURE "PowerPc"

  #endif

#elif defined pyr
  #define SYSTEM_TARGET_ARCHITECTURE "Pyramid 9810"

#elif defined __THW_RS6000 || defined _IBMR2 || defined _POWER || defined _ARCH_PWR || defined _ARCH_PWR2 || defined _ARCH_PWR3 || defined _ARCH_PWR4
  #define SYSTEM_TARGET_ARCHITECTURE "RS/6000"

#elif defined __sparc__ || defined __sparc
  #if defined __sparc_v8__ || defined __sparcv8
    #define SYSTEM_TARGET_ARCHITECTURE "SPARC v8 (SuperSPARC)"

  #elif defined __sparc_v9__ || defined __sparcv9
    #define SYSTEM_TARGET_ARCHITECTURE "SPARC v9 (UltraSPARC)"

  #else
    #define SYSTEM_TARGET_ARCHITECTURE "SPARC"

  #endif

#elif defined __sh__
  #if defined __sh1__
    #define SYSTEM_TARGET_ARCHITECTURE "SuperH 1"

  #elif defined __sh2__
    #define SYSTEM_TARGET_ARCHITECTURE "SuperH 2"

  #elif defined __sh3__ || defined __SH3__
    #define SYSTEM_TARGET_ARCHITECTURE "SuperH 3"

  #elif defined __SH4__
    #define SYSTEM_TARGET_ARCHITECTURE "SuperH 4"

  #elif defined __SH5__
    #define SYSTEM_TARGET_ARCHITECTURE "SuperH 5"

  #else
    #define SYSTEM_TARGET_ARCHITECTURE "SuperH"

  #endif

#elif defined __370__ || defined __THW_370__ || defined __s390__ || defined __s390x__ || defined __zarch__ || defined __SYSC_ZARCH__
  #define SYSTEM_TARGET_ARCHITECTURE "SystemZ"

#elif defined _TMS320C2XX || defined __TMS320C2000__ || defined _TMS320C5X || defined __TMS320C55X__ || defined _TMS320C6X || defined __TMS320C6X__ || defined __TMS470__
  #if defined _TMS320C28X
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320C28XX"

  #elif defined _TMS320C5XX
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320C54X"

  #elif defined __TMS320C55X__
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320C55X"

  #elif defined _TMS320C6200
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320C6200"

  #elif defined _TMS320C6400
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320C6400"

  #elif defined _TMS320C6400_PLUS
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320C6400+"

  #elif defined _TMS320C6600
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320C6600"

  #elif defined _TMS320C6700
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320C6700"

  #elif defined _TMS320C6700_PLUS
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320C6700+"

  #elif defined _TMS320C6740
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320C6740"

  #else
    #define SYSTEM_TARGET_ARCHITECTURE "TMS320"

  #endif

#else
  #define SYSTEM_TARGET_ARCHITECTURE "Unknown"

#endif

typedef enum {
  SYSTEM_TARGET_ARCHITECTURE_ALPHA_EV4,
  SYSTEM_TARGET_ARCHITECTURE_ALPHA_EV5,
  SYSTEM_TARGET_ARCHITECTURE_ALPHA_EV6,

  SYSTEM_TARGET_ARCHITECTURE_AMD64,

  SYSTEM_TARGET_ARCHITECTURE_ARM2,
  SYSTEM_TARGET_ARCHITECTURE_ARM3,
  SYSTEM_TARGET_ARCHITECTURE_ARM4T,
  SYSTEM_TARGET_ARCHITECTURE_ARM5,
  SYSTEM_TARGET_ARCHITECTURE_ARM5T,
  SYSTEM_TARGET_ARCHITECTURE_ARM6,
  SYSTEM_TARGET_ARCHITECTURE_ARM6T2,
  SYSTEM_TARGET_ARCHITECTURE_ARM7,

  SYSTEM_TARGET_ARCHITECTURE_AARCH64,

  SYSTEM_TARGET_ARCHITECTURE_BLACKFIN,

  SYSTEM_TARGET_ARCHITECTURE_CONVEXC1,
  SYSTEM_TARGET_ARCHITECTURE_CONVEXC2,
  SYSTEM_TARGET_ARCHITECTURE_CONVEXC32XX,
  SYSTEM_TARGET_ARCHITECTURE_CONVEXC34XX,
  SYSTEM_TARGET_ARCHITECTURE_CONVEXC38XX,

  SYSTEM_TARGET_ARCHITECTURE_EPIPHANY,

  SYSTEM_TARGET_ARCHITECTURE_HPPARISC1_0,
  SYSTEM_TARGET_ARCHITECTURE_HPPARISC1_1,
  SYSTEM_TARGET_ARCHITECTURE_HPPARISC2_0,

  SYSTEM_TARGET_ARCHITECTURE_I386,
  SYSTEM_TARGET_ARCHITECTURE_X86,

  SYSTEM_TARGET_ARCHITECTURE_IA64,

  SYSTEM_TARGET_ARCHITECTURE_MC68000,
  SYSTEM_TARGET_ARCHITECTURE_MC68010,
  SYSTEM_TARGET_ARCHITECTURE_MC68020,
  SYSTEM_TARGET_ARCHITECTURE_MC68030,
  SYSTEM_TARGET_ARCHITECTURE_MC68040,
  SYSTEM_TARGET_ARCHITECTURE_MC68060,

// SYSTEM_TARGET_ARCHITECTURE_MIPS_ISA_MIPS1
  SYSTEM_TARGET_ARCHITECTURE_MIPSR2000,
// SYSTEM_TARGET_ARCHITECTURE_MIPS_ISA_MIPS1 || defined _R3000
  SYSTEM_TARGET_ARCHITECTURE_MIPSR3000,
// SYSTEM_TARGET_ARCHITECTURE_MIPS_ISA_MIPS2 || defined __MIPS_ISA2__
  SYSTEM_TARGET_ARCHITECTURE_MIPSR6000,
  SYSTEM_TARGET_ARCHITECTURE_MIPSR4000,
// SYSTEM_TARGET_ARCHITECTURE_MIPS_ISA_MIPS3 || defined __MIPS_ISA3__
  SYSTEM_TARGET_ARCHITECTURE_MIPSR4400,
// SYSTEM_TARGET_ARCHITECTURE_MIPS_ISA_MIPS4 || defined __MIPS_ISA4__
  SYSTEM_TARGET_ARCHITECTURE_MIPSR8000,
// SYSTEM_TARGET_ARCHITECTURE_MIPS_ISA_MIPS4 || defined __MIPS_ISA4__
  SYSTEM_TARGET_ARCHITECTURE_MIPSR10000,

  SYSTEM_TARGET_ARCHITECTURE_POWERPC440,
  SYSTEM_TARGET_ARCHITECTURE_PPC440,
  SYSTEM_TARGET_ARCHITECTURE_POWERPC450,
  SYSTEM_TARGET_ARCHITECTURE_PPC450,
  SYSTEM_TARGET_ARCHITECTURE_POWERPC601,
  SYSTEM_TARGET_ARCHITECTURE_PPC601,
  SYSTEM_TARGET_ARCHITECTURE_POWERPC603,
  SYSTEM_TARGET_ARCHITECTURE_PPC603,
  SYSTEM_TARGET_ARCHITECTURE_POWERPC604,
  SYSTEM_TARGET_ARCHITECTURE_PPC604,

  SYSTEM_TARGET_ARCHITECTURE_PYRAMID9810,

  SYSTEM_TARGET_ARCHITECTURE_RS6000,

  SYSTEM_TARGET_ARCHITECTURE_SPARCV8,
  SYSTEM_TARGET_ARCHITECTURE_SPARCV9,

  SYSTEM_TARGET_ARCHITECTURE_SUPERH1,
  SYSTEM_TARGET_ARCHITECTURE_SH1,
  SYSTEM_TARGET_ARCHITECTURE_SUPERH2,
  SYSTEM_TARGET_ARCHITECTURE_SH2,
  SYSTEM_TARGET_ARCHITECTURE_SUPERH3,
  SYSTEM_TARGET_ARCHITECTURE_SH3,
  SYSTEM_TARGET_ARCHITECTURE_SUPERH4,
  SYSTEM_TARGET_ARCHITECTURE_SH4,
  SYSTEM_TARGET_ARCHITECTURE_SUPERH5,
  SYSTEM_TARGET_ARCHITECTURE_SH5,

  SYSTEM_TARGET_ARCHITECTURE_SYSTEMZ,

  SYSTEM_TARGET_ARCHITECTURE_TMS320C2XX,
  SYSTEM_TARGET_ARCHITECTURE_TMS320C28X,
  SYSTEM_TARGET_ARCHITECTURE_TMS320C5XX,
  SYSTEM_TARGET_ARCHITECTURE_TMS320C55X,
  SYSTEM_TARGET_ARCHITECTURE_TMS320C6200,
  SYSTEM_TARGET_ARCHITECTURE_TMS320C6400,
  SYSTEM_TARGET_ARCHITECTURE_TMS320C6400_PLUS,
  SYSTEM_TARGET_ARCHITECTURE_TMS320C6600,
  SYSTEM_TARGET_ARCHITECTURE_TMS320C6700,
  SYSTEM_TARGET_ARCHITECTURE_TMS320C6700_PLUS,
  SYSTEM_TARGET_ARCHITECTURE_TMS320C6740,
} system_target_architecture_t;

#define SYSTEM_TARGET_ARCHITECTURE_BITNESS ((sizeof(void *) == 8) ? "64 bits" : "32 bits")

#endif
