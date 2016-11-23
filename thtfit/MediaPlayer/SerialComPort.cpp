/*Add by xuweiwei 2014-9-17*/
#include "SerialComPort.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <QStringList>
#include "StackBufString.h"
#include <QApplication>
#include <QTextCodec>

QSerialComPort::QSerialComPort()
{
	m_iDevFd = INVALID_FILE_DESCRIPTOR;
	SerialPortParamVal_Init(&m_SerialPortParam);
}


QSerialComPort::~QSerialComPort()
{
	Close();
}

VOID QSerialComPort::SerialPortParamVal_Init(P_SERIAL_PORT_PARAM pSerialPortParamVal)
{
	pSerialPortParamVal->iBaudrate = 9600;
	pSerialPortParamVal->SerialParity = 'N';
	pSerialPortParamVal->iDataBits = 8;
	pSerialPortParamVal->iStopBits = 1;
}

int QSerialComPort::Open(LPCSTR pszSerialComDev)
{
	int iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;
	struct termios newComSettings;

	do
	{
		if(NULL == pszSerialComDev || '\0' == *pszSerialComDev)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		if(INVALID_FILE_DESCRIPTOR != m_iDevFd)
		{
			iOutRet = ERR_ALREADY_OPENED;
			break;
		}

		m_iDevFd = open(pszSerialComDev, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
		if(0 > m_iDevFd)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		iRet = tcgetattr(m_iDevFd, &newComSettings);
		if(0 != iRet)
		{
			iOutRet = ERR_TC_GET_ATTR_FAIL;
			break;
		}
		//local use
		newComSettings.c_cflag &= ~(CRTSCTS);
		newComSettings.c_cflag |= (CLOCAL | CREAD);
		//disable data flow control
		newComSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG | BRKINT | INPCK | ISTRIP | ICRNL | IXON | IXOFF | IXANY);
		newComSettings.c_oflag &= ~(ICANON | ECHO | ECHOE | ISIG | BRKINT | INPCK | ISTRIP | ICRNL | IXON | IXOFF | IXANY);
		newComSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | BRKINT | INPCK | ISTRIP | ICRNL | IXON | IXOFF | IXANY);
		newComSettings.c_cc[VTIME] = 0;
		newComSettings.c_cc[VMIN] = 1;
		//set
		iRet = tcsetattr(m_iDevFd, TCSANOW, &newComSettings);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SET_ATTR_FAIL;
			break;
		}

		m_SerialPortParamNotifier_sp = QSharedPointer <QSocketNotifier> (new QSocketNotifier(m_iDevFd, QSocketNotifier::Read, this));
		if( m_SerialPortParamNotifier_sp.isNull() )
		{
			iOutRet = ERR_OUT_OF_MEMORY;
			break;
		}

		bRet = connect(&(*m_SerialPortParamNotifier_sp), SIGNAL(activated(int)), this, SLOT(readSerialComPortData(int)));
		if( false == bRet )
		{
			iOutRet = ERR_INV_PARAM;
			break;
		}
		
	}while(FALSE);

	return iOutRet;
}

