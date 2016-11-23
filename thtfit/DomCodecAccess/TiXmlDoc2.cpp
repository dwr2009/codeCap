#include <stdio.h>
#include <BaseErrDef.h>
#include <BaseTypeDef.h>
#include "TiXmlDoc2.h"
#include <stdlib.h>
#include <ErrPrintHelper.h>

TiXmlDocument2::TiXmlDocument2()
{

}

TiXmlDocument2::~TiXmlDocument2()
{

}

int TiXmlDocument2::GetDeclare(char *pszVersion, char *pszStandalone, char *pszEncoding)
{
	int iOutRet = ERROR_SUCCESS;
	do{
		char *pszStrBuf = NULL;
		int iLen = 0;
		TiXmlNode* pXmlFirst = FirstChild();
		if (NULL != pXmlFirst)  
		{  
			TiXmlDeclaration* pXmlDec = pXmlFirst->ToDeclaration();  
			if (NULL != pXmlDec)
			{  
				pszStrBuf = (char *)pXmlDec->Version();
				iLen = strlen(pszStrBuf);
				pszStrBuf[iLen+1] = '\0';
				memcpy(pszVersion, pszStrBuf, ((iLen+1) > 128)?128:(iLen+1));
				pszStrBuf = (char *)pXmlDec->Standalone();
				iLen = strlen(pszStrBuf);
				pszStrBuf[iLen+1] = '\0';
				memcpy(pszStandalone, pszStrBuf, ((iLen+1) > 128)?128:(iLen+1));
				pszStrBuf = (char *)pXmlDec->Encoding();
				iLen = strlen(pszStrBuf);
				pszStrBuf[iLen+1] = '\0';
				memcpy(pszEncoding, pszStrBuf, ((iLen+1) > 128)?128:(iLen+1));
			}
			else
			{
				iOutRet = ERROR_NOT_FOUND;
			}
		}
		else
		{
			iOutRet = ERROR_NOT_FOUND;
		}
	}while(FALSE);
	return iOutRet;
}

int TiXmlDocument2::TraverseAttribute(TiXmlElement* pEleRoot)
{
	int iOutRet = ERROR_SUCCESS;
	do
	{
		if (NULL == pEleRoot)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		TiXmlElement* pEle = pEleRoot;
		TiXmlAttribute* pAttr = NULL;
		
		for(pAttr = pEle->FirstAttribute(); pAttr; pAttr = pAttr->Next())
		{
			printf("\t");
			printf("%s = %s\n",pAttr->Name(),pAttr->Value());
		}
		//for (pEle = pEleRoot->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
		//{
			//递归处理子节点
		//	TraverseAttribute(pEle);
		//}
	}while(FALSE);
	return iOutRet;
}

int TiXmlDocument2::Traverse()
{
	int iOutRet = ERROR_SUCCESS;
		
	TiXmlElement* pXmlElem = RootElement();
	printf("Element:%s\n",pXmlElem->Value());
	iOutRet = TraverseAttribute(pXmlElem);
	for(;pXmlElem;pXmlElem = pXmlElem->NextSiblingElement())
		printf("Element:%s\n",pXmlElem->Value());
	pXmlElem = pXmlElem->FirstChildElement();
	printf("Element:%s\n",pXmlElem->Value());
	for(;pXmlElem;pXmlElem = pXmlElem->NextSiblingElement())
		printf("Element:%s\n",pXmlElem->Value());
	return iOutRet;
}

