#if	!defined(_GUI_OSD2_CTRL_H_)
#define	_GUI_OSD2_CTRL_H_

#include <GuiOsd2FrameBufferCtrl.h>
#include <QSharedPointer>
#include <QImage>
#include <QPointer>
#include <QPainter>
#include <QPixmap>
#include <BaseTypeDef.h>

class QGuiOsd2Ctrl
{
public:
	QGuiOsd2Ctrl();
	~QGuiOsd2Ctrl();
	INT_t createGuiOsd2();
	INT_t releaseGuiOsd2();
	INT_t getScreenSize(OUT int & scrWidth, OUT int & scrHeight);
	INT_t ClearScreen();
	INT_t ClearRect(int x, int y, int w, int h);
	INT_t drawTextAlignCenter(int x, int y, int w, int h, const QString & text);
	INT_t setFontSize(int pixelSize);
	INT_t setTextColor(int r, int g, int b, int alpha);
	INT_t fillRect(int x, int y, int w, int h, int r, int g, int b, int a);
	INT_t drawImage(const QString & strFilePath, int scrX, int scrY);
	INT_t drawTiledImage(const QString & strFilePath, int scrX, int scrY, int scrW, int scrH);
       INT_t setFontName(const QString& strFontName);
protected:
#if defined(__mips__)	   
	QSharedPointer <CGuiOsd2FrameBufferCtrl> m_GuiOsd2FbCtrl_sp;
#endif
	QSharedPointer <QImage> m_Osd2FbImg_sp;
	QPainter m_Painter;
	int m_fontPixelSize;
	int m_TextClrR, m_TextClrG, m_TextClrB, m_TextClrA;
       QString m_FontName;
};

#endif	//_GUI_OSD2_CTRL_H_

