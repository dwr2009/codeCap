#ifndef	_CRTERR_H_
#define	_CRTERR_H_

#ifdef	__cplusplus

class CCrtErr
{
public:
	CCrtErr();
	~CCrtErr();
	void SetCrtErrNo(int iCrtErrNo);
	int GetCrtErrNo();
protected:
	int m_iCrtErrNo;
};

#endif	//__cplusplus

#endif	//_CRTERR_H_

