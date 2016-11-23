#include "SysPropOps.h"
#include "SysBaseSrvApi.h"
#include <Checksum.h>
#include <stdlib.h>

// {6ED35B45-104D-40c2-9D95-A82DF36ACB9C}
static const GUID SysPropDataGuid = 
	{ 0x6ed35b45, 0x104d, 0x40c2, { 0x9d, 0x95, 0xa8, 0x2d, 0xf3, 0x6a, 0xcb, 0x9c } };

CSysPropOps::CSysPropValue::CSysPropValue()
{
	m_bPersistent = TRUE;
}

CSysPropOps::CSysPropValue::~CSysPropValue()
{
}

CSysPropOps::CSysPropOps()
{
	m_SysPropDataFile_sp = SharedPtr <CFile> (new CFile);	
}

CSysPropOps::~CSysPropOps()
{
}

INT_t CSysPropOps::Load(LPCSTR pszSysPropDataFilePath, BOOL_t bClearCurVal/* = TRUE*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, BytesRead;
	GUID FileObjGuid;
	UINT8_t ChksumValInFile = 0, ChksumValCalculated = 0;
	BYTE VersionInFile;
	char szKeyBuf[16*1024];
	char szValueBuf[16*1024];
	SharedPtr <CSysPropValue> SysPropVal_sp;

	do
	{
		//init checking
		if(m_SysPropDataFile_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(bClearCurVal)
		{
			//reset map
			m_SysPropKeyValMap.clear();
		}
		//Close first
		iRet = m_SysPropDataFile_sp->Close();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}		
		//then reopen it
		iRet = m_SysPropDataFile_sp->Open(pszSysPropDataFilePath, "rb");
		if(ERROR_SUCCESS != iRet)
		{
			if(ENOENT == m_SysPropDataFile_sp->GetLastCrtErrNo())
			{
				iOutRet = ERROR_NOT_FOUND;
			}
			else
			{
				iOutRet = iRet;
			}
			break;
		}
		//File signature
		BytesRead = 0;
		iRet = m_SysPropDataFile_sp->Read((PBYTE)(&FileObjGuid), sizeof(FileObjGuid), &BytesRead);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(sizeof(FileObjGuid) != BytesRead)
		{
			iOutRet = ERROR_FILE_READ_FAIL;
			break;
		}
		if(0 != memcmp(&FileObjGuid, &SysPropDataGuid, sizeof(GUID)))
		{
			iOutRet = ERR_INVALID_FILE_SIGNATURE;
			break;
		}
		//read checksum
		iRet = m_SysPropDataFile_sp->Read(&ChksumValInFile, sizeof(ChksumValInFile), &BytesRead);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(sizeof(ChksumValInFile) != BytesRead)
		{
			iOutRet = ERROR_FILE_READ_FAIL;
			break;
		}
		//LOG_BLINE("\n");
		//read version
		iRet = m_SysPropDataFile_sp->Read(&VersionInFile, sizeof(VersionInFile), &BytesRead);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//LOG_BLINE("\n");
		if(sizeof(VersionInFile) != BytesRead)
		{
			iOutRet = ERROR_FILE_READ_FAIL;
			break;
		}		
		//LOG_BLINE("VersionInFile=%d\n", VersionInFile);
		if(1 != VersionInFile)
		{
			iOutRet = ERR_INVALID_FILE_VERSION;
			break;
		}
		//LOG_BLINE("\n");
		ChksumValCalculated = CalcCrc8(ChksumValCalculated, &VersionInFile, sizeof(VersionInFile));
		//read key-value pair chunk
		while(TRUE)
		{
			UINT32_t StrLenWithNull;
			SysPropVal_sp = SharedPtr <CSysPropValue> (new CSysPropValue);
			if(SysPropVal_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			SysPropVal_sp->m_bPersistent = TRUE;
			//read key
			iRet = m_SysPropDataFile_sp->Read((PBYTE)(&StrLenWithNull), sizeof(StrLenWithNull), &BytesRead);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if(0 == BytesRead)	//EOF
			{
				break;
			}
			if(sizeof(StrLenWithNull) != BytesRead)
			{
				iOutRet = ERROR_FILE_READ_FAIL;
				break;
			}
			ChksumValCalculated = CalcCrc8(ChksumValCalculated, (PBYTE)(&StrLenWithNull), sizeof(StrLenWithNull));
			if(sizeof(szKeyBuf) < StrLenWithNull)
			{
				iOutRet = ERR_INVALID_FILE_DATA;
				break;
			}
			iRet = m_SysPropDataFile_sp->Read((PBYTE)(szKeyBuf), StrLenWithNull, &BytesRead);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if((typeof(BytesRead))StrLenWithNull != BytesRead)
			{
				iOutRet = ERROR_FILE_READ_FAIL;
				break;
			}
			szKeyBuf[sizeof(szKeyBuf)-1] = '\0';
			ChksumValCalculated = CalcCrc8(ChksumValCalculated, (PBYTE)(szKeyBuf), StrLenWithNull);			
			//read value
			iRet = m_SysPropDataFile_sp->Read((PBYTE)(&StrLenWithNull), sizeof(StrLenWithNull), &BytesRead);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if(sizeof(StrLenWithNull) != BytesRead)
			{
				iOutRet = ERROR_FILE_READ_FAIL;
				break;
			}
			ChksumValCalculated = CalcCrc8(ChksumValCalculated, (PBYTE)(&StrLenWithNull), sizeof(StrLenWithNull));
			if(sizeof(szValueBuf) < StrLenWithNull)
			{
				iOutRet = ERR_INVALID_FILE_DATA;
				break;
			}
			iRet = m_SysPropDataFile_sp->Read((PBYTE)(szValueBuf), StrLenWithNull, &BytesRead);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if((typeof(BytesRead))StrLenWithNull != BytesRead)
			{
				iOutRet = ERROR_FILE_READ_FAIL;
				break;
			}
			szValueBuf[sizeof(szValueBuf)-1] = '\0';
			ChksumValCalculated = CalcCrc8(ChksumValCalculated, (PBYTE)(szValueBuf), StrLenWithNull);
			SysPropVal_sp->m_strValue = szValueBuf;
			//put it into a map table
			try
			{
				m_SysPropKeyValMap[szKeyBuf] = SysPropVal_sp;
			}
			catch(std::bad_alloc & BadAllocEx)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	if(m_SysPropDataFile_sp.isValid())
	{
		iRet = m_SysPropDataFile_sp->Close();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
		}
	}

	if(ERROR_SUCCESS == iOutRet)
	{
		if(ChksumValCalculated == ChksumValInFile)
		{
			iOutRet = ERROR_SUCCESS;
		}
		else	//chksum err
		{
			iOutRet = ERR_INVALID_FILE_DATA;
		}
	}

	//LOG_BLINE("iOutRet=%d\n", iOutRet);

	return iOutRet;
}

INT_t CSysPropOps::BackupSysPropDataFile()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 512);

	do
	{
		iRet = strShellCmd.Format("cp -fa \"%s\" \"%s\"", SYS_PROP_DATA_FILE, SYS_PROP_DATA_BACKUP_FILE);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = system((LPCSTR)strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("BackupSysPropDataFile err %d,del \"%s\"\n", iRet, SYS_PROP_DATA_BACKUP_FILE);
			iRet = remove(SYS_PROP_DATA_BACKUP_FILE);
			if(0 != iRet)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			iOutRet = ERR_FILE_COPY_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOps::RecoverySysPropDataFile()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 512);

	do
	{
		iRet = strShellCmd.Format("cp -fa \"%s\" \"%s\"", SYS_PROP_DATA_BACKUP_FILE, SYS_PROP_DATA_FILE);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = system((LPCSTR)strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_FILE_COPY_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOps::Load(BOOL_t bClearCurVal/* = TRUE*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		iRet = Load(SYS_PROP_DATA_FILE, bClearCurVal);
		if(ERROR_SUCCESS == iRet)
		{
			iRet = BackupSysPropDataFile();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			break;
		}
		iRet = Load(SYS_PROP_DATA_BACKUP_FILE, bClearCurVal);
		if(ERROR_SUCCESS == iRet)
		{
			iRet = RecoverySysPropDataFile();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			break;
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			break;
		}
		iOutRet = ERROR_LOAD_FAIL;
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOps::Save()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, BytesWritten;
	UINT8_t Crc8Value = 0;
	off_t FileOff_ChkSumField;

	do
	{
		if(m_SysPropDataFile_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iRet = m_SysPropDataFile_sp->Close();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = m_SysPropDataFile_sp->Open(SYS_PROP_DATA_FILE, "wb");
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//Data Signature
		iRet = m_SysPropDataFile_sp->Write((PBYTE)(&SysPropDataGuid), sizeof(SysPropDataGuid), &BytesWritten);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(sizeof(SysPropDataGuid) != BytesWritten)
		{
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}
		//Checksum
		FileOff_ChkSumField = sizeof(SysPropDataGuid);
		iRet = m_SysPropDataFile_sp->Seek(sizeof(Crc8Value), SEEK_CUR);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		Crc8Value = 0;
		//Version
		{
			BYTE FileVersion = 1;
			iRet = m_SysPropDataFile_sp->Write(&FileVersion, sizeof(FileVersion), &BytesWritten);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if(sizeof(FileVersion) != BytesWritten)
			{
				iOutRet = ERROR_FILE_WRITE_FAIL;
				break;
			}
			Crc8Value = CalcCrc8(Crc8Value, &FileVersion, sizeof(FileVersion));
		}
		//key-value pair chunk
		{
			SYS_PROP_KEY_VAL_MAP::iterator itPropVal;
			SharedPtr <CSysPropValue> SysPropVal_sp;
			for(itPropVal = m_SysPropKeyValMap.begin(); itPropVal != m_SysPropKeyValMap.end(); itPropVal++)
			{
				SysPropVal_sp = itPropVal->second;
				if(SysPropVal_sp.isValid() && SysPropVal_sp->m_bPersistent)
				{
					UINT32_t StrLenWithNull;
					//Key
					StrLenWithNull = itPropVal->first.GetStrLength() + 1;
					iRet = m_SysPropDataFile_sp->Write((PBYTE)(&StrLenWithNull), sizeof(StrLenWithNull), &BytesWritten);
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
					if(sizeof(StrLenWithNull) != BytesWritten)
					{
						iOutRet = ERROR_FILE_WRITE_FAIL;
						break;
					}
					Crc8Value = CalcCrc8(Crc8Value, (PBYTE)(&StrLenWithNull), sizeof(StrLenWithNull));
					iRet = m_SysPropDataFile_sp->Write((PBYTE)((LPCSTR)(itPropVal->first)), StrLenWithNull, &BytesWritten);
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
					if((INT_t)StrLenWithNull != BytesWritten)
					{
						iOutRet = ERROR_FILE_WRITE_FAIL;
						break;
					}
					Crc8Value = CalcCrc8(Crc8Value, (PBYTE)((LPCSTR)(itPropVal->first)), StrLenWithNull);
					//Value
					StrLenWithNull = SysPropVal_sp->m_strValue.GetStrLength() + 1;
					iRet = m_SysPropDataFile_sp->Write((PBYTE)(&StrLenWithNull), sizeof(StrLenWithNull), &BytesWritten);
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
					if(((INT_t)(sizeof(StrLenWithNull))) != BytesWritten)
					{
						iOutRet = ERROR_FILE_WRITE_FAIL;
						break;
					}
					Crc8Value = CalcCrc8(Crc8Value, (PBYTE)(&StrLenWithNull), sizeof(StrLenWithNull));
					iRet = m_SysPropDataFile_sp->Write((PBYTE)((LPCSTR)(SysPropVal_sp->m_strValue)), StrLenWithNull, &BytesWritten);
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
					if((INT_t)StrLenWithNull != BytesWritten)
					{
						iOutRet = ERROR_FILE_WRITE_FAIL;
						break;
					}
					Crc8Value = CalcCrc8(Crc8Value, (PBYTE)((LPCSTR)(SysPropVal_sp->m_strValue)), StrLenWithNull);
				}
			}
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
		}
		//Update the checksum field
		iRet = m_SysPropDataFile_sp->Seek(FileOff_ChkSumField, SEEK_SET);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = m_SysPropDataFile_sp->Write(&Crc8Value, sizeof(Crc8Value), &BytesWritten);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(sizeof(Crc8Value) != BytesWritten)
		{
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}
	}while(FALSE);

	if(m_SysPropDataFile_sp.isValid())
	{
		iRet = m_SysPropDataFile_sp->Close();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
		}
	}

	return iOutRet;
}

INT_t CSysPropOps::setKeyValPair(LPCSTR pszKey, size_t KeyLen, LPCSTR pszVal, size_t ValueLen, BOOL_t bPersistent)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SYS_PROP_KEY_VAL_MAP::iterator itPropVal;
	SharedPtr <CSysPropValue> SysPropVal_sp;
	CString strKey;

	do
	{
		iRet = strKey.setContent(pszKey, KeyLen);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		itPropVal = m_SysPropKeyValMap.find(strKey);
		if(m_SysPropKeyValMap.end() == itPropVal)
		{
			SysPropVal_sp = SharedPtr <CSysPropValue> (new CSysPropValue);
			if(SysPropVal_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			try
			{
				//LOG_BLINE("Key=%s\n", (LPCSTR)strKey);
				m_SysPropKeyValMap[strKey] = SysPropVal_sp;
			}
			catch(std::bad_alloc & BadAllocEx)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
		}
		else
		{
			SysPropVal_sp = itPropVal->second;
		}
		iRet = SysPropVal_sp->m_strValue.setContent(pszVal, ValueLen);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//LOG_BLINE("strVal:%s\n", (LPCSTR)(SysPropVal_sp->m_strValue));
		SysPropVal_sp->m_bPersistent = bPersistent;
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOps::getKeyValPair(LPCSTR pszKey, OUT CStackBufString & strValue)
{
	INT_t iOutRet = ERROR_SUCCESS;
	SYS_PROP_KEY_VAL_MAP::iterator itPropVal;
	SharedPtr <CSysPropValue> SysPropVal_sp;
	CString strMapKey;

	do
	{
		strMapKey = pszKey;
		itPropVal = m_SysPropKeyValMap.find(strMapKey);
		if(itPropVal == m_SysPropKeyValMap.end())
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		strValue = (LPCSTR)((itPropVal->second)->m_strValue);
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOps::getById(INT_t iPropId, OUT CStackBufString & strPropName, OUT CStackBufString & strPropVal)
{
	INT_t iOutRet = ERROR_SUCCESS;
	SYS_PROP_KEY_VAL_MAP::iterator itPropValPair;
	SharedPtr <CSysPropValue> SysPropVal_sp;

	do
	{
		if(CC_UNLIKELY(0 > iPropId || iPropId >= (typeof(iPropId))m_SysPropKeyValMap.size()))
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		itPropValPair = m_SysPropKeyValMap.begin();
		while(iPropId--)
		{
			itPropValPair++;
		}
		if(itPropValPair == m_SysPropKeyValMap.end())
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		strPropName = itPropValPair->first;
		strPropVal = (LPCSTR)((itPropValPair->second)->m_strValue);
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOps::getCount(OUT INT_t & iPropCount)
{
	INT_t iOutRet = ERROR_SUCCESS;

	iPropCount = m_SysPropKeyValMap.size();
	
	return iOutRet;
}

INT_t CSysPropOps::DelKey(LPCSTR pszKey, size_t KeyLen)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CString strKey;
	SYS_PROP_KEY_VAL_MAP::iterator itPropVal;

	do
	{
		iRet = strKey.setContent(pszKey, KeyLen);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		itPropVal = m_SysPropKeyValMap.find(strKey);
		if(m_SysPropKeyValMap.end() == itPropVal)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		m_SysPropKeyValMap.erase(strKey);
	}while(FALSE);

	return iOutRet;
}

