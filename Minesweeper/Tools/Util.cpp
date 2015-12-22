#include "Util.h"
#include <cstring>
#include <stdio.h>


namespace Util
{

	void FormatTimeFromInt(int nTime, char*& szFormattedTime)
	{
		if (szFormattedTime == nullptr)
			szFormattedTime = new char[10]; // strlen("HH:MM:SS") + 1 - add 1 for the Null Terminator
		
		int timeRemaining = nTime;

		int hours = timeRemaining /3600;
		timeRemaining = nTime % 3600;

		int minutes = timeRemaining / 60;
		timeRemaining = timeRemaining % 60;
		
		int seconds = timeRemaining;

		char strHours[] = "00";
		char strMinutes[] = "00";
		char strSeconds[] = "00";

		// Hours buffer
		if (hours < 10)
			sprintf_s(strHours, 3, "0%d", hours);
		else
			sprintf_s(strHours, 3, "%d", hours);
		
		// Minutes buffer
		if (minutes < 10)
			sprintf_s(strMinutes, 3, "0%d", minutes);
		else
			sprintf_s(strMinutes, 3, "%d", minutes);
		
		// Seconds buffer
		if (seconds < 10)
			sprintf_s(strSeconds, 3, "0%d", seconds);
		else
			sprintf_s(strSeconds, 3, "%d", seconds);

		sprintf_s( szFormattedTime,10,"%s:%s:%s", strHours, strMinutes, strSeconds);
	}

}