int TiXmlDocument2::GetNodePropValue(const char * pszNodePath, 
	const char * pszPropName, /*OUT*/char * pszBuf, /*IN, OUT*/int * piBufLen,
	const char * pszDefaultVal/*NULL*/)
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if('\0' == pszNodePath || '\0' == pszNodePath[0] || '\0' == pszPropName 
			|| '\0' == pszPropName[0] || '\0' == pszBuf)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		TiXmlElement * pXmlEle = RootElement();
		if(NULL == pXmlEle) {iOutRet = ERROR_NO_ROOT_NODE; break;}

		char pszNodeName[128]; const char * pCurCharPos = pszNodePath;
		const char * pPrevSrchPos = pszNodePath - 1;
		int iLen = 0;
		do{
			pCurCharPos = strchr(pCurCharPos, '.');
			if(NULL == pCurCharPos)
			{
				iLen = strlen(pPrevSrchPos+1);
				if((typeof(iLen))sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strcpy(pszNodeName, pPrevSrchPos+1);
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);
				break;
			}
			else
			{				
				iLen = pCurCharPos-(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strncpy(pszNodeName, pPrevSrchPos+1, iLen);
				pszNodeName[iLen] = '\0';
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);				
			}
			pPrevSrchPos = pCurCharPos;
			pCurCharPos += 1;
		}while(pXmlEle);
		if(NULL == pXmlEle && ERROR_SUCCESS == iOutRet)
		{
			iOutRet = ERROR_INVALID_NODE_PATH; 
			break;
		}

		const char * pPropVal = pXmlEle->Attribute(pszPropName);
		if(NULL == pPropVal)
		{
			iOutRet = ERROR_INVALID_PROP_NAME; 
			break;
		}

		iLen = strlen(pPropVal);
		if(*piBufLen < (iLen + 1))
		{
			*piBufLen = iLen + 1;
			iOutRet = ERROR_NOT_ENOUGH_BUFFER; 
			break;
		}

		*piBufLen = iLen + 1;
		memcpy(pszBuf, pPropVal, iLen+1);
	}while(FALSE);

	if(pszDefaultVal && (ERROR_INVALID_PROP_NAME == iOutRet 
		|| ERROR_INVALID_NODE_PATH == iOutRet || ERROR_NO_ROOT_NODE == iOutRet
		|| ERROR_NODE_NAME_TOO_LONG == iOutRet ))
	{
		int iLen = 0;
		iLen = strlen(pszDefaultVal);
		if(*piBufLen < (iLen + 1))
		{
			*piBufLen = iLen + 1;
			iOutRet = ERROR_NOT_ENOUGH_BUFFER; 
		}
		else
		{
			*piBufLen = iLen + 1;
			memcpy(pszBuf, pszDefaultVal, iLen+1);
			iOutRet = ERROR_SUCCESS;
		}
	}

	return iOutRet;
}

//if the property name specified is not exist, create it first.
int TiXmlDocument2::SetNodePropValue(const char * pszNodePath, 
	const char * pszPropName, /*IN*/const char * pszBuf, bool bCreateNodeIfNotExist/*false*/)
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if('\0' == pszNodePath || '\0' == pszNodePath[0] || '\0' == pszPropName 
			|| '\0' == pszPropName[0] || '\0' == pszBuf)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		TiXmlElement * pXmlEle = RootElement(), *pXmlEleParent = NULL;
		if(NULL == pXmlEle) 
		{
			if(false == bCreateNodeIfNotExist)
			{
				iOutRet = ERROR_NO_ROOT_NODE; 
				break;
			}

			TiXmlElement * pXmlEleToAdd = NULL;
			pXmlEleToAdd = new TiXmlElement("root");
			if(NULL == pXmlEleToAdd)
			{
				iOutRet = ERROR_OUT_OF_MEMORY; 
				break;
			}

			TiXmlNode * pXmlNodeAdded = NULL;
			pXmlNodeAdded = InsertEndChild(*pXmlEleToAdd);
			if(pXmlEleToAdd) {delete pXmlEleToAdd; pXmlEleToAdd = NULL;}
			if(NULL == pXmlNodeAdded)
			{
				iOutRet = ERROR_INSERT_NODE_FAIL; 
				break;
			}
			pXmlEle = FirstChildElement("root");		
			if(NULL == pXmlEle)
			{						
				break;
			}
		}

		char pszNodeName[128]; const char * pCurCharPos = pszNodePath;
		const char * pPrevSrchPos = pszNodePath - 1;
		int iLen = 0;
		do{
			pCurCharPos = strchr(pCurCharPos, '.');
			if(NULL == pCurCharPos)
			{
				iLen = strlen(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strcpy(pszNodeName, pPrevSrchPos+1);
				pXmlEleParent = pXmlEle;
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);
				if(NULL == pXmlEle && true == bCreateNodeIfNotExist)
				{
					TiXmlElement * pXmlEleToAdd = NULL;
					pXmlEleToAdd = new TiXmlElement(pszNodeName);
					if(NULL == pXmlEleToAdd)
					{
						iOutRet = ERROR_OUT_OF_MEMORY; 
						break;
					}

					TiXmlNode * pXmlNodeAdded = NULL;
					pXmlNodeAdded = pXmlEleParent->InsertEndChild(*pXmlEleToAdd);
					if(pXmlEleToAdd) {delete pXmlEleToAdd; pXmlEleToAdd = NULL;}
					if(NULL == pXmlNodeAdded)
					{
						iOutRet = ERROR_INSERT_NODE_FAIL; 
						break;
					}
					pXmlEle = pXmlEleParent->FirstChildElement(pszNodeName);		
					if(NULL == pXmlEle)
					{						
						break;
					}
				}	
				break;
			}
			else
			{				
				iLen = pCurCharPos-(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strncpy(pszNodeName, pPrevSrchPos+1, iLen);
				pszNodeName[iLen] = '\0';
				pXmlEleParent = pXmlEle;
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);			
				if(NULL == pXmlEle && true == bCreateNodeIfNotExist)
				{
					TiXmlElement * pXmlEleToAdd = NULL;
					pXmlEleToAdd = new TiXmlElement(pszNodeName);
					if(NULL == pXmlEleToAdd)
					{
						iOutRet = ERROR_OUT_OF_MEMORY; 
						break;
					}

					TiXmlNode * pXmlNodeAdded = NULL;
					pXmlNodeAdded = pXmlEleParent->InsertEndChild(*pXmlEleToAdd);
					if(pXmlEleToAdd) {delete pXmlEleToAdd; pXmlEleToAdd = NULL;}
					if(NULL == pXmlNodeAdded)
					{
						iOutRet = ERROR_INSERT_NODE_FAIL; 
						break;
					}
					pXmlEle = pXmlEleParent->FirstChildElement(pszNodeName);		
					if(NULL == pXmlEle)
					{						
						break;
					}
				}	
			}
			pPrevSrchPos = pCurCharPos;
			pCurCharPos += 1;
		}while(pXmlEle);
		if(NULL == pXmlEle && ERROR_SUCCESS == iOutRet)
		{
			iOutRet = ERROR_INVALID_NODE_PATH; 
			break;
		}

		pXmlEle->SetAttribute(pszPropName, pszBuf);		
	}while(FALSE);

	return iOutRet;
}