void QSerialComPort::Close()
{
	int iRet;

	if(FALSE == m_SerialPortParamNotifier_sp.isNull())
	{
		m_SerialPortParamNotifier_sp->setEnabled(FALSE);
		iRet = QObject::disconnect(&(*m_SerialPortParamNotifier_sp), SIGNAL(activated(int)), this, SLOT(readSerialComPortData(int)));
		if(FALSE == iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		
		m_SerialPortParamNotifier_sp.clear();
	}
	
	if(INVALID_FILE_DESCRIPTOR != m_iDevFd)
	{
		iRet = close(m_iDevFd);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		m_iDevFd = INVALID_FILE_DESCRIPTOR;
	}
}


int QSerialComPort::readSerialComPortData(int ifd)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BYTE szReceiveBuf[MP7XX_MAX_RECEIVE_BUF_SIZE];

	if(m_iDevFd != ifd)
	{
		iOutRet=ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}
	
	iRet = read(ifd, szReceiveBuf, sizeof(szReceiveBuf));
	if(0 < iRet)
	{
		size_t BytesRead = iRet;
		iRet = m_ReceivedData.Append(szReceiveBuf, BytesRead);
		if(ERROR_SUCCESS == iRet)
		{
			if(MP7XX_MAX_RECEIVE_BUF_SIZE < m_ReceivedData.GetSize())
			{
				iRet = m_ReceivedData.RemoveAt(0, m_ReceivedData.GetSize()-MP7XX_MAX_RECEIVE_BUF_SIZE);
				if(ERROR_SUCCESS == iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	
EXIT_PROC:

	return iOutRet;
}

INT_t QSerialComPort::ReadResult(OUT CByteArray2 & resultData)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		iRet = (resultData = m_ReceivedData);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QSerialComPort::getTermIosSpeed(INT_t iBaudrate, OUT speed_t & termBaudrate)
{
	INT_t iOutRet = ERROR_SUCCESS;
	
	switch(iBaudrate)
	{
	case 110:
		termBaudrate = B110;
		break;
	case 300:
		termBaudrate = B300;
		break;
	case 600:
		termBaudrate = B600;
		break;
	case 1200:
		termBaudrate = B1200;
		break;
	case 2400:
		termBaudrate = B2400;
		break;
	case 4800:
		termBaudrate = B4800;
		break;
	case 9600:
		termBaudrate = B9600;
		break;
	case 19200:
		termBaudrate = B19200;
		break;
	case 38400:
		termBaudrate = B38400;
		break;
	case 57600:
		termBaudrate = B57600;
		break;
	case 115200:
		termBaudrate = B115200;
		break;
	default:
		iOutRet = ERROR_INVALID_PARAMETER;
		break;
	}

	return iOutRet;
}


INT_t QSerialComPort::setComParameters(const P_SERIAL_PORT_PARAM serrialComPar)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	struct termios newComSettings;
	speed_t termBaudrate;
	P_SERIAL_PORT_PARAM iserrialComPar;
	iserrialComPar=serrialComPar;

	do
	{
		if(NULL == iserrialComPar)
		{
			iserrialComPar=&m_SerialPortParam;
		}
		
		if(INVALID_FILE_DESCRIPTOR == m_iDevFd)
		{
			iOutRet = ERR_FILE_NOT_OPENED;
			break;
		}
		
		iRet = tcgetattr(m_iDevFd, &newComSettings);
		if(0 != iRet)
		{
			iOutRet = ERR_TC_GET_ATTR_FAIL;
			break;
		}
		
		//baudrate
		iRet = getTermIosSpeed(iserrialComPar->iBaudrate, OUT termBaudrate);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = cfsetispeed(&newComSettings, termBaudrate);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SET_ATTR_FAIL;
			break;
		}
		
		iRet = cfsetospeed(&newComSettings, termBaudrate);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SET_ATTR_FAIL;
			break;
		}
		
		//parity
		if(iserrialComPar->SerialParity.isNull())
		{
		}
		else if(0 == iserrialComPar->SerialParity.compare("N",Qt::CaseSensitive))
		{
			newComSettings.c_cflag &= ~(PARENB | PARODD);
			newComSettings.c_iflag &= ~INPCK;	//disable parity checking for incoming packat
		}
		else if(0 == iserrialComPar->SerialParity.compare("O",Qt::CaseSensitive))
		{
			newComSettings.c_cflag |= PARENB;
			newComSettings.c_cflag |= PARODD; 
			newComSettings.c_iflag |= INPCK;	//enable parity checking for incoming packat
		}
		else if(0 == iserrialComPar->SerialParity.compare("E",Qt::CaseSensitive))
		{
			newComSettings.c_cflag |= PARENB; 
			newComSettings.c_cflag &= ~PARODD; 
			newComSettings.c_iflag |= INPCK;	//enable parity checking for incoming packat
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		//data bits
		newComSettings.c_cflag &= ~CSIZE;
		if(7 ==  iserrialComPar->iDataBits)
		{
			newComSettings.c_cflag |= CS7;
		}
		else if(8 ==  iserrialComPar->iDataBits)
		{
			newComSettings.c_cflag |= CS8;
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		//stop bits
		if(1 == iserrialComPar->iStopBits)
		{
			newComSettings.c_cflag &= ~CSTOPB;
		}
		else if(2 == iserrialComPar->iStopBits)
		{
			newComSettings.c_cflag |= CSTOPB;
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		//set
		iRet = tcsetattr(m_iDevFd, TCSANOW, &newComSettings);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SET_ATTR_FAIL;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}

void QSerialComPort::getComParameters(SERIAL_PORT_PARAM serrialComPar)
{
	serrialComPar=m_SerialPortParam;
}

INT_t QSerialComPort::SendCommand(const QString& pszCmd)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	int   bytesToSend = pszCmd.length();
	long int iOldFdFlags, iNewFdFlags;
	BOOL_t bNeedRestoreFdFlags = FALSE;

	char*  chCmd;
	QByteArray baCmd = pszCmd.toLatin1();
	chCmd=baCmd.data();

	do
	{
		iOldFdFlags = fcntl(m_iDevFd, F_GETFL);
		if(0 > iOldFdFlags)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
		
		iNewFdFlags = iOldFdFlags & (~(O_NONBLOCK));
		iRet = fcntl(m_iDevFd, F_SETFL, iNewFdFlags);
		if(0 > iRet)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
		bNeedRestoreFdFlags = TRUE;
		
		iRet = write(m_iDevFd, chCmd, bytesToSend);
		if(0 > iRet)
		{
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}
		else if(bytesToSend != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			break;
		}
		m_ReceivedData.RemoveAll();
	}while(FALSE);

	if(bNeedRestoreFdFlags)
	{
		bNeedRestoreFdFlags = FALSE;
		iRet = fcntl(m_iDevFd, F_SETFL, iOldFdFlags);
		if(0 > iRet)
		{
			iOutRet = ERROR_IOCTL_FAILED;
		}
	}
	
	return iOutRet;
}

INT_t QSerialComPort::SerialPortParamDesc_To_ParamValue(const QString& pszSerialPortParamDesc)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QString SerialPortParamDesc=pszSerialPortParamDesc;
	QStringList SerialPtPrList = SerialPortParamDesc.split(',');
	
	do
	{
		if(4 > SerialPtPrList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		m_SerialPortParam.iBaudrate = (SerialPtPrList[0]).toInt();
		m_SerialPortParam.SerialParity = SerialPtPrList[1];
		m_SerialPortParam.iDataBits = SerialPtPrList[2].toInt();
		m_SerialPortParam.iStopBits = SerialPtPrList[3].toInt();
	}while(FALSE);

	return iOutRet;
}

INT_t QSerialComPort::SerialPortParmVal_To_Desc(OUT QString& strSerialPortParamDesc)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(istrSerialPortParamDesc, 128);
	
	do
	{
		iRet = istrSerialPortParamDesc.Format("%d,%s,%d,%d", m_SerialPortParam.iBaudrate,qPrintable(m_SerialPortParam.SerialParity),\
			m_SerialPortParam.iDataBits, m_SerialPortParam.iStopBits);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		strSerialPortParamDesc=QString(istrSerialPortParamDesc);
	}while(FALSE);

	return iOutRet;
}




