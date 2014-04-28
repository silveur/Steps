/*
  ==============================================================================

    Scales.h
    Created: 27 Apr 2014 6:53:38pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef SCALES_H_INCLUDED
#define SCALES_H_INCLUDED

class Scale
{
public:
	Scale(const String& name): theScaleName(name)
	{
		if (name == "Major")
		{
			const int major[7] ={0, 2, 4, 5, 7, 9, 11};
			theNotes = Array<int>(major, 7);
		}
		else if (name == "Minor")
		{
			const int minor[7] ={0, 2, 3, 5, 7, 9, 11};
			theNotes = Array<int>(minor, 7);
		}
		else if (name == "Pentatonic Major")
		{
			const int minor[6] ={0, 2, 5, 7, 9, 12};
			theNotes = Array<int>(minor, 6);
		}
		else if (name == "Pentatonic Minor")
		{
			const int minor[6] ={0, 3, 5, 7, 10, 12};
			theNotes = Array<int>(minor, 6);
		}
	}
	
	~Scale()
	{
		
	}
	
	Array<int>& getNotes()
	{
		return theNotes;
	}
	
	String& getName()
	{
		return theScaleName;
	}
	
private:
	String theScaleName;
	Array<int> theNotes;
};



#endif  // SCALES_H_INCLUDED
