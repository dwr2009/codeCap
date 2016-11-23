#ifndef	_MEDIA_PLAYER_API_H_
#define	_MEDIA_PLAYER_API_H_

#define	HwScaler_Relative_MAX_SIZE_W		(4096)
#define	HwScaler_Relative_MAX_SIZE_H		(4096)

//process exit codes of MediaPlayer
#define	MPEC_ERR_OUT_OF_MEM			1

#define	MP_NexusNxClientName			"MediaPlayer"

#ifdef	__cplusplus
namespace MediaPlayer
{
#endif	//__cplusplus


typedef enum {
	HdmiCecCtrlCmd_ImageViewOn = 1,
	HdmiCecCtrlCmd_Standby = 2,
	HdmiCecCtrlCmd_VolumeUp = 3,
	HdmiCecCtrlCmd_VolumeDown = 4,
	HdmiCecCtrlCmd_VolumeMute = 5,
}HDMI_CEC_CTRL_CMD, *P_HDMI_CEC_CTRL_CMD;

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_MEDIA_PLAYER_API_H_
