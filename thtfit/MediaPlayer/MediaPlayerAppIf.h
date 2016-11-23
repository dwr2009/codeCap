#ifndef	_MEDIA_PLAYER_APP_IF_H_
#define	_MEDIA_PLAYER_APP_IF_H_

#include <SharedPtr.h>
#include <WeakPtr.h>
#include <QWeakPointer>
#include <BaseTypeDef.h>
#include <TiXmlDoc2.h>

using namespace CppBase;

class CGeneralApp
{
public:
	int m_iArgc;
	char ** m_ppArgv;
public:
	CGeneralApp(int argc, char * argv[]);
	virtual ~CGeneralApp();
	virtual void setThisWp(const SharedPtr <CGeneralApp> GeneralApp_wp);
	virtual void getCmdLineArgs(OUT int & argc, OUT char ** & argv);	
	virtual SharedPtr <CTinyXmlDoc3> getXmlCfgDoc();
	static SharedPtr <CGeneralApp> getGeneralApp();
private:
	static WeakPtr <CGeneralApp> m_this_wp;
};

#endif	//_MEDIA_PLAYER_APP_IF_H_

