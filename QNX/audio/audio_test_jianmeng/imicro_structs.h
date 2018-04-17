/******************************************************************************
 * file:   imicro_structs.h
 * from: GenASFiles Vers: 2.0.4 on 27th October 2016
 *********************************************  Do not edit the section above !
 *  purpose:    intermicro structures and enums
 ************************************* (C) Copyright 2016 Magneti Marelli ****/

/* generated Im struct types file */
/* do not modify */

/* from: GenASFiles Vers: 2.0.4 on 27th October 2016 */

#ifndef __IMICRO_STRUCTS_H_INCLUDED__
#define __IMICRO_STRUCTS_H_INCLUDED__

#include "imicro_config.h"

/* ********** Defines ********** */

/* Define value                            */
#ifndef IM_MAX_DATETIME_YEAR
#define IM_MAX_DATETIME_YEAR  255
#endif

/* Define value                            */
#ifndef IM_NAVI_LENGTH_STRING
#define IM_NAVI_LENGTH_STRING  80
#endif

/* ********** Enumeration types ********** */

/* Enumeration type:        PWMM_PowerModeStatus_t      */
/* Description:             vehicle power status        */
#ifndef Rte_TypeDef_PWMM_PowerModeStatus_t
#define Rte_TypeDef_PWMM_PowerModeStatus_t
typedef UINT8 PWMM_PowerModeStatus_t;
#define PWMM_PowerModeStatus_t_LowerLimit (0U)
#define PWMM_PowerModeStatus_t_UpperLimit (4U)
# ifndef PWMM_POWER_OFF
#  define PWMM_POWER_OFF (0U)
# endif
# ifndef PWMM_POWER_ACC
#  define PWMM_POWER_ACC (1U)
# endif
# ifndef PWMM_POWER_RUN
#  define PWMM_POWER_RUN (2U)
# endif
# ifndef PWMM_POWER_CRANK
#  define PWMM_POWER_CRANK (3U)
# endif
# ifndef PWMM_POWER_MISSING
#  define PWMM_POWER_MISSING (4U)
# endif
#endif

/* Enumeration type:        PWMM_Kl15Status_t           */
/* Description:             KL15 status   */
#ifndef Rte_TypeDef_PWMM_Kl15Status_t
#define Rte_TypeDef_PWMM_Kl15Status_t
typedef UINT8 PWMM_Kl15Status_t;
#define PWMM_Kl15Status_t_LowerLimit (0U)
#define PWMM_Kl15Status_t_UpperLimit (1U)
# ifndef PWMM_KL15_OFF
#  define PWMM_KL15_OFF (0U)
# endif
# ifndef PWMM_KL15_ON
#  define PWMM_KL15_ON (1U)
# endif
#endif

/* Enumeration type:        Data_Availability_t         */
/* Description:                           */
#ifndef Rte_TypeDef_Data_Availability_t
#define Rte_TypeDef_Data_Availability_t
typedef UINT8 Data_Availability_t;
#define Data_Availability_t_LowerLimit (0U)
#define Data_Availability_t_UpperLimit (2U)
# ifndef DATA_ACTIVE
#  define DATA_ACTIVE (0U)
# endif
# ifndef DATA_ERROR
#  define DATA_ERROR (1U)
# endif
# ifndef DATA_UNAVAILABLE
#  define DATA_UNAVAILABLE (2U)
# endif
#endif

/* Enumeration type:        SPDM_SpeedUnit_t            */
/* Description:                           */
#ifndef Rte_TypeDef_SPDM_SpeedUnit_t
#define Rte_TypeDef_SPDM_SpeedUnit_t
typedef UINT8 SPDM_SpeedUnit_t;
#define SPDM_SpeedUnit_t_LowerLimit (0U)
#define SPDM_SpeedUnit_t_UpperLimit (1U)
# ifndef SPDM_UNIT_IN_KM
#  define SPDM_UNIT_IN_KM (0U)
# endif
# ifndef SPDM_UNIT_IN_MPH
#  define SPDM_UNIT_IN_MPH (1U)
# endif
#endif

/* Enumeration type:        POWP_EffSymbol_t            */
/* Description:                           */
#ifndef Rte_TypeDef_POWP_EffSymbol_t
#define Rte_TypeDef_POWP_EffSymbol_t
typedef UINT8 POWP_EffSymbol_t;
#define POWP_EffSymbol_t_LowerLimit (0U)
#define POWP_EffSymbol_t_UpperLimit (1U)
# ifndef POWP_EFF_NEGATIVE
#  define POWP_EFF_NEGATIVE (0U)
# endif
# ifndef POWP_EFF_POSITIVE
#  define POWP_EFF_POSITIVE (1U)
# endif
#endif

/* Enumeration type:        SOCP_DivFactor_t            */
/* Description:                           */
#ifndef Rte_TypeDef_SOCP_DivFactor_t
#define Rte_TypeDef_SOCP_DivFactor_t
typedef UINT8 SOCP_DivFactor_t;
#define SOCP_DivFactor_t_LowerLimit (1U)
#define SOCP_DivFactor_t_UpperLimit (10U)
# ifndef SOCP_DIV_FAC_1
#  define SOCP_DIV_FAC_1 (1U)
# endif
# ifndef SOCP_DIV_FAC_10
#  define SOCP_DIV_FAC_10 (10U)
# endif
#endif

/* Enumeration type:        OATM_Symbol_t */
/* Description:                           */
#ifndef Rte_TypeDef_OATM_Symbol_t
#define Rte_TypeDef_OATM_Symbol_t
typedef UINT8 OATM_Symbol_t;
#define OATM_Symbol_t_LowerLimit (0U)
#define OATM_Symbol_t_UpperLimit (1U)
# ifndef OATM_SYMBOL_NEGATIVE
#  define OATM_SYMBOL_NEGATIVE (0U)
# endif
# ifndef OATM_SYMBOL_POSITIVE
#  define OATM_SYMBOL_POSITIVE (1U)
# endif
#endif

/* Enumeration type:        OATM_TemperatureUnit_t      */
/* Description:                           */
#ifndef Rte_TypeDef_OATM_TemperatureUnit_t
#define Rte_TypeDef_OATM_TemperatureUnit_t
typedef UINT8 OATM_TemperatureUnit_t;
#define OATM_TemperatureUnit_t_LowerLimit (0U)
#define OATM_TemperatureUnit_t_UpperLimit (1U)
# ifndef OATM_IN_CELS
#  define OATM_IN_CELS (0U)
# endif
# ifndef OATM_IN_FAH
#  define OATM_IN_FAH (1U)
# endif
#endif

/* Enumeration type:        OATM_DivFactor_t            */
/* Description:                           */
#ifndef Rte_TypeDef_OATM_DivFactor_t
#define Rte_TypeDef_OATM_DivFactor_t
typedef UINT8 OATM_DivFactor_t;
#define OATM_DivFactor_t_LowerLimit (1U)
#define OATM_DivFactor_t_UpperLimit (10U)
# ifndef OATM_DIV_FAC_1
#  define OATM_DIV_FAC_1 (1U)
# endif
# ifndef OATM_DIV_FAC_10
#  define OATM_DIV_FAC_10 (10U)
# endif
#endif

/* Enumeration type:        ODOM_OdoUnits_t             */
/* Description:                           */
#ifndef Rte_TypeDef_ODOM_OdoUnits_t
#define Rte_TypeDef_ODOM_OdoUnits_t
typedef UINT8 ODOM_OdoUnits_t;
#define ODOM_OdoUnits_t_LowerLimit (0U)
#define ODOM_OdoUnits_t_UpperLimit (1U)
# ifndef ODOM_UNITS_IN_KM
#  define ODOM_UNITS_IN_KM (0U)
# endif
# ifndef ODOM_UNITS_IN_MPH
#  define ODOM_UNITS_IN_MPH (1U)
# endif
#endif

/* Enumeration type:        TCSM_PressStatus_t          */
/* Description:                           */
#ifndef Rte_TypeDef_TCSM_PressStatus_t
#define Rte_TypeDef_TCSM_PressStatus_t
typedef UINT8 TCSM_PressStatus_t;
#define TCSM_PressStatus_t_LowerLimit (0U)
#define TCSM_PressStatus_t_UpperLimit (3U)
# ifndef TCSM_BUTTON_STATUS_NO_PRESS
#  define TCSM_BUTTON_STATUS_NO_PRESS (0U)
# endif
# ifndef TCSM_BUTTON_STATUS_SHORT_PRESS
#  define TCSM_BUTTON_STATUS_SHORT_PRESS (1U)
# endif
# ifndef TCSM_BUTTON_STATUS_LONG_PRESS
#  define TCSM_BUTTON_STATUS_LONG_PRESS (2U)
# endif
# ifndef TCSM_BUTTON_STATUS_LONG_PRESS_NOT_RELEASE
#  define TCSM_BUTTON_STATUS_LONG_PRESS_NOT_RELEASE (3U)
# endif
#endif

/* Enumeration type:        TCSM_PushBlock_t            */
/* Description:                           */
#ifndef Rte_TypeDef_TCSM_PushBlock_t
#define Rte_TypeDef_TCSM_PushBlock_t
typedef UINT8 TCSM_PushBlock_t;
#define TCSM_PushBlock_t_LowerLimit (0U)
#define TCSM_PushBlock_t_UpperLimit (1U)
# ifndef TCSM_PUSH_BLOCK_NONE
#  define TCSM_PUSH_BLOCK_NONE (0U)
# endif
# ifndef TCSM_PUSH_BLOCK_HAPPEN
#  define TCSM_PUSH_BLOCK_HAPPEN (1U)
# endif
#endif

/* Enumeration type:        TCSM_ButtonUdTxEnable_t     */
/* Description:                           */
#ifndef Rte_TypeDef_TCSM_ButtonUdTxEnable_t
#define Rte_TypeDef_TCSM_ButtonUdTxEnable_t
typedef UINT8 TCSM_ButtonUdTxEnable_t;
#define TCSM_ButtonUdTxEnable_t_LowerLimit (0U)
#define TCSM_ButtonUdTxEnable_t_UpperLimit (1U)
# ifndef TCSM_BUTTON_UD_TX_DISABLE
#  define TCSM_BUTTON_UD_TX_DISABLE (0U)
# endif
# ifndef TCSM_BUTTON_UD_TX_ENABLE
#  define TCSM_BUTTON_UD_TX_ENABLE (1U)
# endif
#endif

/* Enumeration type:        CAVP_DataSymbol_t           */
/* Description:                           */
#ifndef Rte_TypeDef_CAVP_DataSymbol_t
#define Rte_TypeDef_CAVP_DataSymbol_t
typedef UINT8 CAVP_DataSymbol_t;
#define CAVP_DataSymbol_t_LowerLimit (0U)
#define CAVP_DataSymbol_t_UpperLimit (1U)
# ifndef CAVP_DATA_NEGATIVE
#  define CAVP_DATA_NEGATIVE (0U)
# endif
# ifndef CAVP_DATA_POSITIVE
#  define CAVP_DATA_POSITIVE (1U)
# endif
#endif

/* Enumeration type:        FUNC_EnableStatus_t         */
/* Description:                           */
#ifndef Rte_TypeDef_FUNC_EnableStatus_t
#define Rte_TypeDef_FUNC_EnableStatus_t
typedef UINT8 FUNC_EnableStatus_t;
#define FUNC_EnableStatus_t_LowerLimit (0U)
#define FUNC_EnableStatus_t_UpperLimit (1U)
# ifndef FUNC_DISABLED
#  define FUNC_DISABLED (0U)
# endif
# ifndef FUNC_ENABLED
#  define FUNC_ENABLED (1U)
# endif
#endif

/* Enumeration type:        GTWM_Bool_t   */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_Bool_t
#define Rte_TypeDef_GTWM_Bool_t
typedef UINT8 GTWM_Bool_t;
#define GTWM_Bool_t_LowerLimit (0U)
#define GTWM_Bool_t_UpperLimit (1U)
# ifndef GTWM_FALSE
#  define GTWM_FALSE (0U)
# endif
# ifndef GTWM_TRUE
#  define GTWM_TRUE (1U)
# endif
#endif

/* Enumeration type:        GTWM_SysSts_t */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_SysSts_t
#define Rte_TypeDef_GTWM_SysSts_t
typedef UINT8 GTWM_SysSts_t;
#define GTWM_SysSts_t_LowerLimit (0U)
#define GTWM_SysSts_t_UpperLimit (7U)
# ifndef GTWM_SYSSTS_OFF
#  define GTWM_SYSSTS_OFF (0U)
# endif
# ifndef GTWM_SYSSTS_STANDBY
#  define GTWM_SYSSTS_STANDBY (1U)
# endif
# ifndef GTWM_SYSSTS_ACTIVE
#  define GTWM_SYSSTS_ACTIVE (2U)
# endif
# ifndef GTWM_SYSSTS_OVERRIDE
#  define GTWM_SYSSTS_OVERRIDE (3U)
# endif
# ifndef GTWM_SYSSTS_FAULT
#  define GTWM_SYSSTS_FAULT (4U)
# endif
# ifndef GTWM_SYSSTS_RESERVED5
#  define GTWM_SYSSTS_RESERVED5 (5U)
# endif
# ifndef GTWM_SYSSTS_RESERVED6
#  define GTWM_SYSSTS_RESERVED6 (6U)
# endif
# ifndef GTWM_SYSSTS_RESERVED7
#  define GTWM_SYSSTS_RESERVED7 (7U)
# endif
#endif

/* Enumeration type:        GTWM_EPTDrvngMdSwSts_t      */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_EPTDrvngMdSwSts_t
#define Rte_TypeDef_GTWM_EPTDrvngMdSwSts_t
typedef UINT8 GTWM_EPTDrvngMdSwSts_t;
#define GTWM_EPTDrvngMdSwSts_t_LowerLimit (0U)
#define GTWM_EPTDrvngMdSwSts_t_UpperLimit (7U)
# ifndef GTWM_MDSW_E_ECONOMY
#  define GTWM_MDSW_E_ECONOMY (0U)
# endif
# ifndef GTWM_MDSW_N_NORMAL
#  define GTWM_MDSW_N_NORMAL (1U)
# endif
# ifndef GTWM_MDSW_S_SPORT
#  define GTWM_MDSW_S_SPORT (2U)
# endif
# ifndef GTWM_MDSW_H_SOCHOLD
#  define GTWM_MDSW_H_SOCHOLD (3U)
# endif
# ifndef GTWM_MDSW_C_CHARGE
#  define GTWM_MDSW_C_CHARGE (4U)
# endif
# ifndef GTWM_MDSW_RESERVED5
#  define GTWM_MDSW_RESERVED5 (5U)
# endif
# ifndef GTWM_MDSW_RESERVED6
#  define GTWM_MDSW_RESERVED6 (6U)
# endif
# ifndef GTWM_MDSW_RESERVED7
#  define GTWM_MDSW_RESERVED7 (7U)
# endif
#endif

/* Enumeration type:        GTWM_ConUnit_t              */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_ConUnit_t
#define Rte_TypeDef_GTWM_ConUnit_t
typedef UINT8 GTWM_ConUnit_t;
#define GTWM_ConUnit_t_LowerLimit (0U)
#define GTWM_ConUnit_t_UpperLimit (3U)
# ifndef GTWM_CONUNIT_LITRES_100KM
#  define GTWM_CONUNIT_LITRES_100KM (0U)
# endif
# ifndef GTWM_CONUNIT_UKMPG
#  define GTWM_CONUNIT_UKMPG (1U)
# endif
# ifndef GTWM_CONUNIT_USMPG
#  define GTWM_CONUNIT_USMPG (2U)
# endif
# ifndef GTWM_CONUNIT_KM_LITRE
#  define GTWM_CONUNIT_KM_LITRE (3U)
# endif
#endif

/* Enumeration type:        GTWM_SpeedUnit_t            */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_SpeedUnit_t
#define Rte_TypeDef_GTWM_SpeedUnit_t
typedef UINT8 GTWM_SpeedUnit_t;
#define GTWM_SpeedUnit_t_LowerLimit (0U)
#define GTWM_SpeedUnit_t_UpperLimit (1U)
# ifndef GTWM_SPEEDUNIT_KM_H
#  define GTWM_SPEEDUNIT_KM_H (0U)
# endif
# ifndef GTWM_SPEEDUNIT_MPH
#  define GTWM_SPEEDUNIT_MPH (1U)
# endif
#endif

/* Enumeration type:        GTWM_TyrePresUnit_t         */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_TyrePresUnit_t
#define Rte_TypeDef_GTWM_TyrePresUnit_t
typedef UINT8 GTWM_TyrePresUnit_t;
#define GTWM_TyrePresUnit_t_LowerLimit (0U)
#define GTWM_TyrePresUnit_t_UpperLimit (3U)
# ifndef GTWM_TYREPRESUNIT_BAR
#  define GTWM_TYREPRESUNIT_BAR (0U)
# endif
# ifndef GTWM_TYREPRESUNIT_KPA
#  define GTWM_TYREPRESUNIT_KPA (1U)
# endif
# ifndef GTWM_TYREPRESUNIT_PSI
#  define GTWM_TYREPRESUNIT_PSI (2U)
# endif
# ifndef GTWM_TYREPRESUNIT_RESERVED
#  define GTWM_TYREPRESUNIT_RESERVED (3U)
# endif
#endif

/* Enumeration type:        GTWM_TempUnit_t             */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_TempUnit_t
#define Rte_TypeDef_GTWM_TempUnit_t
typedef UINT8 GTWM_TempUnit_t;
#define GTWM_TempUnit_t_LowerLimit (0U)
#define GTWM_TempUnit_t_UpperLimit (3U)
# ifndef GTWM_TEMPUNIT_CELSIUS_DEGREE
#  define GTWM_TEMPUNIT_CELSIUS_DEGREE (0U)
# endif
# ifndef GTWM_TEMPUNIT_FAHRENHEIT_DEGREE
#  define GTWM_TEMPUNIT_FAHRENHEIT_DEGREE (1U)
# endif
# ifndef GTWM_TEMPUNIT_NOTAVAILABLE
#  define GTWM_TEMPUNIT_NOTAVAILABLE (2U)
# endif
# ifndef GTWM_TEMPUNIT_RESERVED
#  define GTWM_TEMPUNIT_RESERVED (3U)
# endif
#endif

/* Enumeration type:        GTWM_ReadySts_t             */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_ReadySts_t
#define Rte_TypeDef_GTWM_ReadySts_t
typedef UINT8 GTWM_ReadySts_t;
#define GTWM_ReadySts_t_LowerLimit (0U)
#define GTWM_ReadySts_t_UpperLimit (1U)
# ifndef GTWM_READYSTS_NOTREADY
#  define GTWM_READYSTS_NOTREADY (0U)
# endif
# ifndef GTWM_READYSTS_READY
#  define GTWM_READYSTS_READY (1U)
# endif
#endif

/* Enumeration type:        GTWM_CallSts_t              */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_CallSts_t
#define Rte_TypeDef_GTWM_CallSts_t
typedef UINT8 GTWM_CallSts_t;
#define GTWM_CallSts_t_LowerLimit (0U)
#define GTWM_CallSts_t_UpperLimit (3U)
# ifndef GTWM_CALLSTS_NOCALL
#  define GTWM_CALLSTS_NOCALL (0U)
# endif
# ifndef GTWM_CALLSTS_CALLING
#  define GTWM_CALLSTS_CALLING (1U)
# endif
# ifndef GTWM_CALLSTS_RESERVE2
#  define GTWM_CALLSTS_RESERVE2 (2U)
# endif
# ifndef GTWM_CALLSTS_RESERVE3
#  define GTWM_CALLSTS_RESERVE3 (3U)
# endif
#endif

/* Enumeration type:        GTWM_NaviMode_t             */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_NaviMode_t
#define Rte_TypeDef_GTWM_NaviMode_t
typedef UINT8 GTWM_NaviMode_t;
#define GTWM_NaviMode_t_LowerLimit (0U)
#define GTWM_NaviMode_t_UpperLimit (1U)
# ifndef GTWM_NAVIMODE_MAPONLY
#  define GTWM_NAVIMODE_MAPONLY (0U)
# endif
# ifndef GTWM_NAVIMODE_NAVIGUIDING
#  define GTWM_NAVIMODE_NAVIGUIDING (1U)
# endif
#endif

/* Enumeration type:        GTWM_DistUnt_t              */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_DistUnt_t
#define Rte_TypeDef_GTWM_DistUnt_t
typedef UINT8 GTWM_DistUnt_t;
#define GTWM_DistUnt_t_LowerLimit (0U)
#define GTWM_DistUnt_t_UpperLimit (1U)
# ifndef GTWM_DISTUNT_KM
#  define GTWM_DISTUNT_KM (0U)
# endif
# ifndef GTWM_DISTUNT_MILS
#  define GTWM_DISTUNT_MILS (1U)
# endif
#endif

/* Enumeration type:        GTWM_TimeDspFmt_t           */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_TimeDspFmt_t
#define Rte_TypeDef_GTWM_TimeDspFmt_t
typedef UINT8 GTWM_TimeDspFmt_t;
#define GTWM_TimeDspFmt_t_LowerLimit (0U)
#define GTWM_TimeDspFmt_t_UpperLimit (1U)
# ifndef GTWM_TIMEDSPFMT_12HOUR_MODE
#  define GTWM_TIMEDSPFMT_12HOUR_MODE (0U)
# endif
# ifndef GTWM_TIMEDSPFMT_24HOUR_MODE
#  define GTWM_TIMEDSPFMT_24HOUR_MODE (1U)
# endif
#endif

/* Enumeration type:        GTWM_Month_t  */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_Month_t
#define Rte_TypeDef_GTWM_Month_t
typedef UINT8 GTWM_Month_t;
#define GTWM_Month_t_LowerLimit (0U)
#define GTWM_Month_t_UpperLimit (15U)
# ifndef GTWM_MONTH_UNKNOWN
#  define GTWM_MONTH_UNKNOWN (0U)
# endif
# ifndef GTWM_MONTH_JANUARY
#  define GTWM_MONTH_JANUARY (1U)
# endif
# ifndef GTWM_MONTH_FEBRUARY
#  define GTWM_MONTH_FEBRUARY (2U)
# endif
# ifndef GTWM_MONTH_MARCH
#  define GTWM_MONTH_MARCH (3U)
# endif
# ifndef GTWM_MONTH_APRIL
#  define GTWM_MONTH_APRIL (4U)
# endif
# ifndef GTWM_MONTH_MAY
#  define GTWM_MONTH_MAY (5U)
# endif
# ifndef GTWM_MONTH_JUNE
#  define GTWM_MONTH_JUNE (6U)
# endif
# ifndef GTWM_MONTH_JULY
#  define GTWM_MONTH_JULY (7U)
# endif
# ifndef GTWM_MONTH_AUGUST
#  define GTWM_MONTH_AUGUST (8U)
# endif
# ifndef GTWM_MONTH_SEPTEMBER
#  define GTWM_MONTH_SEPTEMBER (9U)
# endif
# ifndef GTWM_MONTH_OCTOBER
#  define GTWM_MONTH_OCTOBER (10U)
# endif
# ifndef GTWM_MONTH_NOVEMBER
#  define GTWM_MONTH_NOVEMBER (11U)
# endif
# ifndef GTWM_MONTH_DECEMBER
#  define GTWM_MONTH_DECEMBER (12U)
# endif
# ifndef GTWM_MONTH_RESERVED13
#  define GTWM_MONTH_RESERVED13 (13U)
# endif
# ifndef GTWM_MONTH_RESERVED14
#  define GTWM_MONTH_RESERVED14 (14U)
# endif
# ifndef GTWM_MONTH_RESERVED15
#  define GTWM_MONTH_RESERVED15 (15U)
# endif
#endif

