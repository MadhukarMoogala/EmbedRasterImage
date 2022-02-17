//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "MyRasterImageDefMgd.h"

//-----------------------------------------------------------------------------
//- Template class that wraps GCHandle from mscorlib.dll
#include <gcroot.h>

//-----------------------------------------------------------------------------
//- autodesk interop header, to aid converting between unmanaged ObjectARX and managed ObjectARX.NET
#include "mgdinterop.h"

//-----------------------------------------------------------------------------
ADN::CustomWrapper::MyRasterImageDefMgd::MyRasterImageDefMgd () 
	: Autodesk::AutoCAD::DatabaseServices::RasterImageDef((System::IntPtr)new AcDbMyRasterImageDef (), true)
{

}

//-----------------------------------------------------------------------------
ADN::CustomWrapper::MyRasterImageDefMgd::MyRasterImageDefMgd (System::IntPtr unmanagedPointer, bool bAutoDelete)
	: Autodesk::AutoCAD::DatabaseServices::RasterImageDef (unmanagedPointer, bAutoDelete)
{
}

void ADN::CustomWrapper::MyRasterImageDefMgd::SetEmbeddedImage(System::String^ imageFilePath)
{
	System::IntPtr ip = System::Runtime::InteropServices::Marshal::StringToCoTaskMemUni(imageFilePath);
	AcString sImageFilePath = static_cast<const ACHAR*>(ip.ToPointer());
	System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ip);

	GetImpObj()->setEmbeddedImage(sImageFilePath);
}
