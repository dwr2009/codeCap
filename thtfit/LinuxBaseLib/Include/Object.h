#ifndef	_OBJECT_H_
#define	_OBJECT_H_

#include <string.h>

#define	CLASS_ID(ClassName)		(#ClassName)

#define	DECLARE_RUNTIME_CLASS(ClassName)	\
	private:	\
		static const char * szClassName;	\
	public:	\
		static const char * getClassIdString() {return szClassName;};	\
		virtual bool IsClass(const char * pszClassId);	\
		virtual bool IsInheritClass(const char * pszClassId);
	
#define	BEGIN_IMPL_RUNTIME_CLASS(ClassName)	\
	const char * ClassName::szClassName = CLASS_ID(ClassName);	\
	bool ClassName::IsClass(const char * pszClassId)	\
	{	\
		if(szClassName)	\
		{	\
			if(0 == strcmp(szClassName, pszClassId))	\
			{	\
				return true;	\
			}	\
		}	\
		return false;	\
	}	\
	bool ClassName::IsInheritClass(const char * pszClassId)	\
	{	\
		if(szClassName)	\
		{	\
			if(0 == strcmp(szClassName, pszClassId))	\
			{	\
				return true;	\
			}	\
		}
#define	BASE_IMPL_RUNTIME_CLASS(BaseClassName)	\
		if(BaseClassName::IsInheritClass(pszClassId))	\
		{	\
			return true;	\
		}
#define	END_IMPL_RUNTIME_CLASS()	\
		return false;	\
	}

class CObject
{
	DECLARE_RUNTIME_CLASS(CObject)
public:
	CObject();
	virtual ~CObject();
};

#endif	//_OBJECT_H_