/* Enumeration type:        GTWM_Lang_t   */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_Lang_t
#define Rte_TypeDef_GTWM_Lang_t
typedef UINT8 GTWM_Lang_t;
#define GTWM_Lang_t_LowerLimit (0U)
#define GTWM_Lang_t_UpperLimit (24U)
# ifndef GTWM_LANG_SIMPLIFIEDCHINESE
#  define GTWM_LANG_SIMPLIFIEDCHINESE (0U)
# endif
# ifndef GTWM_LANG_UKENGLISH
#  define GTWM_LANG_UKENGLISH (1U)
# endif
# ifndef GTWM_LANG_NAENGLISH
#  define GTWM_LANG_NAENGLISH (2U)
# endif
# ifndef GTWM_LANG_SWEDISH
#  define GTWM_LANG_SWEDISH (3U)
# endif
# ifndef GTWM_LANG_FRENCH
#  define GTWM_LANG_FRENCH (4U)
# endif
# ifndef GTWM_LANG_SPANISH
#  define GTWM_LANG_SPANISH (5U)
# endif
# ifndef GTWM_LANG_DUTCH
#  define GTWM_LANG_DUTCH (6U)
# endif
# ifndef GTWM_LANG_PORTUGUESE
#  define GTWM_LANG_PORTUGUESE (7U)
# endif
# ifndef GTWM_LANG_NORWEGIAN
#  define GTWM_LANG_NORWEGIAN (8U)
# endif
# ifndef GTWM_LANG_FINNISH
#  define GTWM_LANG_FINNISH (9U)
# endif
# ifndef GTWM_LANG_DANISH
#  define GTWM_LANG_DANISH (10U)
# endif
# ifndef GTWM_LANG_GREEK
#  define GTWM_LANG_GREEK (11U)
# endif
# ifndef GTWM_LANG_JAPANESE
#  define GTWM_LANG_JAPANESE (12U)
# endif
# ifndef GTWM_LANG_ARABIC
#  define GTWM_LANG_ARABIC (13U)
# endif
# ifndef GTWM_LANG_GERMAN
#  define GTWM_LANG_GERMAN (14U)
# endif
# ifndef GTWM_LANG_POLISH
#  define GTWM_LANG_POLISH (15U)
# endif
# ifndef GTWM_LANG_TURKISH
#  define GTWM_LANG_TURKISH (16U)
# endif
# ifndef GTWM_LANG_KOREAN
#  define GTWM_LANG_KOREAN (17U)
# endif
# ifndef GTWM_LANG_TRADITIONALCHINESE
#  define GTWM_LANG_TRADITIONALCHINESE (18U)
# endif
# ifndef GTWM_LANG_ITALIAN
#  define GTWM_LANG_ITALIAN (19U)
# endif
# ifndef GTWM_LANG_HUNGARIAN
#  define GTWM_LANG_HUNGARIAN (20U)
# endif
# ifndef GTWM_LANG_CZECH
#  define GTWM_LANG_CZECH (21U)
# endif
# ifndef GTWM_LANG_SLOVAK
#  define GTWM_LANG_SLOVAK (22U)
# endif
# ifndef GTWM_LANG_RUSSIAN
#  define GTWM_LANG_RUSSIAN (23U)
# endif
# ifndef GTWM_LANG_THAI
#  define GTWM_LANG_THAI (24U)
# endif
#endif

/* Enumeration type:        GTWM_ClstrTabReq_t          */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_ClstrTabReq_t
#define Rte_TypeDef_GTWM_ClstrTabReq_t
typedef UINT8 GTWM_ClstrTabReq_t;
#define GTWM_ClstrTabReq_t_LowerLimit (0U)
#define GTWM_ClstrTabReq_t_UpperLimit (15U)
# ifndef GTWM_TABREQ_TRIP_COMPUTER
#  define GTWM_TABREQ_TRIP_COMPUTER (0U)
# endif
# ifndef GTWM_TABREQ_NAVIGATION
#  define GTWM_TABREQ_NAVIGATION (1U)
# endif
# ifndef GTWM_TABREQ_MEDIA
#  define GTWM_TABREQ_MEDIA (2U)
# endif
# ifndef GTWM_TABREQ_CALL
#  define GTWM_TABREQ_CALL (3U)
# endif
# ifndef GTWM_TABREQ_WARNINGINFORMATIONCENTRE
#  define GTWM_TABREQ_WARNINGINFORMATIONCENTRE (4U)
# endif
# ifndef GTWM_TABREQ_SETTING
#  define GTWM_TABREQ_SETTING (5U)
# endif
# ifndef GTWM_TABREQ_ACTIVESAFETY
#  define GTWM_TABREQ_ACTIVESAFETY (6U)
# endif
# ifndef GTWM_TABREQ_VRC
#  define GTWM_TABREQ_VRC (7U)
# endif
# ifndef GTWM_TABREQ_RESERVED8
#  define GTWM_TABREQ_RESERVED8 (8U)
# endif
# ifndef GTWM_TABREQ_RESERVED9
#  define GTWM_TABREQ_RESERVED9 (9U)
# endif
# ifndef GTWM_TABREQ_RESERVED10
#  define GTWM_TABREQ_RESERVED10 (10U)
# endif
# ifndef GTWM_TABREQ_RESERVED11
#  define GTWM_TABREQ_RESERVED11 (11U)
# endif
# ifndef GTWM_TABREQ_RESERVED12
#  define GTWM_TABREQ_RESERVED12 (12U)
# endif
# ifndef GTWM_TABREQ_RESERVED13
#  define GTWM_TABREQ_RESERVED13 (13U)
# endif
# ifndef GTWM_TABREQ_RESERVED14
#  define GTWM_TABREQ_RESERVED14 (14U)
# endif
# ifndef GTWM_TABREQ_RESERVED15
#  define GTWM_TABREQ_RESERVED15 (15U)
# endif
#endif

/* Enumeration type:        GTWM_BtnSts_t */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_BtnSts_t
#define Rte_TypeDef_GTWM_BtnSts_t
typedef UINT8 GTWM_BtnSts_t;
#define GTWM_BtnSts_t_LowerLimit (0U)
#define GTWM_BtnSts_t_UpperLimit (3U)
# ifndef GTWM_BUTTONSTS_NOPRESS
#  define GTWM_BUTTONSTS_NOPRESS (0U)
# endif
# ifndef GTWM_BUTTONSTS_SHORTPRESS
#  define GTWM_BUTTONSTS_SHORTPRESS (1U)
# endif
# ifndef GTWM_BUTTONSTS_LONGPRESS
#  define GTWM_BUTTONSTS_LONGPRESS (2U)
# endif
# ifndef GTWM_BUTTONSTS_RESERVED
#  define GTWM_BUTTONSTS_RESERVED (3U)
# endif
#endif

/* Enumeration type:        GTWM_DataV_t  */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_DataV_t
#define Rte_TypeDef_GTWM_DataV_t
typedef UINT8 GTWM_DataV_t;
#define GTWM_DataV_t_LowerLimit (0U)
#define GTWM_DataV_t_UpperLimit (1U)
# ifndef GTWM_DATAV_VALID
#  define GTWM_DATAV_VALID (0U)
# endif
# ifndef GTWM_DATAV_INVALID
#  define GTWM_DATAV_INVALID (1U)
# endif
#endif

/* Enumeration type:        GTWM_ClstrIllumLvl_t        */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_ClstrIllumLvl_t
#define Rte_TypeDef_GTWM_ClstrIllumLvl_t
typedef UINT8 GTWM_ClstrIllumLvl_t;
#define GTWM_ClstrIllumLvl_t_LowerLimit (0U)
#define GTWM_ClstrIllumLvl_t_UpperLimit (3U)
# ifndef GTWM_ILLUMLVL_LOW
#  define GTWM_ILLUMLVL_LOW (0U)
# endif
# ifndef GTWM_ILLUMLVL_NORMAL
#  define GTWM_ILLUMLVL_NORMAL (1U)
# endif
# ifndef GTWM_ILLUMLVL_HIGH
#  define GTWM_ILLUMLVL_HIGH (2U)
# endif
# ifndef GTWM_ILLUMLVL_RESERVED
#  define GTWM_ILLUMLVL_RESERVED (3U)
# endif
#endif

/* Enumeration type:        GTWM_DayNightMd_t           */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_DayNightMd_t
#define Rte_TypeDef_GTWM_DayNightMd_t
typedef UINT8 GTWM_DayNightMd_t;
#define GTWM_DayNightMd_t_LowerLimit (0U)
#define GTWM_DayNightMd_t_UpperLimit (3U)
# ifndef GTWM_DAYNIGHTMD_DAY
#  define GTWM_DAYNIGHTMD_DAY (0U)
# endif
# ifndef GTWM_DAYNIGHTMD_NIGHT
#  define GTWM_DAYNIGHTMD_NIGHT (1U)
# endif
# ifndef GTWM_DAYNIGHTMD_AUTO
#  define GTWM_DAYNIGHTMD_AUTO (2U)
# endif
# ifndef GTWM_DAYNIGHTMD_RESERVED
#  define GTWM_DAYNIGHTMD_RESERVED (3U)
# endif
#endif

/* Enumeration type:        GTWM_TPMSType_t             */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_TPMSType_t
#define Rte_TypeDef_GTWM_TPMSType_t
typedef UINT8 GTWM_TPMSType_t;
#define GTWM_TPMSType_t_LowerLimit (0U)
#define GTWM_TPMSType_t_UpperLimit (2U)
# ifndef GTWM_NO_TPMS
#  define GTWM_NO_TPMS (0U)
# endif
# ifndef GTWM_INDIRECT_TPMS
#  define GTWM_INDIRECT_TPMS (1U)
# endif
# ifndef GTWM_DIRECT_TPMS
#  define GTWM_DIRECT_TPMS (2U)
# endif
#endif

/* Enumeration type:        WKMD_ENUM_WorkModeStatus_t  */
/* Description:                           */
#ifndef Rte_TypeDef_WKMD_ENUM_WorkModeStatus_t
#define Rte_TypeDef_WKMD_ENUM_WorkModeStatus_t
typedef UINT8 WKMD_ENUM_WorkModeStatus_t;
#define WKMD_ENUM_WorkModeStatus_t_LowerLimit (0U)
#define WKMD_ENUM_WorkModeStatus_t_UpperLimit (5U)
# ifndef WKMD_SLEEP_STATE
#  define WKMD_SLEEP_STATE (0U)
# endif
# ifndef WKMD_CAN_NWM_ON_WAKEUP
#  define WKMD_CAN_NWM_ON_WAKEUP (1U)
# endif
# ifndef WKMD_FULL_HMI
#  define WKMD_FULL_HMI (2U)
# endif
# ifndef WKMD_NOTIFICATION_HMI
#  define WKMD_NOTIFICATION_HMI (3U)
# endif
# ifndef WKMD_CAN_NWM_OFF_WAKEUP
#  define WKMD_CAN_NWM_OFF_WAKEUP (4U)
# endif
# ifndef WKMD_EPB_STATUS
#  define WKMD_EPB_STATUS (5U)
# endif
#endif

/* Enumeration type:        DIMM_IlluminAdjustLevel_t   */
/* Description:                           */
#ifndef Rte_TypeDef_DIMM_IlluminAdjustLevel_t
#define Rte_TypeDef_DIMM_IlluminAdjustLevel_t
typedef UINT8 DIMM_IlluminAdjustLevel_t;
#define DIMM_IlluminAdjustLevel_t_LowerLimit (0U)
#define DIMM_IlluminAdjustLevel_t_UpperLimit (2U)
# ifndef DIMM_LEVEL_80
#  define DIMM_LEVEL_80 (0U)
# endif
# ifndef DIMM_LEVEL_100
#  define DIMM_LEVEL_100 (1U)
# endif
# ifndef DIMM_LEVEL_120
#  define DIMM_LEVEL_120 (2U)
# endif
#endif

/* Enumeration type:        GTWM_OffOn_t  */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_OffOn_t
#define Rte_TypeDef_GTWM_OffOn_t
typedef UINT8 GTWM_OffOn_t;
#define GTWM_OffOn_t_LowerLimit (0U)
#define GTWM_OffOn_t_UpperLimit (1U)
# ifndef GTWM_OFF
#  define GTWM_OFF (0U)
# endif
# ifndef GTWM_ON
#  define GTWM_ON (1U)
# endif
#endif

/* Enumeration type:        GTWM_VehMntnceSts_t         */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_VehMntnceSts_t
#define Rte_TypeDef_GTWM_VehMntnceSts_t
typedef UINT8 GTWM_VehMntnceSts_t;
#define GTWM_VehMntnceSts_t_LowerLimit (0U)
#define GTWM_VehMntnceSts_t_UpperLimit (3U)
# ifndef GTWM_VEHMNTNCESTS_STATUS_OK
#  define GTWM_VEHMNTNCESTS_STATUS_OK (0U)
# endif
# ifndef GTWM_VEHMNTNCESTS_SUGGEST_TO_MAINTAIN
#  define GTWM_VEHMNTNCESTS_SUGGEST_TO_MAINTAIN (1U)
# endif
# ifndef GTWM_VEHMNTNCESTS_MAINTAIN_IMMEDIATELY
#  define GTWM_VEHMNTNCESTS_MAINTAIN_IMMEDIATELY (2U)
# endif
# ifndef GTWM_VEHMNTNCESTS_RESERVED
#  define GTWM_VEHMNTNCESTS_RESERVED (3U)
# endif
#endif

/* Enumeration type:        GTWM_NavGudcSts_t           */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_NavGudcSts_t
#define Rte_TypeDef_GTWM_NavGudcSts_t
typedef UINT8 GTWM_NavGudcSts_t;
#define GTWM_NavGudcSts_t_LowerLimit (0U)
#define GTWM_NavGudcSts_t_UpperLimit (1U)
# ifndef GTWM_NAVGUDCSTS_NOT_IN_GUIDANCE
#  define GTWM_NAVGUDCSTS_NOT_IN_GUIDANCE (0U)
# endif
# ifndef GTWM_NAVGUDCSTS_IN_GUIDANCE
#  define GTWM_NAVGUDCSTS_IN_GUIDANCE (1U)
# endif
#endif

/* Enumeration type:        GTWM_ClstrThemeSetngAdjFI_t */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_ClstrThemeSetngAdjFI_t
#define Rte_TypeDef_GTWM_ClstrThemeSetngAdjFI_t
typedef UINT8 GTWM_ClstrThemeSetngAdjFI_t;
#define GTWM_ClstrThemeSetngAdjFI_t_LowerLimit (0U)
#define GTWM_ClstrThemeSetngAdjFI_t_UpperLimit (3U)
# ifndef GTWM_THEMESETNGADJFI_DEFAULT
#  define GTWM_THEMESETNGADJFI_DEFAULT (0U)
# endif
# ifndef GTWM_THEMESETNGADJFI_CLASSICAL
#  define GTWM_THEMESETNGADJFI_CLASSICAL (1U)
# endif
# ifndef GTWM_THEMESETNGADJFI_RESERVED2
#  define GTWM_THEMESETNGADJFI_RESERVED2 (2U)
# endif
# ifndef GTWM_THEMESETNGADJFI_RESERVED3
#  define GTWM_THEMESETNGADJFI_RESERVED3 (3U)
# endif
#endif

/* Enumeration type:        GTWM_FICMVcRcgtnSts_t       */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_FICMVcRcgtnSts_t
#define Rte_TypeDef_GTWM_FICMVcRcgtnSts_t
typedef UINT8 GTWM_FICMVcRcgtnSts_t;
#define GTWM_FICMVcRcgtnSts_t_LowerLimit (0U)
#define GTWM_FICMVcRcgtnSts_t_UpperLimit (1U)
# ifndef GTWM_VCRCGTNSTS_VR_NOT_RUNNING
#  define GTWM_VCRCGTNSTS_VR_NOT_RUNNING (0U)
# endif
# ifndef GTWM_VCRCGTNSTS_VR_RUNNING
#  define GTWM_VCRCGTNSTS_VR_RUNNING (1U)
# endif
#endif

/* Enumeration type:        TRCM_ResetTripByHMI_t       */
/* Description:                           */
#ifndef Rte_TypeDef_TRCM_ResetTripByHMI_t
#define Rte_TypeDef_TRCM_ResetTripByHMI_t
typedef UINT8 TRCM_ResetTripByHMI_t;
#define TRCM_ResetTripByHMI_t_LowerLimit (0U)
#define TRCM_ResetTripByHMI_t_UpperLimit (1U)
# ifndef TRCM_RESET_TRIP_BY_HMI_NORESET
#  define TRCM_RESET_TRIP_BY_HMI_NORESET (0U)
# endif
# ifndef TRCM_RESET_TRIP_BY_HMI_RESET
#  define TRCM_RESET_TRIP_BY_HMI_RESET (1U)
# endif
#endif

/* Enumeration type:        SIAM_ResetSIAByHMI_t        */
/* Description:                           */
#ifndef Rte_TypeDef_SIAM_ResetSIAByHMI_t
#define Rte_TypeDef_SIAM_ResetSIAByHMI_t
typedef UINT8 SIAM_ResetSIAByHMI_t;
#define SIAM_ResetSIAByHMI_t_LowerLimit (0U)
#define SIAM_ResetSIAByHMI_t_UpperLimit (1U)
# ifndef SIAM_RESET_SIA_BY_HMI_NORESET
#  define SIAM_RESET_SIA_BY_HMI_NORESET (0U)
# endif
# ifndef SIAM_RESET_SIA_BY_HMI_RESET
#  define SIAM_RESET_SIA_BY_HMI_RESET (1U)
# endif
#endif

/* Enumeration type:        SIAM_SIAInfo_t              */
/* Description:                           */
#ifndef Rte_TypeDef_SIAM_SIAInfo_t
#define Rte_TypeDef_SIAM_SIAInfo_t
typedef UINT8 SIAM_SIAInfo_t;
#define SIAM_SIAInfo_t_LowerLimit (0U)
#define SIAM_SIAInfo_t_UpperLimit (1U)
# ifndef SIAM_SIA_INFO_OFF
#  define SIAM_SIA_INFO_OFF (0U)
# endif
# ifndef SIAM_SIA_INFO_ON
#  define SIAM_SIA_INFO_ON (1U)
# endif
#endif

/* Enumeration type:        GTWM_TireSts_t              */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_TireSts_t
#define Rte_TypeDef_GTWM_TireSts_t
typedef UINT8 GTWM_TireSts_t;
#define GTWM_TireSts_t_LowerLimit (0U)
#define GTWM_TireSts_t_UpperLimit (7U)
# ifndef GTWM_TPS_NORMAL
#  define GTWM_TPS_NORMAL (0U)
# endif
# ifndef GTWM_TPS_UNKOWN
#  define GTWM_TPS_UNKOWN (1U)
# endif
# ifndef GTWM_TPS_PRESSURE_LOW
#  define GTWM_TPS_PRESSURE_LOW (2U)
# endif
# ifndef GTWM_TPS_QUICK_LEAK
#  define GTWM_TPS_QUICK_LEAK (3U)
# endif
# ifndef GTWM_TPS_PRESSURE_HIGH
#  define GTWM_TPS_PRESSURE_HIGH (4U)
# endif
# ifndef GTWM_TPS_TEMPERATURE_HIGH
#  define GTWM_TPS_TEMPERATURE_HIGH (5U)
# endif
# ifndef GTWM_TPS_AXLE_PRESSURE_IMBALANCE
#  define GTWM_TPS_AXLE_PRESSURE_IMBALANCE (6U)
# endif
# ifndef GTWM_TPS_BATTERY_LOW
#  define GTWM_TPS_BATTERY_LOW (7U)
# endif
#endif

/* Enumeration type:        GTWM_ProjectType_t          */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_ProjectType_t
#define Rte_TypeDef_GTWM_ProjectType_t
typedef UINT8 GTWM_ProjectType_t;
#define GTWM_ProjectType_t_LowerLimit (0U)
#define GTWM_ProjectType_t_UpperLimit (3U)
# ifndef GTWM_PROJECT_TYPE_IP3X
#  define GTWM_PROJECT_TYPE_IP3X (0U)
# endif
# ifndef GTWM_PROJECT_TYPE_AS2X
#  define GTWM_PROJECT_TYPE_AS2X (1U)
# endif
# ifndef GTWM_PROJECT_TYPE_IS12_Major
#  define GTWM_PROJECT_TYPE_IS12_Major (2U)
# endif
# ifndef GTWM_PROJECT_TYPE_EP21
#  define GTWM_PROJECT_TYPE_EP21 (3U)
# endif
#endif

/* Enumeration type:        GTWM_BntOpenSts_t           */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_BntOpenSts_t
#define Rte_TypeDef_GTWM_BntOpenSts_t
typedef UINT8 GTWM_BntOpenSts_t;
#define GTWM_BntOpenSts_t_LowerLimit (0U)
#define GTWM_BntOpenSts_t_UpperLimit (2U)
# ifndef GTWM_Bonnet_Closed
#  define GTWM_Bonnet_Closed (0U)
# endif
# ifndef GTWM_Bonnet_Open
#  define GTWM_Bonnet_Open (1U)
# endif
# ifndef GTWM_Bonnet_Switch_Disconnect
#  define GTWM_Bonnet_Switch_Disconnect (2U)
# endif
#endif

/* Enumeration type:        GTWM_LdspcOpenSts_t         */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_LdspcOpenSts_t
#define Rte_TypeDef_GTWM_LdspcOpenSts_t
typedef UINT8 GTWM_LdspcOpenSts_t;
#define GTWM_LdspcOpenSts_t_LowerLimit (0U)
#define GTWM_LdspcOpenSts_t_UpperLimit (1U)
# ifndef GTWM_Load_Space_Closed
#  define GTWM_Load_Space_Closed (0U)
# endif
# ifndef GTWM_Load_Space_Open
#  define GTWM_Load_Space_Open (1U)
# endif
#endif

/* Enumeration type:        GTWM_OpenSts_t              */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_OpenSts_t
#define Rte_TypeDef_GTWM_OpenSts_t
typedef UINT8 GTWM_OpenSts_t;
#define GTWM_OpenSts_t_LowerLimit (0U)
#define GTWM_OpenSts_t_UpperLimit (3U)
# ifndef GTWM_Door_Closed
#  define GTWM_Door_Closed (0U)
# endif
# ifndef GTWM_Door_Open
#  define GTWM_Door_Open (1U)
# endif
# ifndef GTWM_Door_Ajar
#  define GTWM_Door_Ajar (2U)
# endif
# ifndef GTWM_Door_Full_Open
#  define GTWM_Door_Full_Open (3U)
# endif
#endif

