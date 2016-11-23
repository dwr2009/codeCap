/* **************************************************** */
/*!
   @file	MN_Tuner.h
   @brief	Tuner Interface for MN_DMD_Driver
   @author	R.Mori
   @date	2011/6/30
   @param
		(c)	Panasonic
   */
/* **************************************************** */

#ifndef MN_TUNER_H
#define MN_TUNER_H

#ifdef __cplusplus
extern "C" {
#endif

/* **************************************************** */
/*  Tuner dependence functions */
/* **************************************************** */
/* these function is defined by MN_DMD_Tuner_(tuner_name).c */
extern DMD_ERROR_t DMD_Tuner_init(void);
extern DMD_ERROR_t DMD_Tuner_set_system(PMXL603_TUNER_MODE_CFG_T pCfgTuParam);
extern DMD_ERROR_t DMD_Tuner_tune(PMXL603_CHAN_TUNE_CFG_T pChannelInfoCfg);
extern DMD_ERROR_t DMD_Tuner_term(void);

#ifdef __cplusplus
}
#endif

#endif
