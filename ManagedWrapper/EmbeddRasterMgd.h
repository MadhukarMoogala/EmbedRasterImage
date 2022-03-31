// (C) Copyright 2005-2007 by Autodesk, Inc. 
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

//-----------------------------------------------------------------------------
//- MGripMgdWrapper.h
#pragma once
#include "../dbx/AcDbMyRasterImageDef.h"

//-----------------------------------------------------------------------------
using namespace System;
using namespace Autodesk::AutoCAD::Geometry;
using namespace Autodesk::AutoCAD::DatabaseServices;

//-----------------------------------------------------------------------------
//- Wizard Generated Custom Object .NET Wrapper
//- See the mgPoly in the ObjectARX SDK for further information
//-----------------------------------------------------------------------------
namespace ADN {
	namespace CustomWrapper
	{
		[Autodesk::AutoCAD::Runtime::Wrapper("MyRasterImageDef")]
		public ref class MyRasterImageDefMgd : public Autodesk::AutoCAD::DatabaseServices::RasterImageDef
		{
		public:
			//- Constructor
			MyRasterImageDefMgd();

		internal:
			MyRasterImageDefMgd(System::IntPtr unmanagedPointer, bool bAutoDelete);

			//- Returns the unmanaged ARX Object
			inline AcDbMyRasterImageDef* GetImpObj() {
				return (static_cast<AcDbMyRasterImageDef*>(UnmanagedObject.ToPointer()));
			}

		public:
			void SetEmbeddedImage(System::String^ imageFilePath);
		};
	}
}