/* Enumeration type:        OSSM_Bool_t   */
/* Description:                           */
#ifndef Rte_TypeDef_OSSM_Bool_t
#define Rte_TypeDef_OSSM_Bool_t
typedef UINT8 OSSM_Bool_t;
#define OSSM_Bool_t_LowerLimit (0U)
#define OSSM_Bool_t_UpperLimit (1U)
# ifndef OSSM_FALSE
#  define OSSM_FALSE (0U)
# endif
# ifndef OSSM_TRUE
#  define OSSM_TRUE (1U)
# endif
#endif

/* Enumeration type:        GTWM_BMS_Lamp_t             */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_BMS_Lamp_t
#define Rte_TypeDef_GTWM_BMS_Lamp_t
typedef UINT8 GTWM_BMS_Lamp_t;
#define GTWM_BMS_Lamp_t_LowerLimit (0U)
#define GTWM_BMS_Lamp_t_UpperLimit (1U)
# ifndef GTWM_LAMP_ON
#  define GTWM_LAMP_ON (0U)
# endif
# ifndef GTWM_LAMP_OFF
#  define GTWM_LAMP_OFF (1U)
# endif
#endif

/* Enumeration type:        HTMM_RawStatus_t            */
/* Description:                           */
#ifndef Rte_TypeDef_HTMM_RawStatus_t
#define Rte_TypeDef_HTMM_RawStatus_t
typedef UINT8 HTMM_RawStatus_t;
#define HTMM_RawStatus_t_LowerLimit (0U)
#define HTMM_RawStatus_t_UpperLimit (8U)
# ifndef HTMM_RAW_STATUS_REST
#  define HTMM_RAW_STATUS_REST (0U)
# endif
# ifndef HTMM_RAW_STATUS_ENTER
#  define HTMM_RAW_STATUS_ENTER (1U)
# endif
# ifndef HTMM_RAW_STATUS_UP
#  define HTMM_RAW_STATUS_UP (2U)
# endif
# ifndef HTMM_RAW_STATUS_DOWN
#  define HTMM_RAW_STATUS_DOWN (3U)
# endif
# ifndef HTMM_RAW_STATUS_LEFT
#  define HTMM_RAW_STATUS_LEFT (4U)
# endif
# ifndef HTMM_RAW_STATUS_RIGHT
#  define HTMM_RAW_STATUS_RIGHT (5U)
# endif
# ifndef HTMM_RAW_STATUS_SHORT_TO_GND
#  define HTMM_RAW_STATUS_SHORT_TO_GND (6U)
# endif
# ifndef HTMM_RAW_STATUS_SHORT_TO_VCC
#  define HTMM_RAW_STATUS_SHORT_TO_VCC (7U)
# endif
# ifndef HTMM_RAW_STATUS_MAX_NUM
#  define HTMM_RAW_STATUS_MAX_NUM (8U)
# endif
#endif

/* Enumeration type:        HTMM_LongPressButton_t      */
/* Description:                           */
#ifndef Rte_TypeDef_HTMM_LongPressButton_t
#define Rte_TypeDef_HTMM_LongPressButton_t
typedef UINT8 HTMM_LongPressButton_t;
#define HTMM_LongPressButton_t_LowerLimit (0U)
#define HTMM_LongPressButton_t_UpperLimit (5U)
# ifndef HTMM_LONG_PRESS_ENTER
#  define HTMM_LONG_PRESS_ENTER (0U)
# endif
# ifndef HTMM_LONG_PRESS_UP
#  define HTMM_LONG_PRESS_UP (1U)
# endif
# ifndef HTMM_LONG_PRESS_DOWN
#  define HTMM_LONG_PRESS_DOWN (2U)
# endif
# ifndef HTMM_LONG_PRESS_LEFT
#  define HTMM_LONG_PRESS_LEFT (3U)
# endif
# ifndef HTMM_LONG_PRESS_RIGHT
#  define HTMM_LONG_PRESS_RIGHT (4U)
# endif
# ifndef HTMM_LONG_PRESS_NONE
#  define HTMM_LONG_PRESS_NONE (5U)
# endif
#endif

/* Enumeration type:        VRCM_VoiceRequestImageState_t             */
/* Description:                           */
#ifndef Rte_TypeDef_VRCM_VoiceRequestImageState_t
#define Rte_TypeDef_VRCM_VoiceRequestImageState_t
typedef UINT8 VRCM_VoiceRequestImageState_t;
#define VRCM_VoiceRequestImageState_t_LowerLimit (0U)
#define VRCM_VoiceRequestImageState_t_UpperLimit (1U)
# ifndef VRCM_VOICE_REQUEST_IMAGE_STATE_OFF
#  define VRCM_VOICE_REQUEST_IMAGE_STATE_OFF (0U)
# endif
# ifndef VRCM_VOICE_REQUEST_IMAGE_STATE_ON
#  define VRCM_VOICE_REQUEST_IMAGE_STATE_ON (1U)
# endif
#endif

/* Enumeration type:        WKMM_FunctionVoltageState_t */
/* Description:                           */
#ifndef Rte_TypeDef_WKMM_FunctionVoltageState_t
#define Rte_TypeDef_WKMM_FunctionVoltageState_t
typedef UINT8 WKMM_FunctionVoltageState_t;
#define WKMM_FunctionVoltageState_t_LowerLimit (0U)
#define WKMM_FunctionVoltageState_t_UpperLimit (2U)
# ifndef WKMM_FunctionVoltageState_NORMAL
#  define WKMM_FunctionVoltageState_NORMAL (0U)
# endif
# ifndef WKMM_FunctionVoltageState_LOW
#  define WKMM_FunctionVoltageState_LOW (1U)
# endif
# ifndef WKMM_FunctionVoltageState_HIGH
#  define WKMM_FunctionVoltageState_HIGH (2U)
# endif
#endif

/* Enumeration type:        UNIT_ThemeStyle_t           */
/* Description:                           */
#ifndef Rte_TypeDef_UNIT_ThemeStyle_t
#define Rte_TypeDef_UNIT_ThemeStyle_t
typedef UINT8 UNIT_ThemeStyle_t;
#define UNIT_ThemeStyle_t_LowerLimit (0U)
#define UNIT_ThemeStyle_t_UpperLimit (4U)
# ifndef UNIT_CoolStyle
#  define UNIT_CoolStyle (0U)
# endif
# ifndef UNIT_ClassStyle
#  define UNIT_ClassStyle (1U)
# endif
# ifndef UNIT_NormalStyle
#  define UNIT_NormalStyle (2U)
# endif
# ifndef UNIT_EcoStyle
#  define UNIT_EcoStyle (3U)
# endif
# ifndef UNIT_SportStyle
#  define UNIT_SportStyle (4U)
# endif
#endif

/* Enumeration type:        IM_DIMM_LcdDefect_t         */
/* Description:                           */
#ifndef Rte_TypeDef_IM_DIMM_LcdDefect_t
#define Rte_TypeDef_IM_DIMM_LcdDefect_t
typedef UINT8 IM_DIMM_LcdDefect_t;
#define IM_DIMM_LcdDefect_t_LowerLimit (0U)
#define IM_DIMM_LcdDefect_t_UpperLimit (3U)
# ifndef IM_DIMM_LCD_NO_DEFECT
#  define IM_DIMM_LCD_NO_DEFECT (0U)
# endif
# ifndef IM_DIMM_LCD_HAS_DEFECT
#  define IM_DIMM_LCD_HAS_DEFECT (1U)
# endif
# ifndef IM_DIMM_LCD_DEFECT_CONFIRMING
#  define IM_DIMM_LCD_DEFECT_CONFIRMING (2U)
# endif
# ifndef IM_DIMM_LCD_DEFECT_FAIL_BY_GPU
#  define IM_DIMM_LCD_DEFECT_FAIL_BY_GPU (3U)
# endif
#endif

/* Enumeration type:        GTWM_FICMEn_t */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_FICMEn_t
#define Rte_TypeDef_GTWM_FICMEn_t
typedef UINT8 GTWM_FICMEn_t;
#define GTWM_FICMEn_t_LowerLimit (0U)
#define GTWM_FICMEn_t_UpperLimit (2U)
# ifndef GTWM_FICMEN_WITHOUTCAN_WITHOUTLVDS
#  define GTWM_FICMEN_WITHOUTCAN_WITHOUTLVDS (0U)
# endif
# ifndef GTWM_FICMEN_WITHCAN_WITHOUTLVDS
#  define GTWM_FICMEN_WITHCAN_WITHOUTLVDS (1U)
# endif
# ifndef GTWM_FICMEN_WITHCAN_WITHLVDS
#  define GTWM_FICMEN_WITHCAN_WITHLVDS (2U)
# endif
#endif

/* Enumeration type:        GTWM_EnableStatus_t         */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_EnableStatus_t
#define Rte_TypeDef_GTWM_EnableStatus_t
typedef UINT8 GTWM_EnableStatus_t;
#define GTWM_EnableStatus_t_LowerLimit (0U)
#define GTWM_EnableStatus_t_UpperLimit (1U)
# ifndef GTWM_DISABLED
#  define GTWM_DISABLED (0U)
# endif
# ifndef GTWM_ENABLED
#  define GTWM_ENABLED (1U)
# endif
#endif

/* Enumeration type:        GLOBAL_HMIState_t           */
/* Description:                           */
#ifndef Rte_TypeDef_GLOBAL_HMIState_t
#define Rte_TypeDef_GLOBAL_HMIState_t
typedef UINT8 GLOBAL_HMIState_t;
#define GLOBAL_HMIState_t_LowerLimit (0U)
#define GLOBAL_HMIState_t_UpperLimit (12U)
# ifndef HMI_DisplayOff
#  define HMI_DisplayOff (0U)
# endif
# ifndef HMI_NotificationEnable
#  define HMI_NotificationEnable (1U)
# endif
# ifndef HMI_StartKeyOn
#  define HMI_StartKeyOn (2U)
# endif
# ifndef HMI_StartSelfCheck
#  define HMI_StartSelfCheck (3U)
# endif
# ifndef HMI_ModeStart
#  define HMI_ModeStart (4U)
# endif
# ifndef HMI_StartCharging
#  define HMI_StartCharging (5U)
# endif
# ifndef HMI_DisplayCharging
#  define HMI_DisplayCharging (6U)
# endif
# ifndef HMI_DisplayNormal
#  define HMI_DisplayNormal (7U)
# endif
# ifndef HMI_StartModeSwitch
#  define HMI_StartModeSwitch (8U)
# endif
# ifndef HMI_StartKeyOff
#  define HMI_StartKeyOff (9U)
# endif
# ifndef HMI_GaugeIdle
#  define HMI_GaugeIdle (10U)
# endif
# ifndef HMI_GaugeNormal
#  define HMI_GaugeNormal (11U)
# endif
# ifndef HMI_Exit
#  define HMI_Exit (12U)
# endif
#endif

/* Enumeration type:        GPMM_CtrlCmd_t              */
/* Description:                           */
#ifndef Rte_TypeDef_GPMM_CtrlCmd_t
#define Rte_TypeDef_GPMM_CtrlCmd_t
typedef UINT8 GPMM_CtrlCmd_t;
#define GPMM_CtrlCmd_t_LowerLimit (0U)
#define GPMM_CtrlCmd_t_UpperLimit (1U)
# ifndef GPMM_CTRL_CMD_NONE
#  define GPMM_CTRL_CMD_NONE (0U)
# endif
# ifndef GPMM_CTRL_CMD_SHUT_DOWN
#  define GPMM_CTRL_CMD_SHUT_DOWN (1U)
# endif
#endif

/* Enumeration type:        GPMM_ShutDownAck_t          */
/* Description:                           */
#ifndef Rte_TypeDef_GPMM_ShutDownAck_t
#define Rte_TypeDef_GPMM_ShutDownAck_t
typedef UINT8 GPMM_ShutDownAck_t;
#define GPMM_ShutDownAck_t_LowerLimit (0U)
#define GPMM_ShutDownAck_t_UpperLimit (1U)
# ifndef GPMM_NOT_ALLOW_TO_BE_SHUT_DOWN
#  define GPMM_NOT_ALLOW_TO_BE_SHUT_DOWN (0U)
# endif
# ifndef GPMM_ALLOW_TO_BE_SHUT_DOWN
#  define GPMM_ALLOW_TO_BE_SHUT_DOWN (1U)
# endif
#endif

/* Enumeration type:        DIAG_PatternOperation_t     */
/* Description:                           */
#ifndef Rte_TypeDef_DIAG_PatternOperation_t
#define Rte_TypeDef_DIAG_PatternOperation_t
typedef UINT8 DIAG_PatternOperation_t;
#define DIAG_PatternOperation_t_LowerLimit (0U)
#define DIAG_PatternOperation_t_UpperLimit (1U)
# ifndef DIAG_STOP_PATTERN
#  define DIAG_STOP_PATTERN (0U)
# endif
# ifndef DIAG_START_PATTERN
#  define DIAG_START_PATTERN (1U)
# endif
#endif

/* Enumeration type:        DIAG_SelfTestPattern_t      */
/* Description:                           */
#ifndef Rte_TypeDef_DIAG_SelfTestPattern_t
#define Rte_TypeDef_DIAG_SelfTestPattern_t
typedef UINT8 DIAG_SelfTestPattern_t;
#define DIAG_SelfTestPattern_t_LowerLimit (0U)
#define DIAG_SelfTestPattern_t_UpperLimit (4U)
# ifndef DIAG_WHITE_PATTERN
#  define DIAG_WHITE_PATTERN (0U)
# endif
# ifndef DIAG_BLACK_PATTERN
#  define DIAG_BLACK_PATTERN (1U)
# endif
# ifndef DIAG_RED_PATTERN
#  define DIAG_RED_PATTERN (2U)
# endif
# ifndef DIAG_GREEN_PATTERN
#  define DIAG_GREEN_PATTERN (3U)
# endif
# ifndef DIAG_BLUE_PATTERN
#  define DIAG_BLUE_PATTERN (4U)
# endif
#endif

/* Enumeration type:        GTWM_insStylePrm_t          */
/* Description:                           */
#ifndef Rte_TypeDef_GTWM_insStylePrm_t
#define Rte_TypeDef_GTWM_insStylePrm_t
typedef UINT8 GTWM_insStylePrm_t;
#define GTWM_insStylePrm_t_LowerLimit (0U)
#define GTWM_insStylePrm_t_UpperLimit (3U)
# ifndef GTWM_STYLE_SPORT
#  define GTWM_STYLE_SPORT (0U)
# endif
# ifndef GTWM_STYLE_NORMAL
#  define GTWM_STYLE_NORMAL (1U)
# endif
# ifndef GTWM_STYLE_ECO
#  define GTWM_STYLE_ECO (2U)
# endif
# ifndef GTWM_STYLE_DEPEND_DM
#  define GTWM_STYLE_DEPEND_DM (3U)
# endif
#endif

/* Enumeration type:        IM_ALRM_TelltaleStatus_t    */
/* Description:                           */
#ifndef Rte_TypeDef_IM_ALRM_TelltaleStatus_t
#define Rte_TypeDef_IM_ALRM_TelltaleStatus_t
typedef UINT8 IM_ALRM_TelltaleStatus_t;
#define IM_ALRM_TelltaleStatus_t_LowerLimit (0U)
#define IM_ALRM_TelltaleStatus_t_UpperLimit (33U)
# ifndef IM_ALRM_TELLTALE_OFF
#  define IM_ALRM_TELLTALE_OFF (0U)
# endif
# ifndef IM_ALRM_TELLTALE_GREEN
#  define IM_ALRM_TELLTALE_GREEN (1U)
# endif
# ifndef IM_ALRM_TELLTALE_YELLOW
#  define IM_ALRM_TELLTALE_YELLOW (2U)
# endif
# ifndef IM_ALRM_TELLTALE_RED
#  define IM_ALRM_TELLTALE_RED (3U)
# endif
# ifndef IM_ALRM_TELLTALE_WHITE
#  define IM_ALRM_TELLTALE_WHITE (4U)
# endif
# ifndef IM_ALRM_TELLTALE_BLUE
#  define IM_ALRM_TELLTALE_BLUE (5U)
# endif
# ifndef IM_ALRM_TELLTALE_GREY
#  define IM_ALRM_TELLTALE_GREY (6U)
# endif
# ifndef IM_ALRM_TELLTALE_ORANGE
#  define IM_ALRM_TELLTALE_ORANGE (7U)
# endif
# ifndef IM_ALRM_TELLTALE_GREEN_FLASH_0_5HZ
#  define IM_ALRM_TELLTALE_GREEN_FLASH_0_5HZ (8U)
# endif
# ifndef IM_ALRM_TELLTALE_GREEN_FLASH_1HZ
#  define IM_ALRM_TELLTALE_GREEN_FLASH_1HZ (9U)
# endif
# ifndef IM_ALRM_TELLTALE_GREEN_FLASH_2HZ
#  define IM_ALRM_TELLTALE_GREEN_FLASH_2HZ (10U)
# endif
# ifndef IM_ALRM_TELLTALE_YELLOW_FLASH_0_5HZ
#  define IM_ALRM_TELLTALE_YELLOW_FLASH_0_5HZ (11U)
# endif
# ifndef IM_ALRM_TELLTALE_YELLOW_FLASH_1HZ
#  define IM_ALRM_TELLTALE_YELLOW_FLASH_1HZ (12U)
# endif
# ifndef IM_ALRM_TELLTALE_YELLOW_FLASH_2HZ
#  define IM_ALRM_TELLTALE_YELLOW_FLASH_2HZ (13U)
# endif
# ifndef IM_ALRM_TELLTALE_RED_FLASH_0_5HZ
#  define IM_ALRM_TELLTALE_RED_FLASH_0_5HZ (14U)
# endif
# ifndef IM_ALRM_TELLTALE_RED_FLASH_1HZ
#  define IM_ALRM_TELLTALE_RED_FLASH_1HZ (15U)
# endif
# ifndef IM_ALRM_TELLTALE_RED_FLASH_2HZ
#  define IM_ALRM_TELLTALE_RED_FLASH_2HZ (16U)
# endif
# ifndef IM_ALRM_TELLTALE_WHITE_FLASH_0_5HZ
#  define IM_ALRM_TELLTALE_WHITE_FLASH_0_5HZ (17U)
# endif
# ifndef IM_ALRM_TELLTALE_WHITE_FLASH_1HZ
#  define IM_ALRM_TELLTALE_WHITE_FLASH_1HZ (18U)
# endif
# ifndef IM_ALRM_TELLTALE_WHITE_FLASH_2HZ
#  define IM_ALRM_TELLTALE_WHITE_FLASH_2HZ (19U)
# endif
# ifndef IM_ALRM_TELLTALE_BLUE_FLASH_0_5HZ
#  define IM_ALRM_TELLTALE_BLUE_FLASH_0_5HZ (20U)
# endif
# ifndef IM_ALRM_TELLTALE_BLUE_FLASH_1HZ
#  define IM_ALRM_TELLTALE_BLUE_FLASH_1HZ (21U)
# endif
# ifndef IM_ALRM_TELLTALE_BLUE_FLASH_2HZ
#  define IM_ALRM_TELLTALE_BLUE_FLASH_2HZ (22U)
# endif
# ifndef IM_ALRM_TELLTALE_GREY_FLASH_0_5HZ
#  define IM_ALRM_TELLTALE_GREY_FLASH_0_5HZ (23U)
# endif
# ifndef IM_ALRM_TELLTALE_GREY_FLASH_1HZ
#  define IM_ALRM_TELLTALE_GREY_FLASH_1HZ (24U)
# endif
# ifndef IM_ALRM_TELLTALE_GREY_FLASH_2HZ
#  define IM_ALRM_TELLTALE_GREY_FLASH_2HZ (25U)
# endif
# ifndef IM_ALRM_TELLTALE_ORANGE_FLASH_0_5HZ
#  define IM_ALRM_TELLTALE_ORANGE_FLASH_0_5HZ (26U)
# endif
# ifndef IM_ALRM_TELLTALE_ORANGE_FLASH_1HZ
#  define IM_ALRM_TELLTALE_ORANGE_FLASH_1HZ (27U)
# endif
# ifndef IM_ALRM_TELLTALE_ORANGE_FLASH_2HZ
#  define IM_ALRM_TELLTALE_ORANGE_FLASH_2HZ (28U)
# endif
# ifndef IM_ALRM_TELLTALE_COMFORT_MODE
#  define IM_ALRM_TELLTALE_COMFORT_MODE (29U)
# endif
# ifndef IM_ALRM_TELLTALE_ECO_MODE
#  define IM_ALRM_TELLTALE_ECO_MODE (30U)
# endif
# ifndef IM_ALRM_TELLTALE_SPORT_MODE
#  define IM_ALRM_TELLTALE_SPORT_MODE (31U)
# endif
# ifndef IM_ALRM_TELLTALE_OFFROAD_MODE
#  define IM_ALRM_TELLTALE_OFFROAD_MODE (32U)
# endif
# ifndef IM_ALRM_TELLTALE_SNOW_MODE
#  define IM_ALRM_TELLTALE_SNOW_MODE (33U)
# endif
#endif

