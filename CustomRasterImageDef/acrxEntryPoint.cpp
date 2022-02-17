#include "StdAfx.h"
#include "resource.h"

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

// include the libs
// for Atil
#pragma comment (lib, "AdImaging.lib")
#pragma comment (lib, "AdIntImgServices.lib")

#include "StdAfx.h"
#include "AcDbMyRasterImageDef.h"

class CMyTest1App : public AcRxArxApp 
{
public:
	CMyTest1App () : AcRxArxApp () 
	{
	}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) 
	{
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		acrxDynamicLinker->loadModule(L"acismobj24.dbx",true); 
		AcDbMyRasterImageDef::rxInit();
		acrxBuildClassHierarchy();
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) 
	{
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kLoadDwgMsg (void *pkt) 
	{
		AcRx::AppRetCode retCode =AcRxArxApp::On_kLoadDwgMsg (pkt);
		return (retCode) ;
	}
	
	virtual void RegisterServerComponents () 
	{
	}
	
	static void AdskMyTestEmbedImage(void)
	{
		AcDbMyRasterImageDef *pImageDef = new AcDbMyRasterImageDef();
		pImageDef->setEmbeddedImage(ACRX_T("D:\\TestFiles\\MyTexture.jpg"));
		Acad::ErrorStatus es = InsertImageInDwg(pImageDef);
		if(es != Acad::eOk)
		{
			delete pImageDef;
			return;
		}
	}
 
	static Acad::ErrorStatus InsertImageInDwg(AcDbRasterImageDef *pImageDef)
	{
		Acad::ErrorStatus es;
		if(! pImageDef->isLoaded())
		{
			es = pImageDef->load();
			if(es != Acad::eOk)
				return es;
		}
 
		AcApDocument *pActiveDoc = acDocManager->mdiActiveDocument();
		AcDbDatabase *pDB = pActiveDoc->database();
 
		// Get the image dictionary
		// Create it if not available already
		AcDbObjectId dictID
						= AcDbRasterImageDef::imageDictionary(pDB);
		if(dictID == AcDbObjectId::kNull)
		{
			es = AcDbRasterImageDef::createImageDictionary(
													  pDB, dictID);
			if(es != Acad::eOk)
				return es;
		}
 
		AcDbDictionary* pDict;
		es = acdbOpenObject(pDict, dictID, AcDb::kForWrite);
		if(es != Acad::eOk)
				return es;
 
		ACHAR *DICT_NAME = ACRX_T("RASTER_USING_BUFFER");
		BOOL bExist = pDict->has(DICT_NAME);
		AcDbObjectId objID = AcDbObjectId::kNull;
		if (!bExist)
		{
			es = pDict->setAt(DICT_NAME, pImageDef, objID);
			if(es != Acad::eOk)
			{
				pDict->close();
				return es;
			}
		}
		else
		{
			pDict->getAt(DICT_NAME,
						(AcDbObject*&)pImageDef,
						AcDb::kForWrite);
			objID = pImageDef->objectId();
		}
 
		// close Dictionary and Definition.
		pDict->close();
		pImageDef->close();
 
		// Create a raster image using the RasterImage Def
		AcDbRasterImage* pImage = new AcDbRasterImage;
		es = pImage->setImageDefId(objID);
		if (es != Acad::eOk)
		{
			delete pImage;
			return es;
		}
 
		// Add the raster image to the model space
		AcDbBlockTable* pBlockTable;
		AcDbBlockTableRecord* pBTRecord;
		es = acdbCurDwg()->getBlockTable(pBlockTable,
										 AcDb::kForRead);
		assert(es == Acad::eOk);
		es = pBlockTable->getAt(ACDB_MODEL_SPACE,
								pBTRecord,
								AcDb::kForWrite);
		assert(es == Acad::eOk);
 
		es = pBTRecord->appendAcDbEntity(pImage);
		assert(es == Acad::eOk);
 
		pBTRecord->close();
		pBlockTable->close();
 
		AcDbObjectId entID = pImage->objectId();
 
		AcDbObjectPointer<AcDbRasterImageDefReactor>
											rasterImageDefReactor;
		rasterImageDefReactor.create();
 
		es = rasterImageDefReactor->setOwnerId(pImage->objectId());
		if (es == Acad::eOk)
		{
			AcDbObjectId defReactorId;
			es = curDoc()->database()->addAcDbObject(
								defReactorId,
								rasterImageDefReactor.object());
 
			if (es == Acad::eOk)
			{
				pImage->setReactorId(defReactorId);
				AcDbObjectPointer<AcDbRasterImageDef> rasterImagedef
						  (pImage->imageDefId(), AcDb::kForWrite);
				if (rasterImagedef.openStatus() == Acad::eOk)
				{
					rasterImagedef->addPersistentReactor
												   (defReactorId);
				
				}
			}
		}
 
		pImageDef->close();
		pImage->close();

		return Acad::eOk;
	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CMyTest1App)

ACED_ARXCOMMAND_ENTRY_AUTO(CMyTest1App, AdskMyTest, EmbedImage, EmbedImage, ACRX_CMD_MODAL, NULL)
