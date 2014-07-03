/* =====================================================================
 
 * Steps - Midi sequencer
 * Copyright (C) 2014  Silvere Letellier for Nummer Music
 * Contact: <silvere.letellier@gmail.com>
 
 -----------------------------------------------------------------------
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 ===================================================================== */

#ifndef PACKAGEHANDLER_H_INCLUDED
#define PACKAGEHANDLER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "dRowAudio_CURLEasySession.h"
#include "Main.h"

class SequencerApplication;

class Updater: private CURLEasySession::Listener, public Thread
{
public:
	Updater(): Thread("Updater")
	{
		theTempApp = File::getSpecialLocation(File::tempDirectory).getFullPathName() + String("/steps.zip");
		theCurlSession = new CURLEasySession();
		theCurlSession->addListener(this);
		File tempAppFile = File(theTempApp); tempAppFile.deleteFile();
		theCurlSession->setLocalFile(File(theTempApp));
		theServerURL = "http://nummermusic.com/version-request.php?version=";
		theTrackerURL = "http://nummermusic.com/tracker.php";
		startThread();
	}
	
	~Updater()
	{
		File tempFolder = File::getSpecialLocation(File::tempDirectory).getFullPathName();
		tempFolder.deleteRecursively();
	}
	
	void run()
	{
		URL google("http://google.com");
		InputStream* stream = google.createInputStream(false);
		bool hasInternet = (stream != nullptr);
		delete stream;
		
		if (hasInternet)
		{
			theServerURL = theServerURL + ProjectInfo::versionString;
			URL serverURL(theServerURL);
			InputStream* response = serverURL.createInputStream(false, nullptr, nullptr, "", 2000, nullptr);
			if (response != nullptr)
			{
				String responseString = response->readString();
				
				if (!responseString.contains("null"))
				{
					std::cout << "Downloading update..." << std::endl;
					downloadUpdate(responseString);
				}
				
				else std::cout << "Up to date" << std::endl;

				delete response;
			}

			theTrackerInfos.OSVersion = SystemStats::getOperatingSystemName();
			theTrackerInfos.StepsVersion = ProjectInfo::versionString;
			theTrackerInfos.UserName = SystemStats::getLogonName();
			theTrackerInfos.Location = SystemStats::getUserRegion();
			
			theTrackerURL += "?UserName=" + theTrackerInfos.UserName;
			theTrackerURL += "&OSVersion=" + theTrackerInfos.OSVersion;
			theTrackerURL += "&StepsVersion=" + theTrackerInfos.StepsVersion;
			theTrackerURL += "&Location=" + theTrackerInfos.Location;
			
			URL trackerURL(theTrackerURL.trim());
			InputStream* trackerResponse = trackerURL.createInputStream(false, nullptr, nullptr, "", 2000, nullptr);
			if (trackerResponse != nullptr)
			{
				String responseString = trackerResponse->readString();
				std::cout << responseString << std::endl;
				delete trackerResponse;
			}
		}
		else
		{
			std::cout << "No Internet" << std::endl;
		}
		deleteMe();
	}
	
	void downloadUpdate(String url)
	{
		url = url.trimStart().trimEnd();
		theCurlSession->setRemotePath(url);
		theCurlSession->beginTransfer(false, true);
	}
	
	void transferAboutToStart(CURLEasySession* session)	{}
	
	void transferProgressUpdate(CURLEasySession* session) {}
	
	void transferEnded(CURLEasySession* session)
	{
		ZipFile receivedZip(theTempApp);
		receivedZip.uncompressTo(File(theTempApp).getParentDirectory());
		File contents(theTempApp.replace(".zip", ".app") + String("/Contents/"));
		File destinationFolder = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + String("/Contents/"));
		destinationFolder.deleteRecursively();
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
	
	struct TrackerInfos
	{
		String OSVersion;
		String StepsVersion;
		String UserName;
		String Location;
	};
	
private:
	String theServerURL;
	String theTrackerURL;
	String theTempApp;
	TrackerInfos theTrackerInfos;
	ScopedPointer<CURLEasySession> theCurlSession;
};

#endif  // PACKAGEHANDLER_H_INCLUDED