/* Enumeration type:        IM_ALRM_TelltaleId_t        */
/* Description:                           */
#ifndef Rte_TypeDef_IM_ALRM_TelltaleId_t
#define Rte_TypeDef_IM_ALRM_TelltaleId_t
typedef UINT8 IM_ALRM_TelltaleId_t;
#define IM_ALRM_TelltaleId_t_LowerLimit (0U)
#define IM_ALRM_TelltaleId_t_UpperLimit (43U)
# ifndef IM_ALRM_RESERVED_TELLTALE_ID
#  define IM_ALRM_RESERVED_TELLTALE_ID (0U)
# endif
# ifndef IM_ALRM_TURN_LEFT_TELLTALE_ID
#  define IM_ALRM_TURN_LEFT_TELLTALE_ID (1U)
# endif
# ifndef IM_ALRM_TURN_RIGHT_TELLTALE_ID
#  define IM_ALRM_TURN_RIGHT_TELLTALE_ID (2U)
# endif
# ifndef IM_ALRM_HAZARD_TELLTALE_ID
#  define IM_ALRM_HAZARD_TELLTALE_ID (3U)
# endif
# ifndef IM_ALRM_HIGH_BEAM_TELLTALE_ID
#  define IM_ALRM_HIGH_BEAM_TELLTALE_ID (4U)
# endif
# ifndef IM_ALRM_FRONT_FOG_TELLTALE_ID
#  define IM_ALRM_FRONT_FOG_TELLTALE_ID (5U)
# endif
# ifndef IM_ALRM_REAR_FOG_TELLTALE_ID
#  define IM_ALRM_REAR_FOG_TELLTALE_ID (6U)
# endif
# ifndef IM_ALRM_DAY_RUNNING_LIGHT_TELLTALE_ID
#  define IM_ALRM_DAY_RUNNING_LIGHT_TELLTALE_ID (7U)
# endif
# ifndef IM_ALRM_IHBC_INDICATION_TELLTALE_ID
#  define IM_ALRM_IHBC_INDICATION_TELLTALE_ID (8U)
# endif
# ifndef IM_ALRM_POSITION_LIGHT_TELLTALE_ID
#  define IM_ALRM_POSITION_LIGHT_TELLTALE_ID (9U)
# endif
# ifndef IM_ALRM_ABS_MALFUNCTION_TELLTALE_ID
#  define IM_ALRM_ABS_MALFUNCTION_TELLTALE_ID (10U)
# endif
# ifndef IM_ALRM_EBD_FAILURE_TELLTALE_ID
#  define IM_ALRM_EBD_FAILURE_TELLTALE_ID (11U)
# endif
# ifndef IM_ALRM_BRAKE_FLUID_LOW_TELLTALE_ID
#  define IM_ALRM_BRAKE_FLUID_LOW_TELLTALE_ID (12U)
# endif
# ifndef IM_ALRM_EPB_STATUS_TELLTALE_ID
#  define IM_ALRM_EPB_STATUS_TELLTALE_ID (13U)
# endif
# ifndef IM_ALRM_EPB_FAILURE_TELLTALE_ID
#  define IM_ALRM_EPB_FAILURE_TELLTALE_ID (14U)
# endif
# ifndef IM_ALRM_AVH_STATUS_TELLTALE_ID
#  define IM_ALRM_AVH_STATUS_TELLTALE_ID (15U)
# endif
# ifndef IM_ALRM_SRS_MALFUNCTION_TELLTALE_ID
#  define IM_ALRM_SRS_MALFUNCTION_TELLTALE_ID (16U)
# endif
# ifndef IM_ALRM_SEATBELT_WARNING_TELLTALE_ID
#  define IM_ALRM_SEATBELT_WARNING_TELLTALE_ID (17U)
# endif
# ifndef IM_ALRM_MIL_FAILURE_TELLTALE_ID
#  define IM_ALRM_MIL_FAILURE_TELLTALE_ID (18U)
# endif
# ifndef IM_ALRM_SVS_FAILURE_TELLTALE_ID
#  define IM_ALRM_SVS_FAILURE_TELLTALE_ID (19U)
# endif
# ifndef IM_ALRM_ESC_STATUS_TELLTALE_ID
#  define IM_ALRM_ESC_STATUS_TELLTALE_ID (20U)
# endif
# ifndef IM_ALRM_ESC_OFF_TELLTALE_ID
#  define IM_ALRM_ESC_OFF_TELLTALE_ID (21U)
# endif
# ifndef IM_ALRM_ENGINE_COOLANT_TEMP_TELLTALE_ID
#  define IM_ALRM_ENGINE_COOLANT_TEMP_TELLTALE_ID (22U)
# endif
# ifndef IM_ALRM_LOW_FUEL_WARNING_TELLTALE_ID
#  define IM_ALRM_LOW_FUEL_WARNING_TELLTALE_ID (23U)
# endif
# ifndef IM_ALRM_LOW_OIL_LEVEL_TELLTALE_ID
#  define IM_ALRM_LOW_OIL_LEVEL_TELLTALE_ID (24U)
# endif
# ifndef IM_ALRM_BATTERY_CHARGE_TELLTALE_ID
#  define IM_ALRM_BATTERY_CHARGE_TELLTALE_ID (25U)
# endif
# ifndef IM_ALRM_INTELLIGENT_CRUISE_CONTROL_TELLTALE_ID
#  define IM_ALRM_INTELLIGENT_CRUISE_CONTROL_TELLTALE_ID (26U)
# endif
# ifndef IM_ALRM_ADAPTIVE_CRUISE_CONTROL_TELLTALE_ID
#  define IM_ALRM_ADAPTIVE_CRUISE_CONTROL_TELLTALE_ID (27U)
# endif
# ifndef IM_ALRM_ACC_ERROR_TELLTALE_ID
#  define IM_ALRM_ACC_ERROR_TELLTALE_ID (28U)
# endif
# ifndef IM_ALRM_EPS_FAILURE_TELLTALE_ID
#  define IM_ALRM_EPS_FAILURE_TELLTALE_ID (29U)
# endif
# ifndef IM_ALRM_TPMS_TELLTALE_ID
#  define IM_ALRM_TPMS_TELLTALE_ID (30U)
# endif
# ifndef IM_ALRM_LKA_SYSTEM_TELLTALE_ID
#  define IM_ALRM_LKA_SYSTEM_TELLTALE_ID (31U)
# endif
# ifndef IM_ALRM_TCU_FAILURE_TELLTALE_ID
#  define IM_ALRM_TCU_FAILURE_TELLTALE_ID (32U)
# endif
# ifndef IM_ALRM_VEHICL_LAMP_FAILURE_TELLTALE_ID
#  define IM_ALRM_VEHICL_LAMP_FAILURE_TELLTALE_ID (33U)
# endif
# ifndef IM_ALRM_HDC_ASSISTANCE_TELLTALE_ID
#  define IM_ALRM_HDC_ASSISTANCE_TELLTALE_ID (34U)
# endif
# ifndef IM_ALRM_START_STOP_TELLTALE_ID
#  define IM_ALRM_START_STOP_TELLTALE_ID (35U)
# endif
# ifndef IM_ALRM_DRIVING_MODE_TELLTALE_ID
#  define IM_ALRM_DRIVING_MODE_TELLTALE_ID (36U)
# endif
# ifndef IM_ALRM_AFS_OFF_TELLTALE_ID
#  define IM_ALRM_AFS_OFF_TELLTALE_ID (37U)
# endif
# ifndef IM_ALRM_AEB_TELLTALE_ID
#  define IM_ALRM_AEB_TELLTALE_ID (38U)
# endif
# ifndef IM_ALRM_LOW_BEAM_TELLTALE_ID
#  define IM_ALRM_LOW_BEAM_TELLTALE_ID (39U)
# endif
# ifndef IM_ALRM_CRUISE_CONTROL_TELLTALE_ID
#  define IM_ALRM_CRUISE_CONTROL_TELLTALE_ID (40U)
# endif
# ifndef IM_ALRM_SPEED_LIMIT_TELLTALE_ID
#  define IM_ALRM_SPEED_LIMIT_TELLTALE_ID (41U)
# endif
# ifndef IM_ALRM_SPEED_LIMIT_CANCEL_TELLTALE_ID
#  define IM_ALRM_SPEED_LIMIT_CANCEL_TELLTALE_ID (42U)
# endif
# ifndef IM_ALRM_MAX_TELLTALE_ID
#  define IM_ALRM_MAX_TELLTALE_ID (43U)
# endif
#endif

/* Enumeration type:        IM_ALRM_MsgStatus_t         */
/* Description:                           */
#ifndef Rte_TypeDef_IM_ALRM_MsgStatus_t
#define Rte_TypeDef_IM_ALRM_MsgStatus_t
typedef UINT8 IM_ALRM_MsgStatus_t;
#define IM_ALRM_MsgStatus_t_LowerLimit (0U)
#define IM_ALRM_MsgStatus_t_UpperLimit (1U)
# ifndef IM_ALRM_MESSAGE_OFF
#  define IM_ALRM_MESSAGE_OFF (0U)
# endif
# ifndef IM_ALRM_MESSAGE_ON
#  define IM_ALRM_MESSAGE_ON (1U)
# endif
#endif

/* Enumeration type:        IM_ALRM_ButtonStatus_t      */
/* Description:                           */
#ifndef Rte_TypeDef_IM_ALRM_ButtonStatus_t
#define Rte_TypeDef_IM_ALRM_ButtonStatus_t
typedef UINT8 IM_ALRM_ButtonStatus_t;
#define IM_ALRM_ButtonStatus_t_LowerLimit (0U)
#define IM_ALRM_ButtonStatus_t_UpperLimit (2U)
# ifndef IM_ALRM_MESSAGE_BUTTON_NULL
#  define IM_ALRM_MESSAGE_BUTTON_NULL (0U)
# endif
# ifndef IM_ALRM_MESSAGE_BUTTON_CONFIRM
#  define IM_ALRM_MESSAGE_BUTTON_CONFIRM (1U)
# endif
# ifndef IM_ALRM_MESSAGE_BUTTON_REDISPLAY
#  define IM_ALRM_MESSAGE_BUTTON_REDISPLAY (2U)
# endif
#endif

/* Enumeration type:        IM_ALRM_MsgId_t             */
/* Description:                           */
#ifndef Rte_TypeDef_IM_ALRM_MsgId_t
#define Rte_TypeDef_IM_ALRM_MsgId_t
typedef UINT8 IM_ALRM_MsgId_t;
#define IM_ALRM_MsgId_t_LowerLimit (0U)
#define IM_ALRM_MsgId_t_UpperLimit (95U)
# ifndef IM_ALRM_RESERVED_MSG_ID
#  define IM_ALRM_RESERVED_MSG_ID (0U)
# endif
# ifndef IM_ALRM_KEY_NEED_CLOSE_MSG_ID
#  define IM_ALRM_KEY_NEED_CLOSE_MSG_ID (1U)
# endif
# ifndef IM_ALRM_KEY_NOFOUND_MSG_ID
#  define IM_ALRM_KEY_NOFOUND_MSG_ID (2U)
# endif
# ifndef IM_ALRM_IMMO_FAILURE_MSG_ID
#  define IM_ALRM_IMMO_FAILURE_MSG_ID (3U)
# endif
# ifndef IM_ALRM_PRESS_BRAKE_START_MSG_ID
#  define IM_ALRM_PRESS_BRAKE_START_MSG_ID (4U)
# endif
# ifndef IM_ALRM_PRESS_CLUTCH_START_MSG_ID
#  define IM_ALRM_PRESS_CLUTCH_START_MSG_ID (5U)
# endif
# ifndef IM_ALRM_SHIFT_PN_START_MSG_ID
#  define IM_ALRM_SHIFT_PN_START_MSG_ID (6U)
# endif
# ifndef IM_ALRM_IGN_AGAIN_SHIFT_P_MSG_ID
#  define IM_ALRM_IGN_AGAIN_SHIFT_P_MSG_ID (7U)
# endif
# ifndef IM_ALRM_ENGINE_INHIBIT_START_MSG_ID
#  define IM_ALRM_ENGINE_INHIBIT_START_MSG_ID (8U)
# endif
# ifndef IM_ALRM_SEATBELT_CLOSE_DOOR_MSG_ID
#  define IM_ALRM_SEATBELT_CLOSE_DOOR_MSG_ID (9U)
# endif
# ifndef IM_ALRM_WEAR_SEATBELT_MSG_ID
#  define IM_ALRM_WEAR_SEATBELT_MSG_ID (10U)
# endif
# ifndef IM_ALRM_PRESS_BRAKE_PEDAL_MEANWHILE_MSG_ID
#  define IM_ALRM_PRESS_BRAKE_PEDAL_MEANWHILE_MSG_ID (11U)
# endif
# ifndef IM_ALRM_SHIFT_GEAR_LEVER_TO_RIGHT_MSG_ID
#  define IM_ALRM_SHIFT_GEAR_LEVER_TO_RIGHT_MSG_ID (12U)
# endif
# ifndef IM_ALRM_PRESS_BRAKE_MSG_ID
#  define IM_ALRM_PRESS_BRAKE_MSG_ID (13U)
# endif
# ifndef IM_ALRM_PRESS_UNLOCK_KEY_MSG_ID
#  define IM_ALRM_PRESS_UNLOCK_KEY_MSG_ID (14U)
# endif
# ifndef IM_ALRM_PRESS_BRAKE_UNLOCK_MSG_ID
#  define IM_ALRM_PRESS_BRAKE_UNLOCK_MSG_ID (15U)
# endif
# ifndef IM_ALRM_WAIT_STILL_SHIFT_P_MSG_ID
#  define IM_ALRM_WAIT_STILL_SHIFT_P_MSG_ID (16U)
# endif
# ifndef IM_ALRM_GEARBOX_LEARN_PRESS_BRAKE_MSG_ID
#  define IM_ALRM_GEARBOX_LEARN_PRESS_BRAKE_MSG_ID (17U)
# endif
# ifndef IM_ALRM_SLOWDOWN_CONTACT_SALE_MSG_ID
#  define IM_ALRM_SLOWDOWN_CONTACT_SALE_MSG_ID (18U)
# endif
# ifndef IM_ALRM_GEARBOX_OVERHEATED_MSG_ID
#  define IM_ALRM_GEARBOX_OVERHEATED_MSG_ID (19U)
# endif
# ifndef IM_ALRM_GEARBOX_OVERHEATED_WAIT_5_MIN_MSG_ID
#  define IM_ALRM_GEARBOX_OVERHEATED_WAIT_5_MIN_MSG_ID (20U)
# endif
# ifndef IM_ALRM_KEEP_SAFE_LANE_SPACE_MSG_ID
#  define IM_ALRM_KEEP_SAFE_LANE_SPACE_MSG_ID (21U)
# endif
# ifndef IM_ALRM_PRESS_BRAKE_PEDAL_MSG_ID
#  define IM_ALRM_PRESS_BRAKE_PEDAL_MSG_ID (22U)
# endif
# ifndef IM_ALRM_FRONT_RADAR_BLIND_MSG_ID
#  define IM_ALRM_FRONT_RADAR_BLIND_MSG_ID (23U)
# endif
# ifndef IM_ALRM_KEEP_HANDS_ON_WHEEL_MSG_ID
#  define IM_ALRM_KEEP_HANDS_ON_WHEEL_MSG_ID (24U)
# endif
# ifndef IM_ALRM_FRONT_CAMERA_MALFUNCTION_MSG_ID
#  define IM_ALRM_FRONT_CAMERA_MALFUNCTION_MSG_ID (25U)
# endif
# ifndef IM_ALRM_VEHICLE_LIGHT_ON_MSG_ID
#  define IM_ALRM_VEHICLE_LIGHT_ON_MSG_ID (26U)
# endif
# ifndef IM_ALRM_KEY_LOW_BATT_MSG_ID
#  define IM_ALRM_KEY_LOW_BATT_MSG_ID (27U)
# endif
# ifndef IM_ALRM_START_STOP_FAILURE_MSG_ID
#  define IM_ALRM_START_STOP_FAILURE_MSG_ID (28U)
# endif
# ifndef IM_ALRM_IGN1_FAILURE_MSG_ID
#  define IM_ALRM_IGN1_FAILURE_MSG_ID (29U)
# endif
# ifndef IM_ALRM_TPMS_HIGH_PRESSURE_MSG_ID
#  define IM_ALRM_TPMS_HIGH_PRESSURE_MSG_ID (30U)
# endif
# ifndef IM_ALRM_TPMS_LOW_PRESSURE_MSG_ID
#  define IM_ALRM_TPMS_LOW_PRESSURE_MSG_ID (31U)
# endif
# ifndef IM_ALRM_TPMS_FAST_LEAK_MSG_ID
#  define IM_ALRM_TPMS_FAST_LEAK_MSG_ID (32U)
# endif
# ifndef IM_ALRM_TPMS_BATTERY_LOW_VOLTAGE_MSG_ID
#  define IM_ALRM_TPMS_BATTERY_LOW_VOLTAGE_MSG_ID (33U)
# endif
# ifndef IM_ALRM_TPMS_NOT_LEARN_MSG_ID
#  define IM_ALRM_TPMS_NOT_LEARN_MSG_ID (34U)
# endif
# ifndef IM_ALRM_TPMS_OFF_MSG_ID
#  define IM_ALRM_TPMS_OFF_MSG_ID (35U)
# endif
# ifndef IM_ALRM_TPMS_SIGNAL_ERROR_MSG_ID
#  define IM_ALRM_TPMS_SIGNAL_ERROR_MSG_ID (36U)
# endif
# ifndef IM_ALRM_SRS_FAILURE_MSG_ID
#  define IM_ALRM_SRS_FAILURE_MSG_ID (37U)
# endif
# ifndef IM_ALRM_BRAKE_FLUID_LOW_MSG_ID
#  define IM_ALRM_BRAKE_FLUID_LOW_MSG_ID (38U)
# endif
# ifndef IM_ALRM_BATTERY_LOW_VOLTAGE_MSG_ID
#  define IM_ALRM_BATTERY_LOW_VOLTAGE_MSG_ID (39U)
# endif
# ifndef IM_ALRM_ENGINE_COOLANT_OVERHEADED_MSG_ID
#  define IM_ALRM_ENGINE_COOLANT_OVERHEADED_MSG_ID (40U)
# endif
# ifndef IM_ALRM_ENGINE_LOW_OIL_PRESSURE_MSG_ID
#  define IM_ALRM_ENGINE_LOW_OIL_PRESSURE_MSG_ID (41U)
# endif
# ifndef IM_ALRM_HHC_FAILURE_MSG_ID
#  define IM_ALRM_HHC_FAILURE_MSG_ID (42U)
# endif
# ifndef IM_ALRM_EPB_FAILURE_MSG_ID
#  define IM_ALRM_EPB_FAILURE_MSG_ID (43U)
# endif
# ifndef IM_ALRM_AVH_START_WORK_MSG_ID
#  define IM_ALRM_AVH_START_WORK_MSG_ID (44U)
# endif
# ifndef IM_ALRM_AVH_FAILURE_WORK_MSG_ID
#  define IM_ALRM_AVH_FAILURE_WORK_MSG_ID (45U)
# endif
# ifndef IM_ALRM_PARKING_SYS_FAILURE_MSG_ID
#  define IM_ALRM_PARKING_SYS_FAILURE_MSG_ID (46U)
# endif
# ifndef IM_ALRM_EPS_FAILURE_MSG_ID
#  define IM_ALRM_EPS_FAILURE_MSG_ID (47U)
# endif
# ifndef IM_ALRM_PARKING_FAILURE_MSG_ID
#  define IM_ALRM_PARKING_FAILURE_MSG_ID (48U)
# endif
# ifndef IM_ALRM_CALL_SALE_CHECK_GEARBOX_MSG_ID
#  define IM_ALRM_CALL_SALE_CHECK_GEARBOX_MSG_ID (49U)
# endif
# ifndef IM_ALRM_HIGH_SPEED_MSG_ID
#  define IM_ALRM_HIGH_SPEED_MSG_ID (50U)
# endif
# ifndef IM_ALRM_PLS_ADD_FUEL_MSG_ID
#  define IM_ALRM_PLS_ADD_FUEL_MSG_ID (51U)
# endif
# ifndef IM_ALRM_ENTER_ECO_MODE_MSG_ID
#  define IM_ALRM_ENTER_ECO_MODE_MSG_ID (52U)
# endif
# ifndef IM_ALRM_ENTER_SPORT_MODE_MSG_ID
#  define IM_ALRM_ENTER_SPORT_MODE_MSG_ID (53U)
# endif
# ifndef IM_ALRM_UNABLE_ENTER_ECO_MODE_MSG_ID
#  define IM_ALRM_UNABLE_ENTER_ECO_MODE_MSG_ID (54U)
# endif
# ifndef IM_ALRM_UNABLE_ENTER_SPORT_MODE_MSG_ID
#  define IM_ALRM_UNABLE_ENTER_SPORT_MODE_MSG_ID (55U)
# endif
# ifndef IM_ALRM_TAKE_REST_MSG_ID
#  define IM_ALRM_TAKE_REST_MSG_ID (56U)
# endif
# ifndef IM_ALRM_MAINTENANCE_SERVICE_MSG_ID
#  define IM_ALRM_MAINTENANCE_SERVICE_MSG_ID (57U)
# endif
# ifndef IM_ALRM_SERVICE_EXPIRED_MSG_ID
#  define IM_ALRM_SERVICE_EXPIRED_MSG_ID (58U)
# endif
# ifndef IM_ALRM_ESC_OFF_MSG_ID
#  define IM_ALRM_ESC_OFF_MSG_ID (59U)
# endif
# ifndef IM_ALRM_HDC_MALFUNCTION_MSG_ID
#  define IM_ALRM_HDC_MALFUNCTION_MSG_ID (60U)
# endif
# ifndef IM_ALRM_EPB_ACTIVATED_MSG_ID
#  define IM_ALRM_EPB_ACTIVATED_MSG_ID (61U)
# endif
# ifndef IM_ALRM_PTG_ANTIPINCH_MSG_ID
#  define IM_ALRM_PTG_ANTIPINCH_MSG_ID (62U)
# endif
# ifndef IM_ALRM_PTG_FAILURE_MSG_ID
#  define IM_ALRM_PTG_FAILURE_MSG_ID (63U)
# endif
# ifndef IM_ALRM_TPMS_SENSOR_FAILURE_MSG_ID
#  define IM_ALRM_TPMS_SENSOR_FAILURE_MSG_ID (64U)
# endif
# ifndef IM_ALRM_TPMS_HIGH_TEMPERATURE_MSG_ID
#  define IM_ALRM_TPMS_HIGH_TEMPERATURE_MSG_ID (65U)
# endif
# ifndef IM_ALRM_DOOR_SEATBELT_MSG_MSG_ID
#  define IM_ALRM_DOOR_SEATBELT_MSG_MSG_ID (66U)
# endif
# ifndef IM_ALRM_ALOD_OVERRIDE_MSG_ID
#  define IM_ALRM_ALOD_OVERRIDE_MSG_ID (67U)
# endif
# ifndef IM_ALRM_PLS_TAKE_OVER_MSG_ID
#  define IM_ALRM_PLS_TAKE_OVER_MSG_ID (68U)
# endif
# ifndef IM_ALRM_CC_CANCEL_MSG_ID
#  define IM_ALRM_CC_CANCEL_MSG_ID (69U)
# endif
# ifndef IM_ALRM_CC_UNACTIVE_MSG_ID
#  define IM_ALRM_CC_UNACTIVE_MSG_ID (70U)
# endif
# ifndef IM_ALRM_CC_ACTIVE_MSG_ID
#  define IM_ALRM_CC_ACTIVE_MSG_ID (71U)
# endif
# ifndef IM_ALRM_CC_ON_MSG_ID
#  define IM_ALRM_CC_ON_MSG_ID (72U)
# endif
# ifndef IM_ALRM_CC_OFF_MSG_ID
#  define IM_ALRM_CC_OFF_MSG_ID (73U)
# endif
# ifndef IM_ALRM_CC_UNACTIVE_SEATBELT_MSG_ID
#  define IM_ALRM_CC_UNACTIVE_SEATBELT_MSG_ID (74U)
# endif
# ifndef IM_ALRM_CC_UNACTIVE_DOOR_MSG_ID
#  define IM_ALRM_CC_UNACTIVE_DOOR_MSG_ID (75U)
# endif
# ifndef IM_ALRM_CC_UNACTIVE_NOT_D_MSG_ID
#  define IM_ALRM_CC_UNACTIVE_NOT_D_MSG_ID (76U)
# endif
# ifndef IM_ALRM_CC_UNACTIVE_NO_CAR_MSG_ID
#  define IM_ALRM_CC_UNACTIVE_NO_CAR_MSG_ID (77U)
# endif
# ifndef IM_ALRM_FRS_BLOCKED_MSG_ID
#  define IM_ALRM_FRS_BLOCKED_MSG_ID (78U)
# endif
# ifndef IM_ALRM_PRESS_BRAKE_PEDAL_PERSON_MSG_ID
#  define IM_ALRM_PRESS_BRAKE_PEDAL_PERSON_MSG_ID (79U)
# endif
# ifndef IM_ALRM_LIM_ON_MSG_ID
#  define IM_ALRM_LIM_ON_MSG_ID (80U)
# endif
# ifndef IM_ALRM_LIM_CANCEL_MSG_ID
#  define IM_ALRM_LIM_CANCEL_MSG_ID (81U)
# endif
# ifndef IM_ALRM_LIM_OFF_MSG_ID
#  define IM_ALRM_LIM_OFF_MSG_ID (82U)
# endif
# ifndef IM_ALRM_EMS_CC_ON_MSG_ID
#  define IM_ALRM_EMS_CC_ON_MSG_ID (83U)
# endif
# ifndef IM_ALRM_EMS_CC_OFF_MSG_ID
#  define IM_ALRM_EMS_CC_OFF_MSG_ID (84U)
# endif
# ifndef IM_ALRM_EMS_CC_CANCEL_MSG_ID
#  define IM_ALRM_EMS_CC_CANCEL_MSG_ID (85U)
# endif
# ifndef IM_ALRM_EMS_CC_SPEED_UP_MSG_ID
#  define IM_ALRM_EMS_CC_SPEED_UP_MSG_ID (86U)
# endif
# ifndef IM_ALRM_STOP_CONTACT_SALE_MSG_ID
#  define IM_ALRM_STOP_CONTACT_SALE_MSG_ID (87U)
# endif
# ifndef IM_ALRM_PAS_FAILURE_MSG_ID
#  define IM_ALRM_PAS_FAILURE_MSG_ID (88U)
# endif
# ifndef IM_ALRM_ESCL_AUTH_FAILURE_MSG_ID
#  define IM_ALRM_ESCL_AUTH_FAILURE_MSG_ID (89U)
# endif
# ifndef IM_ALRM_EPS_MODE_CHANGE_FAILURE_MSG_ID
#  define IM_ALRM_EPS_MODE_CHANGE_FAILURE_MSG_ID (90U)
# endif
# ifndef IM_ALRM_CC_UNACTIVE_NOT_SUPPORT_MSG_ID
#  define IM_ALRM_CC_UNACTIVE_NOT_SUPPORT_MSG_ID (91U)
# endif
# ifndef IM_ALRM_DRIVE_MODE_CHANGE_FAILURE_MSG_ID
#  define IM_ALRM_DRIVE_MODE_CHANGE_FAILURE_MSG_ID (92U)
# endif
# ifndef IM_ALRM_FRONT_CAMERA_BLIND_MSG_ID
#  define IM_ALRM_FRONT_CAMERA_BLIND_MSG_ID (93U)
# endif
# ifndef IM_ALRM_SHIFT_N_START_MSG_ID
#  define IM_ALRM_SHIFT_N_START_MSG_ID (94U)
# endif
# ifndef IM_ALRM_MAX_MSG_ID
#  define IM_ALRM_MAX_MSG_ID (95U)
# endif
#endif

