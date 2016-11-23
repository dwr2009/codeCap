#ifndef	_MEM_OP_HELPER_H_
#define	_MEM_OP_HELPER_H_

#define	SAFE_FREE(pMem)	  do{if(pMem) {free(pMem); pMem = NULL;}}while(0)
#define	SAFE_DELETE(pMem)	  do{if(pMem) {delete (pMem); pMem = NULL;}}while(0)

#endif	//_MEM_OP_HELPER_H_

