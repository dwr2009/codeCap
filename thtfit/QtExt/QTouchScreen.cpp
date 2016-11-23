#include "Api/QTouchScreen.h"
#include <BaseErrDef.h>
#include <QDir>
#include <QDebug>
#include <linux/input.h>
#include <ErrPrintHelper.h>

#ifdef	SRC_FILE_NAME
#undef	SRC_FILE_NAME
#endif	//SRC_FILE_NAME
#define	SRC_FILE_NAME 		"QTouchScreen.cpp"

INT_t QTouchScreen::ChkDevExistence(OUT BOOL_t & bExist)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		bExist = FALSE;
		
		QDir InputEventListDir("/sys/class/input", "event*", QDir::Name);
		QFileInfoList lstEventFile = InputEventListDir.entryInfoList();
		QFileInfoList::iterator itFileInfo;
		for(itFileInfo = lstEventFile.begin(); itFileInfo != lstEventFile.end(); itFileInfo++)
		{			
			bool bRet;
			QFileInfo & EventFileInfo = (*itFileInfo);
			do
			{
				//ev file
				QString strEventDevCapPath_ev = EventFileInfo.absoluteFilePath() + "/device/capabilities/ev";
				QFile fileEventDevCap_ev(strEventDevCapPath_ev);
				bRet = fileEventDevCap_ev.open(QIODevice::ReadOnly | QIODevice::Text);
				if(false == bRet)
				{
					QLOGFL("Failed to open %s", qPrintable(strEventDevCapPath_ev));
					break;
				}
				QTextStream streamEventDevCap_ev(&fileEventDevCap_ev);
				QString strEvBit = streamEventDevCap_ev.readLine(32);
				quint32 EvBit;
				EvBit = strEvBit.toUInt(NULL, 16);
				//abs file
				QString strEventDevCapPath_abs = EventFileInfo.absoluteFilePath() + "/device/capabilities/abs";
				QFile fileEventDevCap_abs(strEventDevCapPath_abs);
				bRet = fileEventDevCap_abs.open(QIODevice::ReadOnly | QIODevice::Text);
				if(false == bRet)
				{
					QLOGFL("Failed to open %s", qPrintable(strEventDevCapPath_abs));
					break;
				}
				QTextStream streamEventDevCap_abs(&fileEventDevCap_abs);
				QString strAbsBit = streamEventDevCap_abs.readLine(32);
				quint32 AbsBit;
				AbsBit = strAbsBit.toUInt(NULL, 16);
				//Show infos
				//QLOGFL("%s,ev:0x%08x,abs:0x%08x", qPrintable(EventFileInfo.absoluteFilePath()), EvBit, AbsBit);
				if((EvBit & (1 << EV_SYN)) && (EvBit & (1 << EV_KEY)) && (EvBit & (1 << EV_ABS)) &&
					(AbsBit & (1 << ABS_X)) && (AbsBit & (1 << ABS_Y)))
				{
					//Touch screen
					bExist = TRUE;
					break;
				}
			}while(FALSE);
			//not neccessary to continue searching if found one
			if(bExist)
			{
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

