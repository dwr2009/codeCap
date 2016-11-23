#ifndef	_SHARED_PTR_H_
#define	_SHARED_PTR_H_

#include <RefCounter.h>
#include <stdio.h>
#include <Compiler.h>
#include <ErrPrintHelper.h>
#include <Object.h>

#include <WeakPtr.h>

#if	defined(SRC_FILE_NAME)
	#undef	SRC_FILE_NAME
#endif	//SRC_FILE_NAME
#define	SRC_FILE_NAME		"SharedPtr.h"

//#define	DBG_SharedPtr_CALL

namespace CppBase
{

template<class T> class WeakPtr;

template<class T> class SharedPtr
{
private:
    typedef SharedPtr<T> THIS_CLASS_TYPE;	
	template<class Y> friend class WeakPtr;
#if	__GNUC__ == 2
#else	//__GNUC__ != 2
	template<class Y> friend class SharedPtr;
#endif
public:
	SharedPtr();
	explicit SharedPtr(T * pObj);
	template<class Y> explicit SharedPtr(Y * pObj);
	SharedPtr(THIS_CLASS_TYPE const & r);
	template<class Y> SharedPtr(SharedPtr<Y> const & r);
	~SharedPtr();
	T * get() const;
	bool isNull() const;;
	bool isValid() const;;
	T * operator-> () const;
	T & operator * () const;
	void reset();
	void Release();
	void Clear();
	template<class Y> SharedPtr<Y> QueryInterface(int InterfaceId);
	template<class Y> SharedPtr<Y> DynamicCast();
	THIS_CLASS_TYPE & operator=(THIS_CLASS_TYPE const & r);
	template<class Y> SharedPtr<T> & operator=(SharedPtr<Y> const & r);
private:
	CRefCounter * m_pRefCounter;
	T * m_pManagedObj;
};

template<class T> SharedPtr<T>::SharedPtr()
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::SharedPtr()\n");
#endif	//DBG_SharedPtr_CALL
	m_pRefCounter = NULL;
	m_pManagedObj = NULL;
}

template<class T> SharedPtr<T>::SharedPtr(T * pObj)
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::SharedPtr(T * pObj)\n");
#endif	//DBG_SharedPtr_CALL
	do
	{
		m_pRefCounter = NULL;
		m_pManagedObj = NULL;
		if(CC_UNLIKELY(NULL == pObj))
		{
			break;
		}
		
		m_pRefCounter = new CRefCounter;
		if(CC_UNLIKELY(NULL == m_pRefCounter))
		{
			break;
		}
		m_pManagedObj = pObj;
		m_pRefCounter->IncWeak();
		m_pRefCounter->IncStrong();
	}while(false);
}

template<class T> 
template<class Y> SharedPtr<T>::SharedPtr(Y * pObj)
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::SharedPtr(Y * pObj)\n");
#endif	//DBG_SharedPtr_CALL
	do
	{
		m_pRefCounter = NULL;
		m_pManagedObj = NULL;
		if(CC_UNLIKELY(NULL == pObj))
		{
			break;
		}
		
		m_pRefCounter = new CRefCounter;
		if(CC_UNLIKELY(NULL == m_pRefCounter))
		{
			break;
		}
		m_pManagedObj = pObj;
		m_pRefCounter->IncWeak();
		m_pRefCounter->IncStrong();
	}while(false);
}

template<class T> SharedPtr<T>::SharedPtr(THIS_CLASS_TYPE const & r)
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::SharedPtr(THIS_CLASS_TYPE const & r)\n");
#endif	//DBG_SharedPtr_CALL
	do
	{
		m_pRefCounter = NULL;
		m_pManagedObj = NULL;
		operator=(r);
	}while(false);
}

template<class T> 
template<class Y> SharedPtr<T>::SharedPtr(SharedPtr<Y> const & r)
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::SharedPtr(SharedPtr<Y> const & r)\n");
#endif	//DBG_SharedPtr_CALL
	do
	{
		m_pRefCounter = NULL;
		m_pManagedObj = NULL;
		operator=(r);
	}while(false);
}

template<class T> SharedPtr<T>::~SharedPtr()
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::~SharedPtr()\n");
#endif	//DBG_SharedPtr_CALL
	Release();
}

template<class T> T * SharedPtr<T>::get() const
{
	return m_pManagedObj;
}

template<class T> bool SharedPtr<T>::isNull() const
{
	return (NULL == m_pManagedObj);
}

template<class T> bool SharedPtr<T>::isValid() const
{
	return (m_pManagedObj);
}

template<class T> T * SharedPtr<T>::operator-> () const
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::operator-> () const\n");
#endif	//DBG_SharedPtr_CALL
	return m_pManagedObj;
}

template<class T>
T & SharedPtr<T>::operator * () const
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::operator * () const\n");
#endif	//DBG_SharedPtr_CALL
	return (*m_pManagedObj);
}

