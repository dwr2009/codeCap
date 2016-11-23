#include "Api/GlobalStatus.h"

CGlobalStatus::CGlobalStatus(LPCSTR pszStatusFilePath, size_t sizeStatusData) : 
	m_StatusMapFile(pszStatusFilePath, sizeStatusData)
{
	m_strStatusFilePath = pszStatusFilePath;
}

CGlobalStatus::~CGlobalStatus()
{
}

