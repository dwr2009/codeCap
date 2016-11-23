#include <GuiOsd2Ctrl.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <QFontDatabase>
#include <QPainter>
#include <QDebug>

QGuiOsd2Ctrl::QGuiOsd2Ctrl()
{
	m_fontPixelSize = 24;
	m_TextClrR = m_TextClrG = m_TextClrB = m_TextClrA = 255;
}

QGuiOsd2Ctrl::~QGuiOsd2Ctrl()
{
	releaseGuiOsd2();
}

INT_t QGuiOsd2Ctrl::createGuiOsd2()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	UINT8_t * pSurfaceLumaVirtAddr = NULL;
	UINT32_t surfaceWidth, surfaceHeight;

#if defined(__mips__)	
	if(m_GuiOsd2FbCtrl_sp.isNull())
	{
		m_GuiOsd2FbCtrl_sp = QSharedPointer <CGuiOsd2FrameBufferCtrl> (new CGuiOsd2FrameBufferCtrl);
		if(m_GuiOsd2FbCtrl_sp.isNull())
		{
			iOutRet = ERR_OUT_OF_MEMORY;
			goto EXIT_PROC;
		}
	}

	iRet = m_GuiOsd2FbCtrl_sp->getSurfaceInfo(OUT pSurfaceLumaVirtAddr, OUT surfaceWidth, OUT surfaceHeight);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
#endif

	if(FALSE == m_Osd2FbImg_sp.isNull())
	{
		if(pSurfaceLumaVirtAddr != m_Osd2FbImg_sp->bits() || (int)surfaceWidth != m_Osd2FbImg_sp->width() ||
			(int)surfaceHeight != m_Osd2FbImg_sp->height())
		{
			m_Osd2FbImg_sp.clear();
		}
	}
	if(m_Osd2FbImg_sp.isNull())
	{
		m_Osd2FbImg_sp = QSharedPointer <QImage> (new QImage(pSurfaceLumaVirtAddr, surfaceWidth, surfaceHeight, QImage::Format_ARGB32));
		if(m_Osd2FbImg_sp.isNull())
		{
			iOutRet = ERR_OUT_OF_MEMORY;
			goto EXIT_PROC;
		}
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QGuiOsd2Ctrl::releaseGuiOsd2()
{
	INT_t iOutRet = ERR_SUCCESS;

	//LOG_BLINE("QGuiOsd2Ctrl::releaseGuiOsd2\n");

	m_Osd2FbImg_sp.clear();

#if defined(__mips__)	
	m_GuiOsd2FbCtrl_sp.clear();
#endif

	return iOutRet;
}

INT_t QGuiOsd2Ctrl::getScreenSize(OUT int & scrWidth, OUT int & scrHeight)
{
	INT_t iOutRet = ERR_SUCCESS;
	bool bRet = false;

	if(m_Osd2FbImg_sp.isNull())
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	scrWidth = m_Osd2FbImg_sp->width();
	scrHeight = m_Osd2FbImg_sp->height();
	
EXIT_PROC:	
	return iOutRet;
}

INT_t QGuiOsd2Ctrl::ClearScreen()
{
	INT_t iOutRet = ERR_SUCCESS;
	bool bRet;

	//LOG_BLINE("QGuiOsd2Ctrl::ClearScreen\n");

	if(m_Osd2FbImg_sp.isNull())
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	bRet = m_Painter.begin(&(*m_Osd2FbImg_sp));
	if(false == bRet)
	{
		iOutRet = ERR_PAINTER_BEGIN_FAIL;
		goto EXIT_PROC;
	}

	m_Painter.setCompositionMode(QPainter::CompositionMode_Source);
	m_Painter.fillRect(0, 0, m_Osd2FbImg_sp->width(), m_Osd2FbImg_sp->height(), QColor(0, 0, 0, 0));

	bRet = m_Painter.end();
	if(false == bRet)
	{
		LOG_BLINE("Painter.end err\n");
	}
	
EXIT_PROC:	
	return iOutRet;
}

INT_t QGuiOsd2Ctrl::ClearRect(int x, int y, int w, int h)
{
	INT_t iOutRet = ERR_SUCCESS;
	bool bRet;

	if(m_Osd2FbImg_sp.isNull())
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	bRet = m_Painter.begin(&(*m_Osd2FbImg_sp));
	if(false == bRet)
	{
		iOutRet = ERR_PAINTER_BEGIN_FAIL;
		goto EXIT_PROC;
	}

	m_Painter.setCompositionMode(QPainter::CompositionMode_Source);
	m_Painter.fillRect(x, y, w, h, QColor(0, 0, 0, 0));

	bRet = m_Painter.end();
	if(false == bRet)
	{
		LOG_BLINE("Painter.end err\n");
	}
	
EXIT_PROC:	
	return iOutRet;
}

INT_t QGuiOsd2Ctrl::drawTextAlignCenter(int x, int y, int w, int h, const QString & text)
{
	INT_t iOutRet = ERR_SUCCESS;
	bool bRet;
	QFont font;

	if(m_Osd2FbImg_sp.isNull())
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	bRet = m_Painter.begin(&(*m_Osd2FbImg_sp));
	if(false == bRet)
	{
		iOutRet = ERR_PAINTER_BEGIN_FAIL;
		goto EXIT_PROC;
	}

	m_Painter.setCompositionMode(QPainter::CompositionMode_Source);
	m_Painter.setPen(QColor(m_TextClrR,m_TextClrG,m_TextClrB,m_TextClrA));
	if ( m_FontName.isNull())
	{
		font = m_Painter.font();	
	}
	else
	{
		font=QFont(m_FontName);
	}
	font.setPixelSize(m_fontPixelSize);
	m_Painter.setFont(font);
	m_Painter.drawText(QRect(x,y,w,h), Qt::AlignCenter, text);

	bRet = m_Painter.end();
	if(false == bRet)
	{
		LOG_BLINE("Painter.end err\n");
	}
	
EXIT_PROC:	
	return iOutRet;
}

INT_t QGuiOsd2Ctrl::setFontSize(int pixelSize)
{
	INT_t iOutRet = ERR_SUCCESS;

 	m_fontPixelSize = pixelSize;
 	
	return iOutRet;
}

INT_t QGuiOsd2Ctrl::setTextColor(int r, int g, int b, int alpha)
{
	m_TextClrR = r;
	m_TextClrG = g;
	m_TextClrB = b;
	m_TextClrA = alpha;
	
	return ERR_SUCCESS;
}

INT_t QGuiOsd2Ctrl::fillRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	INT_t iOutRet = ERR_SUCCESS;
	bool bRet;

	if(m_Osd2FbImg_sp.isNull())
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	bRet = m_Painter.begin(&(*m_Osd2FbImg_sp));
	if(false == bRet)
	{
		iOutRet = ERR_PAINTER_BEGIN_FAIL;
		goto EXIT_PROC;
	}

	m_Painter.setCompositionMode(QPainter::CompositionMode_Source);
	m_Painter.fillRect(x, y, w, h, QColor(r,g,b,a));

	bRet = m_Painter.end();
	if(false == bRet)
	{
		LOG_BLINE("Painter.end err\n");
	}
	
EXIT_PROC:	
	return iOutRet;
}

