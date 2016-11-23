#include	"UrlHandle.h"
#include	<stdio.h>
#include	<string.h>
#include	"string_ex.h"
#include	<BaseErrDef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

URL_TYPE DetermineUrlType(const char * pszUrl, char * pszOutUrl, INT iOutSize)
{
	URL_TYPE eOutRet = TYPE_URL_UNKNOWN;
	char szFormattedUrl[2048];
	int iUrlLen = 0;

	iUrlLen = strlen(pszUrl);

	do{
		//parameters check
		if(NULL == pszUrl || (pszOutUrl && 0 >= iOutSize))
		{
			break;
		}

		//duplicates a copy to the formatted url buffer
		strncpy(szFormattedUrl, pszUrl, sizeof(szFormattedUrl));
		szFormattedUrl[sizeof(szFormattedUrl) - 1] = '\0';
		
		//TYPE_URL_ASX_PLAY_LIST
		do{
			if(9 >= iUrlLen) {break;}
			
			if((0 == strncasecmp("http://", pszUrl, 7) 
				&& (0 == strncasecmp(".asx", pszUrl+iUrlLen-4, 4))))
			{
				eOutRet = TYPE_URL_ASX_PLAY_LIST;
				break;
			}	

			{
				char pszBuf[512];
				int iLen = 0;
				iLen = sizeof(pszBuf) - 1;
				strncpy(pszBuf, pszUrl, iLen);
				pszBuf[iLen] = '\0';
				StrToUpper(pszBuf);
				if(strstr(pszBuf, ".ASX?"))
				{
					eOutRet = TYPE_URL_ASX_PLAY_LIST;
					break;
				}
			}
		}while(FALSE);
		if(TYPE_URL_ASX_PLAY_LIST == eOutRet) {break;}

		//TYPE_URL_RTSP_LINK_UNKNOWN
		if(0 == strncasecmp(RTSP_URL_PREFIX, pszUrl, strlen(RTSP_URL_PREFIX)))
		{
			eOutRet = TYPE_URL_RTSP_LINK_UNKNOWN;
		}
		if(TYPE_URL_RTSP_LINK_UNKNOWN == eOutRet) {break;}

		//TYPE_URL_MMS_PLAY_LINK
		do{
			if(9 >= iUrlLen) {break;}
			
			if(0 == strncasecmp("mms://", pszUrl, 6) 
				|| 0 == strncasecmp("mmst://", pszUrl, 7))
			{
				eOutRet = TYPE_URL_MMS_PLAY_LINK;
				break;
			}
		}while(FALSE);
		if(TYPE_URL_MMS_PLAY_LINK == eOutRet) {break;}

		//TYPE_URL_UDP_LINK_UNKNOWN
		if(0 == strncasecmp(UDP_URL_PREFIX, pszUrl, strlen(UDP_URL_PREFIX)))
		{
			eOutRet = TYPE_URL_UDP_LINK_UNKNOWN;
		}
		if(TYPE_URL_UDP_LINK_UNKNOWN == eOutRet) {break;}

		// TYPE_URL_HTTP_MEDIA_FILE
		do {
			if (7 < iUrlLen && (0 == strncasecmp("http://", pszUrl, 7))) 
			{
				if ((0 == strncasecmp(".mp3", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".wma", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".wmv", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".asf", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".mpg", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".mpeg", pszUrl+iUrlLen-5, 5))
					|| (0 == strncasecmp(".mp4", pszUrl+iUrlLen-4, 4))		
					|| (0 == strncasecmp(".ts", pszUrl+iUrlLen-3, 3))		
					|| (0 == strncasecmp(".vob", pszUrl+iUrlLen-4, 4))		
					|| (0 == strncasecmp(".avi", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".pls", pszUrl+iUrlLen-4, 4)))
				{
					eOutRet = TYPE_URL_HTTP_MEDIA_FILE;
					break;				
				}
			}
		}while(FALSE);
		if(TYPE_URL_HTTP_MEDIA_FILE == eOutRet) {break;}

		// TYPE_URL_HTTPV_MEDIA_FILE
		do {
			if (8 < iUrlLen && (0 == strncasecmp("httpv://", pszUrl, 8))) {
				if ((0 == strncasecmp(".mp3", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".wma", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".wmv", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".asf", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".mpg", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".mp4", pszUrl+iUrlLen-4, 4))		
					|| (0 == strncasecmp(".ts", pszUrl+iUrlLen-3, 3))		
					|| (0 == strncasecmp(".vob", pszUrl+iUrlLen-4, 4))		
					|| (0 == strncasecmp(".avi", pszUrl+iUrlLen-4, 4))
					)
				{
					eOutRet = TYPE_URL_HTTPV_MEDIA_FILE;
					snprintf(szFormattedUrl, sizeof(szFormattedUrl), "http://%s", &(pszUrl[8]));
					szFormattedUrl[sizeof(szFormattedUrl)-1] = '\0';
					break;				
				}
			}
		}while(FALSE);
		if(TYPE_URL_HTTPV_MEDIA_FILE == eOutRet) {break;}

		// TYPE_URL_HTTPS_MEDIA_FILE
		do {
			if (8 < iUrlLen && (0 == strncasecmp("https://", pszUrl, 8))) {
				if ((0 == strncasecmp(".mp3", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".wma", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".wmv", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".asf", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".mpg", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".mp4", pszUrl+iUrlLen-4, 4))		
					|| (0 == strncasecmp(".ts", pszUrl+iUrlLen-3, 3))		
					|| (0 == strncasecmp(".vob", pszUrl+iUrlLen-4, 4))		
					|| (0 == strncasecmp(".avi", pszUrl+iUrlLen-4, 4))
					)
				{
					eOutRet = TYPE_URL_HTTPS_MEDIA_FILE;
					break;				
				}
			}
		}while(FALSE);
		if(TYPE_URL_HTTPS_MEDIA_FILE == eOutRet) {break;}

		// TYPE_URL_HTTPSV_MEDIA_FILE
		do {
			if (9 < iUrlLen && (0 == strncasecmp("httpsv://", pszUrl, 9))) {
				if ((0 == strncasecmp(".mp3", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".wma", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".wmv", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".asf", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".mpg", pszUrl+iUrlLen-4, 4))
					|| (0 == strncasecmp(".mp4", pszUrl+iUrlLen-4, 4))		
					|| (0 == strncasecmp(".ts", pszUrl+iUrlLen-3, 3))		
					|| (0 == strncasecmp(".vob", pszUrl+iUrlLen-4, 4))		
					|| (0 == strncasecmp(".avi", pszUrl+iUrlLen-4, 4))
					)
				{
					eOutRet = TYPE_URL_HTTPSV_MEDIA_FILE;
					snprintf(szFormattedUrl, sizeof(szFormattedUrl), "https://%s", &(pszUrl[9]));
					szFormattedUrl[sizeof(szFormattedUrl)-1] = '\0';
					break;				
				}
			}
		}while(FALSE);
		if(TYPE_URL_HTTPSV_MEDIA_FILE == eOutRet) {break;}

		//TYPE_URL_HTTP_LINK_UNKNOWN
		do{
			if(7 < iUrlLen && ((0 == strncasecmp("http://", pszUrl, 7))))
			{
				eOutRet = TYPE_URL_HTTP_LINK_UNKNOWN;
				break;				
			}
		}while(FALSE);
		if(TYPE_URL_HTTP_LINK_UNKNOWN == eOutRet) {break;}
		
		//TYPE_URL_HTTPV_LINK_UNKNOWN
		do{
			if(8 < iUrlLen && ((0 == strncasecmp("httpv://", pszUrl, 8))))
			{
				eOutRet = TYPE_URL_HTTPV_LINK_UNKNOWN;
				snprintf(szFormattedUrl, sizeof(szFormattedUrl), "http://%s", &(pszUrl[8]));
				szFormattedUrl[sizeof(szFormattedUrl)-1] = '\0';
				break;				
			}
		}while(FALSE);
		if(TYPE_URL_HTTPV_LINK_UNKNOWN == eOutRet) {break;}
		
		//TYPE_URL_HTTPS_LINK_UNKNOWN
		do{
			if(8 < iUrlLen && ((0 == strncasecmp("https://", pszUrl, 8))))
			{
				eOutRet = TYPE_URL_HTTPS_LINK_UNKNOWN;
				break;				
			}
		}while(FALSE);
		if(TYPE_URL_HTTPS_LINK_UNKNOWN == eOutRet) {break;}
		
		//TYPE_URL_HTTPSV_LINK_UNKNOWN
		do{
			if(9 < iUrlLen && ((0 == strncasecmp("httpsv://", pszUrl, 9))))
			{
				eOutRet = TYPE_URL_HTTPSV_LINK_UNKNOWN;
				snprintf(szFormattedUrl, sizeof(szFormattedUrl), "https://%s", &(pszUrl[9]));
				szFormattedUrl[sizeof(szFormattedUrl)-1] = '\0';
				break;				
			}
		}while(FALSE);
		if(TYPE_URL_HTTPSV_LINK_UNKNOWN == eOutRet) {break;}
	}while(FALSE);

	if (pszOutUrl) {
		strncpy(pszOutUrl, szFormattedUrl, iOutSize);
		pszOutUrl[iOutSize - 1] = '\0';
	}

	return eOutRet;
}

