#ifndef	_TRANSACTION_MSG_ID_H_
#define	_TRANSACTION_MSG_ID_H_

#define	MSG_CMD_EXIT_THREAD				1
#define	MSG_CMD_setDataSource			2
#define	MSG_CMD_Prepare					3
#define	MSG_CMD_Play					4
#define	MSG_CMD_Stop					5
#define	MSG_CMD_QuitPlay				6
#define	MSG_CMD_setPlayMode				7
#define	MSG_CMD_setVideoPosition		8
#define	MSG_CMD_ChangeVidOutMode		9
#define	MSG_CMD_setPlaySpeedCtrl		10
#define	MSG_CMD_getPlaySpeedCtrl		11
#define	MSG_CMD_DispOnLedScr			12
#define	MSG_CMD_getPlayMode				13
#define	MSG_CMD_setOutputSpdifMode	    14
#define	MSG_CMD_getOutputSpdifMode	    21
#define	MSG_CMD_StartPlay				15/*added by lxj 2013-1-4*/
#define	MSG_CMD_setVideoInputWindowSize	16
#define	MSG_CMD_setShowClosedCaption	17
#define   MSG_CMD_setVideoRotation           18   /*Add by xuweiwei 2014-2-18*/
#define   MSG_CMD_setEnableAudio6Ch       19   /*Add by xuweiwei 2014-3-17*/
#define   MSG_CMD_getEnableAudio6Ch       20
#define	MSG_CMD_scanI2cDev				51
#define	MSG_CMD_setGpioOutputVal		52
#define	MSG_CMD_setHdmi3DTvMode			53
#define	MSG_CMD_getMonitorInfo			54
#define	MSG_CMD_setVidDispAspectRatio	55
#define	MSG_CMD_getVidDispAspectRatio	56

#define	MSG_CMD_getDisplayOutputMode	57
#define	MSG_CMD_setDisplayOutputMode	58

#define	MSG_CMD_getvideoColorSpace		59
#define	MSG_CMD_setvideoColorSpace		60

#ifdef ENABLE_DTV
#define	MSG_CMD_PlayTuner				80
#define	MSG_CMD_TunerScan				81
#define	MSG_CMD_GetSignalStrength 		82
#define	MSG_CMD_ResetDuration			83
#define	MSG_CMD_TunerMode				84
#define	MSG_CMD_ChannelUp				85
#define	MSG_CMD_ChannelDown				86
#define	MSG_CMD_ReLoadProgram			87
#define	MSG_CMD_GetEntryChannelInfo 	88
#define	MSG_CMD_GetFirstChNum			89
#define	MSG_CMD_GetTunerMode			90
#define	MSG_CMD_SetTunerChannelMode 	91
#define	MSG_CMD_GetTunerChannelMode 	92
#endif

//player notification coming from other threads
#define	MsgSignal_PlaybackEos			(201)
#define MsgSignal_VidDecStreamChanged	(202)

#endif	//_TRANSACTION_MSG_ID_H_

