#include <RefCounter.h>
#include <Compiler.h>
#include <AtomicOperation.h>
#include <stdio.h>

CRefCounter::CRefCounter()
{
	//printf("CRefCounter()\n");
	InitAtomic(&m_StrongRefAtomic);
	InitAtomic(&m_WeakRefAtomic);
}

CRefCounter::~CRefCounter()
{
	//printf("~CRefCounter()\n");
}

int CRefCounter::IncStrong(int iToAdd/* = 1*/)
{
	return atomic_add_return(&m_StrongRefAtomic, iToAdd);
}

int CRefCounter::DecStrong(int iToDec/* = 1*/)
{
	return atomic_sub_return(&m_StrongRefAtomic, iToDec);
}

int CRefCounter::IncWeak(int iToAdd/* = 1*/)
{
	return atomic_add_return(&m_WeakRefAtomic, iToAdd);
}

int CRefCounter::DecWeak(int iToDec/* = 1*/)
{
	return atomic_sub_return(&m_WeakRefAtomic, iToDec);
}

int CRefCounter::StrongRef()
{
	return atomic_read(&m_StrongRefAtomic);
}

int CRefCounter::WeakRef()
{	
	return atomic_read(&m_WeakRefAtomic);
}

