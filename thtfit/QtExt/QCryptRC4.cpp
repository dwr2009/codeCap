#include "Api/QCryptRC4.h"
#include <BaseErrDef.h>
#include <EncDecrypt_RC4.h>

QCryptRC4::QCryptRC4()
{
}

QCryptRC4::~QCryptRC4()
{
}

INT_t QCryptRC4::EncDecrypt(const QByteArray & InData, OUT QByteArray & OutData, const QString & strKey)
{
	INT_t iOutRet = ERROR_SUCCESS;
	INT_t iBytesConverted = 0;

	do
	{
		OutData.resize(InData.size());
		iOutRet = EncDecrypt_RC4((PBYTE)(InData.data()), InData.size(), (PBYTE)(OutData.data()), OutData.size(), &iBytesConverted, 
			strKey.toUtf8());
		if(OutData.size() != iBytesConverted)
		{
			iOutRet = ERR_CONVERT_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

