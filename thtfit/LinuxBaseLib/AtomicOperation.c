#include <AtomicOperation.h>

void InitAtomic(P_ATOMIC_T pAtomic)
{
	pAtomic->Counter = 0;	
}

int atomic_read(P_ATOMIC_T pAtomic)
{
#if	defined(__mips__)	//defined by gcc
	return pAtomic->Counter;
#elif	defined(__i486__)	//defined by gcc
	return pAtomic->Counter;
#elif	defined(__arm__) 
	return pAtomic->Counter;
#else
	#error "Not supported"
#endif
}

void atomic_set(P_ATOMIC_T pAtomic, int iToSet)
{
#if	defined(__mips__)	//defined by gcc
	pAtomic->Counter = iToSet;
#elif	defined(__i486__)	//defined by gcc
	pAtomic->Counter = iToSet;
#elif	defined(__arm__)
	pAtomic->Counter = iToSet;
#else
	#error "Not supported"
#endif
}

int atomic_add_return(P_ATOMIC_T pAtomic, int iToAdd)
{
#if	defined(__mips__) && defined(ANDROID)	//defined by gcc

	unsigned long temp, result;

	__asm__ __volatile__(
		".set push				 # atomic_add_return\n"
		".set noreorder 							\n"
		"1:   ll	  %1, %2						\n"
		"	  addu	  %0, %1, %3					\n"
		"	  sc	  %0, %2						\n"
		"	  beqz	  %0, 1b						\n"
		"	  addu	  %0, %1, %3					\n"
		".set pop									\n"
		: "=&r" (result), "=&r" (temp), "=m" (pAtomic->Counter)
		: "Ir" (iToAdd), "m" (pAtomic->Counter)
		: "memory");

	return result;		

#elif	defined(__i486__)	//defined by gcc
	return __sync_add_and_fetch(&(pAtomic->Counter), iToAdd);
#elif	defined(__mips__)	//defined by gcc
	return __sync_add_and_fetch(&(pAtomic->Counter), iToAdd);
#elif	defined(__arm__) 
	
	return __sync_add_and_fetch(&(pAtomic->Counter), iToAdd);;
#else
	#error "Not supported"
#endif
}

int atomic_sub_return(P_ATOMIC_T pAtomic, int iToSub)
{
#if	defined(__mips__) && defined(ANDROID)	//defined by gcc
	unsigned long temp, result;

	__asm__ __volatile__(
		".set push									 \n"
		".set noreorder 		  # atomic_sub_return\n"
		"1:   ll	%1, %2							 \n"
		"	  subu	%0, %1, %3						 \n"
		"	  sc	%0, %2							 \n"
		"	  beqz	%0, 1b							 \n"
		"	  subu	%0, %1, %3						 \n"
		".set pop									 \n"
		: "=&r" (result), "=&r" (temp), "=m" (pAtomic->Counter)
		: "Ir" (iToSub), "m" (pAtomic->Counter)
		: "memory");

	return result;

#elif	defined(__i486__)	//defined by gcc
	return __sync_sub_and_fetch(&(pAtomic->Counter), iToSub);
#elif	defined(__mips__)	//defined by gcc
	return __sync_sub_and_fetch(&(pAtomic->Counter), iToSub);
#elif	defined(__arm__) 	
	return __sync_sub_and_fetch(&(pAtomic->Counter), iToSub);;
#else
	#error "Not supported"
#endif
}