//delete a property of a node
int TiXmlDocument2::DelNodeProperty(const char * pszNodePath, const char * pszPropName)
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if('\0' == pszNodePath || '\0' == pszNodePath[0] || '\0' == pszPropName 
			|| '\0' == pszPropName[0])
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}

		TiXmlElement * pXmlEle = RootElement();
		if(NULL == pXmlEle) {iOutRet = ERROR_NO_ROOT_NODE; break;}

		char pszNodeName[128]; const char * pCurCharPos = pszNodePath;
		const char * pPrevSrchPos = pszNodePath - 1;
		int iLen = 0;
		do{
			pCurCharPos = strchr(pCurCharPos, '.');
			if(NULL == pCurCharPos)
			{
				iLen = strlen(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strcpy(pszNodeName, pPrevSrchPos+1);
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);
				break;
			}
			else
			{				
				iLen = pCurCharPos-(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strncpy(pszNodeName, pPrevSrchPos+1, iLen);
				pszNodeName[iLen] = '\0';
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);				
			}
			pPrevSrchPos = pCurCharPos;
			pCurCharPos += 1;
		}while(pXmlEle);
		if(NULL == pXmlEle && ERROR_SUCCESS == iOutRet)
		{
			iOutRet = ERROR_INVALID_NODE_PATH; 
			break;
		}

		pXmlEle->RemoveAttribute(pszPropName);		
	}while(FALSE);

	return iOutRet;
}

