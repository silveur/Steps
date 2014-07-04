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

#ifndef SCALES_H_INCLUDED
#define SCALES_H_INCLUDED

class Suite
{
public:
	Suite(const String& name, SuiteType suiteType): theSuiteName(name), theSuiteType(suiteType)
	{
		if (theSuiteType == SCALE)
		{
			if (name == "Major")
			{
				const int scale[7] ={0, 2, 4, 5, 7, 9, 11};
				theNotes = Array<int>(scale, 7);
			}
			else if (name == "Minor")
			{
				const int scale[7] ={0, 2, 3, 5, 7, 9, 11};
				theNotes = Array<int>(scale, 7);
			}
			else if (name == "Harmonic Minor")
			{
				const int scale[7] ={0, 2, 3, 5, 7, 8, 11};
				theNotes = Array<int>(scale, 7);
			}
			else if (name == "Pentatonic Major")
			{
				const int scale[6] ={0, 2, 5, 7, 9, 12};
				theNotes = Array<int>(scale, 6);
			}
			else if (name == "Pentatonic Minor")
			{
				const int scale[6] ={0, 3, 5, 7, 10, 12};
				theNotes = Array<int>(scale, 6);
			}
		}
		else if (theSuiteType == CHORD)
		{
			if (name == "Major")
			{
				const int scale[3] ={0, 4, 7};
				theNotes = Array<int>(scale, 3);
			}
			else if (name == "Minor")
			{
				const int scale[3] ={0, 3, 7};
				theNotes = Array<int>(scale, 3);
			}
			else if (name == "5")
			{
				const int scale[2] ={0, 7};
				theNotes = Array<int>(scale, 2);
			}
			else if (name == "6")
			{
				const int scale[4] ={0, 4, 7 , 9};
				theNotes = Array<int>(scale, 4);
			}
			else if (name == "Dominant 7th")
			{
				const int scale[4] ={0, 4, 7, 10};
				theNotes = Array<int>(scale, 4);
			}
			else if (name == "Major 7th")
			{
				const int scale[4] ={0, 4, 7, 11};
				theNotes = Array<int>(scale, 4);
			}
			else if (name == "Minor 7th")
			{
				const int scale[4] ={0, 3, 7, 10};
				theNotes = Array<int>(scale, 4);
			}
		}
	}
	
	~Suite() {}
	
	static void initialiseSuitesArray()
	{
		const char* const scaleStrings[] = {"Major", "Harmonic Minor", "Pentatonic Major", "Pentatonic Minor", nullptr};
		const char* const chordStrings[] = {"Major", "Minor", "5", "6", "Dominant 7th", "Major 7th", "Minor 7th", nullptr};
		StringArray scaleSuites(scaleStrings); StringArray chordSuites(chordStrings);
		
		for (int i=0;i<scaleSuites.size();i++) Suite::theSuites.add(new Suite(scaleSuites[i], SCALE));
		for (int i=0;i<chordSuites.size();i++) Suite::theSuites.add(new Suite(chordStrings[i], CHORD));
	}
	
	Array<int>& getNotes()
	{
		return theNotes;
	}
	
	String& getName()
	{
		return theSuiteName;
	}
	
	SuiteType getSuiteType()
	{
		return theSuiteType;
	}
	
	static Suite* getSuiteWithId(int Id)
	{
		return theSuites[Id];
	}
	
	static OwnedArray<Suite> theSuites;
	
private:
	String theSuiteName;
	SuiteType theSuiteType;
	Array<int> theNotes;
};

#endif  // SCALES_H_INCLUDED
