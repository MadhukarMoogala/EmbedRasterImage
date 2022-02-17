using System;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.EditorInput;

using ADN.CustomWrapper;
using System.IO;

[assembly: CommandClass(typeof(ManagedTest.Commands))]

namespace ManagedTest
{
    public class Commands: IExtensionApplication
    {
        public void Initialize()
        {
        }

        public void Terminate()
        {
        }

        [CommandMethod("EmbedImageMgd")]
        static public void EmbedImageMgdMethod()
        {
            Document doc = Application.DocumentManager.MdiActiveDocument;
            Database db = doc.Database;
            Editor ed = doc.Editor;

            PromptPointOptions ppo = new PromptPointOptions("\nSelect image position: ");
            PromptPointResult ppr = ed.GetPoint(ppo);
            if (ppr.Status != PromptStatus.OK)
                return;
            Point3d imagePos = ppr.Value;

            using (Transaction Tx = db.TransactionManager.StartTransaction())
            {
                BlockTable bt = Tx.GetObject(db.BlockTableId, OpenMode.ForRead) 
                    as BlockTable;

                BlockTableRecord model = Tx.GetObject(bt[BlockTableRecord.ModelSpace], OpenMode.ForWrite) 
                    as BlockTableRecord;

                //Image dictionary name constant
                String dictName =  "RASTER_USING_BUFFER";
                MyRasterImageDefMgd imageDef;
                ObjectId imageDefId;
                ObjectId imageDictId = MyRasterImageDefMgd.GetImageDictionary(db);

                if(imageDictId.IsNull)
                {
                    //Image dictionary doesn't exist, create new
                    imageDictId = MyRasterImageDefMgd.CreateImageDictionary(db);
                }
                //Open the image dictionary
                DBDictionary imageDict = Tx.GetObject(imageDictId, OpenMode.ForRead) as DBDictionary;

                //See if our raster def in the image dict exists, 
                //if not, create new
                if(imageDict.Contains(dictName))
                {
                    //Get the raster image def
                    imageDefId = imageDict.GetAt(dictName);
                    imageDef = Tx.GetObject(imageDefId, OpenMode.ForWrite) as MyRasterImageDefMgd;
                }
                else
                {
                    //Create a raster image definition
                    //Create a new image definition
                    imageDef = new MyRasterImageDefMgd();
                    //imageDef.SetEmbeddedImage(@"D:\TestFiles\MyTexture.jpg");
                    var per = ed.GetString("Set Embedded Image Path");
                    if (per.Status != PromptStatus.OK) return;
                    if (!File.Exists(per.StringResult))
                    {
                        ed.WriteMessage($"File Not Found:{per.StringResult}");
                    }
                    imageDef.SetEmbeddedImage(per.StringResult);
                    imageDict.UpgradeOpen();
                    imageDefId = imageDict.SetAt(dictName, imageDef);
                    Tx.AddNewlyCreatedDBObject(imageDef, true);
                }

                // Now create the raster image that references the definition
                RasterImage image = new RasterImage
                {
                    ImageDefId = imageDefId,

                    Orientation = new CoordinateSystem3d(imagePos, Vector3d.XAxis, Vector3d.YAxis),

                    //And some other properties
                    ShowImage = true
                };

                // Add the image to ModelSpace
                model.AppendEntity(image);
                Tx.AddNewlyCreatedDBObject(image, true);

                // Create a reactor between the RasterImage
                // and the RasterImageDef to avoid the "Unreferenced" 
                // warning the XRef palette
                RasterImage.EnableReactors(true);
                image.AssociateRasterDef(imageDef);
                
                Tx.Commit();
            }
        }
    }
}