/* Enumeration type:        IM_ALRM_Data_Availability_t */
/* Description:                           */
#ifndef Rte_TypeDef_IM_ALRM_Data_Availability_t
#define Rte_TypeDef_IM_ALRM_Data_Availability_t
typedef UINT8 IM_ALRM_Data_Availability_t;
#define IM_ALRM_Data_Availability_t_LowerLimit (0U)
#define IM_ALRM_Data_Availability_t_UpperLimit (2U)
# ifndef IM_ALRM_DATA_NOT_APPLICABLE
#  define IM_ALRM_DATA_NOT_APPLICABLE (0U)
# endif
# ifndef IM_ALRM_DATA_UNAVAILABLE
#  define IM_ALRM_DATA_UNAVAILABLE (1U)
# endif
# ifndef IM_ALRM_DATA_ACTIVE
#  define IM_ALRM_DATA_ACTIVE (2U)
# endif
#endif

/* Enumeration type:        IM_OBCM_FCUnit_t            */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_FCUnit_t
#define Rte_TypeDef_IM_OBCM_FCUnit_t
typedef UINT8 IM_OBCM_FCUnit_t;
#define IM_OBCM_FCUnit_t_LowerLimit (0U)
#define IM_OBCM_FCUnit_t_UpperLimit (3U)
# ifndef IM_AFC_LPER100KM
#  define IM_AFC_LPER100KM (0U)
# endif
# ifndef IM_AFC_KMPERL
#  define IM_AFC_KMPERL (1U)
# endif
# ifndef IM_AFC_USMPG
#  define IM_AFC_USMPG (2U)
# endif
# ifndef IM_AFC_UKMPG
#  define IM_AFC_UKMPG (3U)
# endif
#endif

/* Enumeration type:        TCSM_SW_Trip_PressStatus_t  */
/* Description:                           */
#ifndef Rte_TypeDef_TCSM_SW_Trip_PressStatus_t
#define Rte_TypeDef_TCSM_SW_Trip_PressStatus_t
typedef UINT8 TCSM_SW_Trip_PressStatus_t;
#define TCSM_SW_Trip_PressStatus_t_LowerLimit (0U)
#define TCSM_SW_Trip_PressStatus_t_UpperLimit (3U)
# ifndef TCSM_SW_TRIP_NO_PRESS
#  define TCSM_SW_TRIP_NO_PRESS (0U)
# endif
# ifndef TCSM_SW_TRIP_PRESS
#  define TCSM_SW_TRIP_PRESS (1U)
# endif
# ifndef TCSM_SW_TRIP_RESERVED
#  define TCSM_SW_TRIP_RESERVED (2U)
# endif
# ifndef TCSM_SW_TRIP_INVALID
#  define TCSM_SW_TRIP_INVALID (3U)
# endif
#endif

/* Enumeration type:        TCSM_WorkMode_t             */
/* Description:                           */
#ifndef Rte_TypeDef_TCSM_WorkMode_t
#define Rte_TypeDef_TCSM_WorkMode_t
typedef UINT8 TCSM_WorkMode_t;
#define TCSM_WorkMode_t_LowerLimit (0U)
#define TCSM_WorkMode_t_UpperLimit (2U)
# ifndef TCSM_WORK_MODE_MMI_MODE
#  define TCSM_WORK_MODE_MMI_MODE (0U)
# endif
# ifndef TCSM_WORK_MODE_IPK_MODE
#  define TCSM_WORK_MODE_IPK_MODE (1U)
# endif
# ifndef TCSM_WORK_MODE_HUD_MODE
#  define TCSM_WORK_MODE_HUD_MODE (2U)
# endif
#endif

/* Enumeration type:        ATIM_SubState_t             */
/* Description:                           */
#ifndef Rte_TypeDef_ATIM_SubState_t
#define Rte_TypeDef_ATIM_SubState_t
typedef UINT8 ATIM_SubState_t;
#define ATIM_SubState_t_LowerLimit (0U)
#define ATIM_SubState_t_UpperLimit (3U)
# ifndef ATIM_SUB_STATE_IDLE
#  define ATIM_SUB_STATE_IDLE (0U)
# endif
# ifndef ATIM_SUB_STATE_DISPLAY_NONE
#  define ATIM_SUB_STATE_DISPLAY_NONE (1U)
# endif
# ifndef ATIM_SUB_STATE_DATA_FAULT
#  define ATIM_SUB_STATE_DATA_FAULT (2U)
# endif
# ifndef ATIM_SUB_STATE_DATA_NORMAL
#  define ATIM_SUB_STATE_DATA_NORMAL (3U)
# endif
#endif

/* Enumeration type:        IM_OBCM_AVSUnit_t           */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_AVSUnit_t
#define Rte_TypeDef_IM_OBCM_AVSUnit_t
typedef UINT8 IM_OBCM_AVSUnit_t;
#define IM_OBCM_AVSUnit_t_LowerLimit (0U)
#define IM_OBCM_AVSUnit_t_UpperLimit (1U)
# ifndef IM_AVS_VALUE_KMH
#  define IM_AVS_VALUE_KMH (0U)
# endif
# ifndef IM_AVS_VALUE_MPH
#  define IM_AVS_VALUE_MPH (1U)
# endif
#endif

/* Enumeration type:        IM_GearDisplay_t            */
/* Description:                           */
#ifndef Rte_TypeDef_IM_GearDisplay_t
#define Rte_TypeDef_IM_GearDisplay_t
typedef UINT8 IM_GearDisplay_t;
#define IM_GearDisplay_t_LowerLimit (0U)
#define IM_GearDisplay_t_UpperLimit (23U)
# ifndef IM_ATGEAR_NODISPLAY
#  define IM_ATGEAR_NODISPLAY (0U)
# endif
# ifndef IM_ATGEAR_DISPLAY_P
#  define IM_ATGEAR_DISPLAY_P (1U)
# endif
# ifndef IM_ATGEAR_DISPLAY_R
#  define IM_ATGEAR_DISPLAY_R (2U)
# endif
# ifndef IM_ATGEAR_DISPLAY_N
#  define IM_ATGEAR_DISPLAY_N (3U)
# endif
# ifndef IM_ATGEAR_DISPLAY_D
#  define IM_ATGEAR_DISPLAY_D (4U)
# endif
# ifndef IM_ATGEAR_DISPLAY_D1
#  define IM_ATGEAR_DISPLAY_D1 (5U)
# endif
# ifndef IM_ATGEAR_DISPLAY_D2
#  define IM_ATGEAR_DISPLAY_D2 (6U)
# endif
# ifndef IM_ATGEAR_DISPLAY_D3
#  define IM_ATGEAR_DISPLAY_D3 (7U)
# endif
# ifndef IM_ATGEAR_DISPLAY_D4
#  define IM_ATGEAR_DISPLAY_D4 (8U)
# endif
# ifndef IM_ATGEAR_DISPLAY_D5
#  define IM_ATGEAR_DISPLAY_D5 (9U)
# endif
# ifndef IM_ATGEAR_DISPLAY_D6
#  define IM_ATGEAR_DISPLAY_D6 (10U)
# endif
# ifndef IM_ATGEAR_DISPLAY_M1
#  define IM_ATGEAR_DISPLAY_M1 (11U)
# endif
# ifndef IM_ATGEAR_DISPLAY_M2
#  define IM_ATGEAR_DISPLAY_M2 (12U)
# endif
# ifndef IM_ATGEAR_DISPLAY_M3
#  define IM_ATGEAR_DISPLAY_M3 (13U)
# endif
# ifndef IM_ATGEAR_DISPLAY_M4
#  define IM_ATGEAR_DISPLAY_M4 (14U)
# endif
# ifndef IM_ATGEAR_DISPLAY_M5
#  define IM_ATGEAR_DISPLAY_M5 (15U)
# endif
# ifndef IM_ATGEAR_DISPLAY_M6
#  define IM_ATGEAR_DISPLAY_M6 (16U)
# endif
# ifndef IM_ATGEAR_DISPLAY_M7
#  define IM_ATGEAR_DISPLAY_M7 (17U)
# endif
# ifndef IM_MTGEAR_DISPLAY1
#  define IM_MTGEAR_DISPLAY1 (18U)
# endif
# ifndef IM_MTGEAR_DISPLAY2
#  define IM_MTGEAR_DISPLAY2 (19U)
# endif
# ifndef IM_MTGEAR_DISPLAY3
#  define IM_MTGEAR_DISPLAY3 (20U)
# endif
# ifndef IM_MTGEAR_DISPLAY4
#  define IM_MTGEAR_DISPLAY4 (21U)
# endif
# ifndef IM_MTGEAR_DISPLAY5
#  define IM_MTGEAR_DISPLAY5 (22U)
# endif
# ifndef IM_MTGEAR_DISPLAY6
#  define IM_MTGEAR_DISPLAY6 (23U)
# endif
#endif

/* Enumeration type:        IM_Gear_ArrowDisplay_t      */
/* Description:                           */
#ifndef Rte_TypeDef_IM_Gear_ArrowDisplay_t
#define Rte_TypeDef_IM_Gear_ArrowDisplay_t
typedef UINT8 IM_Gear_ArrowDisplay_t;
#define IM_Gear_ArrowDisplay_t_LowerLimit (0U)
#define IM_Gear_ArrowDisplay_t_UpperLimit (2U)
# ifndef IM_GEAR_NO_ARROW
#  define IM_GEAR_NO_ARROW (0U)
# endif
# ifndef IM_GEAR_UP_ARROW
#  define IM_GEAR_UP_ARROW (1U)
# endif
# ifndef IM_GEAR_DOWN_ARROW
#  define IM_GEAR_DOWN_ARROW (2U)
# endif
#endif

/* Enumeration type:        IM_ALRM_SoundId_t           */
/* Description:                           */
#ifndef Rte_TypeDef_IM_ALRM_SoundId_t
#define Rte_TypeDef_IM_ALRM_SoundId_t
typedef UINT8 IM_ALRM_SoundId_t;
#define IM_ALRM_SoundId_t_LowerLimit (0U)
#define IM_ALRM_SoundId_t_UpperLimit (100U)
# ifndef IM_ALRM_RESERVED_SOUND_ID
#  define IM_ALRM_RESERVED_SOUND_ID (0U)
# endif
# ifndef IM_ALRM_MAX_SOUND_ID
#  define IM_ALRM_MAX_SOUND_ID (100U)
# endif
#endif

/* Enumeration type:        IM_OBCM_TyrePrUnit_t        */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_TyrePrUnit_t
#define Rte_TypeDef_IM_OBCM_TyrePrUnit_t
typedef UINT8 IM_OBCM_TyrePrUnit_t;
#define IM_OBCM_TyrePrUnit_t_LowerLimit (0U)
#define IM_OBCM_TyrePrUnit_t_UpperLimit (2U)
# ifndef IM_TYREPR_VALUE_KPA
#  define IM_TYREPR_VALUE_KPA (0U)
# endif
# ifndef IM_TYREPR_VALUE_PSI
#  define IM_TYREPR_VALUE_PSI (1U)
# endif
# ifndef IM_TYREPR_VALUE_BAR
#  define IM_TYREPR_VALUE_BAR (2U)
# endif
#endif

/* Enumeration type:        IM_OBCM_TyreTempUnit_t      */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_TyreTempUnit_t
#define Rte_TypeDef_IM_OBCM_TyreTempUnit_t
typedef UINT8 IM_OBCM_TyreTempUnit_t;
#define IM_OBCM_TyreTempUnit_t_LowerLimit (0U)
#define IM_OBCM_TyreTempUnit_t_UpperLimit (1U)
# ifndef IM_TYRETEMP_VALUE_C
#  define IM_TYRETEMP_VALUE_C (0U)
# endif
# ifndef IM_TYRETEMP_VALUE_F
#  define IM_TYRETEMP_VALUE_F (1U)
# endif
#endif

/* Enumeration type:        IM_OBCM_TyreStatus_t        */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_TyreStatus_t
#define Rte_TypeDef_IM_OBCM_TyreStatus_t
typedef UINT8 IM_OBCM_TyreStatus_t;
#define IM_OBCM_TyreStatus_t_LowerLimit (0U)
#define IM_OBCM_TyreStatus_t_UpperLimit (2U)
# ifndef IM_OBCM_DATA_VALID
#  define IM_OBCM_DATA_VALID (0U)
# endif
# ifndef IM_OBCM_DATA_INVALID
#  define IM_OBCM_DATA_INVALID (1U)
# endif
# ifndef IM_OBCM_TYRE_WARNING
#  define IM_OBCM_TYRE_WARNING (2U)
# endif
#endif

/* Enumeration type:        IM_OBCM_MaintenanceWarningType_t          */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_MaintenanceWarningType_t
#define Rte_TypeDef_IM_OBCM_MaintenanceWarningType_t
typedef UINT8 IM_OBCM_MaintenanceWarningType_t;
#define IM_OBCM_MaintenanceWarningType_t_LowerLimit (0U)
#define IM_OBCM_MaintenanceWarningType_t_UpperLimit (2U)
# ifndef IM_OBCM_NO_MAINTENANCE_WARNING
#  define IM_OBCM_NO_MAINTENANCE_WARNING (0U)
# endif
# ifndef IM_OBCM_NEED_MAINTENANC
#  define IM_OBCM_NEED_MAINTENANC (1U)
# endif
# ifndef IM_OBCM_NOW_MAINTENANCE
#  define IM_OBCM_NOW_MAINTENANCE (2U)
# endif
#endif

/* Enumeration type:        IM_OBCM_FCSLineType_t       */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_FCSLineType_t
#define Rte_TypeDef_IM_OBCM_FCSLineType_t
typedef UINT8 IM_OBCM_FCSLineType_t;
#define IM_OBCM_FCSLineType_t_LowerLimit (0U)
#define IM_OBCM_FCSLineType_t_UpperLimit (3U)
# ifndef IM_OBCM_FCS_NO_DISPLAY
#  define IM_OBCM_FCS_NO_DISPLAY (0U)
# endif
# ifndef IM_OBCM_FCS_SOLID_LINE
#  define IM_OBCM_FCS_SOLID_LINE (1U)
# endif
# ifndef IM_OBCM_FCS_DSAH_LINE
#  define IM_OBCM_FCS_DSAH_LINE (2U)
# endif
# ifndef IM_OBCM_FCS_RESERVED
#  define IM_OBCM_FCS_RESERVED (3U)
# endif
#endif

/* Enumeration type:        IM_OBCM_FCSLineColor_t      */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_FCSLineColor_t
#define Rte_TypeDef_IM_OBCM_FCSLineColor_t
typedef UINT8 IM_OBCM_FCSLineColor_t;
#define IM_OBCM_FCSLineColor_t_LowerLimit (0U)
#define IM_OBCM_FCSLineColor_t_UpperLimit (3U)
# ifndef IM_OBCM_FCS_GREY
#  define IM_OBCM_FCS_GREY (0U)
# endif
# ifndef IM_OBCM_FCS_WHITE
#  define IM_OBCM_FCS_WHITE (1U)
# endif
# ifndef IM_OBCM_FCS_BLUE
#  define IM_OBCM_FCS_BLUE (2U)
# endif
# ifndef IM_OBCM_FCS_RED
#  define IM_OBCM_FCS_RED (3U)
# endif
#endif

/* Enumeration type:        TCSM_AccIccOnOff_t          */
/* Description:                           */
#ifndef Rte_TypeDef_TCSM_AccIccOnOff_t
#define Rte_TypeDef_TCSM_AccIccOnOff_t
typedef UINT8 TCSM_AccIccOnOff_t;
#define TCSM_AccIccOnOff_t_LowerLimit (0U)
#define TCSM_AccIccOnOff_t_UpperLimit (1U)
# ifndef TCSM_ACC_ICC_OFF
#  define TCSM_ACC_ICC_OFF (0U)
# endif
# ifndef TCSM_ACC_ICC_ON
#  define TCSM_ACC_ICC_ON (1U)
# endif
#endif

/* Enumeration type:        TCSM_AccIccOnOffAvailable_t */
/* Description:                           */
#ifndef Rte_TypeDef_TCSM_AccIccOnOffAvailable_t
#define Rte_TypeDef_TCSM_AccIccOnOffAvailable_t
typedef UINT8 TCSM_AccIccOnOffAvailable_t;
#define TCSM_AccIccOnOffAvailable_t_LowerLimit (0U)
#define TCSM_AccIccOnOffAvailable_t_UpperLimit (1U)
# ifndef TCSM_ACC_ICC_UNAVAILABLE
#  define TCSM_ACC_ICC_UNAVAILABLE (0U)
# endif
# ifndef TCSM_ACC_ICC_AVAILABLE
#  define TCSM_ACC_ICC_AVAILABLE (1U)
# endif
#endif

/* Enumeration type:        IM_OBCM_ObjectType_t        */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_ObjectType_t
#define Rte_TypeDef_IM_OBCM_ObjectType_t
typedef UINT8 IM_OBCM_ObjectType_t;
#define IM_OBCM_ObjectType_t_LowerLimit (0U)
#define IM_OBCM_ObjectType_t_UpperLimit (4U)
# ifndef IM_OBCM_OBJECT_NONE
#  define IM_OBCM_OBJECT_NONE (0U)
# endif
# ifndef IM_OBCM_OBJECT_CAR
#  define IM_OBCM_OBJECT_CAR (1U)
# endif
# ifndef IM_OBCM_OBJECT_PEDESTRIAN
#  define IM_OBCM_OBJECT_PEDESTRIAN (2U)
# endif
# ifndef IM_OBCM_OBJECT_BIKECYCLE
#  define IM_OBCM_OBJECT_BIKECYCLE (3U)
# endif
# ifndef IM_OBCM_OBJECT_ANIMAL
#  define IM_OBCM_OBJECT_ANIMAL (4U)
# endif
#endif

/* Enumeration type:        IM_OBCM_ObjectColor_t       */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_ObjectColor_t
#define Rte_TypeDef_IM_OBCM_ObjectColor_t
typedef UINT8 IM_OBCM_ObjectColor_t;
#define IM_OBCM_ObjectColor_t_LowerLimit (0U)
#define IM_OBCM_ObjectColor_t_UpperLimit (5U)
# ifndef IM_OBCM_OBJECT_GREY
#  define IM_OBCM_OBJECT_GREY (0U)
# endif
# ifndef IM_OBCM_OBJECT_BLUE
#  define IM_OBCM_OBJECT_BLUE (1U)
# endif
# ifndef IM_OBCM_OBJECT_YELLOW
#  define IM_OBCM_OBJECT_YELLOW (2U)
# endif
# ifndef IM_OBCM_OBJECT_RED
#  define IM_OBCM_OBJECT_RED (3U)
# endif
# ifndef IM_OBCM_OBJECT_WHITE
#  define IM_OBCM_OBJECT_WHITE (4U)
# endif
# ifndef IM_OBCM_OBJECT_ORANGE
#  define IM_OBCM_OBJECT_ORANGE (5U)
# endif
#endif

/* Enumeration type:        IM_OBCM_LaneColor_t         */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_LaneColor_t
#define Rte_TypeDef_IM_OBCM_LaneColor_t
typedef UINT8 IM_OBCM_LaneColor_t;
#define IM_OBCM_LaneColor_t_LowerLimit (0U)
#define IM_OBCM_LaneColor_t_UpperLimit (3U)
# ifndef IM_OBCM_LANE_GREY
#  define IM_OBCM_LANE_GREY (0U)
# endif
# ifndef IM_OBCM_LANE_GREEN
#  define IM_OBCM_LANE_GREEN (1U)
# endif
# ifndef IM_OBCM_LANE_BLUE
#  define IM_OBCM_LANE_BLUE (2U)
# endif
# ifndef IM_OBCM_LANE_RED
#  define IM_OBCM_LANE_RED (3U)
# endif
#endif

