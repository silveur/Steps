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
	Scale(const String& root, const String& name): theScaleName(name)
	{
		if (root == "C")
		{
			if (name == "Major")
			{
				const int notes[7] = {0, 2, 4, 5, 7, 9, 11};
				theNotes = Array<int>(notes, 7);
			}
		}
	}
	
	~Scale()
	{
		
	}
	
	Array<int>& getNotes()
	{
		return theNotes;
	}
	
private:
	String theScaleName;
	Array<int> theNotes;
};



#endif  // SCALES_H_INCLUDED
