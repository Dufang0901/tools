/******************************************************************************/
/* @F_NAME:           type.h                                                  */
/* @F_PURPOSE:        Definition of basic types and define                    */
/* @F_CREATED_BY:     Vincent RIOUAL                                          */
/* @F_CREATION_DATE:  21/12/2000                                              */
/* @F_MPROC_TYPE:     NEC_V850, MC9S12xx, MC9S08xx, TX49, IMX53               */
/************************************** (C) Copyright 2011 Magneti Marelli ****/

#ifndef TYPE_H
#define TYPE_H

/*_____ I N C L U D E - F I L E S ____________________________________________*/


/*_____ G L O B A L - D E F I N E ____________________________________________*/

#define _USED_      1
#define _NOT_USED_  0

#ifndef NULL
#define NULL ((void*)0)
#endif

#define DISABLE ((ubyte) 0)
#define ENABLE  ((ubyte) 1)

#define NO  ((ubyte) 0)
#define YES ((ubyte) 1)

#define NOK   ((sbyte)  1)
#define OK    ((sbyte)  0)
#ifndef ERROR
#define ERROR ((sbyte) -1)
#endif /* ERROR */

#ifdef __TX49__
#ifndef __LITTLE_ENDIAN__
/* little endian format (Intel) */
#define __LITTLE_ENDIAN__
#endif
#endif /* __TX49__ */

#ifdef __MC9S12xx__
/* big endian format (Motorola) */
#define __BIG_ENDIAN__
#endif /* __MC9S12xx__ */

#ifdef __MC9S08xx__
/* big endian format (Motorola) */
#define __BIG_ENDIAN__
#endif /* __MC9S12xx__ */

#ifdef __NEC_V850__
/* little endian format (Intel) */
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#endif
#endif /* __NEC_V850__ */

#ifdef __FSL_IMX53x__
/* little endian format (Intel) */
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#endif
#endif /* __FSL_IMX53x__ */

#if (defined(C_COMP_COSMIC_MC9S12) || defined(C_COMP_GHS_V850) || \
     defined(C_COMP_COSMIC_MC9S08) || defined(C_COMP_GHS_TX49) || defined(C_COMP_GHS_ARM))
#define __BITFIELD_LSB_FIRST__
#endif /* C_COMP_COSMIC_MC9S12 || C_COMP_GHS_V850 ||
          C_COMP_COSMIC_MC9S08 || C_COMP_GHS_TX49 || C_COMP_GHS_ARM */

#ifdef C_COMP_GHS_TX49
#define __EEPROM__
#define __NEAR__
#define __NEAR_FUNC__
#define __FAR__
#define __SDA__
#define __INTERRUPT__
#define __INLINE__       __inline__
#define __BANKED__
#define __NON_BANKED__
#define __NO_INIT__
#define __GCONST__
#define __FDA__
#endif /* C_COMP_GHS_TX49 */

#ifdef C_COMP_COSMIC_MC9S12
#define __EEPROM__       @eeprom
#if defined(__MC9S12XHZ__)
#define __NEAR__
#define __NEAR_FUNC__    @near
#else
#define __NEAR__         @near
#define __NEAR_FUNC__    @near
#endif
#define __FAR__          @far
#define __SDA__          @dir    /* SDA : Small Data Area */
#if defined(__MC9S12XHZ__)
#define __INTERRUPT__    @svpage @interrupt
#else
#define __INTERRUPT__    @interrupt
#endif
#define __INLINE__       @inline
#define __BANKED__
#define __NON_BANKED__   @near
#define __NO_INIT__
#if defined(__MC9S12XHZ__)
#define __GCONST__       @far @gpage
#define __FDA__          @far    /* FDA : Far Data Area*/
#else
#define __GCONST__
#define __FDA__
#endif
#endif /* C_COMP_COSMIC_MC9S12 */

#ifdef C_COMP_COSMIC_MC9S08
#define __EEPROM__
#define __NEAR__         @near
#define __NEAR_FUNC__    @near
#define __FAR__
#define __SDA__          @tiny
#define __INTERRUPT__    @interrupt @near @svlreg
#define __INLINE__       @inline
#define __BANKED__
#define __NON_BANKED__
#define __NO_INIT__
#define __GCONST__
#define __FDA__
#endif /* C_COMP_COSMIC_MC9S08 */

