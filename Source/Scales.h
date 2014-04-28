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
