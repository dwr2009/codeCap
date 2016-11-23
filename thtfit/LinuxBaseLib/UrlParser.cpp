#include <UrlParser.h>
#include <BaseErrDef.h>
#include <string.h>
#include <string_ex.h>
#include <StrOpMacro.h>
#include <stdlib.h>

CUrlParser::CUrlParser()
{
}

CUrlParser::~CUrlParser()
{
}

INT_t CUrlParser::UrlParser_ParseUrl(P_URL_PARSER pThis, LPCSTR pszUrl)
{
	INT_t iOutRet = 0;
	const char * pCurPos = NULL;
	enum URL_SCAN_STAT eUrlScanStat = URL_SCANSTAT_EXPECT_PROTOCOL;
	LPSTR pFldBuf = NULL;
	INT_t iOffInField = 0;

	do{
		if(NULL == pThis || NULL == pszUrl)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		pFldBuf = (char*)malloc(URL_FIELD_BUF_SIZE);
		if(NULL == pFldBuf)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}	
		//initialize the members
		pThis->pszProtocol[0] = '\0';
		pThis->pszUserName[0] = '\0';
		pThis->pszPassword[0] = '\0';
		pThis->pszHostName[0] = '\0';
		pThis->pszHostPort[0] = '\0';
		pThis->pszPath[0] = '\0';
		pCurPos = pszUrl;
		eUrlScanStat = URL_SCANSTAT_EXPECT_PROTOCOL;
		iOffInField = 0;

		while(*pCurPos)
		{
			if(URL_SCANSTAT_EXPECT_PROTOCOL == eUrlScanStat)
			{
				//���Կո�
				if(' ' == *pCurPos)		
				{

				}
				else if(0 == strcmp("://", pCurPos))
				{
					//��Э������
					pThis->pszProtocol[0] = '\0';
				}
				else
				{
					eUrlScanStat = URL_SCANSTAT_IN_PROTOCOL;
					//Э��������ʼ
					iOffInField = 0;
					pFldBuf[iOffInField] = *pCurPos;
					iOffInField++;
				}
			}
			else if(URL_SCANSTAT_IN_PROTOCOL == eUrlScanStat)
			{
				if(0 == strncmp("://", pCurPos, 3))
				{
					//Э����������
					pFldBuf[iOffInField] = '\0';
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszProtocol, pFldBuf);
					iOffInField++;
					pCurPos += 2;
					//�����һλ�õ�'/'֮���Ƿ���@���ţ���ȷ���Ƿ�
					//��Ҫ����û�����
					if('\0' == *(pCurPos+1))
					{
						//����
					}
					else if('/' == *(pCurPos+1))
					{
						//ֱ�ӵ�path
						eUrlScanStat = URL_SCANSTAT_IN_PATH;
					}
					else
					{
						const char * pNextBiasPos = NULL;
						const char * pAtSign = NULL;
						pNextBiasPos = strchr(pCurPos+1, '/');
						if(pNextBiasPos)
						{
							//��б��
							pAtSign = StrChr(pCurPos+1, '@', pNextBiasPos-pCurPos+1);
						}
						else
						{
							//��б��
							pAtSign = strchr(pCurPos+1, '@');
						}
						if(pAtSign)
						{
							//��@, �����û���
							eUrlScanStat = URL_SCANSTAT_EXPECT_USERNAME;
						}
						else
						{
							//��@, ����������
							eUrlScanStat = URL_SCANSTAT_EXPECT_HOSTNAME;
						}
					}
				}
				else if('\0' == *(pCurPos+1))
				{
					//��������
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					pFldBuf[iOffInField] = '\0';		
					iOffInField++;
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszProtocol, pFldBuf);
				}
				else
				{
					//Э����������
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
				}
			}
			else if(URL_SCANSTAT_EXPECT_USERNAME == eUrlScanStat)
			{
				if(' ' == *pCurPos)
				{
					//����ǰ���ո�
				}
				else if(':' == *pCurPos)
				{
					pThis->pszUserName[0] = '\0';
					eUrlScanStat = URL_SCANSTAT_EXPECT_PASSWORD;
				}
				else if('@' == *pCurPos)
				{
					pThis->pszUserName[0] = '\0';
					eUrlScanStat = URL_SCANSTAT_EXPECT_HOSTNAME;
				}
				else
				{
					//���û�������
					eUrlScanStat = URL_SCANSTAT_IN_USERNAME;
					iOffInField = 0;
					pFldBuf[iOffInField] = *pCurPos;
					iOffInField++;
				}
			}
			else if(URL_SCANSTAT_IN_USERNAME == eUrlScanStat)
			{
				if(':' == *pCurPos)
				{
					pFldBuf[iOffInField] = '\0';
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszUserName, pFldBuf);
					eUrlScanStat = URL_SCANSTAT_EXPECT_PASSWORD;
				}
				else if('@' == *pCurPos)
				{
					pFldBuf[iOffInField] = '\0';
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszUserName, pFldBuf);
					
					eUrlScanStat = URL_SCANSTAT_EXPECT_HOSTNAME;
				}
				else if('\0' == *(pCurPos+1))
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					pFldBuf[iOffInField] = '\0';		
					iOffInField++;
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszUserName, pFldBuf);
				}
				else
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
				}
			}
			else if(URL_SCANSTAT_EXPECT_PASSWORD == eUrlScanStat)
			{
				if('@' == *pCurPos)
				{
					pFldBuf[iOffInField] = '\0';
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszPassword, pFldBuf);
					eUrlScanStat = URL_SCANSTAT_EXPECT_HOSTNAME;					
				}
				else if('\0' == *(pCurPos+1))
				{
					pThis->pszPassword[0] = *pCurPos;
					pThis->pszPassword[1] = '\0';
				}
				else
				{
					//����������
					eUrlScanStat = URL_SCANSTAT_IN_PASSWORD;
					iOffInField = 0;
					pFldBuf[iOffInField] = *pCurPos;
					iOffInField++;
				}
			}
			else if(URL_SCANSTAT_IN_PASSWORD == eUrlScanStat)
			{
				if('@' == *pCurPos)
				{
					pFldBuf[iOffInField] = '\0';
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszPassword, pFldBuf);
					eUrlScanStat = URL_SCANSTAT_EXPECT_HOSTNAME;					
				}
				else if('\0' == *(pCurPos+1))
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					pFldBuf[iOffInField] = '\0';		
					iOffInField++;
					SAFE_STRNCPY(pThis->pszPassword, pFldBuf);
				}
				else
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
				}
			}
			else if(URL_SCANSTAT_EXPECT_HOSTNAME == eUrlScanStat)
			{
				if(' ' == *pCurPos)
				{
					//ignore space
				}
				else if('/' == *pCurPos)
				{
					pThis->pszHostName[0] = '\0';
					eUrlScanStat = URL_SCANSTAT_IN_PATH;
				}
				else if(':' == *pCurPos)
				{
					pThis->pszHostName[0] = '\0';
					eUrlScanStat = URL_SCANSTAT_EXPECT_HOSTPORT;
				}
				else
				{
					iOffInField = 0;
					pFldBuf[iOffInField] = *pCurPos;
					iOffInField++;
					eUrlScanStat = URL_SCANSTAT_IN_HOSTNAME;
				}
			}
			else if(URL_SCANSTAT_IN_HOSTNAME == eUrlScanStat)
			{
				if(' ' == *pCurPos)
				{
					//ignore space
				}
				else if(':' == *pCurPos)
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = '\0';
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszHostName, pFldBuf);
					eUrlScanStat = URL_SCANSTAT_EXPECT_HOSTPORT;
				}
				else if('/' == *pCurPos)
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = '\0';
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszHostName, pFldBuf);
					
					iOffInField = 0;
					pFldBuf[iOffInField] = *pCurPos;
					iOffInField++;					
					eUrlScanStat = URL_SCANSTAT_IN_PATH;
				}
				else if('\0' == *(pCurPos+1))
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = '\0';
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszHostName, pFldBuf);
				}
				else
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
				}
			}
			else if(URL_SCANSTAT_EXPECT_HOSTPORT == eUrlScanStat)
			{
				if(' ' == *pCurPos)
				{
					//ignore space
				}
				else if('/' == *pCurPos)
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = '\0';
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszHostPort, pFldBuf);
					eUrlScanStat = URL_SCANSTAT_IN_PATH;
				}
				else if('\0' == *(pCurPos+1))
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = '\0';
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}

					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszHostPort, pFldBuf);
				}
				else
				{
					iOffInField = 0;
					pFldBuf[iOffInField] = *pCurPos;
					iOffInField++;
					eUrlScanStat = URL_SCANSTAT_IN_HOSTPORT;
				}
			}
			else if(URL_SCANSTAT_IN_HOSTPORT == eUrlScanStat)
			{
				if(' ' == *pCurPos)
				{
					//ignore space
				}
				else if('/' == *pCurPos)
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = '\0';
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszHostPort, pFldBuf);
					
					iOffInField = 0;
					pFldBuf[iOffInField] = *pCurPos;
					iOffInField++;
					eUrlScanStat = URL_SCANSTAT_IN_PATH;
				}
				else if('\0' == *(pCurPos+1))
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = '\0';
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}

					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszHostPort, pFldBuf);
				}
				else
				{
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = *pCurPos;
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
				}
			}
			else if(URL_SCANSTAT_IN_PATH == eUrlScanStat)
			{
				if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
				{
					if(URL_FIELD_BUF_SIZE-1 > iOffInField)
					{
						pFldBuf[iOffInField] = *pCurPos;
					}
					else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
					{
						pFldBuf[iOffInField] = '\0';							
					}
					iOffInField++;
				}

				if('\0' == *(pCurPos+1))
				{					
					if(URL_FIELD_BUF_SIZE-1 >= iOffInField)
					{
						if(URL_FIELD_BUF_SIZE-1 > iOffInField)
						{
							pFldBuf[iOffInField] = '\0';
						}
						else if(URL_FIELD_BUF_SIZE-1 == iOffInField)
						{
							pFldBuf[iOffInField] = '\0';							
						}
						iOffInField++;
					}
					TrimRight(pFldBuf);
					SAFE_STRNCPY(pThis->pszPath, pFldBuf);
				}
			}
			pCurPos++;
		}
	}while(FALSE);

	if(pFldBuf)
	{
		free(pFldBuf);
		pFldBuf = NULL;
	}
	
	return iOutRet;
}