#ifdef C_COMP_GHS_V850
#define __EEPROM__
#define __NEAR__
#define __NEAR_FUNC__
#define __FAR__
#define __SDA__
#define __INTERRUPT__    __interrupt
#define __INLINE__       __inline__
#define __BANKED__
#define __NON_BANKED__
#define __NO_INIT__
#define __GCONST__
#define __FDA__
#endif /* C_COMP_GHS_V850 */

#ifdef C_COMP_GHS_ARM
#define __EEPROM__
#define __NEAR__
#define __NEAR_FUNC__
#define __FAR__
#define __SDA__
#define __INTERRUPT__
#define __INLINE__       __inline__
#define __BANKED__
#define __NON_BANKED__
#define __NO_INIT__
#define __GCONST__
#define __FDA__
#endif /* C_COMP_GHS_ARM */

#ifdef __PC_SIMULATION__
#define __EEPROM__
#define __NEAR__
#define __NEAR_FUNC__
#define __FAR__
#define __SDA__
#define __INTERRUPT__
#define __INLINE__      __inline
#define __BANKED__
#define __NON_BANKED__
#define __NO_INIT__
#define __GCONST__
#define __FDA__
#endif /* __PC_SIMULATION__ */


/* Index to be used for array element access in the bitfield_xxx_t */
/* TYPE_FIRST_MS_ID  : First  Most Significant Index */
/* TYPE_SECOND_MS_ID : Second Most Significant Index */
/* TYPE_THIRD_MS_ID  : Third  Most Significant Index */
/* TYPE_FOURTH_MS_ID : Fourth Most Significant Index */

#ifdef __LITTLE_ENDIAN__
#define TYPE_FIRST_MS_ID  3
#define TYPE_SECOND_MS_ID 2
#define TYPE_THIRD_MS_ID  1
#define TYPE_FOURTH_MS_ID 0
#endif

#ifdef __BIG_ENDIAN__
#define TYPE_FIRST_MS_ID  0
#define TYPE_SECOND_MS_ID 1
#define TYPE_THIRD_MS_ID  2
#define TYPE_FOURTH_MS_ID 3
#endif


/*_____ G L O B A L - T Y P E S ______________________________________________*/

/* basic types definition */
typedef unsigned char       ubyte;   /* unsigned byte       (1 byte ) */
typedef unsigned short int  ushort;  /* short unsigned word (2 bytes) */
typedef unsigned long  int  ulong;   /* long unsigned word  (4 bytes) */
#if defined(C_COMP_GHS_TX49) || defined(C_COMP_GHS_ARM) || defined(__PC_SIMULATION__)
typedef unsigned long long  ulonglong; /* long unsigned long  (8 bytes) */
#endif /* C_COMP_GHS_TX49 || C_COMP_GHS_ARM || __PC_SIMULATION__ */
typedef signed   char       sbyte;   /* signed byte         (1 byte ) */
typedef signed   short int  sshort;  /* short signed word   (2 bytes) */
typedef signed   long  int  slong;   /* long signed word    (4 bytes) */
#if defined(C_COMP_GHS_TX49) || defined(C_COMP_GHS_ARM) || defined(__PC_SIMULATION__)
typedef signed   long long  slonglong; /* long signed long    (8 bytes) */
#endif /* C_COMP_GHS_TX49 || C_COMP_GHS_ARM || __PC_SIMULATION__ */
#if defined(C_COMP_GHS_ARM) || defined(__PC_SIMULATION__) || defined(C_COMP_GHS_V850)
#ifndef _MATH_H
/* To avoid typedef redefinition in case math.h is already included and use the
 * float_t of math.h */
typedef float               float_t;
#endif /* _MATH_H */
typedef double              dfloat_t;
typedef long double         lfloat_t;
#endif /* C_COMP_GHS_ARM || __PC_SIMULATION__ */



/* boolean type definition */
#ifndef FALSE
  #define FALSE 0
#endif
#ifndef TRUE
  #define TRUE  1
#endif

typedef ubyte bool_t;

/* Type definition for byte and bit acces */
typedef union
{
  struct
  {
  #ifdef __BITFIELD_LSB_FIRST__
    ubyte BIT0 :1;    /* lsb */
    ubyte BIT1 :1;
    ubyte BIT2 :1;
    ubyte BIT3 :1;
    ubyte BIT4 :1;
    ubyte BIT5 :1;
    ubyte BIT6 :1;
    ubyte BIT7 :1;    /* msb */
  #endif /* __BITFIELD_LSB_FIRST__ */

  #ifdef __BITFIELD_MSB_FIRST__
    ubyte BIT7 :1;    /* msb */
    ubyte BIT6 :1;
    ubyte BIT5 :1;
    ubyte BIT4 :1;
    ubyte BIT3 :1;
    ubyte BIT2 :1;
    ubyte BIT1 :1;
    ubyte BIT0 :1;    /* lsb */
  #endif /* __BITFIELD_MSB_FIRST__ */

  } _bit; /* for bit  acces */

  ubyte _byte; /* for byte acces */
} bitfield_byte_t;

