#ifndef	_AUDIO_VIDEO_IN_OUT_INFO_H_
#define	_AUDIO_VIDEO_IN_OUT_INFO_H_

#include <BaseTypeDef.h>
#include <MediaPlayerAppDef.h>
#include <QString>

using namespace MediaPlayer;

namespace MP7XX
{

class QAudioVideoInOutInfo
{
public:
	static VOID getVoDesc(CONST MediaPlayer::VIDEO_OUTPUT_MODE eVoMode, QString & strVoDesc);
	static VOID getVoDescForLedDisp(CONST MediaPlayer::VIDEO_OUTPUT_MODE eVoMode, QString & strVoDesc);
};

}

#endif	//_AUDIO_VIDEO_IN_OUT_INFO_H_