/* Enumeration type:        IM_Transmission_t           */
/* Description:                           */
#ifndef Rte_TypeDef_IM_Transmission_t
#define Rte_TypeDef_IM_Transmission_t
typedef UINT8 IM_Transmission_t;
#define IM_Transmission_t_LowerLimit (0U)
#define IM_Transmission_t_UpperLimit (1U)
# ifndef IM_TRANSMISSION_MT
#  define IM_TRANSMISSION_MT (0U)
# endif
# ifndef IM_TRANSMISSION_AT
#  define IM_TRANSMISSION_AT (1U)
# endif
#endif

/* Enumeration type:        IM_DRVM_BCM_DM_TargetModeReq_t            */
/* Description:                           */
#ifndef Rte_TypeDef_IM_DRVM_BCM_DM_TargetModeReq_t
#define Rte_TypeDef_IM_DRVM_BCM_DM_TargetModeReq_t
typedef UINT8 IM_DRVM_BCM_DM_TargetModeReq_t;
#define IM_DRVM_BCM_DM_TargetModeReq_t_LowerLimit (0U)
#define IM_DRVM_BCM_DM_TargetModeReq_t_UpperLimit (7U)
# ifndef IM_DRVM_TM_COMFORT_MODE
#  define IM_DRVM_TM_COMFORT_MODE (0U)
# endif
# ifndef IM_DRVM_TM_ECO_MODE
#  define IM_DRVM_TM_ECO_MODE (1U)
# endif
# ifndef IM_DRVM_TM_SPORT_MODE
#  define IM_DRVM_TM_SPORT_MODE (2U)
# endif
# ifndef IM_DRVM_TM_OFFLOAD_MODE
#  define IM_DRVM_TM_OFFLOAD_MODE (3U)
# endif
# ifndef IM_DRVM_TM_SAND_MODE
#  define IM_DRVM_TM_SAND_MODE (4U)
# endif
# ifndef IM_DRVM_TM_MUD_MODE
#  define IM_DRVM_TM_MUD_MODE (5U)
# endif
# ifndef IM_DRVM_TM_ROCK_MODE
#  define IM_DRVM_TM_ROCK_MODE (6U)
# endif
# ifndef IM_DRVM_TM_RESERVE_7
#  define IM_DRVM_TM_RESERVE_7 (7U)
# endif
#endif

/* Enumeration type:        IM_DRVM_BCM_DM_SwitchModeSts_t            */
/* Description:                           */
#ifndef Rte_TypeDef_IM_DRVM_BCM_DM_SwitchModeSts_t
#define Rte_TypeDef_IM_DRVM_BCM_DM_SwitchModeSts_t
typedef UINT8 IM_DRVM_BCM_DM_SwitchModeSts_t;
#define IM_DRVM_BCM_DM_SwitchModeSts_t_LowerLimit (0U)
#define IM_DRVM_BCM_DM_SwitchModeSts_t_UpperLimit (255U)
# ifndef IM_DRVM_SMS_COMFORT_MODE
#  define IM_DRVM_SMS_COMFORT_MODE (0U)
# endif
# ifndef IM_DRVM_SMS_ECO_MODE
#  define IM_DRVM_SMS_ECO_MODE (1U)
# endif
# ifndef IM_DRVM_SMS_SPORT_MODE
#  define IM_DRVM_SMS_SPORT_MODE (2U)
# endif
# ifndef IM_DRVM_SMS_OFFLOAD_MODE
#  define IM_DRVM_SMS_OFFLOAD_MODE (3U)
# endif
# ifndef IM_DRVM_SMS_SNOW_MODE
#  define IM_DRVM_SMS_SNOW_MODE (4U)
# endif
# ifndef IM_DRVM_SMS_RESERVE_5
#  define IM_DRVM_SMS_RESERVE_5 (5U)
# endif
# ifndef IM_DRVM_SMS_ROCK_MODE
#  define IM_DRVM_SMS_ROCK_MODE (6U)
# endif
# ifndef IM_DRVM_SMS_RESERVE_7
#  define IM_DRVM_SMS_RESERVE_7 (7U)
# endif
# ifndef IM_DRVM_SMS_UNAVAILABLE
#  define IM_DRVM_SMS_UNAVAILABLE (255U)
# endif
#endif

/* Enumeration type:        IM_OBCM_LimStatus_t         */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_LimStatus_t
#define Rte_TypeDef_IM_OBCM_LimStatus_t
typedef UINT8 IM_OBCM_LimStatus_t;
#define IM_OBCM_LimStatus_t_LowerLimit (0U)
#define IM_OBCM_LimStatus_t_UpperLimit (4U)
# ifndef IM_OBCM_LIM_ON
#  define IM_OBCM_LIM_ON (0U)
# endif
# ifndef IM_OBCM_LIM_ACTIVE
#  define IM_OBCM_LIM_ACTIVE (1U)
# endif
# ifndef IM_OBCM_LIM_OVER_SPEED
#  define IM_OBCM_LIM_OVER_SPEED (2U)
# endif
# ifndef IM_OBCM_LIM_INACTIVE
#  define IM_OBCM_LIM_INACTIVE (3U)
# endif
# ifndef IM_OBCM_LIM_OFF
#  define IM_OBCM_LIM_OFF (4U)
# endif
#endif

/* Enumeration type:        IM_UTLM_MenuDisplayStatus_t */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_MenuDisplayStatus_t
#define Rte_TypeDef_IM_UTLM_MenuDisplayStatus_t
typedef UINT8 IM_UTLM_MenuDisplayStatus_t;
#define IM_UTLM_MenuDisplayStatus_t_LowerLimit (0U)
#define IM_UTLM_MenuDisplayStatus_t_UpperLimit (1U)
# ifndef IM_UTLM_NORMAL
#  define IM_UTLM_NORMAL (0U)
# endif
# ifndef IM_UTLM_TURN_GRAY
#  define IM_UTLM_TURN_GRAY (1U)
# endif
#endif

/* Enumeration type:        IM_UTLM_MenuStatus_t        */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_MenuStatus_t
#define Rte_TypeDef_IM_UTLM_MenuStatus_t
typedef UINT8 IM_UTLM_MenuStatus_t;
#define IM_UTLM_MenuStatus_t_LowerLimit (0U)
#define IM_UTLM_MenuStatus_t_UpperLimit (1U)
# ifndef IM_UTLM_UNSELECT
#  define IM_UTLM_UNSELECT (0U)
# endif
# ifndef IM_UTLM_SELECT
#  define IM_UTLM_SELECT (1U)
# endif
#endif

/* Enumeration type:        IM_UTLM_StateReq_t          */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_StateReq_t
#define Rte_TypeDef_IM_UTLM_StateReq_t
typedef UINT8 IM_UTLM_StateReq_t;
#define IM_UTLM_StateReq_t_LowerLimit (0U)
#define IM_UTLM_StateReq_t_UpperLimit (3U)
# ifndef IM_UTLM_NO_ACTION
#  define IM_UTLM_NO_ACTION (0U)
# endif
# ifndef IM_UTLM_OFF_REQUEST
#  define IM_UTLM_OFF_REQUEST (1U)
# endif
# ifndef IM_UTLM_ON_REQUEST
#  define IM_UTLM_ON_REQUEST (2U)
# endif
# ifndef IM_UTLM_RESERVED
#  define IM_UTLM_RESERVED (3U)
# endif
#endif

/* Enumeration type:        IM_UTLM_SwitchAndType_t     */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_SwitchAndType_t
#define Rte_TypeDef_IM_UTLM_SwitchAndType_t
typedef UINT8 IM_UTLM_SwitchAndType_t;
#define IM_UTLM_SwitchAndType_t_LowerLimit (0U)
#define IM_UTLM_SwitchAndType_t_UpperLimit (3U)
# ifndef IM_UTLM_FUNCTION_OFF_WARNING_OFF
#  define IM_UTLM_FUNCTION_OFF_WARNING_OFF (0U)
# endif
# ifndef IM_UTLM_FUNCTION_ON_WARNING_OFF
#  define IM_UTLM_FUNCTION_ON_WARNING_OFF (1U)
# endif
# ifndef IM_UTLM_FUNCTION_ON_WARNING_ON
#  define IM_UTLM_FUNCTION_ON_WARNING_ON (2U)
# endif
# ifndef IM_UTLM_FUNCTION_OFF_WARNING_ON
#  define IM_UTLM_FUNCTION_OFF_WARNING_ON (3U)
# endif
#endif

/* Enumeration type:        IM_UTLM_LKAMode_t           */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_LKAMode_t
#define Rte_TypeDef_IM_UTLM_LKAMode_t
typedef UINT8 IM_UTLM_LKAMode_t;
#define IM_UTLM_LKAMode_t_LowerLimit (2U)
#define IM_UTLM_LKAMode_t_UpperLimit (4U)
# ifndef IM_UTLM_LKA_ALARM_ONLY
#  define IM_UTLM_LKA_ALARM_ONLY (2U)
# endif
# ifndef IM_UTLM_LKA_AUXILIARY_WEAK
#  define IM_UTLM_LKA_AUXILIARY_WEAK (3U)
# endif
# ifndef IM_UTLM_LKA_AUXILIARY_STRONG
#  define IM_UTLM_LKA_AUXILIARY_STRONG (4U)
# endif
#endif

/* Enumeration type:        IM_UTLM_LKAFeedback_t       */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_LKAFeedback_t
#define Rte_TypeDef_IM_UTLM_LKAFeedback_t
typedef UINT8 IM_UTLM_LKAFeedback_t;
#define IM_UTLM_LKAFeedback_t_LowerLimit (1U)
#define IM_UTLM_LKAFeedback_t_UpperLimit (3U)
# ifndef IM_UTLM_LKA_SOUND
#  define IM_UTLM_LKA_SOUND (1U)
# endif
# ifndef IM_UTLM_LKA_VIBRATION
#  define IM_UTLM_LKA_VIBRATION (2U)
# endif
# ifndef IM_UTLM_LKA_SOUND_AND_VIBRATION
#  define IM_UTLM_LKA_SOUND_AND_VIBRATION (3U)
# endif
#endif

/* Enumeration type:        IM_ALRM_SeatbeltStatus_t    */
/* Description:                           */
#ifndef Rte_TypeDef_IM_ALRM_SeatbeltStatus_t
#define Rte_TypeDef_IM_ALRM_SeatbeltStatus_t
typedef UINT8 IM_ALRM_SeatbeltStatus_t;
#define IM_ALRM_SeatbeltStatus_t_LowerLimit (0U)
#define IM_ALRM_SeatbeltStatus_t_UpperLimit (2U)
# ifndef IM_ALRM_SEATBELT_FASTENED
#  define IM_ALRM_SEATBELT_FASTENED (0U)
# endif
# ifndef IM_ALRM_SEATBELT_UNFASTENED
#  define IM_ALRM_SEATBELT_UNFASTENED (1U)
# endif
# ifndef IM_ALRM_SEATBELT_WARNING
#  define IM_ALRM_SEATBELT_WARNING (2U)
# endif
#endif

/* Enumeration type:        IM_UTLM_CruiseMode_t        */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_CruiseMode_t
#define Rte_TypeDef_IM_UTLM_CruiseMode_t
typedef UINT8 IM_UTLM_CruiseMode_t;
#define IM_UTLM_CruiseMode_t_LowerLimit (0U)
#define IM_UTLM_CruiseMode_t_UpperLimit (1U)
# ifndef IM_UTLM_ACC
#  define IM_UTLM_ACC (0U)
# endif
# ifndef IM_UTLM_ICC
#  define IM_UTLM_ICC (1U)
# endif
#endif

/* Enumeration type:        IM_UTLM_PrivateCustomMode_t */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_PrivateCustomMode_t
#define Rte_TypeDef_IM_UTLM_PrivateCustomMode_t
typedef UINT8 IM_UTLM_PrivateCustomMode_t;
#define IM_UTLM_PrivateCustomMode_t_LowerLimit (0U)
#define IM_UTLM_PrivateCustomMode_t_UpperLimit (1U)
# ifndef IM_UTLM_COMFORT
#  define IM_UTLM_COMFORT (0U)
# endif
# ifndef IM_UTLM_SPORT
#  define IM_UTLM_SPORT (1U)
# endif
#endif

/* Enumeration type:        IM_UTLM_OBCMenustatus_t     */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_OBCMenustatus_t
#define Rte_TypeDef_IM_UTLM_OBCMenustatus_t
typedef UINT8 IM_UTLM_OBCMenustatus_t;
#define IM_UTLM_OBCMenustatus_t_LowerLimit (0U)
#define IM_UTLM_OBCMenustatus_t_UpperLimit (2U)
# ifndef IM_SWITCH_OFF
#  define IM_SWITCH_OFF (0U)
# endif
# ifndef IM_SWITCH_ON
#  define IM_SWITCH_ON (1U)
# endif
# ifndef IM_SWITCH_RESET
#  define IM_SWITCH_RESET (2U)
# endif
#endif

/* Enumeration type:        IM_UTLM_VolumeMode_t        */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_VolumeMode_t
#define Rte_TypeDef_IM_UTLM_VolumeMode_t
typedef UINT8 IM_UTLM_VolumeMode_t;
#define IM_UTLM_VolumeMode_t_LowerLimit (0U)
#define IM_UTLM_VolumeMode_t_UpperLimit (2U)
# ifndef IM_VOLUME_LOW
#  define IM_VOLUME_LOW (0U)
# endif
# ifndef IM_VOLUME_MEDIUM
#  define IM_VOLUME_MEDIUM (1U)
# endif
# ifndef IM_VOLUME_HIGH
#  define IM_VOLUME_HIGH (2U)
# endif
#endif

/* Enumeration type:        IM_Language_t */
/* Description:                           */
#ifndef Rte_TypeDef_IM_Language_t
#define Rte_TypeDef_IM_Language_t
typedef UINT8 IM_Language_t;
#define IM_Language_t_LowerLimit (0U)
#define IM_Language_t_UpperLimit (1U)
# ifndef IM_LANGUAGE_CHINESE
#  define IM_LANGUAGE_CHINESE (0U)
# endif
# ifndef IM_LANGUAGE_ENGLISH
#  define IM_LANGUAGE_ENGLISH (1U)
# endif
#endif

/* Enumeration type:        IM_TemperatureUnit_t        */
/* Description:                           */
#ifndef Rte_TypeDef_IM_TemperatureUnit_t
#define Rte_TypeDef_IM_TemperatureUnit_t
typedef UINT8 IM_TemperatureUnit_t;
#define IM_TemperatureUnit_t_LowerLimit (0U)
#define IM_TemperatureUnit_t_UpperLimit (1U)
# ifndef IM_TEMPERATURE_C
#  define IM_TEMPERATURE_C (0U)
# endif
# ifndef IM_TEMPERATURE_F
#  define IM_TEMPERATURE_F (1U)
# endif
#endif

/* Enumeration type:        IM_TimeFormat_t             */
/* Description:                           */
#ifndef Rte_TypeDef_IM_TimeFormat_t
#define Rte_TypeDef_IM_TimeFormat_t
typedef UINT8 IM_TimeFormat_t;
#define IM_TimeFormat_t_LowerLimit (0U)
#define IM_TimeFormat_t_UpperLimit (1U)
# ifndef IM_TIME_FORMAT_12H
#  define IM_TIME_FORMAT_12H (0U)
# endif
# ifndef IM_TIME_FORMAT_24H
#  define IM_TIME_FORMAT_24H (1U)
# endif
#endif

/* Enumeration type:        IM_OBCM_FatigueWarning_t    */
/* Description:                           */
#ifndef Rte_TypeDef_IM_OBCM_FatigueWarning_t
#define Rte_TypeDef_IM_OBCM_FatigueWarning_t
typedef UINT8 IM_OBCM_FatigueWarning_t;
#define IM_OBCM_FatigueWarning_t_LowerLimit (0U)
#define IM_OBCM_FatigueWarning_t_UpperLimit (1U)
# ifndef IM_OBCM_NO_RESET
#  define IM_OBCM_NO_RESET (0U)
# endif
# ifndef IM_OBCM_RESET
#  define IM_OBCM_RESET (1U)
# endif
#endif

/* Enumeration type:        IM_Obcm_Status_t            */
/* Description:                           */
#ifndef Rte_TypeDef_IM_Obcm_Status_t
#define Rte_TypeDef_IM_Obcm_Status_t
typedef UINT8 IM_Obcm_Status_t;
#define IM_Obcm_Status_t_LowerLimit (0U)
#define IM_Obcm_Status_t_UpperLimit (1U)
# ifndef IM_OBCM_OFF
#  define IM_OBCM_OFF (0U)
# endif
# ifndef IM_OBCM_ON
#  define IM_OBCM_ON (1U)
# endif
#endif

/* Enumeration type:        IM_Uds_ControlState_t       */
/* Description:                           */
#ifndef Rte_TypeDef_IM_Uds_ControlState_t
#define Rte_TypeDef_IM_Uds_ControlState_t
typedef UINT8 IM_Uds_ControlState_t;
#define IM_Uds_ControlState_t_LowerLimit (1U)
#define IM_Uds_ControlState_t_UpperLimit (15U)
# ifndef IM_ALL_PIXEL_WHITE
#  define IM_ALL_PIXEL_WHITE (1U)
# endif
# ifndef IM_ALL_PIXEL_BLACK
#  define IM_ALL_PIXEL_BLACK (2U)
# endif
# ifndef IM_ALL_PIXEL_RED
#  define IM_ALL_PIXEL_RED (3U)
# endif
# ifndef IM_ALL_PIXEL_GREEN
#  define IM_ALL_PIXEL_GREEN (4U)
# endif
# ifndef IM_ALL_PIXEL_BLUE
#  define IM_ALL_PIXEL_BLUE (5U)
# endif
# ifndef IM_ALL_PIXEL_WHITE
#  define IM_ALL_PIXEL_WHITE (6U)
# endif
# ifndef IM_CHESS_BOARD_RED
#  define IM_CHESS_BOARD_RED (7U)
# endif
# ifndef IM_CHESS_BOARD_GREEN
#  define IM_CHESS_BOARD_GREEN (8U)
# endif
# ifndef IM_CHESS_BOARD_BLUE
#  define IM_CHESS_BOARD_BLUE (9U)
# endif
# ifndef IM_INVERT_CHESS_BOARD_WHITE
#  define IM_INVERT_CHESS_BOARD_WHITE (10U)
# endif
# ifndef IM_INVERT_CHESS_BOARD_RED
#  define IM_INVERT_CHESS_BOARD_RED (11U)
# endif
# ifndef IM_INVERT_CHESS_BOARD_GREEN
#  define IM_INVERT_CHESS_BOARD_GREEN (12U)
# endif
# ifndef IM_INVERT_CHESS_BOARD_BLUE
#  define IM_INVERT_CHESS_BOARD_BLUE (13U)
# endif
# ifndef IM_START_LOGO_PATTERN
#  define IM_START_LOGO_PATTERN (14U)
# endif
# ifndef IM_FOUR_COLOR_PATTERN
#  define IM_FOUR_COLOR_PATTERN (15U)
# endif
#endif

/* Enumeration type:        IM_ControlParameter_t       */
/* Description:                           */
#ifndef Rte_TypeDef_IM_ControlParameter_t
#define Rte_TypeDef_IM_ControlParameter_t
typedef UINT8 IM_ControlParameter_t;
#define IM_ControlParameter_t_LowerLimit (0U)
#define IM_ControlParameter_t_UpperLimit (3U)
# ifndef IM_IOCONTROL_RCTE
#  define IM_IOCONTROL_RCTE (0U)
# endif
# ifndef IM_IOCONTROL_RTD
#  define IM_IOCONTROL_RTD (1U)
# endif
# ifndef IM_IOCONTROL_FCS
#  define IM_IOCONTROL_FCS (2U)
# endif
# ifndef IM_IOCONTROL_STA
#  define IM_IOCONTROL_STA (3U)
# endif
#endif

/* Enumeration type:        IM_UTLM_PageId_t            */
/* Description:                           */
#ifndef Rte_TypeDef_IM_UTLM_PageId_t
#define Rte_TypeDef_IM_UTLM_PageId_t
typedef UINT8 IM_UTLM_PageId_t;
#define IM_UTLM_PageId_t_LowerLimit (0U)
#define IM_UTLM_PageId_t_UpperLimit (8U)
# ifndef IM_UTLM_PAGE_ID_TRIP
#  define IM_UTLM_PAGE_ID_TRIP (0U)
# endif
# ifndef IM_UTLM_PAGE_ID_AFC
#  define IM_UTLM_PAGE_ID_AFC (1U)
# endif
# ifndef IM_UTLM_PAGE_ID_SAFC
#  define IM_UTLM_PAGE_ID_SAFC (2U)
# endif
# ifndef IM_UTLM_PAGE_ID_IFC
#  define IM_UTLM_PAGE_ID_IFC (3U)
# endif
# ifndef IM_UTLM_PAGE_ID_DTE
#  define IM_UTLM_PAGE_ID_DTE (4U)
# endif
# ifndef IM_UTLM_PAGE_ID_TRAVELTIME
#  define IM_UTLM_PAGE_ID_TRAVELTIME (5U)
# endif
# ifndef IM_UTLM_PAGE_ID_AVS
#  define IM_UTLM_PAGE_ID_AVS (6U)
# endif
# ifndef IM_UTLM_PAGE_ID_TYRESTATE
#  define IM_UTLM_PAGE_ID_TYRESTATE (7U)
# endif
# ifndef IM_UTLM_PAGE_ID_SINGLETRIP
#  define IM_UTLM_PAGE_ID_SINGLETRIP (8U)
# endif
#endif

/* Enumeration type:        IM_FrsAlodStatus_t          */
/* Description:                           */
#ifndef Rte_TypeDef_IM_FrsAlodStatus_t
#define Rte_TypeDef_IM_FrsAlodStatus_t
typedef UINT8 IM_FrsAlodStatus_t;
#define IM_FrsAlodStatus_t_LowerLimit (0U)
#define IM_FrsAlodStatus_t_UpperLimit (10U)
# ifndef IM_ALOD_STATUS_OFF_MODE
#  define IM_ALOD_STATUS_OFF_MODE (0U)
# endif
# ifndef IM_ALOD_STATUS_PASSIVE_MODE
#  define IM_ALOD_STATUS_PASSIVE_MODE (1U)
# endif
# ifndef IM_ALOD_STATUS_STANDBY_MODE
#  define IM_ALOD_STATUS_STANDBY_MODE (2U)
# endif
# ifndef IM_ALOD_STATUS_ACTIVE_CONTROL_MODE
#  define IM_ALOD_STATUS_ACTIVE_CONTROL_MODE (3U)
# endif
# ifndef IM_ALOD_STATUS_BRAKE_ONLY_MODE
#  define IM_ALOD_STATUS_BRAKE_ONLY_MODE (4U)
# endif
# ifndef IM_ALOD_STATUS_OVERRIDE
#  define IM_ALOD_STATUS_OVERRIDE (5U)
# endif
# ifndef IM_ALOD_STATUS_STAND_ACTIVE
#  define IM_ALOD_STATUS_STAND_ACTIVE (6U)
# endif
# ifndef IM_ALOD_STATUS_STAND_WAIT
#  define IM_ALOD_STATUS_STAND_WAIT (7U)
# endif
# ifndef IM_ALOD_STATUS_TEMPORARY_FAILURE
#  define IM_ALOD_STATUS_TEMPORARY_FAILURE (8U)
# endif
# ifndef IM_ALOD_STATUS_PERMANENT_FAILURE
#  define IM_ALOD_STATUS_PERMANENT_FAILURE (9U)
# endif
# ifndef IM_ALOD_STATUS_RESERVED
#  define IM_ALOD_STATUS_RESERVED (10U)
# endif
#endif