int TiXmlDocument2::AddNode(const char * pszNodePath, const char * pszNodeNameToAdd)
{
	int iOutRet = ERROR_SUCCESS;

	TiXmlElement * pXmlEleToAdd = NULL;
	
	do{
		if('\0' == pszNodePath || '\0' == pszNodePath[0] || '\0' == pszNodeNameToAdd
			|| '\0' == pszNodeNameToAdd[0])
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}

		TiXmlElement * pXmlEle = RootElement();
		if(NULL == pXmlEle) {iOutRet = ERROR_NO_ROOT_NODE; break;}

		char pszNodeName[128]; const char * pCurCharPos = pszNodePath;
		const char * pPrevSrchPos = pszNodePath - 1;
		int iLen = 0;
		do{
			pCurCharPos = strchr(pCurCharPos, '.');
			if(NULL == pCurCharPos)
			{
				iLen = strlen(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strcpy(pszNodeName, pPrevSrchPos+1);
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);
				break;
			}
			else
			{				
				iLen = pCurCharPos-(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strncpy(pszNodeName, pPrevSrchPos+1, iLen);
				pszNodeName[iLen] = '\0';
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);				
			}
			pPrevSrchPos = pCurCharPos;
			pCurCharPos += 1;
		}while(pXmlEle);
		if(NULL == pXmlEle && ERROR_SUCCESS == iOutRet)
		{
			iOutRet = ERROR_INVALID_NODE_PATH; 
			break;
		}

		pXmlEleToAdd = new TiXmlElement(pszNodeNameToAdd);
		if(NULL == pXmlEleToAdd)
		{
			iOutRet = ERROR_OUT_OF_MEMORY; 
			break;
		}

		TiXmlNode * pXmlNodeAdded = NULL;
		pXmlNodeAdded = pXmlEle->InsertEndChild(*pXmlEleToAdd);
		if(NULL == pXmlNodeAdded)
		{
			iOutRet = ERROR_INSERT_NODE_FAIL; 
			break;
		}
	}while(FALSE);

	if(pXmlEleToAdd) {delete pXmlEleToAdd; pXmlEleToAdd = NULL;}

	return iOutRet;
}

int TiXmlDocument2::DelNode(const char * pszNodePath, const char * pszNodeNameToDel)
{
	int iOutRet = ERROR_SUCCESS; BOOL bRet = FALSE;

	do{
		if('\0' == pszNodePath || '\0' == pszNodePath[0] || '\0' == pszNodeNameToDel
			|| '\0' == pszNodeNameToDel[0])
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}

		TiXmlElement * pXmlEle = RootElement();
		if(NULL == pXmlEle) {iOutRet = ERROR_NO_ROOT_NODE; break;}

		char pszNodeName[128]; const char * pCurCharPos = pszNodePath;
		const char * pPrevSrchPos = pszNodePath - 1;
		int iLen = 0;
		do{
			pCurCharPos = strchr(pCurCharPos, '.');
			if(NULL == pCurCharPos)
			{
				iLen = strlen(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strcpy(pszNodeName, pPrevSrchPos+1);
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);
				break;
			}
			else
			{				
				iLen = pCurCharPos-(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strncpy(pszNodeName, pPrevSrchPos+1, iLen);
				pszNodeName[iLen] = '\0';
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);				
			}
			pPrevSrchPos = pCurCharPos;
			pCurCharPos += 1;
		}while(pXmlEle);
		if(NULL == pXmlEle && ERROR_SUCCESS == iOutRet)
		{
			iOutRet = ERROR_INVALID_NODE_PATH; 
			break;
		}

		TiXmlElement * pXmlEleToDel = NULL;
		pXmlEleToDel = pXmlEle->FirstChildElement(pszNodeNameToDel);
		if(NULL == pXmlEleToDel)
		{
			iOutRet = ERROR_NOT_FOUND_NODE; 
			break;
		}

		bRet = pXmlEle->RemoveChild(pXmlEleToDel);
		if(FALSE == bRet)
		{
			iOutRet = ERROR_DEL_NODE_FAIL; 
			break;			
		}
	}while(FALSE);

	return iOutRet;
}

bool TiXmlDocument2::ExistNode(const char * pszNodePath)
{
	int iOutRet = ERROR_SUCCESS; BOOL bRet = FALSE;

	do{
		if('\0' == pszNodePath || '\0' == pszNodePath[0])
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}

		TiXmlElement * pXmlEle = RootElement();
		if(NULL == pXmlEle) {iOutRet = ERROR_NO_ROOT_NODE; break;}

		char pszNodeName[128]; const char * pCurCharPos = pszNodePath;
		const char * pPrevSrchPos = pszNodePath - 1;
		int iLen = 0;
		do{
			pCurCharPos = strchr(pCurCharPos, '.');
			if(NULL == pCurCharPos)
			{
				iLen = strlen(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strcpy(pszNodeName, pPrevSrchPos+1);
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);
				break;
			}
			else
			{				
				iLen = pCurCharPos-(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strncpy(pszNodeName, pPrevSrchPos+1, iLen);
				pszNodeName[iLen] = '\0';
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);				
			}
			pPrevSrchPos = pCurCharPos;
			pCurCharPos += 1;
		}while(pXmlEle);
		if(NULL == pXmlEle && ERROR_SUCCESS == iOutRet)
		{
			iOutRet = ERROR_INVALID_NODE_PATH; 
			break;
		}
	}while(FALSE);

	return (ERROR_SUCCESS == iOutRet)?TRUE:FALSE;
}

