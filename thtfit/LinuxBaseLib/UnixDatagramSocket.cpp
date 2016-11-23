#include <UnixDatagramSocket.h>
#include <BaseErrDef.h>

CUnixDatagramSocket::CUnixDatagramSocket()
{

}

CUnixDatagramSocket::~CUnixDatagramSocket()
{

}

INT CUnixDatagramSocket::Create(LPCSTR pszPathToBind/* = NULL*/, BOOL_t bAutoGenePath/* = FALSE*/)
{
	INT iOutRet = ERROR_SUCCESS;

	iOutRet = CUnixSocket::Create(SOCK_DGRAM, pszPathToBind, bAutoGenePath);

	return iOutRet;
}