/* Enumeration type:        IM_FcsAladType_t            */
/* Description:                           */
#ifndef Rte_TypeDef_IM_FcsAladType_t
#define Rte_TypeDef_IM_FcsAladType_t
typedef UINT8 IM_FcsAladType_t;
#define IM_FcsAladType_t_LowerLimit (0U)
#define IM_FcsAladType_t_UpperLimit (4U)
# ifndef IM_ALAD_TYPE_RESERVED
#  define IM_ALAD_TYPE_RESERVED (0U)
# endif
# ifndef IM_ALAD_TYPE_OFF
#  define IM_ALAD_TYPE_OFF (1U)
# endif
# ifndef IM_ALAD_TYPE_LDW
#  define IM_ALAD_TYPE_LDW (2U)
# endif
# ifndef IM_ALAD_TYPE_RDPANDLDW
#  define IM_ALAD_TYPE_RDPANDLDW (3U)
# endif
# ifndef IM_ALAD_TYPE_LKSANDLDW
#  define IM_ALAD_TYPE_LKSANDLDW (4U)
# endif
#endif

/* Enumeration type:        IM_FcsAladStatus_t          */
/* Description:                           */
#ifndef Rte_TypeDef_IM_FcsAladStatus_t
#define Rte_TypeDef_IM_FcsAladStatus_t
typedef UINT8 IM_FcsAladStatus_t;
#define IM_FcsAladStatus_t_LowerLimit (0U)
#define IM_FcsAladStatus_t_UpperLimit (8U)
# ifndef IM_ALAD_STATUS_OFF
#  define IM_ALAD_STATUS_OFF (0U)
# endif
# ifndef IM_ALAD_STATUS_PASSIVE
#  define IM_ALAD_STATUS_PASSIVE (1U)
# endif
# ifndef IM_ALAD_STATUS_STANDBY
#  define IM_ALAD_STATUS_STANDBY (2U)
# endif
# ifndef IM_ALAD_STATUS_ACTIVE
#  define IM_ALAD_STATUS_ACTIVE (3U)
# endif
# ifndef IM_ALAD_STATUS_TAKE_OVER_REQ
#  define IM_ALAD_STATUS_TAKE_OVER_REQ (4U)
# endif
# ifndef IM_ALAD_STATUS_SAFE_STOP
#  define IM_ALAD_STATUS_SAFE_STOP (5U)
# endif
# ifndef IM_ALAD_STATUS_TEMPORARY_ERROR
#  define IM_ALAD_STATUS_TEMPORARY_ERROR (6U)
# endif
# ifndef IM_ALAD_STATUS_PERMANENT_ERROR
#  define IM_ALAD_STATUS_PERMANENT_ERROR (7U)
# endif
# ifndef IM_ALAD_STATUS_RESERVED
#  define IM_ALAD_STATUS_RESERVED (8U)
# endif
#endif

/* Enumeration type:        IM_Utlm_NavDisMode_t        */
/* Description:                           */
#ifndef Rte_TypeDef_IM_Utlm_NavDisMode_t
#define Rte_TypeDef_IM_Utlm_NavDisMode_t
typedef UINT8 IM_Utlm_NavDisMode_t;
#define IM_Utlm_NavDisMode_t_LowerLimit (1U)
#define IM_Utlm_NavDisMode_t_UpperLimit (2U)
# ifndef IM_NAV_SIMPLE_MODE
#  define IM_NAV_SIMPLE_MODE (1U)
# endif
# ifndef IM_NAV_FULL_MAP_MODE
#  define IM_NAV_FULL_MAP_MODE (2U)
# endif
#endif

/* Enumeration type:        IM_Utlm_AEBSenLevel_t       */
/* Description:                           */
#ifndef Rte_TypeDef_IM_Utlm_AEBSenLevel_t
#define Rte_TypeDef_IM_Utlm_AEBSenLevel_t
typedef UINT8 IM_Utlm_AEBSenLevel_t;
#define IM_Utlm_AEBSenLevel_t_LowerLimit (0U)
#define IM_Utlm_AEBSenLevel_t_UpperLimit (2U)
# ifndef IM_UTLM_SENSITIVE_LOW
#  define IM_UTLM_SENSITIVE_LOW (0U)
# endif
# ifndef IM_UTLM_SENSITIVE_MEDIUM
#  define IM_UTLM_SENSITIVE_MEDIUM (1U)
# endif
# ifndef IM_UTLM_SENSITIVE_HIGH
#  define IM_UTLM_SENSITIVE_HIGH (2U)
# endif
#endif

/* ********** Structures ********** */

/* Structure :IM_WDog_t */
/* Description :IM_WDog_t */
#ifndef Rte_TypeDef_IM_WDog_t
#define Rte_TypeDef_IM_WDog_t
typedef struct
{
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Cnt     : 4;
  UINT8  CntCmpl : 4;
#else
  UINT8  CntCmpl : 4; /*  0 */
  UINT8  Cnt     : 4; /*  4 */
#endif
} IM_WDog_t;

#endif

/* Structure :PA_HTMM_DtcStatus_t */
/* Description :dtc status */
#ifndef Rte_TypeDef_PA_HTMM_DtcStatus_t
#define Rte_TypeDef_PA_HTMM_DtcStatus_t
typedef struct
{
  UINT32               HTMM_Dtc1Status;
  UINT32               HTMM_Dtc2Status;
  UINT32               HTMM_Dtc3Status;
  UINT32               HTMM_Dtc4Status;
  UINT32               HTMM_Dtc5Status;
  UINT32               HTMM_Dtc6Status;
  Data_Availability_t  Data_Availability;
} PA_HTMM_DtcStatus_t;

#endif

/* Structure :PA_GLOBAL_State_t */
/* Description :HMI Status */
#ifndef Rte_TypeDef_PA_GLOBAL_State_t
#define Rte_TypeDef_PA_GLOBAL_State_t
typedef struct
{
  GLOBAL_HMIState_t  GLOBAL_HMIState;
} PA_GLOBAL_State_t;

#endif

/* Structure :PA_SYSM_GpuVersion_t */
/* Description :Gpu Client's version in Hex format. */
#ifndef Rte_TypeDef_PA_SYSM_GpuVersion_t
#define Rte_TypeDef_PA_SYSM_GpuVersion_t
typedef struct
{
  UINT8                GpuMajorVersion;
  UINT8                GpuMinorVersion;
  UINT8                GpuPatchVersion;
  Data_Availability_t  Data_Availability;
} PA_SYSM_GpuVersion_t;

#endif

/* Structure :PA_SYSM_LogInfo_t */
/* Description : */
#ifndef Rte_TypeDef_PA_SYSM_LogInfo_t
#define Rte_TypeDef_PA_SYSM_LogInfo_t
typedef struct
{
  UINT8  LogFlag; /* log information for debug */
  UINT8  LogInfo[19];
} PA_SYSM_LogInfo_t;

#endif

/* Structure :IMCP_McuAsil_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuAsil_t
#define Rte_TypeDef_IMCP_McuAsil_t
typedef struct
{
  UINT8   Ctrl_Counter;
  UINT8   WorkModeStatus; /* Work Mode Status */
  UINT8   WKMM_FunctionVoltageState;
  UINT8   WKMM_Voltage;
  UINT8   WKMM_Data_Availability;
  UINT8   Cmp_WorkModeStatus;
  UINT8   Cmp_WKMM_FunctionVoltageState;
  UINT8   Cmp_WKMM_Voltage;
  UINT8   Cmp_WKMM_Data_Availability;
  UINT8   Ret_PA_WkmmWorkModeStatus;
  UINT8   DIMM_IlluminAdjustLevel; /* DIMM LCD Illumination */
  UINT8   Cmp_DIMM_IlluminAdjustLevel;
  UINT16  DIMM_LCD_Illumination;
  UINT16  Cmp_DIMM_LCD_Illumination;
  UINT8   Ret_PA_DimmIllumination;
  UINT8   GPMM_CtrlCmd; /* Shut Down GPU Request */
  UINT8   GPMM_Data_Availability;
  UINT8   Cmp_GPMM_CtrlCmd;
  UINT8   Cmp_GPMM_Data_Availability;
  UINT8   Ret_PA_GpmmShutDownRequest;
  UINT8   PowerModeStatus; /* Power Mode Status */
  UINT8   Kl15Status;
  UINT8   PepsPowerModeStatus;
  UINT8   Kl15Fault;
  UINT8   PWMM_Data_Availability;
  UINT8   Cmp_PowerModeStatus;
  UINT8   Cmp_Kl15Status;
  UINT8   Cmp_PepsPowerModeStatus;
  UINT8   Cmp_Kl15Fault;
  UINT8   Cmp_PWMM_Data_Availability;
  UINT8   Ret_PA_PwmmSendPowerModeStatus;
  UINT32  WKSS_TaskState; /* WKSS Task State */
  UINT32  WKSS_TaskStateReserve;
  UINT32  Cmp_WKSS_TaskState;
  UINT32  Cmp_WKSS_TaskStateReserve;
  UINT8   Ret_PA_WkssTaskState;
  UINT8   NETC_WorkState; /* NETC Work State */
  UINT8   NETM_WorkState; /* NETM Work State */
  UINT8   NET_Data_Availability;
  UINT8   Cmp_NETC_WorkState;
  UINT8   Cmp_NETM_WorkState;
  UINT8   Cmp_NET_Data_Availability;
  UINT8   Ret_PA_NetWorkState;
  UINT8   LcdReadBackPin;
  UINT8   TftHwReady;
  UINT8   SafmInfoAvail;
  UINT8   Cmp_LcdReadBackPin;
  UINT8   Cmp_TftHwReady;
  UINT8   Cmp_SafmInfoAvail;
  UINT8   Ret_IM_SafmInfo;
} IMCP_McuAsil_t;

#endif

/* Structure :IMCP_GpuAsil_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_GpuAsil_t
#define Rte_TypeDef_IMCP_GpuAsil_t
typedef struct
{
  UINT8   Ctrl_Counter;
  UINT32  WdgThreadTimeout;
  UINT32  Cmp_WdgThreadTimeout;
  UINT8   WdgGpuRefresh;
  UINT8   Cmp_WdgGpuRefresh;
  UINT8   Ret_Safm_WdgGpuInfo;
  UINT8   AliveStatus;
  UINT8   AliveCounter;
  UINT8   Cmp_AliveStatus;
  UINT8   Cmp_AliveCounter;
  UINT8   Ret_SoundDriverAlive;
  UINT8   DimmWorkStatus;
  UINT8   Cmp_DimmWorkStatus;
  UINT8   Ret_Safm_GpuDimmStatus;
  UINT8   TftFaultPinDetect;
  UINT8   TftFaultPinDetectAvail;
  UINT8   Cmp_TftFaultPinDetect;
  UINT8   Cmp_TftFaultPinDetectAvail;
  UINT8   Ret_IM_TftFaultPinDefect;
  UINT8   TftPowerDetect;
  UINT8   TftPowerDetectAvail;
  UINT8   Cmp_TftPowerDetect;
  UINT8   Cmp_TftPowerDetectAvail;
  UINT8   Ret_IM_TftPowerDefect;
  UINT8   BacklightDtcStatus;
  UINT8   BacklightDtcAvail;
  UINT8   Cmp_BacklightDtcStatus;
  UINT8   Cmp_BacklightDtcAvail;
  UINT8   Ret_IM_BacklightDTC;
} IMCP_GpuAsil_t;

#endif

/* Structure :IMCP_McuSend10ms_G1M_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend10ms_G1M_t
#define Rte_TypeDef_IMCP_McuSend10ms_G1M_t
typedef struct
{
  UINT16  TripMeterValue;
  UINT8   TripMeterUnit;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_TripMeter  : 4;
  UINT8  TripMeterState : 4;
#else
  UINT8  TripMeterState : 4; /*  0 */
  UINT8  Ret_TripMeter  : 4; /*  4 */
#endif
  UINT16  SingleTripMeterValue;
  UINT8   SingleTripMeterUnit;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_SingleTripMeter  : 4;
  UINT8  SingleTripMeterState : 4;
#else
  UINT8  SingleTripMeterState : 4; /*  0 */
  UINT8  Ret_SingleTripMeter  : 4; /*  4 */
#endif
  UINT16               LevelPercentage;
  UINT16               TotalCapacity;
  Data_Availability_t  Fuel_DataValidity;
  UINT8                Ret_PA_FuelLevel;
  UINT8                WatmIndication;
  UINT8                Ret_WatmIndication;
  UINT16               SPDM_ValueInKm;
  UINT16               SPDM_ValueInMph;
  SPDM_SpeedUnit_t     SPDM_DisplayUnit;
  UINT8                SPDM_AlphaValue;
  Data_Availability_t  SPDM_Data_Availability;
  UINT8                Ret_PA_SPDM_SendFilterSpeedDisplayValue;
  UINT32               ODOM_ValueInKm;
  UINT32               ODOM_ValueInMph;
  ODOM_OdoUnits_t      ODOM_OdoUnit;
  Data_Availability_t  ODOM_Data_Availability;
  UINT8                Ret_PA_ODOM_SendOdoDisplayValue;
  UINT8                TACP_EngineRunningFlag;
  UINT16               TACP_TachoMeterValue;
  Data_Availability_t  TACP_TachoValidity;
  UINT8                Ret_PA_TACM_TachoMeter;
  UNIT_ThemeStyle_t    UNIT_SetStyle;
  Data_Availability_t  UNIT_SetStyleData_Availability;
  UINT8                Ret_PA_UNIT_ThemeSet;
} IMCP_McuSend10ms_G1M_t;

#endif

/* Structure :IMCP_McuSend10ms_G1F_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend10ms_G1F_t
#define Rte_TypeDef_IMCP_McuSend10ms_G1F_t
typedef struct
{
  UINT8                        ALRM_TelltaleStatus      [43];
  IM_ALRM_Data_Availability_t  ALRM_Data_Availability   [43];
  UINT8                        Ret_PA_AlrmTelltaleStatus;
} IMCP_McuSend10ms_G1F_t;

#endif

/* Structure :IMCP_McuSend10ms_G2F_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend10ms_G2F_t
#define Rte_TypeDef_IMCP_McuSend10ms_G2F_t
typedef struct
{
  UINT16  ALRM_WICMsgId   [10];
  UINT16  ALRM_WICTextId  [10];
  UINT16  ALRM_WICSymbolId[10];
  UINT8   Ret_PA_AlrmWIC;
} IMCP_McuSend10ms_G2F_t;

#endif

/* Structure :IMCP_McuSend10ms_G2M_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend10ms_G2M_t
#define Rte_TypeDef_IMCP_McuSend10ms_G2M_t
typedef struct
{
  UINT32               SoundDuration;
  UINT16               SoundReplayTime;
  IM_ALRM_SoundId_t    SoundId;
  UINT8                SequenceNumber;
  UINT8                Ret_PA_Alrm_SoundInfo;
  UINT16               ALRM_MsgId;
  UINT16               ALRM_TextId;
  UINT16               ALRM_SymbolId;
  IM_ALRM_MsgStatus_t  ALRM_MsgStatus;
  UINT8                Ret_PA_AlrmMessage;
  UINT16               ADASMsgId;
  UINT16               ADASTextId;
  UINT16               ADASSymbolId;
  IM_ALRM_MsgStatus_t  ADASMsgStatus;
  UINT8                Ret_ALRM_ADASMessage;
  UINT16               VehicleSpeedAlarm;
  UINT8                SLIFSpeedLimit;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  MSeatbeltUnbuckle    : 2;
  UINT8  LSeatbeltUnbuckle    : 2;
  UINT8  PassSeatbeltUnbuckle : 2;
  UINT8  DrvSeatbeltUnbuckle  : 2;
#else
  UINT8  DrvSeatbeltUnbuckle  : 2; /*  0 */
  UINT8  PassSeatbeltUnbuckle : 2; /*  2 */
  UINT8  LSeatbeltUnbuckle    : 2; /*  4 */
  UINT8  MSeatbeltUnbuckle    : 2; /*  6 */
#endif
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  FrontRightDoorUnlock : 2;
  UINT8  RearLeftDoorUnlock   : 2;
  UINT8  FrontLeftDoorUnlock  : 2;
  UINT8  RSeatbeltUnbuckle    : 2;
#else
  UINT8  RSeatbeltUnbuckle    : 2; /*  0 */
  UINT8  FrontLeftDoorUnlock  : 2; /*  2 */
  UINT8  RearLeftDoorUnlock   : 2; /*  4 */
  UINT8  FrontRightDoorUnlock : 2; /*  6 */
#endif
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  ChangeModeFailureReason     : 3;
  UINT8  ChangeModeFailureControllor : 3;
  UINT8  RearRightDoorUnlock         : 2;
#else
  UINT8  RearRightDoorUnlock         : 2; /*  0 */
  UINT8  ChangeModeFailureControllor : 3; /*  2 */
  UINT8  ChangeModeFailureReason     : 3; /*  5 */
#endif
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_Alrm_WarningExtras : 1;
  UINT8  TrunkOpen              : 1;
  UINT8  HoodOpen               : 1;
  UINT8  RearRightDoorOpen      : 1;
  UINT8  FrontRightDoorOpen     : 1;
  UINT8  RearLeftDoorOpen       : 1;
  UINT8  FrontLeftDoorOpen      : 1;
  UINT8  SLIFWarning            : 1;
#else
  UINT8  SLIFWarning            : 1; /*  0 */
  UINT8  FrontLeftDoorOpen      : 1; /*  1 */
  UINT8  RearLeftDoorOpen       : 1; /*  2 */
  UINT8  FrontRightDoorOpen     : 1; /*  3 */
  UINT8  RearRightDoorOpen      : 1; /*  4 */
  UINT8  HoodOpen               : 1; /*  5 */
  UINT8  TrunkOpen              : 1; /*  6 */
  UINT8  Ret_Alrm_WarningExtras : 1; /*  7 */
#endif
} IMCP_McuSend10ms_G2M_t;

#endif

/* Structure :IMCP_GpuSend10ms_G1F_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_GpuSend10ms_G1F_t
#define Rte_TypeDef_IMCP_GpuSend10ms_G1F_t
typedef struct
{
  UINT16                  ALRM_CBMsgId;
  IM_ALRM_MsgStatus_t     ALRM_CBMsgStatus;
  IM_ALRM_ButtonStatus_t  ALRM_ButtonPrsStatus;
  UINT8                   Ret_CB_AlrmMessage;
  UINT8                   SoundState;
  UINT8                   SequenceNumber;
  UINT8                   Ret_CB_AlrmSound;
  IM_DIMM_LcdDefect_t     DefectHwInput;
  Data_Availability_t     HwInputAvailability;
  UINT8                   Ret_PA_DIMM_LcdDefectHwInput;
  IM_DIMM_LcdDefect_t     DefectPowerInput;
  Data_Availability_t     DefectPowerInputAvailability;
  UINT8                   Ret_PA_DIMM_LcdDefectPowerInput;
  IM_UTLM_MenuStatus_t    AEBSwitch;
  IM_UTLM_MenuStatus_t    AEBWarning;
  Data_Availability_t     AEBDataValidity;
  IM_Utlm_AEBSenLevel_t   SenLevel;
  UINT8                   Ret_CB_UTLM_AEBSelect;
  IM_UTLM_LKAMode_t       LKAMode;
  IM_UTLM_LKAFeedback_t   LKAFeedback;
  IM_UTLM_MenuStatus_t    LKAEnable;
  Data_Availability_t     LKADataValidity;
  UINT8                   Ret_CB_UTLM_LKASelect;
  IM_UTLM_MenuStatus_t    IHBCSwitch;
  Data_Availability_t     IHBCDataValidity;
  UINT8                   Ret_CB_UTL_IHBCSelect;
  IM_UTLM_MenuStatus_t    SLIFSwitch;
  IM_UTLM_MenuStatus_t    SoundWarning;
  Data_Availability_t     SLIFDataValidity;
  UINT8                   Ret_CB_UTLM_SLIFSelect;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_TftBacklightDTC : 4;
  UINT8  DtcValue            : 4;
#else
  UINT8  DtcValue            : 4; /*  0 */
  UINT8  Ret_TftBacklightDTC : 4; /*  4 */
#endif
  UINT8                 DispStsFrnt;
  UINT8                 Ret_DispStsFrnt;
  UINT8                 Hour;
  UINT8                 Minute;
  UINT8                 Second;
  UINT8                 TimeFormat;
  Data_Availability_t   Time_Availability;
  UINT8                 Ret_CB_Time;
  UINT8                 Year;
  UINT8                 Month;
  UINT8                 Day;
  Data_Availability_t   Date_Availability;
  UINT8                 Ret_CB_Date;
  IM_UTLM_CruiseMode_t  CruiseMode;
  UINT8                 ACCDataValidity;
  UINT8                 Ret_CB_UTLM_ACCSelect;
} IMCP_GpuSend10ms_G1F_t;

#endif

/* Structure :IMCP_McuSend10ms_G3F_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend10ms_G3F_t
#define Rte_TypeDef_IMCP_McuSend10ms_G3F_t
typedef struct
{
  UINT8  Tcsm_ClustrUpButtonPressSts; /* Trip computer switch status */
  UINT8  Tcsm_ClustrDownButtonPressSts;
  UINT8  Tcsm_ClustrLeftButtonPressSts;
  UINT8  Tcsm_ClustrRightButtonPressSts;
  UINT8  Tcsm_ClustrConfirmButtonPressSts;
  UINT8  Tcsm_ClustrReturnButtonPressSts; /* Home button */
  UINT8  Tcsm_WorkMode; /* Menu Button */
  UINT8  Tcsm_SwTripButtonPressSts;
  UINT8  Tcsm_RollingCounter;
  UINT8  Tcsm_CheckSum;
  UINT8  Tcsm_AccIccOnOff;
  UINT8  Tcsm_AccIccOnOffAvailable;
  UINT8  Ret_PA_TCSM_SwitchStatus;
  UINT8  LIMSetSpeed;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_ALRM_LIMInfo : 4;
  UINT8  LIMWarning          : 1;
  UINT8  LIMActive           : 1;
  UINT8  LIMOn               : 1;
  UINT8  LIMSetSpeedInvalid  : 1;
#else
  UINT8  LIMSetSpeedInvalid  : 1; /*  0 */
  UINT8  LIMOn               : 1; /*  1 */
  UINT8  LIMActive           : 1; /*  2 */
  UINT8  LIMWarning          : 1; /*  3 */
  UINT8  Ret_PA_ALRM_LIMInfo : 4; /*  4 */
