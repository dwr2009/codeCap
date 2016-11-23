#ifndef	_EVENT_APP_H_
#define	_EVENT_APP_H_

#include "EventCenter.h"

class CEventApp : public CEventCenter
{
public:
	CEventApp();
	virtual ~CEventApp();
	virtual INT InitInstance(int argc, char * argv[]);
	virtual INT ExitInstance();
	virtual INT Run(int argc, char * argv[]);
};

CEventApp * GetApp();

#endif	//_EVENT_APP_H_

