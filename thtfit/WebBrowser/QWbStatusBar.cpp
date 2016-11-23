#include "QWbStatusBar.h"
#include <QDesktopWidget>
#include <QApplication>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>

#define	QWB_STATUS_BAR_HEIGHT				30
#define	QWB_STATUS_BAR_BOTTOM_DISTANCE		30

QWbStatusBarItem::QWbStatusBarItem(QWeakPointer <IWbStatusBar> & WbStatusBarIf_wp)
{
	bool bRet;
	
	m_WbStatusBarIf_wp = WbStatusBarIf_wp;
	m_uiItemId = 0;	//invalid
	m_bVisible = FALSE;

	bRet = this->connect(&m_timerTimeoutRemove, SIGNAL(timeout()), this, SLOT(On_TimeoutHide()));
	if(false == bRet)
	{
		LOG_BLINE("ConnSig(TimeoutRemvoe) err\n");
	}
}

QWbStatusBarItem::~QWbStatusBarItem()
{
	INT_t iRet;

	iRet = remove();
	if(ERROR_NOT_FOUND != iRet && ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

INT_t QWbStatusBarItem::setText(const QString & strText, int iTimeoutMs/* = -1*/)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_strText = strText;
		m_bVisible = TRUE;
		if(0 > iTimeoutMs)
		{
			m_timerTimeoutRemove.stop();
		}
		else	// 0 <= iTimeoutMs
		{
			m_timerTimeoutRemove.setSingleShot(TRUE);
			m_timerTimeoutRemove.start(iTimeoutMs);
		}
		QSharedPointer <IWbStatusBar> WbStatusBar_sp = m_WbStatusBarIf_wp.toStrongRef();
		if(FALSE == WbStatusBar_sp.isNull())
		{
			WbStatusBar_sp->On_WbStatusBarItem_Changed(this);
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QWbStatusBarItem::remove()
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		QSharedPointer <IWbStatusBar> WbStatusBar_sp = m_WbStatusBarIf_wp.toStrongRef();
		if(FALSE == WbStatusBar_sp.isNull())
		{
			iOutRet = WbStatusBar_sp->remove(this);
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QWbStatusBarItem::Hide(QWbStatusBarItem * pWbStatusBarItem)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		QSharedPointer <IWbStatusBar> WbStatusBar_sp = m_WbStatusBarIf_wp.toStrongRef();
		if(FALSE == WbStatusBar_sp.isNull())
		{
			iOutRet = WbStatusBar_sp->Hide(pWbStatusBarItem);
		}
	}while(FALSE);

	return iOutRet;
}

void QWbStatusBarItem::On_TimeoutHide()
{
	INT_t iRet;

	iRet = Hide(this);
	if(ERROR_NOT_FOUND != iRet && ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

QWbStatusBar::QWbStatusBar() : m_lblStatusText(this)
{
	m_uiLastItemId = 0;	//invalid
	//set window flags
	Qt::WindowFlags	curWinFlags = windowFlags();	
	setWindowFlags(curWinFlags | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	//focus policy
	setFocusPolicy(Qt::NoFocus);
	//set window size and position
	QDesktopWidget * pDesktopWidget = QApplication::desktop();
	if(pDesktopWidget)
	{
		QRect AvailableDestiopGeometry = pDesktopWidget->availableGeometry();
		QRect rcThisWidget = AvailableDestiopGeometry;
		rcThisWidget.adjust(0, (AvailableDestiopGeometry.height() - QWB_STATUS_BAR_HEIGHT - QWB_STATUS_BAR_BOTTOM_DISTANCE), 
			0, -QWB_STATUS_BAR_BOTTOM_DISTANCE);
		setGeometry(rcThisWidget);
	}
	//label size
	m_lblStatusText.resize(size());
	m_lblStatusText.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//label text
	//m_lblStatusText.setText("Hello, world!");
}

QWbStatusBar::~QWbStatusBar()
{
}

void QWbStatusBar::On_WbStatusBarItem_Changed(QWbStatusBarItem * pWbStatusBarItem)
{	
	do
	{
		if(NULL == pWbStatusBarItem)
		{
			break;
		}
		RecalcDisplay();
	}while(FALSE);
}

QSharedPointer <QWbStatusBarItem> QWbStatusBar::RegisterStatusBarItem()
{
	QSharedPointer <QWbStatusBarItem> WbStatusBarItem_sp;
	BOOL_t bFound;
	WbStatusBarItem_WP_LIST::iterator itWbStatusBarItem;

	do
	{
		QWeakPointer<IWbStatusBar> WbStatusBarIf_wp = m_this_wp;
		WbStatusBarItem_sp = QSharedPointer <QWbStatusBarItem> (new QWbStatusBarItem(WbStatusBarIf_wp));
		if(WbStatusBarItem_sp.isNull())
		{
			break;
		}
		bFound = FALSE;
		while(TRUE)
		{
			m_uiLastItemId++;
			if(0 == m_uiLastItemId)
			{
				m_uiLastItemId++;
			}
			for(itWbStatusBarItem = m_lstWbStatusBarItem.begin(); itWbStatusBarItem != m_lstWbStatusBarItem.end(); itWbStatusBarItem++)
			{
				QSharedPointer <QWbStatusBarItem> WbStatusBarItem_sp = *itWbStatusBarItem;
				if(FALSE == WbStatusBarItem_sp.isNull())
				{
					if(m_uiLastItemId == WbStatusBarItem_sp->m_uiItemId)
					{
						bFound = TRUE;
						break;
					}
				}
			}
			if(FALSE == bFound)
			{
				break;
			}
		}
		WbStatusBarItem_sp->m_uiItemId = m_uiLastItemId;
		WbStatusBarItem_sp->m_WbStatusBarIf_wp = m_this_wp;
		m_lstWbStatusBarItem.push_back(WbStatusBarItem_sp);

		size_t VisibleItemsCnt = 0;
		for(itWbStatusBarItem = m_lstWbStatusBarItem.begin(); itWbStatusBarItem != m_lstWbStatusBarItem.end(); itWbStatusBarItem++)
		{
			QSharedPointer <QWbStatusBarItem> WbStatusBarItem_sp = *itWbStatusBarItem;
			if(FALSE == WbStatusBarItem_sp.isNull())
			{
				if(WbStatusBarItem_sp->m_bVisible)
				{
					VisibleItemsCnt++;
				}
			}
		}
		if(0 < VisibleItemsCnt)
		{
			show();
		}
	}while(FALSE);

	return WbStatusBarItem_sp;
}

INT_t QWbStatusBar::remove(QWbStatusBarItem * pWbStatusBarItem)
{
	INT_t iOutRet = ERROR_SUCCESS;
	WbStatusBarItem_WP_LIST::iterator itWbStatusBarItem;
	BOOL_t bFound;

	do
	{
		if(NULL == pWbStatusBarItem)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		bFound = FALSE;
		for(itWbStatusBarItem = m_lstWbStatusBarItem.begin(); itWbStatusBarItem != m_lstWbStatusBarItem.end(); itWbStatusBarItem++)
		{
			QSharedPointer <QWbStatusBarItem> WbStatusBarItem_sp = *itWbStatusBarItem;
			if(FALSE == WbStatusBarItem_sp.isNull())
			{
				if(m_uiLastItemId == WbStatusBarItem_sp->m_uiItemId)
				{
					bFound = TRUE;
					m_lstWbStatusBarItem.erase(itWbStatusBarItem);
					break;
				}
			}
		}
		if(FALSE == bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		RecalcDisplay();
	}while(FALSE);

	size_t VisibleItemsCnt = 0;
	for(itWbStatusBarItem = m_lstWbStatusBarItem.begin(); itWbStatusBarItem != m_lstWbStatusBarItem.end(); itWbStatusBarItem++)
	{
		QSharedPointer <QWbStatusBarItem> WbStatusBarItem_sp = *itWbStatusBarItem;
		if(FALSE == WbStatusBarItem_sp.isNull())
		{
			if(WbStatusBarItem_sp->m_bVisible)
			{
				VisibleItemsCnt++;
			}
		}
	}
	if(0 >= VisibleItemsCnt)
	{
		hide();
	}

	return iOutRet;
}

INT_t QWbStatusBar::Hide(QWbStatusBarItem * pWbStatusBarItem)
{
	INT_t iOutRet = ERROR_SUCCESS;
	WbStatusBarItem_WP_LIST::iterator itWbStatusBarItem;
	BOOL_t bFound, bItemChanged;

	do
	{
		if(NULL == pWbStatusBarItem)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		bFound = FALSE;
		bItemChanged = FALSE;
		for(itWbStatusBarItem = m_lstWbStatusBarItem.begin(); itWbStatusBarItem != m_lstWbStatusBarItem.end(); itWbStatusBarItem++)
		{
			QSharedPointer <QWbStatusBarItem> WbStatusBarItem_sp = *itWbStatusBarItem;
			if(FALSE == WbStatusBarItem_sp.isNull())
			{
				if(m_uiLastItemId == WbStatusBarItem_sp->m_uiItemId)
				{
					bFound = TRUE;
					if(WbStatusBarItem_sp->m_bVisible)
					{
						bItemChanged = TRUE;
					}
					WbStatusBarItem_sp->m_bVisible = FALSE;
					break;
				}
			}
		}
		if(FALSE == bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		if(bItemChanged)
		{
			RecalcDisplay();
		}
	}while(FALSE);

	return iOutRet;
}

void QWbStatusBar::RecalcDisplay()
{
	bool bRescan;
	QString strMultiplexedText;
	size_t VisibleItemsCnt = 0;

	bRescan = TRUE;
	while(bRescan)
	{
		bRescan = FALSE;
		WbStatusBarItem_WP_LIST::iterator itWbStatusBarItem;
		for(itWbStatusBarItem = m_lstWbStatusBarItem.begin(); itWbStatusBarItem != m_lstWbStatusBarItem.end(); itWbStatusBarItem++)
		{
			QSharedPointer <QWbStatusBarItem> WbStatusBarItem_sp = *itWbStatusBarItem;
			if(WbStatusBarItem_sp.isNull())
			{
				bRescan = TRUE;
				m_lstWbStatusBarItem.erase(itWbStatusBarItem);
				break;
			}
			if(WbStatusBarItem_sp->m_bVisible)
			{
				VisibleItemsCnt++;
				if(FALSE == strMultiplexedText.isEmpty())
				{
					strMultiplexedText += ",";
				}
				strMultiplexedText += "[" + WbStatusBarItem_sp->m_strText + "]";
			}
		}
	}
	//label text
	m_lblStatusText.setText(strMultiplexedText);

	if(FALSE == isVisible() && 0 < VisibleItemsCnt)
	{
		show();
		raise();
	}
	else if(isVisible() && 0 >= VisibleItemsCnt)
	{
		hide();
		lower();
	}
}


QInputChannelWin::QInputChannelWin(QWidget * parentObj) : m_lblStatusText(this)
{
	//set window flags
	Qt::WindowFlags	curWinFlags = windowFlags();	
	setWindowFlags(curWinFlags | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	//focus policy
	setFocusPolicy(Qt::NoFocus);
	//set window size and position
	QDesktopWidget * pDesktopWidget = QApplication::desktop();
	if(pDesktopWidget)
	{
		QRect AvailableDestiopGeometry = pDesktopWidget->availableGeometry();
		QRect rcThisWidget = AvailableDestiopGeometry;
		
		rcThisWidget.adjust(20, (AvailableDestiopGeometry.height() - QWB_STATUS_BAR_HEIGHT - QWB_STATUS_BAR_BOTTOM_DISTANCE - 20), 
			-AvailableDestiopGeometry.width() + 120, -QWB_STATUS_BAR_BOTTOM_DISTANCE);

		//<< (AvailableDestiopGeometry.height() - QWB_STATUS_BAR_HEIGHT - QWB_STATUS_BAR_BOTTOM_DISTANCE);
		setGeometry(rcThisWidget);
	}
	//label size
	m_lblStatusText.resize(size());
	m_lblStatusText.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	m_lblStatusText.setFocusPolicy(Qt::NoFocus);

	QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        m_lblStatusText.setFont(font);
	
	bool bRet;
	bRet = this->connect(&m_timerTimeoutHide, SIGNAL(timeout()), this, SLOT(On_TimeoutHide()));
	if(false == bRet)
	{
		LOG_BLINE("ConnSig(TimeoutRemvoe) err\n");
	}
}

QInputChannelWin::~QInputChannelWin()
{

}


INT_t QInputChannelWin::setText(const QString & strText, int iTimeoutMs)
{
	INT_t iOutRet = ERROR_SUCCESS;
	do
	{
		m_strText = strText;
		if(0 > iTimeoutMs)
		{
			m_timerTimeoutHide.stop();
		}
		else	// 0 <= iTimeoutMs
		{
			m_timerTimeoutHide.setSingleShot(TRUE);
			m_timerTimeoutHide.start(iTimeoutMs);
		}

		m_lblStatusText.setText(m_strText);	
		show();
		raise();
	}while(FALSE);

	return iOutRet;
}

void QInputChannelWin::On_TimeoutHide()
{	
	if(m_timerTimeoutHide.isActive())
	{
		m_timerTimeoutHide.stop();
	}
	hide();
	lower();
	emit inputHide_timeOut(m_strText);
}


void QInputChannelWin::keyPressEvent(QKeyEvent * event)
{
	QWidget::keyPressEvent(event);
}


