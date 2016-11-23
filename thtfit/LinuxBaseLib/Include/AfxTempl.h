#ifndef	_AFXTEMPL_H_
#define	_AFXTEMPL_H_

#define	AFX_INLINE		inline

template<class TYPE> void DestructElements (TYPE* pElements, int nCount);

template<class TYPE>
AFX_INLINE  void DestructElements(TYPE* pElements, int nCount)
{
	// call the destructor(s)
	for (; nCount--; pElements++)
		pElements->~TYPE();
}

#endif	//_AFXTEMPL_H_

