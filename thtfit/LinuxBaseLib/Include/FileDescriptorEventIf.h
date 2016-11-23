#ifndef	_FILE_DESCRIPTOR_EVENT_IF_H_
#define	_FILE_DESCRIPTOR_EVENT_IF_H_

#include <BaseTypeDef.h>

class IFileDescriptorEventIf
{
public:
	typedef enum
	{
		FD_EVT_NONE = 0x00,
		FD_EVT_READABLE = 0x01,
		FD_EVT_WRITABLE = 0x02,
		FD_EVT_EXCEPTION = 0x04,
	}FD_EVENT_FLAGS, *P_FD_EVENT_FLAGS;
public:
	virtual ~IFileDescriptorEventIf() {};
	virtual INT_t OnFdEvent(int iFd, CONST IFileDescriptorEventIf::FD_EVENT_FLAGS FdEvtFlags) = 0;
};

#endif	//_FILE_DESCRIPTOR_EVENT_IF_H_

