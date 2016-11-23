#include "MemoryDevice.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <sys/mman.h>
#include <MemoryDevice.h>
#include <Compiler.h>

CMemoryDevice::CMemoryDevice()
{
	int iRet;
	
	mInited = FALSE;
	
	SetCrtErrNo(0);
	mMemoryPageSize = getpagesize();
	mMemDevFd = -1;
	mMappedSize = 0;
	mMappedProtectFlag = (PROT_READ | PROT_WRITE);
	mMappedMemPhyAddr = 0;
	mMappedMemVirAddr = NULL;
	
	do
	{
		mMemDevFd = open(MEMORY_DEVICE_DEV_PATH, (O_RDWR | O_SYNC));
		if(-1 == mMemDevFd)
		{
			mMemDevFd = open(MEMORY_DEVICE_DEV_PATH, (O_RDONLY | O_SYNC));
		}
		if(-1 == mMemDevFd)
		{
			SetCrtErrNo(errno);
			break;
		}		
		//set F_GETFD to the file descriptor flag
		{
			int iFileDescriptorFlag = fcntl(mMemDevFd, F_GETFD);
			if(-1 != iFileDescriptorFlag)
			{
				iFileDescriptorFlag |= FD_CLOEXEC;
				iRet = fcntl(mMemDevFd, F_SETFD, iFileDescriptorFlag);
				if(-1 == iRet)
				{
					PRINT_BFILE_LINENO_CRT_ERRINFO;
				}
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}

		mInited = TRUE;
	}while(FALSE);
}

CMemoryDevice::~CMemoryDevice()
{
	int iRet;

	UnmapCur();

	if(INVALID_FILE_DESCRIPTOR != mMemDevFd)
	{
		iRet = close(mMemDevFd);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		mMemDevFd = INVALID_FILE_DESCRIPTOR;
	}
}

VOID CMemoryDevice::UnmapCur()
{
	INT_t iRet;
	
	do
	{
		if(mMappedMemVirAddr)
		{
			iRet = munmap(mMappedMemVirAddr, mMappedSize);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			mMappedMemVirAddr = NULL;
			mMappedSize = 0;
		}
	}while(FALSE);
}

INT_t CMemoryDevice::ReadMemory(void * pMemAddr, size_t sizeToRead, PBYTE pDataBuf)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(FALSE == mInited)
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}

		if(NULL == pDataBuf || 0 >= sizeToRead)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		UnmapCur();

		mMappedProtectFlag = PROT_READ;
		mMappedMemPhyAddr = (((UINT32_t)pMemAddr) & (~(mMemoryPageSize-1)));
		mMappedSize = ((((UINT32_t)pMemAddr) - mMappedMemPhyAddr + sizeToRead) + (mMemoryPageSize-1)) / 
			mMemoryPageSize * mMemoryPageSize;
		mMappedMemVirAddr = mmap(0, mMappedSize, mMappedProtectFlag, MAP_SHARED, mMemDevFd, 
			mMappedMemPhyAddr);
		if(MAP_FAILED == mMappedMemVirAddr)
		{
			mMappedMemVirAddr = NULL;
			iOutRet = ERROR_MEM_MAP_FAIL;
			//PRINT_BFILE_LINENO_CRT_ERRINFO;
			LOGP("MappedMemPhyAddr=0x%08llx,MappedSize=%d\n", mMappedMemPhyAddr, mMappedSize);
			break;
		}

		memcpy(pDataBuf, 
			(PVOID)(((UINT32_t)mMappedMemVirAddr)+(((UINT32_t)pMemAddr) - mMappedMemPhyAddr)), 
			sizeToRead);

		UnmapCur();
	}while(FALSE);

	return iOutRet;
}

CMemDevMapper::CMemDevMapper()
{

}

CMemDevMapper::~CMemDevMapper()
{

}

INT_t CMemDevMapper::ReadPhyMem(PVOID pPhyMemAddr, size_t sizeToRead, PBYTE pDataBuf)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	off_t MappedMemPhyAddr;
	size_t MappedSize;
	PVOID MappedMemVirAddr = NULL;

	do
	{
		if(FALSE == mInited)
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}

		if(NULL == pDataBuf || 0 >= sizeToRead)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		MappedMemPhyAddr = (((UINT32_t)pPhyMemAddr) & (~(mMemoryPageSize-1)));
		MappedSize = ((((UINT32_t)pPhyMemAddr) - MappedMemPhyAddr + sizeToRead) + (mMemoryPageSize-1)) / 
			mMemoryPageSize * mMemoryPageSize;
		MappedMemVirAddr = mmap(0, MappedSize, PROT_READ, MAP_SHARED, mMemDevFd, MappedMemPhyAddr);
		if(MAP_FAILED == MappedMemVirAddr)
		{
			MappedMemVirAddr = NULL;
			iOutRet = ERROR_MEM_MAP_FAIL;
			//PRINT_BFILE_LINENO_CRT_ERRINFO;
			LOGP("MappedMemPhyAddr=0x%08llx,MappedSize=%d\n", MappedMemPhyAddr, MappedSize);
			break;
		}

		memcpy(pDataBuf, 
			(PVOID)(((UINT32_t)MappedMemVirAddr)+(((UINT32_t)pPhyMemAddr) - MappedMemPhyAddr)), 
			sizeToRead);
	}while(FALSE);

	if(MappedMemVirAddr)
	{
		iRet = munmap(MappedMemVirAddr, MappedSize);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		MappedMemVirAddr = NULL;
	}

	return iOutRet;
}

INT_t CMemDevMapper::MapPhyMem(PVOID pPhyMemAddr, size_t sizeToMap, PVOID * ppMappedVirAddr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	C_UNUSED(iRet);

	do
	{
		if(FALSE == mInited)
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}

		if(NULL == ppMappedVirAddr || 0 >= sizeToMap)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		UnmapCur();

		mMappedMemPhyAddr = (((UINT32_t)pPhyMemAddr) & (~(mMemoryPageSize-1)));
		mMappedSize = ((((UINT32_t)pPhyMemAddr) - mMappedMemPhyAddr + sizeToMap) + (mMemoryPageSize-1)) / 
			mMemoryPageSize * mMemoryPageSize;
		mMappedMemVirAddr = mmap(0, mMappedSize, PROT_READ | PROT_WRITE, MAP_SHARED, mMemDevFd, mMappedMemPhyAddr);
		if(MAP_FAILED == mMappedMemVirAddr)
		{
			mMappedMemVirAddr = NULL;
			iOutRet = ERROR_MEM_MAP_FAIL;
			//PRINT_BFILE_LINENO_CRT_ERRINFO;
			LOGP("MappedMemPhyAddr=0x%08llx,MappedSize=%d\n", mMappedMemPhyAddr, mMappedSize);
			break;
		}
		*ppMappedVirAddr = (PVOID)(((UINT_t)mMappedMemVirAddr) + (((UINT32_t)pPhyMemAddr) - mMappedMemPhyAddr));
	}while(FALSE);

	return iOutRet;
}

PVOID CMemDevMapper::getMappedVirAddr()
{
	return mMappedMemVirAddr;
}

