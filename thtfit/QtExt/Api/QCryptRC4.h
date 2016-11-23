#ifndef	_Q_CRYPT_RC4_H_
#define	_Q_CRYPT_RC4_H_

#include <BaseTypeDef.h>
#include <QByteArray>
#include <QString>

class QCryptRC4
{
public:
	QCryptRC4();
	~QCryptRC4();
	static INT_t EncDecrypt(const QByteArray & InData, OUT QByteArray & OutData, const QString & strKey);
};

#endif	//_Q_CRYPT_RC4_H_
