// (C) Copyright 2002-2007 by Autodesk, Inc. 
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
#pragma once

#include "StdAfx.h"
#include "AcDbMyRasterImageDef.h"
#include <tchar.h>
#include <array>

#include "Image.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 AcDbMyRasterImageDef::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	AcDbMyRasterImageDef, AcDbRasterImageDef,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, ACDBMYRASTERIMAGEDEF,
	MYTEST1APP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------
AcDbMyRasterImageDef::AcDbMyRasterImageDef() : AcDbRasterImageDef()
{
	m_pAtilImage = NULL;
}

AcDbMyRasterImageDef::~AcDbMyRasterImageDef()
{
	if (m_pAtilImage != NULL)
	{
		delete m_pAtilImage;
		m_pAtilImage = NULL;
	}
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus AcDbMyRasterImageDef::dwgOutFields(AcDbDwgFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbRasterImageDef::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(AcDbMyRasterImageDef::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	if (m_pAtilImage)
	{
		Atil::Size size = m_pAtilImage->size();
		Int32 width = size.width;
		Int32 height = size.height;

		pFiler->writeInt32(width);
		pFiler->writeInt32(height);

		// Write the image data on to the Atil image
		// using an Image Context
		Atil::Offset upperLeft(0, 0);
		Atil::ImageContext* pImgContext = m_pAtilImage->createContext(
			Atil::ImageContext::kRead,
			size,
			upperLeft
		);
		if (pImgContext != NULL)
		{
			for (int xf = 0; xf < width; xf++)
			{
				for (int yf = 0; yf < height; yf++)
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

	return (pFiler->filerStatus());
}

Acad::ErrorStatus AcDbMyRasterImageDef::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbRasterImageDef::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > AcDbMyRasterImageDef::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < AcDbMyRasterImageDef::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params

	Int32 width = 0;
	Int32 height = 0;

	pFiler->readInt32(&width);
	pFiler->readInt32(&height);

	acutPrintf(_T("\nHeight: %d.\n"), width);
	acutPrintf(_T("\nWidth: %d.\n"), height);

	// Create an Atil::Image.
	// The AcDbRasterImageDef::setImage requires it
	Atil::ImagePixel initialImage;
	initialImage.setToZero();
	initialImage.type = Atil::DataModelAttributes::kBgra;
	initialImage.value.rgba = 0xff000000;

	Atil::Size size(width, height);
	const Atil::RgbModel* pDm = new Atil::RgbModel(
		Atil::RgbModelAttributes::k4Channels,
		Atil::DataModelAttributes::kBlueGreenRedAlpha);

	if (m_pAtilImage != NULL)
	{
		delete m_pAtilImage;
		m_pAtilImage = NULL;
	}
	m_pAtilImage = new Atil::Image(size, pDm, initialImage);

	// Write the image data on to the Atil image
	// using an Image Context
	Atil::Offset upperLeft(0, 0);
	Atil::ImageContext* pImgContext = m_pAtilImage->createContext(
		Atil::ImageContext::kWrite,
		size,
		upperLeft
	);
	if (pImgContext != NULL)
	{
		for (int xf = 0; xf < width; xf++)
		{
			for (int yf = 0; yf < height; yf++)
			{
				Int32 value;
				pFiler->readInt32(&value);
				pImgContext->put32(xf, yf, value);
			}
		}
	}
	pImgContext->flush();
	delete pImgContext;

	setImage(m_pAtilImage, NULL);

	return (pFiler->filerStatus());
}

//----- deepClone
Acad::ErrorStatus AcDbMyRasterImageDef::subDeepClone(AcDbObject * pOwnerObject, AcDbObject * &pClonedObject, AcDbIdMapping & idMap, Adesk::Boolean isPrimary) const {
	assertReadEnabled();
	return (AcDbRasterImageDef::subDeepClone(pOwnerObject, pClonedObject, idMap, isPrimary));
}

//----- wblockClone
Acad::ErrorStatus AcDbMyRasterImageDef::subWblockClone(AcRxObject * pOwnerObject, AcDbObject * &pClonedObject, AcDbIdMapping & idMap, Adesk::Boolean isPrimary) const {
	assertReadEnabled();
	return (AcDbRasterImageDef::subWblockClone(pOwnerObject, pClonedObject, idMap, isPrimary));
}

std::string convert(const std::wstring& wstr) {
	const int BUFF_SIZE = 7;
	if (MB_CUR_MAX >= BUFF_SIZE) 
		throw std::invalid_argument("BUFF_SIZE too small");
	std::string result;
	bool shifts = std::wctomb(nullptr, 0);  // reset the conversion state
	for (const wchar_t wc : wstr) {
		std::array<char, BUFF_SIZE> buffer;
		const int ret = std::wctomb(buffer.data(), wc);
		if (ret < 0) 
			throw std::invalid_argument("inconvertible wide characters in the current locale");
		buffer[ret] = '\0';  // make 'buffer' contain a C-style string
		result += std::string(buffer.data());
	}
	return result;
}

// Create directly from in-memory IE image and/or file descriptor.
Acad::ErrorStatus  AcDbMyRasterImageDef::setEmbeddedImage(const ACHAR * imageFilePath)
{
	std::string imgPath = convert(imageFilePath);
	Image img(imgPath.c_str(), 4);
	int width = img.w;
	int height = img.h;

	// Create an Atil::Image.
	// The AcDbRasterImageDef::setImage requires it
	Atil::ImagePixel initialImage;
	initialImage.setToZero();
	initialImage.type = Atil::DataModelAttributes::kBgra;
	initialImage.value.rgba = 0xff000000;

	Atil::Size size(width, height);
	acutPrintf(_T("\nHeight: %d.\n"), width);
	acutPrintf(_T("\nWidth: %d.\n"), height);

	const Atil::RgbModel* pDm = new Atil::RgbModel(
		Atil::RgbModelAttributes::k4Channels,
		Atil::DataModelAttributes::kBlueGreenRedAlpha);

	if (m_pAtilImage != NULL)
	{
		delete m_pAtilImage;
		m_pAtilImage = NULL;
	}
	m_pAtilImage = new Atil::Image(size, pDm, initialImage);

	// Write the image data on to the Atil image
	// using an Image Context
	Atil::Offset upperLeft(0, 0);
	Atil::ImageContext* pImgContext
		= m_pAtilImage->createContext(
			Atil::ImageContext::kWrite,
			size,
			upperLeft
		);

	if (pImgContext != NULL)
	{
		for (int xf = 0; xf < width; xf++)
		{
			for (int yf = 0; yf < height; yf++)
			{
				BYTE alpha = 0x0;
				Atil::RgbColor p;

				int bytePerPixel = img.channels;
				BYTE* pixelOffset = img.data + (xf + width * yf) * bytePerPixel;
				BYTE rr = pixelOffset[0];
				BYTE gg = pixelOffset[1];
				BYTE bb = pixelOffset[2];
				alpha = img.channels >= 4 ? pixelOffset[3] : 0xff;

				p.set(rr, gg, bb, alpha);
				pImgContext->put32(xf, yf, p);
			}
		}
	}
	pImgContext->flush();
	delete pImgContext;

	bool isImageValid = m_pAtilImage->isValid();
	assert(isImageValid);

	return setImage(m_pAtilImage, NULL);
}
