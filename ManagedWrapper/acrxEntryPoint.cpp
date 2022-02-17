//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "MyRasterImageDefMgd.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("Adsk")

//-----------------------------------------------------------------------------
static AcMgObjectFactoryBase** g_PEs = NULL;

//----- ObjectARX EntryPoint
class CManagedWrapperApp : public AcRxArxApp 
{

public:
	CManagedWrapperApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) 
	{
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxDbxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		AcMgObjectFactoryBase* PEs[] = 
		{
			new AcMgObjectFactory<ADN::CustomWrapper::MyRasterImageDefMgd, AcDbMyRasterImageDef>(), 
				NULL
		};

		g_PEs = PEs;

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) 
	{
		// TODO: Add your code here
		// clean up
		int i=0;
		while (g_PEs[i] != NULL)
			delete g_PEs[i++];

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxDbxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		return (retCode) ;
	}

	virtual void RegisterServerComponents () 
	{

	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CManagedWrapperApp)