/* Type definition for short, byte and bit acces */
typedef union
{
  struct
  {
  #ifdef __BITFIELD_LSB_FIRST__
    ushort BIT0  :1;    /* lsb */
    ushort BIT1  :1;
    ushort BIT2  :1;
    ushort BIT3  :1;
    ushort BIT4  :1;
    ushort BIT5  :1;
    ushort BIT6  :1;
    ushort BIT7  :1;
    ushort BIT8  :1;
    ushort BIT9  :1;
    ushort BIT10 :1;
    ushort BIT11 :1;
    ushort BIT12 :1;
    ushort BIT13 :1;
    ushort BIT14 :1;
    ushort BIT15 :1;    /* msb */
  #endif /* __BITFIELD_LSB_FIRST__ */

  #ifdef __BITFIELD_MSB_FIRST__
    ushort BIT15 :1;    /* msb */
    ushort BIT14 :1;
    ushort BIT13 :1;
    ushort BIT12 :1;
    ushort BIT11 :1;
    ushort BIT10 :1;
    ushort BIT9  :1;
    ushort BIT8  :1;
    ushort BIT7  :1;
    ushort BIT6  :1;
    ushort BIT5  :1;
    ushort BIT4  :1;
    ushort BIT3  :1;
    ushort BIT2  :1;
    ushort BIT1  :1;
    ushort BIT0  :1;    /* lsb */
  #endif /* __BITFIELD_MSB_FIRST__ */

  } _bit; /* for bit  acces */

  struct
  {
  #ifdef __LITTLE_ENDIAN__
  /* little endian format (Intel) */
  ubyte low;
  ubyte high;
  #endif /* __LITTLE_ENDIAN__ */

  #ifdef __BIG_ENDIAN__
  /* big endian format (Motorola) */
  ubyte high;
  ubyte low;
  #endif /* __BIG_ENDIAN__ */
  } _byte; /* for byte access */

  ushort _short; /* for word access */
} bitfield_short_t;


/* Type definition for long, short, byte and bit acces */
typedef union
{
  struct
  {
  #ifdef __BITFIELD_LSB_FIRST__
    ulong BIT0  :1;    /* lsb */
    ulong BIT1  :1;
    ulong BIT2  :1;
    ulong BIT3  :1;
    ulong BIT4  :1;
    ulong BIT5  :1;
    ulong BIT6  :1;
    ulong BIT7  :1;
    ulong BIT8  :1;
    ulong BIT9  :1;
    ulong BIT10 :1;
    ulong BIT11 :1;
    ulong BIT12 :1;
    ulong BIT13 :1;
    ulong BIT14 :1;
    ulong BIT15 :1;    /* msb */
    ulong BIT16 :1;    /* lsb */
    ulong BIT17 :1;
    ulong BIT18 :1;
    ulong BIT19 :1;
    ulong BIT20 :1;
    ulong BIT21 :1;
    ulong BIT22 :1;
    ulong BIT23 :1;
    ulong BIT24 :1;
    ulong BIT25 :1;
    ulong BIT26 :1;
    ulong BIT27 :1;
    ulong BIT28 :1;
    ulong BIT29 :1;
    ulong BIT30 :1;
    ulong BIT31 :1;    /* msb */
  #endif /* __BITFIELD_LSB_FIRST__ */

  #ifdef __BITFIELD_MSB_FIRST__
    ulong BIT31 :1;    /* msb */
    ulong BIT30 :1;
    ulong BIT29 :1;
    ulong BIT28 :1;
    ulong BIT27 :1;
    ulong BIT26 :1;
    ulong BIT25 :1;
    ulong BIT24 :1;
    ulong BIT23 :1;
    ulong BIT22 :1;
    ulong BIT21 :1;
    ulong BIT20 :1;
    ulong BIT19 :1;
    ulong BIT18 :1;
    ulong BIT17 :1;
    ulong BIT16 :1;    /* lsb */
    ulong BIT15 :1;    /* msb */
    ulong BIT14 :1;
    ulong BIT13 :1;
    ulong BIT12 :1;
    ulong BIT11 :1;
    ulong BIT10 :1;
    ulong BIT9  :1;
    ulong BIT8  :1;
    ulong BIT7  :1;
    ulong BIT6  :1;
    ulong BIT5  :1;
    ulong BIT4  :1;
    ulong BIT3  :1;
    ulong BIT2  :1;
    ulong BIT1  :1;
    ulong BIT0  :1;    /* lsb */
  #endif /* __BITFIELD_MSB_FIRST__ */

  } _bit; /* for bit  acces */

  struct
  {
  #ifdef __LITTLE_ENDIAN__
  /* little endian format (Intel) */
  ubyte low_l;
  ubyte high_l;
  ubyte low_m;
  ubyte high_m;
  #endif /* __LITTLE_ENDIAN__ */

  #ifdef __BIG_ENDIAN__
  /* big endian format (Motorola) */
  ubyte high_m;
  ubyte low_m;
  ubyte high_l;
  ubyte low_l;
  #endif /* __BIG_ENDIAN__ */
  } _byte; /* for byte access */

  struct
  {
  #ifdef __LITTLE_ENDIAN__
  /* little endian format (Intel) */
  ushort low;
  ushort high;
  #endif /* __LITTLE_ENDIAN__ */

  #ifdef __BIG_ENDIAN__
  /* big endian format (Motorola) */
  ushort high;
  ushort low;
  #endif /* __BIG_ENDIAN__ */
  } _short;  /* for word access */

  ubyte _IndexedByte[4]; /* to access byte by an index */
  ushort IndexedWord[2]; /* to access word by an index */
  ulong _long; /* for longword access */
} bitfield_long_t;

