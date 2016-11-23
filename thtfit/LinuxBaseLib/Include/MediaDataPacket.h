#ifndef	_MEDIA_DATA_PACKET_H_
#define	_MEDIA_DATA_PACKET_H_

//#include <utils/RefBase.h>
#include <BaseTypeDef.h>
#include <MediaDataType.h>

class CMediaDataPacket 
{
public:
	CMediaDataPacket();
	CMediaDataPacket(size_t PreAllocSize);
	virtual ~CMediaDataPacket();
	virtual operator PBYTE ();
	virtual size_t getDataBufSize();
	virtual INT_t setAvailableDataSize(size_t AvailableDataSize);
	virtual size_t getAvailableDataSize();
	virtual INT_t setMediaDataPacketType(MEDIA_DATA_PACKET_TYPE eMediaDataPacketType);
	virtual INT_t getMediaDataPacketType(MEDIA_DATA_PACKET_TYPE & OUT eMediaDataPacketType);
protected:
	PBYTE m_pDataBuf;
	size_t m_DataBufSize;
	size_t m_AvailableDataSize;
	MEDIA_DATA_PACKET_TYPE m_eMediaDataPacketType;
};

#endif	//_MEDIA_DATA_PACKET_H_