INT UrlToFullPath(LPCSTR pszSrcUrl, LPSTR pszDestUrl, INT iDestBufSize)
{
	INT iOutRet = ERROR_SUCCESS, iSrcLen;

	if(NULL == pszSrcUrl || NULL == pszDestUrl || 0 >= iDestBufSize)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	iSrcLen = strlen(pszSrcUrl);
	if(iSrcLen + 16 > iDestBufSize)
	{
		iOutRet = ERROR_NOT_ENOUGH_BUFFER;
		goto EXIT_PROC;
	}

	if(0 == strncasecmp("http://", pszSrcUrl, 7) || 0 == strncasecmp("https://", pszSrcUrl, 8) ||
		0 == strncasecmp("httpv://", pszSrcUrl, 8) || 0 == strncasecmp("file://", pszSrcUrl, 8))
	{
		strcpy(pszDestUrl, pszSrcUrl);
	}
	else
	{
		struct stat FileStat;

		if(ERROR_SUCCESS == stat(pszSrcUrl, &FileStat) && 
			(S_ISREG(FileStat.st_mode) || S_ISDIR(FileStat.st_mode)))
		{
			if('/' == pszSrcUrl[0])
			{
				snprintf(pszDestUrl, iDestBufSize, "file://%s", pszSrcUrl);
				pszDestUrl[iDestBufSize - 1] = '\0';			
			}
			else
			{
				char szBuf[64];
				LPCSTR pszCurWorkDir = getenv("PWD");
				if(NULL == pszCurWorkDir)
				{
					pszCurWorkDir = getcwd(szBuf, sizeof(szBuf));
					szBuf[sizeof(szBuf)-1] = '\0';
				}
				snprintf(pszDestUrl, iDestBufSize, "file://%s/%s", pszCurWorkDir, pszSrcUrl);
				pszDestUrl[iDestBufSize - 1] = '\0';
			}
		}
		else
		{
			snprintf(pszDestUrl, iDestBufSize, "http://%s", pszSrcUrl);
			pszDestUrl[iDestBufSize - 1] = '\0';
		}
	}

EXIT_PROC:
	
	return iOutRet;
}

