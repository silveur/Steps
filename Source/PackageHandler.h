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
#include "Main.h"

class SequencerApplication;

class PackageHandler: private CURLEasySession::Listener
{
public:
	PackageHandler()
	{
		theTempApp = File::getSpecialLocation(File::tempDirectory).getFullPathName() + String("/sequencer.zip");
		theCurlSession = new CURLEasySession();
		theCurlSession->addListener(this);
		File tempAppFile = File(theTempApp); tempAppFile.deleteFile();
		theCurlSession->setLocalFile(File(theTempApp));
		theServerURL = "http://nummermusic.com/version-request.php?version=";
		checkForUpdate();
	}
	~PackageHandler()
	{
		File tempFolder = File::getSpecialLocation(File::tempDirectory).getFullPathName();
		tempFolder.deleteRecursively();
	}
	void checkForUpdate()
	{
		theServerURL = theServerURL + ProjectInfo::versionString;
		URL serverURL(theServerURL);
		InputStream* response = serverURL.createInputStream(false, nullptr, nullptr,
															"",
															2000, nullptr);
		if (response != nullptr)
		{
			String responseString = response->readString();
			if (!responseString.contains("null"))
			{
				downloadUpdate(responseString);
			}
			else
			{
				deleteMe();
				std::cout << "Up to date" << std::endl;
			}
			delete response;
		}
		else
		{
			std::cout << "No Internet" << std::endl;
			deleteMe();
		}
	}
	void downloadUpdate(String url)
	{
		url = url.trimStart().trimEnd();
		DBG("Downloading: " << url);
		theCurlSession->setRemotePath(url);
		theCurlSession->beginTransfer(false, true);
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
		contents.copyDirectoryTo(destinationFolder);
		String chmod = ("chmod +x " + destinationFolder.getFullPathName() + "/MacOS/*");
		system(chmod.toRawUTF8());
		deleteMe();
	}
	
	void deleteMe()
	{
		SequencerApplication* app = (SequencerApplication*)JUCEApplication::getInstance();
		app->updateCallback();
	}
	
private:
	String theServerURL;
	String theTempApp;
	ScopedPointer<CURLEasySession> theCurlSession;
};



#endif  // PACKAGEHANDLER_H_INCLUDED
