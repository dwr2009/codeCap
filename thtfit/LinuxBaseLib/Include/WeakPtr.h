#ifndef	_WEAK_PTR_H_
#define	_WEAK_PTR_H_

#include <RefCounter.h>
#include <stdio.h>
#include <Compiler.h>
#include <ErrPrintHelper.h>

#include <SharedPtr.h>

#if	defined(SRC_FILE_NAME)
	#undef	SRC_FILE_NAME
#endif	//SRC_FILE_NAME
#define	SRC_FILE_NAME		"WeakPtr.h"

namespace CppBase
{

template<class T> class SharedPtr;

template<class T> class WeakPtr
{
private:
    typedef WeakPtr<T> THIS_CLASS_TYPE;	
	template<class Y> friend class SharedPtr;
	template<class Y> friend class WeakPtr;
public:
	WeakPtr();
	WeakPtr(THIS_CLASS_TYPE const & r);
	WeakPtr(SharedPtr<T> const & r);
	template<class Y> WeakPtr(WeakPtr<Y> const & r);
	~WeakPtr();
	void reset();
	void Release();
	void Clear();
	WeakPtr & operator=(SharedPtr<T> const & r);
	WeakPtr & operator=(THIS_CLASS_TYPE const & r);
	template<class Y> WeakPtr<T> & operator=(WeakPtr<Y> const & r);	
	SharedPtr<T> Promote() const;
private:
	CRefCounter * m_pRefCounter;
	T * m_pManagedObj;
};

template<class T> WeakPtr<T>::WeakPtr()
{
	m_pRefCounter = NULL;
	m_pManagedObj = NULL;	
}

template<class T> WeakPtr<T>::WeakPtr(THIS_CLASS_TYPE const & r)
{
	do
	{
		m_pRefCounter = NULL;
		m_pManagedObj = NULL;	
		operator=(r);
	}while(false);
}

template<class T> WeakPtr<T>::WeakPtr(SharedPtr<T> const & r)
{
	do
	{
		m_pRefCounter = NULL;
		m_pManagedObj = NULL;	
		operator=(r);
	}while(false);
}

template<class T> 
template<class Y> WeakPtr<T>::WeakPtr(WeakPtr<Y> const & r)
{
	do
	{
		m_pRefCounter = NULL;
		m_pManagedObj = NULL;	
		operator=(r);
	}while(false);
}

template<class T> WeakPtr<T>::~WeakPtr()
{
	Release();
}

template<class T> void WeakPtr<T>::reset()
{
	int iCurWeakRef;
	
	do
	{
		if(CC_UNLIKELY(NULL == m_pManagedObj))
		{
			break;
		}
		m_pManagedObj = NULL;
		if(CC_UNLIKELY(NULL == m_pRefCounter))
		{
			break;
		}
		iCurWeakRef = m_pRefCounter->DecWeak();
		if(0 == iCurWeakRef)
		{
			int iCurStrongRef;
			iCurStrongRef = m_pRefCounter->StrongRef();
			if(0 == iCurStrongRef)
			{
				delete m_pRefCounter;
				m_pRefCounter = NULL;
			}
			else if(0 > iCurStrongRef)
			{
				PRINT_BFILE_LINENO_BUG_STR;
			}
		}
		else if(0 > iCurWeakRef)
		{
			LOG_BLINE("BUG,CurWeakRef=%d\n", iCurWeakRef);;
		}
		m_pRefCounter = NULL;
	}while(false);
}

template<class T> void WeakPtr<T>::Release()
{
	reset();
}

template<class T> void WeakPtr<T>::Clear()
{
	Release();
}

template<class T> WeakPtr<T> & WeakPtr<T>::operator=(SharedPtr<T> const & r)
{
	do
	{
		reset();
		if(CC_UNLIKELY(NULL == r.m_pManagedObj))
		{
			break;
		}
		if(CC_UNLIKELY(NULL == r.m_pRefCounter))
		{
			break;
		}
		int iCurStrongRef = r.m_pRefCounter->StrongRef();
		if(CC_UNLIKELY(0 >= iCurStrongRef))
		{
			PRINT_BFILE_LINENO_BUG_STR;
		}
		int iCurWeakRef = r.m_pRefCounter->IncWeak();
		if(CC_UNLIKELY(1 > iCurWeakRef))
		{
			PRINT_BFILE_LINENO_BUG_STR;
		}
		m_pManagedObj = r.m_pManagedObj;
		m_pRefCounter = r.m_pRefCounter;
	}while(false);

	return *this;
}

template<class T> WeakPtr<T> & WeakPtr<T>::operator=(THIS_CLASS_TYPE const & r)
{
	do
	{
		if(CC_UNLIKELY(this == &r))
		{
			break;
		}
		reset();
		if(CC_UNLIKELY(NULL == r.m_pManagedObj))
		{
			break;
		}
		if(CC_UNLIKELY(NULL == r.m_pRefCounter))
		{
			break;
		}
		int iCurStrongRef = r.m_pRefCounter->StrongRef();
		if(CC_UNLIKELY(0 == iCurStrongRef))
		{
			break;
		}
		else if(CC_UNLIKELY(0 > iCurStrongRef))
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		int iCurWeakRef = r.m_pRefCounter->IncWeak();
		if(CC_UNLIKELY(1 >= iCurWeakRef))
		{
			PRINT_BFILE_LINENO_BUG_STR;
		}
		m_pManagedObj = r.m_pManagedObj;
		m_pRefCounter = r.m_pRefCounter;
	}while(false);

	return *this;
}

template<class T> 
template<class Y> WeakPtr<T> & WeakPtr<T>::operator=(WeakPtr<Y> const & r)
{
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
		int iCurStrongRef = r.m_pRefCounter->StrongRef();
		if(CC_UNLIKELY(0 == iCurStrongRef))
		{
			break;
		}
		else if(CC_UNLIKELY(0 > iCurStrongRef))
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		int iCurWeakRef = r.m_pRefCounter->IncWeak();
		if(CC_UNLIKELY(1 >= iCurWeakRef))
		{
			PRINT_BFILE_LINENO_BUG_STR;
		}
		m_pManagedObj = r.m_pManagedObj;
		m_pRefCounter = r.m_pRefCounter;
	}while(false);

	return *this;
}

template<class T> SharedPtr<T> WeakPtr<T>::Promote() const
{
	SharedPtr<T> sp;

	do
	{
		if(CC_UNLIKELY(NULL == m_pManagedObj))
		{
			break;
		}
		if(CC_UNLIKELY(NULL == m_pRefCounter))
		{
			break;
		}
		int iCurStrongRef = m_pRefCounter->StrongRef();
		if(CC_UNLIKELY(0 == iCurStrongRef))
		{
			break;
		}
		else if(CC_UNLIKELY(0 > iCurStrongRef))
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		m_pRefCounter->IncWeak();
		m_pRefCounter->IncStrong();
		sp.m_pManagedObj = m_pManagedObj;
		sp.m_pRefCounter = m_pRefCounter;
	}while(false);
	
	return sp;
}

}	//namespace CppBase

#endif	//_WEAK_PTR_H_