INT_t QGuiOsd2Ctrl::drawImage(const QString & strFilePath, int scrX, int scrY)
{
	INT_t iOutRet = ERR_SUCCESS;
	bool bRet;

	QPixmap pixmap(strFilePath);

	if(m_Osd2FbImg_sp.isNull())
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	bRet = m_Painter.begin(&(*m_Osd2FbImg_sp));
	if(false == bRet)
	{
		iOutRet = ERR_PAINTER_BEGIN_FAIL;
		goto EXIT_PROC;
	}

	m_Painter.setCompositionMode(QPainter::CompositionMode_Source);
	m_Painter.drawPixmap(scrX, scrY, pixmap);

	bRet = m_Painter.end();
	if(false == bRet)
	{
		LOG_BLINE("Painter.end err\n");
	}
	
EXIT_PROC:	
	return iOutRet;
}

INT_t QGuiOsd2Ctrl::drawTiledImage(const QString & strFilePath, int scrX, int scrY, int scrW, int scrH)
{
	INT_t iOutRet = ERR_SUCCESS;
	bool bRet;
	
	QPixmap pixmap(strFilePath);
	
	if(m_Osd2FbImg_sp.isNull())
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	bRet = m_Painter.begin(&(*m_Osd2FbImg_sp));
	if(false == bRet)
	{
		iOutRet = ERR_PAINTER_BEGIN_FAIL;
		goto EXIT_PROC;
	}
	
	pixmap = pixmap.scaled(scrW,scrH);
	m_Painter.setCompositionMode(QPainter::CompositionMode_Source);
	m_Painter.drawTiledPixmap(scrX, scrY, scrW, scrH, pixmap);
	
	bRet = m_Painter.end();
	if(false == bRet)
	{
		LOG_BLINE("Painter.end err\n");
	}
	
EXIT_PROC:	
	return iOutRet;
}

/*---------------------add by xuweiwei 2013-11-13 for Select a different font------------------------------*/
INT_t QGuiOsd2Ctrl::setFontName(const QString& strFontName)
{
	INT_t iOutRet = ERR_SUCCESS;
  	QFontDatabase database;
  	bool bRet = false;

	do
	{
		if(strFontName.trimmed() == "")
		{	
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		foreach(const QString &family, database.families())  
		{  
	      		//qDebug ()<<family;  
	     		if(family == strFontName )
	     		{
					m_FontName=strFontName;
					m_Painter.setCompositionMode(QPainter::CompositionMode_Source);
		         	m_Painter.setFont(QFont(strFontName));
					iOutRet  = ERR_SUCCESS;
				    break;
	     		}
	 			iOutRet = ERROR_INVALID_PARAMETER;
		} 
		
		bRet = m_Painter.end();
		if(false == bRet)
		{
			LOG_BLINE("Painter.end err\n");
		}
	}while(false);

	return iOutRet;
}
/*---------------------------------------end-----------------------------------------------------*/