#if defined(C_COMP_GHS_TX49) || defined(C_COMP_GHS_ARM)
/* Type definition for longlong, long, short, byte and bit acces */
typedef union
{
  struct
  {
  #ifdef __BITFIELD_LSB_FIRST__
    ulonglong BIT0  :1;    /* lsb */
    ulonglong BIT1  :1;
    ulonglong BIT2  :1;
    ulonglong BIT3  :1;
    ulonglong BIT4  :1;
    ulonglong BIT5  :1;
    ulonglong BIT6  :1;
    ulonglong BIT7  :1;
    ulonglong BIT8  :1;
    ulonglong BIT9  :1;
    ulonglong BIT10 :1;
    ulonglong BIT11 :1;
    ulonglong BIT12 :1;
    ulonglong BIT13 :1;
    ulonglong BIT14 :1;
    ulonglong BIT15 :1;    /* msb */
    ulonglong BIT16 :1;    /* lsb */
    ulonglong BIT17 :1;
    ulonglong BIT18 :1;
    ulonglong BIT19 :1;
    ulonglong BIT20 :1;
    ulonglong BIT21 :1;
    ulonglong BIT22 :1;
    ulonglong BIT23 :1;
    ulonglong BIT24 :1;
    ulonglong BIT25 :1;
    ulonglong BIT26 :1;
    ulonglong BIT27 :1;
    ulonglong BIT28 :1;
    ulonglong BIT29 :1;
    ulonglong BIT30 :1;
    ulonglong BIT31 :1;    /* msb */
    ulonglong BIT32 :1;    /* lsb */
    ulonglong BIT33 :1;
    ulonglong BIT34 :1;
    ulonglong BIT35 :1;
    ulonglong BIT36 :1;
    ulonglong BIT37 :1;
    ulonglong BIT38 :1;
    ulonglong BIT39 :1;
    ulonglong BIT40 :1;
    ulonglong BIT41 :1;
    ulonglong BIT42 :1;
    ulonglong BIT43 :1;
    ulonglong BIT44 :1;
    ulonglong BIT45 :1;
    ulonglong BIT46 :1;
    ulonglong BIT47 :1;    /* msb */
    ulonglong BIT48 :1;    /* lsb */
    ulonglong BIT49 :1;
    ulonglong BIT50 :1;
    ulonglong BIT51 :1;
    ulonglong BIT52 :1;
    ulonglong BIT53 :1;
    ulonglong BIT54 :1;
    ulonglong BIT55 :1;
    ulonglong BIT56 :1;
    ulonglong BIT57 :1;
    ulonglong BIT58 :1;
    ulonglong BIT59 :1;
    ulonglong BIT60 :1;
    ulonglong BIT61 :1;
    ulonglong BIT62 :1;
    ulonglong BIT63 :1;    /* msb */
  #endif /* __BITFIELD_LSB_FIRST__ */

  #ifdef __BITFIELD_MSB_FIRST__
    ulonglong BIT63 :1;
    ulonglong BIT62 :1;    /* msb */
    ulonglong BIT61 :1;
    ulonglong BIT60 :1;
    ulonglong BIT59 :1;
    ulonglong BIT58 :1;
    ulonglong BIT57 :1;
    ulonglong BIT56 :1;
    ulonglong BIT55 :1;
    ulonglong BIT54 :1;
    ulonglong BIT53 :1;
    ulonglong BIT52 :1;
    ulonglong BIT51 :1;
    ulonglong BIT50 :1;
    ulonglong BIT49 :1;
    ulonglong BIT48 :1;
    ulonglong BIT47 :1;    /* lsb */
    ulonglong BIT46 :1;    /* msb */
    ulonglong BIT45 :1;
    ulonglong BIT44 :1;
    ulonglong BIT43 :1;
    ulonglong BIT42 :1;
    ulonglong BIT41 :1;
    ulonglong BIT40 :1;
    ulonglong BIT39 :1;
    ulonglong BIT38 :1;
    ulonglong BIT37 :1;
    ulonglong BIT36 :1;
    ulonglong BIT35 :1;
    ulonglong BIT34 :1;
    ulonglong BIT33 :1;
    ulonglong BIT32 :1;    /* lsb */
    ulonglong BIT31 :1;    /* msb */
    ulonglong BIT30 :1;
    ulonglong BIT29 :1;
    ulonglong BIT28 :1;
    ulonglong BIT27 :1;
    ulonglong BIT26 :1;
    ulonglong BIT25 :1;
    ulonglong BIT24 :1;
    ulonglong BIT23 :1;
    ulonglong BIT22 :1;
    ulonglong BIT21 :1;
    ulonglong BIT20 :1;
    ulonglong BIT19 :1;
    ulonglong BIT18 :1;
    ulonglong BIT17 :1;
    ulonglong BIT16 :1;    /* lsb */
    ulonglong BIT15 :1;    /* msb */
    ulonglong BIT14 :1;
    ulonglong BIT13 :1;
    ulonglong BIT12 :1;
    ulonglong BIT11 :1;
    ulonglong BIT10 :1;
    ulonglong BIT9  :1;
    ulonglong BIT8  :1;
    ulonglong BIT7  :1;
    ulonglong BIT6  :1;
    ulonglong BIT5  :1;
    ulonglong BIT4  :1;
    ulonglong BIT3  :1;
    ulonglong BIT2  :1;
    ulonglong BIT1  :1;
    ulonglong BIT0  :1;    /* lsb */
  #endif /* __BITFIELD_MSB_FIRST__ */

  } _bit; /* for bit  acces */

  struct
  {
  #ifdef __LITTLE_ENDIAN__
  /* little endian format (Intel) */
  ubyte low_l;
  ubyte high_l;
  ubyte low_ml;
  ubyte high_ml;
  ubyte low_mh;
  ubyte high_mh;
  ubyte low_h;
  ubyte high_h;
  #endif /* __LITTLE_ENDIAN__ */

  #ifdef __BIG_ENDIAN__
  /* big endian format (Motorola) */
  ubyte high_h;
  ubyte low_h;
  ubyte high_mh;
  ubyte low_mh;
  ubyte high_ml;
  ubyte low_ml;
  ubyte high_l;
  ubyte low_l;
  #endif /* __BIG_ENDIAN__ */
  } _byte; /* for byte access */

  struct
  {
  #ifdef __LITTLE_ENDIAN__
  /* little endian format (Intel) */
  ushort low;
  ushort medium_low;
  ushort medium_high;
  ushort high;
  #endif /* __LITTLE_ENDIAN__ */

  #ifdef __BIG_ENDIAN__
  /* big endian format (Motorola) */
  ushort high;
  ushort medium_high;
  ushort medium_low;
  ushort low;
  #endif /* __BIG_ENDIAN__ */
  } _short;  /* for word access */

  ubyte _IndexedByte[8]; /* to access byte by an index */
  ushort IndexedWord[4]; /* to access word by an index */
  ulong _long[2]; /* for longword access */
  ulonglong _longlong; /* for long long access */
} bitfield_long_long_t;
#endif /* C_COMP_GHS_TX49 || C_COMP_GHS_ARM */

#endif /* TYPE_H */


/*_____ E N D _____ (type.h) _________________________________________________*/
