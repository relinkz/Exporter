#include "maya_includes.h"
#include "SkelAnimExport.h"
#include "HeaderStructs.h"
#include "MeshExport.h"
#include <maya/MFnPlugin.h>


using namespace std;

MCallbackIdArray myCallbackArray;
//fstream outFile("//DESKTOP-BOKNO6D/server/knulla.BBF", std::fstream::out | std::fstream::binary);
fstream outFile("pillar.BBF", std::fstream::out | std::fstream::binary);

EXPORT MStatus initializePlugin(MObject obj)
{
    SkelAnimExport cSkelAnim;
	//QWidget * layoutWidget = MQtUtil::findWindow("mainwindow");
	/*MGlobal::executeCommand("string $dialog = `loadUI - uiFile ""mainwindow.ui""`");
	MGlobal::executeCommand("showWindow $dialog");
	MString hejsan = MGlobal::executeCommandStringResult("checkBox -q -val skelBox");
	MGlobal::displayInfo(hejsan);*/

	// most functions will use this variable to indicate for errors
	MStatus res = MS::kSuccess;

	//ofstream outFile("test", std::ofstream::binary);
	if (!outFile.is_open())
	{
		MGlobal::displayError("ERROR: the binary file is not open");
		
	}

	MFnPlugin myPlugin(obj, "Maya plugin", "1.0", "Any", &res);
	if (MFAIL(res)) {
		CHECK_MSTATUS(res);
	}
	
	MGlobal::displayInfo("Maya plugin loaded!");

    /*Iterate all skin clusters in scene.*/
    cSkelAnim.IterateSkinClusters();

    /*Iterate all joints in scene.*/
    cSkelAnim.IterateJoints();
	
	/*writing a temporary mainheader for one mesh*/
	MainHeader tempHead{ 1 };
	outFile.write((char*)&tempHead, sizeof(MainHeader));

	MItDag meshIt(MItDag::kBreadthFirst, MFn::kTransform, &res);
	for (; !meshIt.isDone(); meshIt.next())
	{
		MFnTransform trans = meshIt.currentItem();
		if (trans.child(0).hasFn(MFn::kMesh))
		{
			//Createmesh(meshIt.currentItem(), cSkelAnim);
			MeshExport newMesh(&outFile, &cSkelAnim.skinList);
			newMesh.exportMesh(meshIt.currentItem());
		}
	}

	return res;
}

EXPORT MStatus uninitializePlugin(MObject obj)
{
	// simply initialize the Function set with the MObject that represents
	// our plugin
	MFnPlugin plugin(obj);

	outFile.close();
	// if any resources have been allocated, release and free here before
	// returning...
	MMessage::removeCallbacks(myCallbackArray);


	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}