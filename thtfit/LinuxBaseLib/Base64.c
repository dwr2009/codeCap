#include	"Base64.h"
#include	<BaseErrDef.h>

static const char g_szBase64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char GetB64Char(int index)
{
	if (index >= 0 && index < 64)
		return g_szBase64Table[index];
	
	return '=';
}

int GetB64Index(char ch)
{
	int index = -1;
	
	if (ch >= 'A' && ch <= 'Z')
	{
		index = ch - 'A';
	}
	else if (ch >= 'a' && ch <= 'z')
	{
		index = ch - 'a' + 26;
	}
	else if (ch >= '0' && ch <= '9')
	{
		index = ch - '0' + 52;
	}
	else if (ch == '+')
	{
		index = 62;
	}
	else if (ch == '/')
	{
		index = 63;
	}

	return index;
}

INT Base64Encode(CONST LPBYTE pInBuf, INT iInSize, LPBYTE pOutBuf, INT * piOutSize)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		INT nLen = 0, nId; 
		LPBYTE pSrc = pInBuf, pDst = pOutBuf;
		
		//Check out output buffer size
		if(((iInSize+2) / 3 * 4) > (*piOutSize))
		{
			iOutRet = ERROR_NOT_ENOUGH_BUFFER;
			break;
		}
		//编码
		for (nId = 0; nId < iInSize - 3; nId += 3)
		{
			DWORD ulTmp;
			int b0, b1, b2, b3;
			
			memcpy(&ulTmp, pSrc, 4);
			
			b0 = GetB64Char((BYTE0(ulTmp) >> 2) & 0x3F); 
			b1 = GetB64Char((BYTE0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F); 
			b2 = GetB64Char((B1(ulTmp) << 4 >> 2 | B2(ulTmp) >> 6) & 0x3F); 
			b3 = GetB64Char((B2(ulTmp) << 2 >> 2) & 0x3F); 
			*((LPDWORD)pDst) = (b0 | b1 << 8 | b2 << 16 | b3 << 24);
			nLen += 4;
			pDst  += 4; 
			pSrc += 3;
		}
		
		// 处理最后余下的不足3字节的饿数据
		if (nId < iInSize)
		{
			int j = 0;
			int rest = iInSize - nId;
			DWORD ulTmp = 0;
			//for(j = 0; j < rest; ++j)
			for(j = 0; j < rest; j++)
			{
				*(((unsigned char*)&ulTmp) + j) = *pSrc++;
			}
			
			pDst[0] = GetB64Char((BYTE0(ulTmp) >> 2) & 0x3F); 
			pDst[1] = GetB64Char((BYTE0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F); 
			pDst[2] = rest > 1 ? GetB64Char((B1(ulTmp) << 4 >> 2 | B2(ulTmp) >> 6) & 0x3F) : '='; 
			pDst[3] = rest > 2 ? GetB64Char((B2(ulTmp) << 2 >> 2) & 0x3F) : '='; 
			pDst += 4; 
			nLen += 4;
		}
		
		*piOutSize = nLen;	
	}while(FALSE);

	return iOutRet;
}

//Base64解码
INT Base64Decode(CONST LPBYTE pInBuf, INT iInSize, LPBYTE pOutBuf, INT * piOutSize)
{
	INT iOutRet = ERROR_SUCCESS;
	int nLen = 0, nId = 0; 
	LPBYTE pSrc = pInBuf;	
	LPBYTE pDst = pOutBuf;

	do
	{
		if(NULL == piOutSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 == iInSize)
		{
			*piOutSize = 0;
			break;
		}
		if(4 > iInSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		//Check output buffer size
		if(((iInSize+3)*3/4) > (*piOutSize))
		{
			iOutRet = ERROR_NOT_ENOUGH_BUFFER;
			break;
		}

		for (nId = 0; nId < iInSize - 4; nId += 4)
		{
			DWORD ulTmp;
			int b0, b1, b2;
			
			memcpy(&ulTmp, pSrc, 4);
			
			b0 = (GetB64Index((char)BYTE0(ulTmp)) << 2 | 
				GetB64Index((char)B1(ulTmp)) << 2 >> 6) & 0xFF;
			b1 = (GetB64Index((char)B1(ulTmp)) << 4 | 
				GetB64Index((char)B2(ulTmp)) << 2 >> 4) & 0xFF;
			b2 = (GetB64Index((char)B2(ulTmp)) << 6 | 
				GetB64Index((char)B3(ulTmp)) << 2 >> 2) & 0xFF;

			ulTmp = (b0 | b1 << 8 | b2 << 16);
			memcpy(pDst, &ulTmp, 3);
			pSrc  += 4; 
			pDst += 3;
			nLen += 3;
		}

		// 处理最后余下的不足4字节的饿数据
		if (nId < iInSize)
		{
			int j = 0, b0 = 0;
			int rest = iInSize - nId;
			DWORD ulTmp = 0;
			//for(j = 0; j < rest; ++j)
			for(j = 0; j < rest; j++)
			{
				*(((unsigned char*)&ulTmp) + j) = *pSrc++;
			}
			
			b0 = (GetB64Index((char)BYTE0(ulTmp)) << 2 | 
				GetB64Index((char)B1(ulTmp)) << 2 >> 6) & 0xFF;
			*pDst++ = b0;
			nLen++;

			if ('=' != B1(ulTmp) && '=' != B2(ulTmp))
			{
				int b1 = (GetB64Index((char)B1(ulTmp)) << 4 | 
					GetB64Index((char)B2(ulTmp)) << 2 >> 4) & 0xFF;
				*pDst++ = b1;
				nLen++;
			}
			
			if ('=' != B2(ulTmp) && '=' != B3(ulTmp))
			{
				int b2 = (GetB64Index((char)B2(ulTmp)) << 6 | 
					GetB64Index((char)B3(ulTmp)) << 2 >> 2) & 0xFF;
				*pDst++ = b2;
				nLen++;
			}
		}
		*piOutSize = nLen;
	}while(FALSE);

	return ERROR_SUCCESS;
}

