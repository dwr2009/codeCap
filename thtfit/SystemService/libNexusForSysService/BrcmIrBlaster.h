

#ifndef __BRCMIRBLASTER_H__
#define __BRCMIRBLASTER_H__
#include "../Api/IrBlasterInterface.h"
#include "nexus_platform.h"
#include "nexus_ir_blaster.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class BrcmIrBlaster: virtual public IrBlasterInterface
{
protected:
	BrcmIrBlaster();
public:
	virtual ~BrcmIrBlaster();
	static IrBlasterInterface *getIrBlasterInstance();
	virtual int sendIrCodeOut(int iIRCode);
	virtual int sendIrLevelOut(char *pIRLevelCode);	
protected:
	virtual int startIrBlaster();
	virtual int stopIrBlaster();
protected:
	NEXUS_IrBlasterHandle m_hIrBlaster;
	NEXUS_IrBlasterSettings m_setIrBlaster;
	NEXUS_IrBlasterMode m_iIrBlasterMode;
	bool m_bEnableHeaderPulse;
};


#endif

