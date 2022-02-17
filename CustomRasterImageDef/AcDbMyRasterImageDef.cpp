//-----------------------------------------------------------------------------
//----- AcDbMyRasterImageDef.cpp : Implementation of AcDbMyRasterImageDef
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "AcDbMyRasterImageDef.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 AcDbMyRasterImageDef::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	AcDbMyRasterImageDef, AcDbRasterImageDef,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, ACDBMYRASTERIMAGEDEF,
MYTEST1APP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
AcDbMyRasterImageDef::AcDbMyRasterImageDef () : AcDbRasterImageDef () 
{
	m_pAtilImage = NULL;
}

AcDbMyRasterImageDef::~AcDbMyRasterImageDef () 
{
	if(m_pAtilImage != NULL)
	{
		delete m_pAtilImage;
		m_pAtilImage = NULL;
	}
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus AcDbMyRasterImageDef::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbRasterImageDef::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (AcDbMyRasterImageDef::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	if(m_pAtilImage)
	{
		Atil::Size size = m_pAtilImage->size();
		Int32 width = size.width;
		Int32 height = size.height;

		pFiler->writeInt32(width);
		pFiler->writeInt32(height);

		// Write the image data on to the Atil image
		// using an Image Context
		Atil::Offset upperLeft(0,0);
		Atil::ImageContext *pImgContext = m_pAtilImage->createContext(
										Atil::ImageContext::kRead,
										size,
										upperLeft
										);
		if (pImgContext != NULL)
		{
			for (int xf=0;xf<width;xf++)
			{
				for (int yf=0;yf<height;yf++)
				{
					Atil::RgbColor p;
					p = pImgContext->get32(xf, yf);
					pFiler->writeInt32(p.packed);
				}
			}
 
		}
		pImgContext->flush();
		delete pImgContext;
	}

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus AcDbMyRasterImageDef::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbRasterImageDef::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > AcDbMyRasterImageDef::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < AcDbMyRasterImageDef::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	
	Int32 width = 0;
	Int32 height = 0;

	pFiler->readInt32(&width);
	pFiler->readInt32(&height);

	// Create an Atil::Image.
    // The AcDbRasterImageDef::setImage requires it
    Atil::ImagePixel initialImage;
    initialImage.setToZero();
    initialImage.type = Atil::DataModelAttributes::kBgra;
    initialImage.value.rgba = 0xff000000;
 
    Atil::Size size(width, height);
    const Atil::RgbModel *pDm = new Atil::RgbModel(
        Atil::RgbModelAttributes::k4Channels,
        Atil::DataModelAttributes::kBlueGreenRedAlpha);
 
	if(m_pAtilImage != NULL)
	{
		delete m_pAtilImage;
		m_pAtilImage = NULL;
	}
    m_pAtilImage = new Atil::Image(size, pDm, initialImage);

	// Write the image data on to the Atil image
	// using an Image Context
	Atil::Offset upperLeft(0,0);
	Atil::ImageContext *pImgContext = m_pAtilImage->createContext(
									Atil::ImageContext::kWrite,
									size,
									upperLeft
									);
	if (pImgContext != NULL)
	{
		for (int xf=0;xf<width;xf++)
		{
			for (int yf=0;yf<height;yf++)
			{
				Int32 value;
				pFiler->readInt32(&value);
				pImgContext->put32(xf, yf, value);
			}
		}
	}
	pImgContext->flush();
	delete pImgContext;

	setImage (m_pAtilImage, NULL) ;

	return (pFiler->filerStatus ()) ;
}

//----- deepClone
Acad::ErrorStatus AcDbMyRasterImageDef::subDeepClone (AcDbObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary) const {
	assertReadEnabled () ;
	return (AcDbRasterImageDef::subDeepClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
}

//----- wblockClone
Acad::ErrorStatus AcDbMyRasterImageDef::subWblockClone (AcRxObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary) const {
	assertReadEnabled () ;
	return (AcDbRasterImageDef::subWblockClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
}

 // Create directly from in-memory IE image and/or file descriptor.
Acad::ErrorStatus  AcDbMyRasterImageDef::setEmbeddedImage(const ACHAR* imageFilePath)
{
    AcString imagePath(imageFilePath);
 
    // Load the image to get access to its image buffer
    AcTcImage tc;
    tc.Load(imagePath);
 
    HBITMAP bmp=0;
    tc.GetHBITMAP(RGB(0xff,0xff,0xff),bmp);
 
    if (bmp == NULL)
		return Acad::eFileNotFound;
 
    BITMAP _bmp={0};
    GetObject(bmp,sizeof BITMAP,&_bmp);
 
    HDC hdcScr=GetDC(NULL);
    HDC    hdcMem=CreateCompatibleDC(hdcScr);
    SelectObject(hdcMem,bmp);
 
    // Create an Atil::Image.
    // The AcDbRasterImageDef::setImage requires it
    Atil::ImagePixel initialImage;
    initialImage.setToZero();
	initialImage.type = Atil::DataModelAttributes::kBgra;
    initialImage.value.rgba = 0xff000000;
 
    Atil::Size size(_bmp.bmWidth, _bmp.bmHeight);
    const Atil::RgbModel *pDm = new Atil::RgbModel(
        Atil::RgbModelAttributes::k4Channels,
        Atil::DataModelAttributes::kBlueGreenRedAlpha);
 
	if(m_pAtilImage != NULL)
	{
		delete m_pAtilImage;
		m_pAtilImage = NULL;
	}
    m_pAtilImage = new Atil::Image(size, pDm, initialImage);

	
 
    // Write the image data on to the Atil image
    // using an Image Context
    Atil::Offset upperLeft(0,0);
    Atil::ImageContext *pImgContext
        = m_pAtilImage->createContext(
                                    Atil::ImageContext::kWrite,
                                    size,
                                    upperLeft
                                    );
    if (pImgContext != NULL)
    {
        for (int xf=0;xf<_bmp.bmWidth;xf++)
        {
            for (int yf=0;yf<_bmp.bmHeight;yf++)
            {
                BYTE alpha=0x0;
 
                COLORREF pix=GetPixel(hdcMem,xf,yf);
 
                BYTE rr = (pix&0xff);
                BYTE gg = (pix>>8)&0xff;
                BYTE bb = (pix>>16)&0xff;
 
                // Alpha channel to account for transparency
                if ((rr!=0xff) || (gg!=0xff) || (bb!=0xff))
                    alpha=0xff;
 
                Atil::RgbColor p;
                p.set(rr, gg, bb, alpha);
                pImgContext->put32(xf, yf, p);
            }
        }
    }
    pImgContext->flush();
    delete pImgContext;
 
    bool isImageValid = m_pAtilImage->isValid();
    assert(isImageValid);
 
    // Cleanup
    DeleteDC(hdcMem);
    ReleaseDC(NULL,hdcScr);
 
    DeleteObject( bmp);

	return setImage (m_pAtilImage, NULL) ;
}