#endif
} IMCP_McuSend10ms_G3F_t;

#endif

/* Structure :IMCP_McuSend20ms_G1M_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend20ms_G1M_t
#define Rte_TypeDef_IMCP_McuSend20ms_G1M_t
typedef struct
{
  UINT32                  AfcValue;
  IM_OBCM_FCUnit_t        AfcUnit;
  UINT8                   AfcValidity;
  UINT8                   Ret_PA_ObcmAFCValue;
  UINT32                  SafcValue;
  IM_OBCM_FCUnit_t        SafcUnit;
  UINT8                   SafcValidity;
  UINT8                   Ret_PA_ObcmSAFCValue;
  UINT32                  IfcValue;
  IM_OBCM_FCUnit_t        IfcUnit;
  UINT8                   IfcValidity;
  UINT8                   Ret_PA_ObcmIFCValue;
  UINT16                  DteValue;
  UINT8                   DteValidity;
  UINT8                   Ret_PA_ObcmDTEValue;
  UINT16                  TravelTimeDisValue;
  UINT8                   TravelTimeDisValue_Validity;
  UINT8                   Ret_PA_ObcmTravelTime;
  UINT8                   AVSValue;
  IM_OBCM_AVSUnit_t       AVSUnit;
  UINT8                   AVSValidity;
  UINT8                   Ret_PA_ObcmAVSValue;
  IM_GearDisplay_t        GearDisplay;
  IM_Gear_ArrowDisplay_t  GearArrow;
  UINT8                   GearValidity;
  UINT8                   Ret_PA_ObcmGear;
  UINT8                   DisplayTimeGap;
  UINT8                   Ret_Obcm_DisplayTimeGap;
  UINT8                   SetSpeed;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_Obcm_DisplaySetSpeed      : 4;
  UINT8  DisplaySetSpeedFunctionEnable : 2;
  UINT8  DisplaySetSpeedValidity       : 2;
#else
  UINT8  DisplaySetSpeedValidity       : 2; /*  0 */
  UINT8  DisplaySetSpeedFunctionEnable : 2; /*  2 */
  UINT8  Ret_Obcm_DisplaySetSpeed      : 4; /*  4 */
#endif
  UINT16  Distance;
#ifdef FIRST_BIT_IS_HIGHEST
  IM_OBCM_MaintenanceWarningType_t  _bitfield_padd_0 : 1; /* to complete bitfield */
  IM_OBCM_MaintenanceWarningType_t  Type             : 7;
#else
  IM_OBCM_MaintenanceWarningType_t  Type             : 7; /*  0 */
  IM_OBCM_MaintenanceWarningType_t  _bitfield_padd_0 : 1; /*  7 to complete bitfield */
#endif
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  _bitfield_padd_1            : 7; /* to complete bitfield */
  UINT8  Ret_Obcm_DisplayMaintenance : 1;
#else
  UINT8  Ret_Obcm_DisplayMaintenance : 1; /*  0 */
  UINT8  _bitfield_padd_1            : 7; /*  1 to complete bitfield */
#endif
  SINT8                   LeftLineDistance;
  IM_OBCM_FCSLineType_t   LeftLineType;
  IM_OBCM_FCSLineColor_t  LeftLineColor;
  SINT8                   RightLineDistance;
  IM_OBCM_FCSLineType_t   RightLineType;
  IM_OBCM_FCSLineColor_t  RightLineColor;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_ObcmLineInfo : 1;
  UINT8  FunctionEnable      : 1;
  UINT8  RightLineWarning    : 3;
  UINT8  LeftLineWarning     : 3;
#else
  UINT8  LeftLineWarning     : 3; /*  0 */
  UINT8  RightLineWarning    : 3; /*  3 */
  UINT8  FunctionEnable      : 1; /*  6 */
  UINT8  Ret_PA_ObcmLineInfo : 1; /*  7 */
#endif
} IMCP_McuSend20ms_G1M_t;

#endif

/* Structure :IMCP_McuSend20ms_G2M_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend20ms_G2M_t
#define Rte_TypeDef_IMCP_McuSend20ms_G2M_t
typedef struct
{
  UINT8                  F1DistanceX;
  SINT8                  F1DistanceY;
  IM_OBCM_ObjectType_t   F1Type;
  IM_OBCM_ObjectColor_t  F1ObjectInfoColor;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_OBCM_ObjFront1 : 4;
  UINT8  F1FunctionEnable      : 4;
#else
  UINT8  F1FunctionEnable      : 4; /*  0 */
  UINT8  Ret_PA_OBCM_ObjFront1 : 4; /*  4 */
#endif
  UINT8                  F2DistanceX;
  SINT8                  F2DistanceY;
  IM_OBCM_ObjectType_t   F2Type;
  IM_OBCM_ObjectColor_t  F2ObjectInfoColor;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_OBCM_ObjFront2 : 4;
  UINT8  F2FunctionEnable      : 4;
#else
  UINT8  F2FunctionEnable      : 4; /*  0 */
  UINT8  Ret_PA_OBCM_ObjFront2 : 4; /*  4 */
#endif
  UINT8                  FLDistanceX;
  SINT8                  FLDistanceY;
  IM_OBCM_ObjectType_t   FLType;
  IM_OBCM_ObjectColor_t  FLObjectInfoColor;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_OBCM_ObjFrontLeft : 4;
  UINT8  FLFunctionEnable         : 4;
#else
  UINT8  FLFunctionEnable         : 4; /*  0 */
  UINT8  Ret_PA_OBCM_ObjFrontLeft : 4; /*  4 */
#endif
  UINT8                  FRDistanceX;
  SINT8                  FRDistanceY;
  IM_OBCM_ObjectType_t   FRType;
  IM_OBCM_ObjectColor_t  FRObjectInfoColor;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_OBCM_ObjFrontRight : 4;
  UINT8  FRFunctionEnable          : 4;
#else
  UINT8  FRFunctionEnable          : 4; /*  0 */
  UINT8  Ret_PA_OBCM_ObjFrontRight : 4; /*  4 */
#endif
  SINT16               Radium;
  IM_OBCM_LaneColor_t  LaneInfoColor;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_ObcmLaneInfo     : 4;
  UINT8  LaneInfo_FunctionEnable : 4;
#else
  UINT8  LaneInfo_FunctionEnable : 4; /*  0 */
  UINT8  Ret_PA_ObcmLaneInfo     : 4; /*  4 */
#endif
  UINT8  SpeedLimit;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_ObcmSignInfo : 4;
  UINT8  Validity            : 4;
#else
  UINT8  Validity            : 4; /*  0 */
  UINT8  Ret_PA_ObcmSignInfo : 4; /*  4 */
#endif
  UINT8                LimSpeed;
  IM_OBCM_LimStatus_t  LimStatus;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_ObcmLimWarning : 4;
  UINT8  LimFunctionEnable     : 4;
#else
  UINT8  LimFunctionEnable     : 4; /*  0 */
  UINT8  Ret_PA_ObcmLimWarning : 4; /*  4 */
#endif
  IM_UTLM_SwitchAndType_t      AEBSwitch_WarningType;
  IM_UTLM_MenuDisplayStatus_t  Status;
  IM_Utlm_AEBSenLevel_t        SenLevel;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_UTLM_AEBSetting   : 4;
  UINT8  AEBSettingFunctionEnable : 4;
#else
  UINT8  AEBSettingFunctionEnable : 4; /*  0 */
  UINT8  Ret_PA_UTLM_AEBSetting   : 4; /*  4 */
#endif
  IM_UTLM_LKAMode_t      LKAMode;
  IM_UTLM_LKAFeedback_t  LKAFeedback;
  IM_UTLM_MenuStatus_t   LKAEnable;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_UTLM_LKASetting   : 4;
  UINT8  LKASettingFunctionEnable : 4;
#else
  UINT8  LKASettingFunctionEnable : 4; /*  0 */
  UINT8  Ret_PA_UTLM_LKASetting   : 4; /*  4 */
#endif
  IM_UTLM_MenuStatus_t         IHBCSwitch;
  IM_UTLM_MenuDisplayStatus_t  IHBCStatus;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_UTLM_IHBCSetting : 4;
  UINT8  IHBCFunctionEnable      : 4;
#else
  UINT8  IHBCFunctionEnable      : 4; /*  0 */
  UINT8  Ret_PA_UTLM_IHBCSetting : 4; /*  4 */
#endif
  IM_UTLM_MenuStatus_t         NavDisplaySwitch;
  IM_Utlm_NavDisMode_t         NavDisMode;
  UINT8                        Ret_PA_Utlm_NavigationSetting;
  IM_UTLM_MenuDisplayStatus_t  RearSideRadarStatus;
  IM_UTLM_MenuStatus_t         LaneChangeAssistSwitch;
  IM_UTLM_MenuStatus_t         LaneChangeAssistSoundSwitch;
  IM_UTLM_MenuStatus_t         RearCollisionWarningSwitch;
  IM_UTLM_MenuStatus_t         DoorOpenWarningSwitch;
  IM_UTLM_MenuStatus_t         RearCrossTrafficAlertSwitch;
  UINT8                        Ret_PA_Utlm_RearSideRadarSetting;
} IMCP_McuSend20ms_G2M_t;

#endif

/* Structure :IMCP_McuSend20ms_G4M_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend20ms_G4M_t
#define Rte_TypeDef_IMCP_McuSend20ms_G4M_t
typedef struct
{
  UINT16                  Tpms_FLTyrePrValue;
  UINT16                  Tpms_FRTyrePrValue;
  UINT16                  Tpms_RLTyrePrValue;
  UINT16                  Tpms_RRTyrePrValue;
  SINT16                  Tpms_FLTyreTempValue;
  SINT16                  Tpms_FRTyreTempValue;
  SINT16                  Tpms_RLTyreTempValue;
  SINT16                  Tpms_RRTyreTempValue;
  IM_OBCM_TyreStatus_t    Tpms_FLTyrePrStatus;
  IM_OBCM_TyreStatus_t    Tpms_FRTyrePrStatus;
  IM_OBCM_TyreStatus_t    Tpms_RLTyrePrStatus;
  IM_OBCM_TyreStatus_t    Tpms_RRTyrePrStatus;
  IM_OBCM_TyrePrUnit_t    TyrePrUnit;
  IM_OBCM_TyreStatus_t    Tpms_FLTyreTempStatus;
  IM_OBCM_TyreStatus_t    Tpms_FRTyreTempStatus;
  IM_OBCM_TyreStatus_t    Tpms_RLTyreTempStatus;
  IM_OBCM_TyreStatus_t    Tpms_RRTyreTempStatus;
  IM_OBCM_TyreTempUnit_t  TyreTempUnit;
  IM_OBCM_TyreStatus_t    Tpms_FLTyreVoltageStatus;
  IM_OBCM_TyreStatus_t    Tpms_FRTyreVoltageStatus;
  IM_OBCM_TyreStatus_t    Tpms_RLTyreVoltageStatus;
  IM_OBCM_TyreStatus_t    Tpms_RRTyreVoltageStatus;
  UINT8                   TpmsOff;
  UINT8                   TpmsFunctionEnable;
  UINT8                   Ret_PA_ObcmTPMSValue;
  IM_FrsAlodStatus_t      FrsAlodStatus;
  IM_FcsAladType_t        FcsAladType;
  IM_FcsAladStatus_t      FcsAladStatus;
  UINT8                   Ret_PA_Obcm_AdasStatus;
} IMCP_McuSend20ms_G4M_t;

#endif

/* Structure :IMCP_McuSend20ms_G3M_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend20ms_G3M_t
#define Rte_TypeDef_IMCP_McuSend20ms_G3M_t
typedef struct
{
  IM_UTLM_SwitchAndType_t      SLIFSwitch_WarningType;
  IM_UTLM_MenuDisplayStatus_t  SLIFStatus;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_UTLM_SLIFSetting : 4;
  UINT8  SLIFFunctionEnable      : 4;
#else
  UINT8  SLIFFunctionEnable      : 4; /*  0 */
  UINT8  Ret_PA_UTLM_SLIFSetting : 4; /*  4 */
#endif
  IM_UTLM_CruiseMode_t         CruiseMode;
  IM_UTLM_MenuDisplayStatus_t  MenuDisplayStatus;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_UTLM_ACCSetting   : 4;
  UINT8  ACCSettingFunctionEnable : 4;
#else
  UINT8  ACCSettingFunctionEnable : 4; /*  0 */
  UINT8  Ret_PA_UTLM_ACCSetting   : 4; /*  4 */
#endif
  IM_UTLM_MenuStatus_t         LIMSelect;
  UINT8                        Ret_PA_UTLM_LIMSelect;
  IM_UTLM_MenuStatus_t         OverSpeedEnable;
  UINT8                        OverSpeedThreshold;
  UINT8                        Ret_PA_UTLM_OverSpeedSelect;
  UINT8                        DrivingModeRelated;
  IM_UTLM_PrivateCustomMode_t  PrivateCustomMode;
  UINT8                        Ret_PA_UTLM_EPSModeSelect;
  IM_UTLM_OBCMenustatus_t      TripMeter;
  IM_UTLM_OBCMenustatus_t      AverFuelConsumption;
  IM_UTLM_MenuStatus_t         SingleFuelConsumption;
  IM_UTLM_MenuStatus_t         InsFuelConsumption;
  IM_UTLM_MenuStatus_t         DistanceToEmpty;
  IM_UTLM_OBCMenustatus_t      TravelTime;
  IM_UTLM_OBCMenustatus_t      AverageVehicleSpeed;
  IM_UTLM_MenuStatus_t         TyreCondition;
  IM_UTLM_MenuStatus_t         SingleTrip;
  IM_UTLM_PageId_t             TripSelectedPageId;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_PA_TripComputerSetting : 4;
  UINT8  ResetResultFB              : 4;
#else
  UINT8  ResetResultFB              : 4; /*  0 */
  UINT8  Ret_PA_TripComputerSetting : 4; /*  4 */
#endif
  IM_UTLM_MenuStatus_t      SpeedLimitSelect;
  IM_UTLM_MenuStatus_t      SpeedLimitAlertSelect;
  UINT8                     Ret_PA_UTLM_SpeedLimAlert;
  IM_UTLM_VolumeMode_t      VolumeMode;
  UINT8                     Ret_PA_UTLM_AlertVolume;
  IM_Language_t             Language;
  IM_OBCM_AVSUnit_t         SpeedUnit;
  IM_TemperatureUnit_t      TemperatureUnit;
  IM_OBCM_TyrePrUnit_t      TyrePreUnit;
  IM_OBCM_FCUnit_t          FuelConsumUnit;
  UINT8                     Ret_PA_UTLM_UnitSelect;
  IM_TimeFormat_t           TimeFormat;
  UINT8                     Ret_PA_UTLM_ClockSelect;
  UINT16                    DrivingTime;
  IM_OBCM_FatigueWarning_t  FatigueWarning;
  UINT8                     Ret_PA_OBCM_DispFatigueDriving;
  UINT8                     TDLM_Hour;
  UINT8                     TDLM_Minute;
  UINT8                     TDLM_Second;
  UINT8                     TDLM_TimeFormat;
  Data_Availability_t       TDLM_Availability;
  UINT8                     Ret_PA_Tdlm_Time;
  UINT8                     TDLM_Year;
  UINT8                     TDLM_Month;
  UINT8                     TDLM_Day;
  Data_Availability_t       TDLM_DateAvailability;
  UINT8                     Ret_PA_Tdlm_Date;
  UINT8                     SetSpeed;
  IM_Obcm_Status_t          CC_Status;
  Data_Availability_t       CC_Validity;
  UINT8                     CC_FunctionEnable;
  UINT8                     Ret_PA_Obcm_CCDispSetSpeed;
} IMCP_McuSend20ms_G3M_t;

#endif

/* Structure :IMCP_McuSend50ms_G1F_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend50ms_G1F_t
#define Rte_TypeDef_IMCP_McuSend50ms_G1F_t
typedef struct
{
  SINT16  Atim_TemperatureInCels;
  SINT16  Atim_TemperatureInFah;
  UINT8   Atim_SymbolForCels; /* 0-Positive;1-Negative */
  UINT8   Atim_SymbolForFah; /* 0-Positive;1-Negative */
  UINT8   Atim_TemperatureUnits; /* 0-Cels;1-Fah */
  UINT8   Atim_DataFactor; /* Display=Temperature/DataFactor */
  UINT8   Atim_SubState; /* Current Display Sub-State */
  UINT8   Ret_PA_ATIM_TemperatureData;
  UINT8   WakePKCounter;
  UINT8   Ret_PA_WakePKCounter;
  UINT8   Drvm_Transmission;
  UINT8   Drvm_TargetModeReq;
  UINT8   Drvm_SwitchModeStsDisp;
  UINT8   Ret_PA_DRVM_DriveModeLcdDisp;
  UINT16  SYSM_Identity;
  UINT8   SYSM_Flags;
  UINT8   SYSM_TotalCpuload; /* total cpuload, 1% precision */
  UINT32  SYSM_CpuloadMax; /* individual cpuload, 0.001 precision */
  UINT32  SYSM_Cpuload; /* individual cpuload, 0.001 precision */
  UINT16  SYSM_JitterMax; /* task jitter, 0.01ms */
  UINT16  SYSM_JitterCur; /* task jitter, 0.01ms */
  UINT16  SYSM_StackUsage;
  UINT8   Ret_PA_SYSM_DiagOutputInfo;
} IMCP_McuSend50ms_G1F_t;

#endif

/* Structure :IMCP_GpuSend50ms_G1F_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_GpuSend50ms_G1F_t
#define Rte_TypeDef_IMCP_GpuSend50ms_G1F_t
typedef struct
{
  UINT8  Drvm_SteeringModeSelection;
  UINT8  Drvm_PrivateModeSelection;
  UINT8  Ret_PA_DRVM_EpsModeSetting;
} IMCP_GpuSend50ms_G1F_t;

#endif

/* Structure :IMCP_GpuSend10ms_G2F_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_GpuSend10ms_G2F_t
#define Rte_TypeDef_IMCP_GpuSend10ms_G2F_t
typedef struct
{
  UINT8                        DspmState;
  UINT8                        Ret_CB_DspmGlobalStateCB;
  IM_UTLM_MenuStatus_t         LIMSelect;
  Data_Availability_t          LIMDataValidity;
  UINT8                        Ret_CB_UTLM_LIMSelect;
  IM_UTLM_MenuStatus_t         OverSpeedEnable;
  UINT8                        OverSpeedThreshold;
  Data_Availability_t          OverSpeedDataValidity;
  UINT8                        Ret_CB_UTLM_OverSpeedSelect;
  UINT8                        DrivingModeRelated;
  IM_UTLM_PrivateCustomMode_t  PrivateCustomMode;
  Data_Availability_t          EPSModeDataValidity;
  UINT8                        Ret_CB_UTLM_EPSModeSelect;
  IM_UTLM_OBCMenustatus_t      TripMeter;
  IM_UTLM_OBCMenustatus_t      AverFuelConsumption;
  IM_UTLM_MenuStatus_t         SingleFuelConsumption;
  IM_UTLM_MenuStatus_t         InsFuelConsumption;
  IM_UTLM_MenuStatus_t         DistanceToEmpty;
  IM_UTLM_OBCMenustatus_t      TravelTime;
  IM_UTLM_OBCMenustatus_t      AverageVehicleSpeed;
  IM_UTLM_MenuStatus_t         TyreCondition;
  IM_UTLM_MenuStatus_t         SingleTrip;
  Data_Availability_t          TripComputerDataValidity;
  IM_UTLM_PageId_t             TripSelectedPageId;
#ifdef FIRST_BIT_IS_HIGHEST
  UINT8  Ret_CB_TripComputerSelect : 4;
  UINT8  MaintenanceClear          : 2;
  UINT8  AllClear                  : 2;
#else
  UINT8  AllClear                  : 2; /*  0 */
  UINT8  MaintenanceClear          : 2; /*  2 */
  UINT8  Ret_CB_TripComputerSelect : 4; /*  4 */
#endif
  IM_UTLM_MenuStatus_t  SpeedLimitSelect;
  IM_UTLM_MenuStatus_t  SpeedLimitAlertSelect;
  UINT8                 SpeedLimitDataValidity;
  UINT8                 Ret_CB_UTLM_SpeedLimAlert;
  IM_UTLM_VolumeMode_t  VolumeMode;
  UINT8                 AlerVolumeDataValidity;
  UINT8                 Ret_CB_UTLM_AlertVolume;
  IM_Language_t         Language;
  IM_OBCM_AVSUnit_t     SpeedUnit;
  IM_TemperatureUnit_t  TemperatureUnit;
  IM_OBCM_TyrePrUnit_t  TyrePreUnit;
  IM_OBCM_FCUnit_t      FuelConsumUnit;
  Data_Availability_t   UnitDataValidity;
  UINT8                 Ret_CB_UTLM_UnitSelect;
  IM_TimeFormat_t       TimeFormat;
  Data_Availability_t   ClockDataValidity;
  UINT8                 Ret_CB_UTLM_ClockSelect;
} IMCP_GpuSend10ms_G2F_t;

#endif

/* Structure :IMCP_GpuSend10ms_G3F_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_GpuSend10ms_G3F_t
#define Rte_TypeDef_IMCP_GpuSend10ms_G3F_t
typedef struct
{
  IM_UTLM_MenuStatus_t  DisplaySwitch;
  IM_Utlm_NavDisMode_t  NavDisMode;
  Data_Availability_t   NavigationDataValidity;
  UINT8                 Ret_CB_Utlm_NavigationSelec;
  IM_UTLM_MenuStatus_t  LaneChangeAssistSwitch;
  IM_UTLM_MenuStatus_t  LaneChangeAssistSoundSwitch;
  IM_UTLM_MenuStatus_t  RearCollisionWarningSwitch;
  IM_UTLM_MenuStatus_t  DoorOpenWarningSwitch;
  IM_UTLM_MenuStatus_t  RearCrossTrafficAlertSwitch;
  Data_Availability_t   DataValidity;
  UINT8                 Ret_CB_Utlm_RearSideRadarSelect;
  UINT8                 SpeedValue;
  SPDM_SpeedUnit_t      SpeedUnit;
  Data_Availability_t   SpeedValidity;
  UINT8                 Ret_CB_Spdm_RealDisSpeed;
} IMCP_GpuSend10ms_G3F_t;

#endif

/* Structure :IMCP_McuSend10ms_G1L_t */
/* Description : */
#ifndef Rte_TypeDef_IMCP_McuSend10ms_G1L_t
#define Rte_TypeDef_IMCP_McuSend10ms_G1L_t
typedef struct
{
  IM_ControlParameter_t  LCDIOControlParameter;
  IM_Uds_ControlState_t  LCDControlState;
  UINT8                  Ret_PA_UDS_LCDTestMessage;
} IMCP_McuSend10ms_G1L_t;

#endif

#endif /* __IMICRO_STRUCTS_H_INCLUDED__ */
