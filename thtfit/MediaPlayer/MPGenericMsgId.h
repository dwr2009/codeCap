#ifndef	_GENERIC_MSG_ID_H_
#define	_GENERIC_MSG_ID_H_

#define	MP_MSG_PLAYBACK_START				300+1
#define	MP_MSG_PLAYBACK_EOS					300+2
#define	MP_MSG_DISP_OUT_UPDATE				300+3
#define	MP_MSG_VIDEO_OUTPUT_MODE_IS_SET		300+4
#ifdef ENABLE_DTV
#define	MP_MSG_STARTSCAN					300+5
#define	MP_MSG_SCANPROGRESS					300+6
#define	MP_MSG_SCANDONE						300+7
#define	MP_MSG_PROPMT_MSG_DONE				300+8
#endif
#define	MP_MSG_PLAYBCAK_PLAYNEXTFILE		300+9
#define	MP_MSG_PLAYBCAK_TOTUNERSCAN			300+10
#define	MP_MSG_PLAYBACK_READY_TO_PLAY   	300+11/*added by lxj 2013-1-4*/

#define	MP_SIGNAL_WakeupMainThread			"WakeupMainThread"
#define	MP_SIGNAL_VidOutModeChanged			"VidOutModeChanged"
#define	MP_SIGNAL_PlaybackStart				"PlaybackStart"
#define	MP_SIGNAL_PlaybackEos				"PlaybackEos"
#define	MP_SIGNAL_PlaybackEos2				"PlaybackEos2"
#define	MP_SIGNAL_PlayNextFile				"PlayNextFile"
#define	MP_SIGNAL_ShowBarForScan			"Tuner_StartScan"
#define	MP_SIGNAL_NotifyScanProgress		"NotifyScanProgress"
#define	MP_SIGNAL_HideBarForScan			"Tuner_StopScan"
#define	MP_SIGNAL_TuPlayerPropmtMsg			"NotifyTuPlayerPropmtMsg"

#endif	//_GENERIC_MSG_ID_H_

