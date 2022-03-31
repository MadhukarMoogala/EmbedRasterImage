// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#pragma warning( disable : 4278 )

#include <windows.h>
#include <stdio.h>

#include "RgbModel.h"
#include "FileWriteDescriptor.h"
#include "RowProviderInterface.h"
#include "FileSpecifier.h"
#include "DataModelAttributes.h"
#include "DataModel.h"
#include "Image.h"
#include "dbmleaderstyle.h"

#include "StdAfx.h"
#include "resource.h"
#include "AcDbMyRasterImageDef.h"

// include the libs
// for Atil
#pragma comment (lib, "AdImaging.lib")
#pragma comment (lib, "AdIntImgServices.lib")

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CdbxApp : public AcRxDbxApp {

public:
	CdbxApp () : AcRxDbxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode = AcRxDbxApp::On_kInitAppMsg(pkt);
		acrxDynamicLinker->loadModule(L"acismobj24.dbx", true);
		AcDbMyRasterImageDef::rxInit();
		acrxBuildClassHierarchy();
		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadAppMsg (pkt) ;
		deleteAcRxClass(AcDbMyRasterImageDef::desc());
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CdbxApp)

