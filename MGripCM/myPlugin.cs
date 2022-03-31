// (C) Copyright 2021 by  
//
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Runtime;
using System;
using ADN.CustomWrapper;
using System.IO;
using System.Reflection;

// This line is not mandatory, but improves loading performances
[assembly: ExtensionApplication(typeof(ManagedEmbeddedRasterImage.MyPlugin))]

namespace ManagedEmbeddedRasterImage
{
    public class MyPlugin : IExtensionApplication
    {
        void IExtensionApplication.Initialize()
        {
            string path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            SystemObjects.DynamicLinker.LoadModule(path + "\\ADSKdbx.dbx", false, false);
            SystemObjects.DynamicLinker.LoadModule(path + "\\ADSKManagedWrapper.dll", false, false);
        }

        void IExtensionApplication.Terminate()
        {
        }
    }
}