INT_t TiXmlDocument2::getElementPropertyValue(LPCSTR pszPath, LPCSTR pszPropertyName, OUT CString & strValue, 
	LPCSTR pszDefaultValue/* = NULL*/)
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == pszPath || '\0' == pszPath[0] || NULL == pszPropertyName || '\0' == pszPropertyName[0])
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		TiXmlElement * pXmlEle = RootElement();
		if(NULL == pXmlEle) {iOutRet = ERROR_NO_ROOT_NODE; break;}

		char pszNodeName[1024]; 
		const char * pCurCharPos = pszPath;
		const char * pPrevSrchPos = pszPath - 1;
		int iLen = 0;
		do{
			pCurCharPos = strchr(pCurCharPos, '.');
			if(NULL == pCurCharPos)
			{
				iLen = strlen(pPrevSrchPos+1);
				if((typeof(iLen))sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strcpy(pszNodeName, pPrevSrchPos+1);
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);
				break;
			}
			else
			{				
				iLen = pCurCharPos-(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strncpy(pszNodeName, pPrevSrchPos+1, iLen);
				pszNodeName[iLen] = '\0';
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);				
			}
			pPrevSrchPos = pCurCharPos;
			pCurCharPos += 1;
		}while(pXmlEle);
		if(NULL == pXmlEle && ERROR_SUCCESS == iOutRet)
		{
			iOutRet = ERROR_INVALID_NODE_PATH; 
			break;
		}

		const char * pPropVal = pXmlEle->Attribute(pszPropertyName);
		if(NULL == pPropVal)
		{
			iOutRet = ERROR_INVALID_PROP_NAME; 
			break;
		}

		strValue = pPropVal;
		if(ERROR_SUCCESS != strValue.GetLastErrNo())
		{
			iOutRet = strValue.GetLastErrNo();
			break;
		}
	}while(FALSE);

	if(pszDefaultValue && (ERROR_INVALID_PROP_NAME == iOutRet || ERROR_INVALID_NODE_PATH == iOutRet || 
		ERROR_NO_ROOT_NODE == iOutRet || ERROR_NODE_NAME_TOO_LONG == iOutRet))
	{
		strValue = pszDefaultValue;		
		if(ERROR_SUCCESS == strValue.GetLastErrNo())
		{
			iOutRet = ERROR_SUCCESS;
		}
		else
		{
			iOutRet = strValue.GetLastErrNo();
		}
	}

	return iOutRet;
}

