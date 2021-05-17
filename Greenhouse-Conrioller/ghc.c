/**Defines the entry point for the console application.
 * *@version ghc.c
 * */
#include "ghcontrol.h"

int main(void)
{
	time_t now;
	struct readings creadings = {0};
	struct controls ctrl = {0};
	struct setpoints sets = {0};
	int logged;
	alarmlimit_s alimits = {0};
	alarm_s warn[NALARMS] = {0};
	sets = GhSetSetpoints();
	alimits = GhSetAlarmLimits();
	GhControllerInit();
	while(1)
	{
		now = time(NULL);
		GhGetSetpoints();
		creadings = GhGetReadings();
		logged = GhLogData("ghdata.txt",creadings);
		ctrl = GhSetControls(sets,creadings);
		GhSetAlarms(warn,alimits,creadings);
		GhDisplayAll(creadings,sets);
		GhDisplayReadings(creadings);
		GhDisplaySetpoints(sets);
		GhDisplayControls(ctrl);
		GhDisplayAlarms(warn);
		GhDelay(GHUPDATE);
	}		
	return 1;
}

