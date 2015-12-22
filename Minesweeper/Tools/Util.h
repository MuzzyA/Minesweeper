#pragma once

#include <stdio.h>
#include <vector>

namespace Util
{
	// FormatTimeFromInt()
	// Takes 'nTime' and creates a string in the format of '00:00:00' (HH:MM:SS)
	// nTime - The time, in miliseconds, to format
	// szFormattedTime - The string that the formatted time will be saved to
	//
	// NOTE: This function creates new memory 'szFormattedTime' is null.  If 'szFormattedTime'
	//       contains data, then it will be overwritten. Also if 'szFormattedTime' contains 
	//	     data, it should have a strlen of 9 including the nullterminator.
	void FormatTimeFromInt(int nTime, char*& szFormattedTime);

	// VectorContainsItem()
	// Searches 'List' for any instances of 'Item'.  Returns the index of the
	// item if it was found.  Returns '-1' if 'Item' was not found.
	// List - The list to search through for the item
	// Item - The item to search for in 'List'
	template< typename T >
	int VectorContainsItem( std::vector<T> List,T Item)
	{
		for (unsigned i = 0; i < List.size(); ++i)
		{
			if (List[i] == Item)
				return i;
		}

		// If we have made it here, then the list does not contain 'Item'
		return -1;
	}
}