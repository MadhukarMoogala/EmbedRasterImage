# EmbedRasterImage
This project demonstrates how to embed raster image graphics in AutoCAD drawing
In this blog post we will look at creating a custom object derived from AcDbRasterImageDef that saves/loads the image data to/from the drawing using ATIL. This will ensure that your drawing is independent of the external image file and the image data will get loaded to the AcDbRasterImageDef if the arx is loaded in AutoCAD.

## Build Instructions

- You need to have ObjectARX 2022 SDK 
- You need to reset the SDK path under Include and Lib paths.

```
git clone https://github.com/MadhukarMoogala/EmbedRasterImage
cd  EmbedRasterImage
devenv embedImage.sln
```
