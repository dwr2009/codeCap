#ifndef	_REF_COUNTER_H_
#define	_REF_COUNTER_H_

#include <AtomicOperation.h>

class CRefCounter
{
public:
	CRefCounter();
	virtual ~CRefCounter();
	virtual int IncStrong(int iToAdd = 1);
	virtual int DecStrong(int iToDec = 1);
	virtual int IncWeak(int iToAdd = 1);
	virtual int DecWeak(int iToDec = 1);
	virtual int StrongRef();
	virtual int WeakRef();
public:
	
protected:
	ATOMIC_T m_StrongRefAtomic;
	ATOMIC_T m_WeakRefAtomic;
};

#endif	//_REF_COUNTER_H_