template<class T> SharedPtr<T> & SharedPtr<T>::operator=(SharedPtr<T> const & r)
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::operator=(SharedPtr<T> const & r)\n");
#endif	//DBG_SharedPtr_CALL
	do
	{
		if(CC_UNLIKELY(this == &r))
		{
			break;
		}
		reset();
		if(CC_UNLIKELY(NULL == r.m_pManagedObj || NULL == r.m_pRefCounter))
		{
			break;
		}
		m_pManagedObj = r.m_pManagedObj;
		r.m_pRefCounter->IncWeak();
		r.m_pRefCounter->IncStrong();
		m_pRefCounter = r.m_pRefCounter;
	}while(false);

	return *this;
}

template<class T> 
template<class Y> SharedPtr<T> & SharedPtr<T>::operator=(SharedPtr<Y> const & r)
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::operator=(SharedPtr<Y> const & r)\n");
#endif	//DBG_SharedPtr_CALL
	do
	{
		if(CC_UNLIKELY(this == (typeof(this))(&r)))
		{
			break;
		}
		reset();
		if(CC_UNLIKELY(NULL == r.m_pManagedObj || NULL == r.m_pRefCounter))
		{
			break;
		}
		m_pManagedObj = r.m_pManagedObj;
		r.m_pRefCounter->IncWeak();
		r.m_pRefCounter->IncStrong();
		m_pRefCounter = r.m_pRefCounter;
	}while(false);

	return *this;
}

template<class T> void SharedPtr<T>::reset()
{
	if(CC_LIKELY(m_pManagedObj))
	{
		if(CC_LIKELY(m_pRefCounter))
		{
			int iCurStrongRefCount, iCurWeakRefCount;
			iCurStrongRefCount = m_pRefCounter->DecStrong();
			iCurWeakRefCount = m_pRefCounter->DecWeak();
			if(0 == iCurStrongRefCount)
			{
				delete m_pManagedObj;
				if(0 == iCurWeakRefCount)
				{
					delete m_pRefCounter;
					m_pRefCounter = NULL;
				}
				else if(0 > iCurWeakRefCount)
				{
					LOG_BLINE("BUG,CurWeakRef=%d\n", iCurWeakRefCount);;
				}
			}
			else if(CC_UNLIKELY(0 > iCurStrongRefCount))
			{
				PRINT_BFILE_LINENO_BUG_STR;
			}
			m_pManagedObj = NULL;
		}
		else
		{
			PRINT_BFILE_LINENO_BUG_STR;
		}
		m_pManagedObj = NULL;
	}
}

template<class T> void SharedPtr<T>::Release()
{
	reset();
}

template<class T> void SharedPtr<T>::Clear()
{
	Release();
}

template<class T>
template<class Y> SharedPtr<Y> SharedPtr<T>::QueryInterface(int InterfaceId)
{
	SharedPtr<Y> YInterface_sp;
	Y * pYInterface = NULL;
	
	do
	{
		if(NULL == m_pManagedObj || NULL == m_pRefCounter)
		{
			break;
		}
		pYInterface = (typeof(pYInterface))(m_pManagedObj->GetInterface(InterfaceId));
		if(NULL == pYInterface)
		{
			break;
		}
		m_pRefCounter->IncWeak();
		m_pRefCounter->IncStrong();
		YInterface_sp.m_pManagedObj = pYInterface;
		YInterface_sp.m_pRefCounter = m_pRefCounter;
	}while(false);
	
	return YInterface_sp;
}

template<class T>
template<class Y> SharedPtr<Y> SharedPtr<T>::DynamicCast()
{
#ifdef	DBG_SharedPtr_CALL
	LOG_BLINE("SharedPtr<T>::DynamicCast()\n");
#endif	//DBG_SharedPtr_CALL
	SharedPtr<Y> YCastCls_sp;
	
	do
	{
		if(NULL == m_pManagedObj || NULL == m_pRefCounter)
		{
			break;
		}
#ifdef	__GXX_RTTI
		YCastCls_sp.m_pManagedObj = dynamic_cast<Y *>(m_pManagedObj);
		if(YCastCls_sp.m_pManagedObj)
		{
			m_pRefCounter->IncWeak();
			m_pRefCounter->IncStrong();
			YCastCls_sp.m_pRefCounter = m_pRefCounter;
		}
#else	//!__GXX_RTTI	
		if(m_pManagedObj->IsInheritClass(Y::getClassIdString()))
		{
			m_pRefCounter->IncWeak();
			m_pRefCounter->IncStrong();
			YCastCls_sp.m_pManagedObj = (Y *)m_pManagedObj;
			YCastCls_sp.m_pRefCounter = m_pRefCounter;
		}
#endif	//__GXX_RTTI
	}while(false);
	
	return YCastCls_sp;
}

}	//namespace CppBase

#endif	//_SHARED_PTR_H_

