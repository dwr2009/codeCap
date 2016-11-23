

#ifndef __IR_BLASTER_INTERFACE_H__
#define __IR_BLASTER_INTERFACE_H__

class IrBlasterInterface
{
public:
	IrBlasterInterface():m_bIsWork(false){};
	virtual ~IrBlasterInterface(){};
	virtual int sendIrCodeOut(int iIRCode)=0;
	virtual int sendIrLevelOut(char *pIRLevelCode)=0;
	virtual bool isWork(){return m_bIsWork;};
protected:
	virtual int configIrBlaster(void *pParameter1,void *pParameter2)
	{
		/*
		BSTD_UNUSED(pParameter1);
		BSTD_UNUSED(pParameter2);
		*/
		return 0;
	};
	
	virtual int startIrBlaster(){return 0;};
	virtual int stopIrBlaster(){return 0;};
protected:
	bool m_bIsWork;	
};

IrBlasterInterface *getBrcmIrBlasterInstance();

static IrBlasterInterface* getIrBlasterInstance()
{
	return getBrcmIrBlasterInstance();
}

#endif



