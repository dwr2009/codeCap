#ifndef	_TIXMLDOC2_H_
#define	_TIXMLDOC2_H_

#include <tinyxml.h>
#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include <String.h>
#include <ErrChkMutex.h>

#define	ERROR_NO_ROOT_NODE			(ERR_BASE_MAX+1)
#define	ERROR_INVALID_NODE_PATH		(ERR_BASE_MAX+2)
#define	ERROR_NODE_NAME_TOO_LONG	(ERR_BASE_MAX+3)
#define	ERROR_INVALID_PROP_NAME		(ERR_BASE_MAX+4)
#define	ERROR_INSERT_NODE_FAIL		(ERR_BASE_MAX+5)
#define	ERROR_NOT_FOUND_NODE		(ERR_BASE_MAX+6)
#define	ERROR_DEL_NODE_FAIL			(ERR_BASE_MAX+7)

class TiXmlDocument2 : public TiXmlDocument
{
public:
	TiXmlDocument2();
	virtual ~TiXmlDocument2();
	int GetDeclare(char *pszVersion, char *pszStandalone, char *pszEncoding);
	int TraverseAttribute(TiXmlElement* pEleRoot);
	int Traverse();
	int GetNodePropValue(const char * pszNodePath, const char * pszPropName, 
		/*OUT*/char * pszBuf, /*IN, OUT*/int * piBufLen, const char * pszDefaultVal = NULL);
	int SetNodePropValue(const char * pszNodePath, const char * pszPropName, 
		/*IN*/const char * pszBuf, bool bCreateNodeIfNotExist = false);
	int DelNodeProperty(const char * pszNodePath, const char * pszPropName);
	int AddNode(const char * pszNodePath, const char * pszNodeNameToAdd);
	int DelNode(const char * pszNodePath, const char * pszNodeNameToDel);
	bool ExistNode(const char * pszNodePath);
	static bool IsXmlFile(const char * pszXmlFile);
	INT_t getElementPropertyValue(LPCSTR pszPath, LPCSTR pszPropertyName, OUT CString & strValue, LPCSTR pszDefaultValue = NULL);
	INT_t setElementPropertyValue(LPCSTR pszPath, LPCSTR pszPropertyName, LPCSTR pszValue, bool bCreateNodeIfNotExist = false);
	INT_t setElementPropertyValue(LPCSTR pszPath, LPCSTR pszPropertyName, int iValue, bool bCreateNodeIfNotExist = false);
};

class CTinyXmlDoc3 : public TiXmlDocument2
{
public:
	CErrChkMutex m_Mutex;
};

#endif	//_TIXMLDOC2_H_

