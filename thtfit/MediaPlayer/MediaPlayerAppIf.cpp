#include "MediaPlayerAppIf.h"

WeakPtr <CGeneralApp> CGeneralApp::m_this_wp;

CGeneralApp::CGeneralApp(int argc, char * argv[])
{
	m_iArgc = argc;
	m_ppArgv = argv;
}

CGeneralApp::~CGeneralApp()
{
}

void CGeneralApp::setThisWp(const SharedPtr <CGeneralApp> GeneralApp_wp)
{
	m_this_wp = GeneralApp_wp;
}

void CGeneralApp::getCmdLineArgs(OUT int & argc, OUT char ** & argv)
{
	argc = m_iArgc;
	argv = m_ppArgv;
}

SharedPtr <CTinyXmlDoc3> CGeneralApp::getXmlCfgDoc()
{
	return (SharedPtr <CTinyXmlDoc3>());
}

SharedPtr <CGeneralApp> CGeneralApp::getGeneralApp()
{
	return m_this_wp.Promote();
}

