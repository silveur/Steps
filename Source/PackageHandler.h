/*
  ==============================================================================

    PackageHandler.h
    Created: 10 May 2014 11:42:46am
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef PACKAGEHANDLER_H_INCLUDED
#define PACKAGEHANDLER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "dRowAudio_CURLEasySession.h"

class PackageHandler: private CURLEasySession::Listener
{
public:
	PackageHandler()
	{
		theTempApp = File::getSpecialLocation(File::tempDirectory).getFullPathName() + String("/sequencer.zip");
		theCurlSession = new CURLEasySession();
		theCurlSession->addListener(this);
		DBG("Temp folder:" << theTempApp);
		File tempAppFile = File(theTempApp); tempAppFile.deleteFile();
		theCurlSession->setLocalFile(File(theTempApp));
		theServerURL = "http://nummermusic.com/version-request.php?version=";
		checkForUpdate();
	}
	~PackageHandler()
	{
		
	}
	bool checkForUpdate()
	{
		theServerURL = theServerURL + ProjectInfo::versionString;
		URL serverURL(theServerURL);
		InputStream* response = serverURL.createInputStream(false, nullptr, nullptr,
															"",
															0, nullptr);
		String responseString = response->readString();
		if (response != nullptr)
		{
			if (responseString != "null")
			{
				downloadUpdate(responseString);
			}
			else return false;
			delete response;
		}
	}
	void downloadUpdate(String url)
	{
		url = url.trimStart().trimEnd();
		DBG("Downloading: " << url);
		theCurlSession->setRemotePath(url);
		theCurlSession->beginTransfer(false, true);
	}
	bool installUpdate()
	{
		
	}
	
	void transferAboutToStart(CURLEasySession* session)
	{
		
	}
	void transferProgressUpdate(CURLEasySession* session)
	{
		DBG("Progress: " << String(session->getProgress()));
	}
	void transferEnded(CURLEasySession* session)
	{
		ZipFile receivedZip(theTempApp);
		receivedZip.uncompressTo(File(theTempApp).getParentDirectory());
		File contents(theTempApp.replace(".zip", ".app") + String("/Contents/"));
		File destinationFolder = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + String("/Contents/"));
		destinationFolder.deleteRecursively();
		DBG("Source:" << contents.getFullPathName());
		DBG("Destination: " << destinationFolder.getFullPathName());
		if (contents.copyDirectoryTo(destinationFolder) == true)
		{
			DBG("Copied");
		}
		else
			DBG("ERROR");
		String chmod = ("chmod +x " + destinationFolder.getFullPathName() + "/MacOS/*");
		system(chmod.toRawUTF8());
	}
	
private:
	String theServerURL;
	String theTempApp;
	ScopedPointer<CURLEasySession> theCurlSession;
};



#endif  // PACKAGEHANDLER_H_INCLUDED
