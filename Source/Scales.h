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
