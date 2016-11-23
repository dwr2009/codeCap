/*Add by xuweiwei 2014-9-17*/

#ifndef _SERIAL_COM_PORT_H_
#define _SERIAL_COM_PORT_H_
#include <QObject>
#include <BaseTypeDef.h>
#include <termios.h>
#include <ByteArray2.h>
#include <Array.h>
#include <WeakPtr.h>
#include <QSocketNotifier>
#include <QSharedPointer>


using namespace CppBase;
#define	MP7XX_MAX_RECEIVE_BUF_SIZE		1024


class QSerialComPort :public QObject
{
	Q_OBJECT
		
public:
	typedef struct
	{
		INT_t iBaudrate;
		QString  SerialParity;
		INT_t iDataBits;
		INT_t iStopBits;
	}SERIAL_PORT_PARAM, *P_SERIAL_PORT_PARAM;

public:
	QSerialComPort();
	virtual ~QSerialComPort();
	VOID SerialPortParamVal_Init(P_SERIAL_PORT_PARAM pSerialPortParamVal);
	virtual int Open(LPCSTR pszSerialComDev);
	virtual void Close();
	virtual INT_t ReadResult(OUT CByteArray2 & resultData);
	virtual INT_t setComParameters(const P_SERIAL_PORT_PARAM serrialComPar = NULL);
	virtual	void  getComParameters(SERIAL_PORT_PARAM serrialComPar);
	virtual INT_t SendCommand(const QString& pszCmd);
	INT_t   SerialPortParamDesc_To_ParamValue(const QString& pszSerialPortParamDesc);
	INT_t   SerialPortParmVal_To_Desc(OUT QString& strSerialPortParamDesc);
	INT_t   getTermIosSpeed(INT_t iBaudrate, OUT speed_t & termBaudrate);

signals:

public:
	WeakPtr <QSerialComPort> m_this_wp;

public slots:
	int readSerialComPortData(int ifd);

private:
	
	int m_iDevFd;
	CByteArray2 m_ReceivedData;
	SERIAL_PORT_PARAM m_SerialPortParam;
	QSharedPointer <QSocketNotifier> m_SerialPortParamNotifier_sp;
};

#endif
