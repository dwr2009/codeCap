#include <MediaDataPacket.h>
#include <Compiler.h>
#include <BaseErrDef.h>

#define	MEDIA_DATA_PACKET_MAX_SIZE				(4*1024*1024)

CMediaDataPacket::CMediaDataPacket()
{
	m_pDataBuf = NULL;
	m_DataBufSize = 0;
	m_AvailableDataSize = 0;
	m_eMediaDataPacketType = MDP_TYPE_UNKNOWN;
}

CMediaDataPacket::CMediaDataPacket(size_t PreAllocSize)
{
	CMediaDataPacket();

	if(CC_LIKELY(0 < PreAllocSize && MEDIA_DATA_PACKET_MAX_SIZE >= PreAllocSize))
	{
		m_pDataBuf = new BYTE[PreAllocSize];
		if(CC_LIKELY(m_pDataBuf))
		{
			m_DataBufSize = PreAllocSize;
		}
	}
}

CMediaDataPacket::~CMediaDataPacket()
{
	if(CC_LIKELY(m_pDataBuf))
	{
		delete m_pDataBuf;
		m_pDataBuf = NULL;
		m_DataBufSize = 0;
	}
}

CMediaDataPacket::operator PBYTE ()
{
	return m_pDataBuf;
}

size_t CMediaDataPacket::getDataBufSize()
{
	return m_DataBufSize;
}

INT_t CMediaDataPacket::setAvailableDataSize(size_t AvailableDataSize)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(CC_UNLIKELY(0 > AvailableDataSize || m_DataBufSize < AvailableDataSize))
		{
			iOutRet = ERROR_OUT_OF_RANGE;
			break;
		}
		m_AvailableDataSize = AvailableDataSize;
	}while(FALSE);

	return iOutRet;
}

size_t CMediaDataPacket::getAvailableDataSize()
{
	return m_AvailableDataSize;
}

INT_t CMediaDataPacket::setMediaDataPacketType(MEDIA_DATA_PACKET_TYPE eMediaDataPacketType)
{
	m_eMediaDataPacketType = eMediaDataPacketType;

	return ERROR_SUCCESS;
}

INT_t CMediaDataPacket::getMediaDataPacketType(MEDIA_DATA_PACKET_TYPE & OUT eMediaDataPacketType)
{
	eMediaDataPacketType = m_eMediaDataPacketType;

	return ERROR_SUCCESS;
}

