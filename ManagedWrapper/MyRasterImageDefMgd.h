//-----------------------------------------------------------------------------
//- asdkCustomCircleMgd.h
#pragma once

#include "../CustomRasterImageDef/AcDbMyRasterImageDef.h"

//-----------------------------------------------------------------------------
using namespace System ;
using namespace Autodesk::AutoCAD::Geometry ;
using namespace Autodesk::AutoCAD::DatabaseServices ;

//-----------------------------------------------------------------------------
//- Wizard Generated Custom Object .NET Wrapper
//- See the mgPoly in the ObjectARX SDK for further information
//-----------------------------------------------------------------------------
namespace ADN {
	namespace CustomWrapper 
	{
		[Autodesk::AutoCAD::Runtime::Wrapper("MyRasterImageDef")]
		public ref class MyRasterImageDefMgd : 
			public Autodesk::AutoCAD::DatabaseServices::RasterImageDef
			{

		public:
			//- Constructor
			MyRasterImageDefMgd () ;

		internal:

			MyRasterImageDefMgd (System::IntPtr unmanagedPointer, bool bAutoDelete) ;

			//- Returns the unmanaged ARX Object
			inline AcDbMyRasterImageDef *GetImpObj () 
			{
				return (static_cast<AcDbMyRasterImageDef *>(UnmanagedObject.ToPointer ())) ;
			}

		public:
			
			void SetEmbeddedImage(System::String^ imageFilePath);

		} ;
	}
}