INT_t TiXmlDocument2::setElementPropertyValue(LPCSTR pszPath, LPCSTR pszPropertyName, LPCSTR pszValue, 
	bool bCreateNodeIfNotExist/* = false*/)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(NULL == pszPath || '\0' == pszPath[0] || '\0' == pszPropertyName || '\0' == pszPropertyName[0])
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(NULL == pszValue)
		{
			pszValue = "";
		}

		TiXmlElement * pXmlEle = RootElement(), *pXmlEleParent = NULL;
		if(NULL == pXmlEle) 
		{
			if(false == bCreateNodeIfNotExist)
			{
				iOutRet = ERROR_NO_ROOT_NODE; 
				break;
			}

			TiXmlElement * pXmlEleToAdd = NULL;
			pXmlEleToAdd = new TiXmlElement("root");
			if(NULL == pXmlEleToAdd)
			{
				iOutRet = ERROR_OUT_OF_MEMORY; 
				break;
			}

			TiXmlNode * pXmlNodeAdded = NULL;
			pXmlNodeAdded = InsertEndChild(*pXmlEleToAdd);
			if(pXmlEleToAdd) {delete pXmlEleToAdd; pXmlEleToAdd = NULL;}
			if(NULL == pXmlNodeAdded)
			{
				iOutRet = ERROR_INSERT_NODE_FAIL; 
				break;
			}
			pXmlEle = FirstChildElement("root");		
			if(NULL == pXmlEle)
			{						
				break;
			}
		}

		char pszNodeName[1024]; 
		const char * pCurCharPos = pszPath;
		const char * pPrevSrchPos = pszPath - 1;
		int iLen = 0;
		do
		{
			pCurCharPos = strchr(pCurCharPos, '.');
			if(NULL == pCurCharPos)
			{
				iLen = strlen(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strcpy(pszNodeName, pPrevSrchPos+1);
				pXmlEleParent = pXmlEle;
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);
				if(NULL == pXmlEle && true == bCreateNodeIfNotExist)
				{
					TiXmlElement * pXmlEleToAdd = NULL;
					pXmlEleToAdd = new TiXmlElement(pszNodeName);
					if(NULL == pXmlEleToAdd)
					{
						iOutRet = ERROR_OUT_OF_MEMORY; 
						break;
					}

					TiXmlNode * pXmlNodeAdded = NULL;
					pXmlNodeAdded = pXmlEleParent->InsertEndChild(*pXmlEleToAdd);
					if(pXmlEleToAdd) {delete pXmlEleToAdd; pXmlEleToAdd = NULL;}
					if(NULL == pXmlNodeAdded)
					{
						iOutRet = ERROR_INSERT_NODE_FAIL; 
						break;
					}
					pXmlEle = pXmlEleParent->FirstChildElement(pszNodeName);		
					if(NULL == pXmlEle)
					{						
						break;
					}
				}	
				break;
			}
			else
			{				
				iLen = pCurCharPos-(pPrevSrchPos+1);
				if(sizeof(pszNodeName) -1 < iLen)
				{
					iOutRet = ERROR_NODE_NAME_TOO_LONG; 
					break;
				}
				strncpy(pszNodeName, pPrevSrchPos+1, iLen);
				pszNodeName[iLen] = '\0';
				pXmlEleParent = pXmlEle;
				pXmlEle = pXmlEle->FirstChildElement(pszNodeName);			
				if(NULL == pXmlEle && true == bCreateNodeIfNotExist)
				{
					TiXmlElement * pXmlEleToAdd = NULL;
					pXmlEleToAdd = new TiXmlElement(pszNodeName);
					if(NULL == pXmlEleToAdd)
					{
						iOutRet = ERROR_OUT_OF_MEMORY; 
						break;
					}

					TiXmlNode * pXmlNodeAdded = NULL;
					pXmlNodeAdded = pXmlEleParent->InsertEndChild(*pXmlEleToAdd);
					if(pXmlEleToAdd) {delete pXmlEleToAdd; pXmlEleToAdd = NULL;}
					if(NULL == pXmlNodeAdded)
					{
						iOutRet = ERROR_INSERT_NODE_FAIL; 
						break;
					}
					pXmlEle = pXmlEleParent->FirstChildElement(pszNodeName);		
					if(NULL == pXmlEle)
					{						
						break;
					}
				}	
			}
			pPrevSrchPos = pCurCharPos;
			pCurCharPos += 1;
		}while(pXmlEle);
		if(NULL == pXmlEle && ERROR_SUCCESS == iOutRet)
		{
			iOutRet = ERROR_INVALID_NODE_PATH; 
			break;
		}

		pXmlEle->SetAttribute(pszPropertyName, pszValue);		
	}while(FALSE);

	return iOutRet;
}

INT_t TiXmlDocument2::setElementPropertyValue(LPCSTR pszPath, LPCSTR pszPropertyName, int iValue,
	bool bCreateNodeIfNotExist/* = false*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;	
	char szValue[64];

	do
	{
		iRet = snprintf(szValue, sizeof(szValue)-1, "%d", iValue);
		if(0 >= iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		szValue[sizeof(szValue)-1] = '\0';
		iOutRet = setElementPropertyValue(pszPath, pszPropertyName, szValue, bCreateNodeIfNotExist);
	}while(FALSE);

	return iOutRet;
}

bool TiXmlDocument2::IsXmlFile(const char * pszXmlFile)
{
	BOOL bOutRet = FALSE;

	do{
		if(NULL == pszXmlFile) {break;}

		TiXmlDocument oXmlDoc;
		bOutRet = oXmlDoc.LoadFile(pszXmlFile);
		oXmlDoc.Clear();
	}while(false);

	return bOutRet;
}
