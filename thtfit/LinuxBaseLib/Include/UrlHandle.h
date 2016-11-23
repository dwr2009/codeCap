#ifndef	_URL_HANDLE_H_
#define	_URL_HANDLE_H_

#include <BaseTypeDef.h>

#ifdef	__cplusplus
extern "C"
{
#endif

typedef enum
{
	TYPE_URL_UNKNOWN = 0,
	TYPE_URL_ASX_PLAY_LIST,
	TYPE_URL_PLS_PLAY_LIST,
	TYPE_URL_MMS_PLAY_LINK,
	TYPE_URL_HTTP_MEDIA_FILE,
	TYPE_URL_HTTPV_MEDIA_FILE,
	TYPE_URL_HTTPS_MEDIA_FILE,
	TYPE_URL_HTTPSV_MEDIA_FILE,
	TYPE_URL_HTTP_LINK_UNKNOWN,
	TYPE_URL_HTTPV_LINK_UNKNOWN,
	TYPE_URL_HTTPS_LINK_UNKNOWN,
	TYPE_URL_HTTPSV_LINK_UNKNOWN,
	TYPE_URL_RTSP_LINK_UNKNOWN,
	TYPE_URL_UDP_LINK_UNKNOWN,
}URL_TYPE, *P_URL_TYPE;

#define	RTSP_URL_PREFIX		"rtsp://"
#define	UDP_URL_PREFIX		"udp://"

URL_TYPE DetermineUrlType(const char * pszUrl, char * pszOutUrl, INT iOutSize);
INT UrlToFullPath(LPCSTR pszSrcUrl, LPSTR pszDestUrl, INT iDestBufSize);

#ifdef	__cplusplus
}
#endif

#endif	//_URL_HANDLE_H_

