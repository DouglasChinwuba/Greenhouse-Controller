/** Function definitions for Gh control code
 * @version ghcontrol.c
 */

#include <stdio.h>
#include "ghcontrol.h"

//Alarm Message Array
const char alarmnames[NALARMS][ALARMNMSZ] = {"No Alarms","High Temperature","Low Temperature","High Humidity","Low Humidity","High Pressure","Low Pressure"};

/** Prints Gh Controller Title
 * @version 09APR2020
 * @author Paul Moggach 
 * @author Douglas Chinwuba
 * @param sname string with Operator's name
 * @return void
 */
void GhDisplayHeader(const char *sname)
{
	fprintf(stdout,"\n%s's CENG153 Greenhouse Controller\n",sname);
}

/** Gets random integer
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param int range
 * @return int rand()%range 
 */
int GhGetRandom(int range)
{ 
	return rand()%range;
}

/** Delays program for two seconds
 * @version 09APR2020 
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param int milliseconds
 * @return void
 */
void GhDelay(int milliseconds)
{
	long wait;
	clock_t now,start;

	wait = milliseconds*(CLOCKS_PER_SEC/1000);
	start = clock();
	now = start;
	while( (now-start) < wait )
	{
		now = clock();
	}
}

/** Passes operator name to GhDisplayHeader 
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param void
 * @return void
 */
void GhControllerInit(void)
{
	srand((unsigned) time(NULL));
	GhDisplayHeader("Douglas Chinwuba");
#if SENSEHAT
	wiringPiSetup();
	ShInit();
#endif
}

void GhGetControls()
{
}


// Structure that sets the heater and humidifier on or off 
control_s GhSetControls(setpoint_s target, reading_s rdata)
{
	control_s cset = {0};
	if (rdata.temperature < target.temperature)
	{
		cset.heater = ON;
	}
	else
	{
		cset.heater = OFF;
	}
	
	if (rdata.humidity < target.humidity)
	{
		cset.humidifier = ON;
	}
	else
	{
		cset.humidifier = OFF;
	}
	return cset;
}

void GhGetSetpoints(void)
{
}

// Structure that sets the temparature and humidity setpoints
setpoint_s GhSetSetpoints(void)
{
	setpoint_s cpoints = {0};
	cpoints.temperature = STEMP;
	cpoints.humidity = SHUMID;
	cpoints = GhRetrieveSetpoints("setpoints.dat");
	if (cpoints.temperature == 0)
	{
		cpoints.temperature = STEMP;
		cpoints.humidity = SHUMID;
	        GhSaveSetpoints("setpoints.dat",cpoints);	
	}
	
	return cpoints;
}

/** Prints time and readings
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param struct readings rdata
 * @return void
 */
void GhDisplayReadings(reading_s rdata)
{	
	fprintf(stdout,"\n%sReadings\tT: %4.1lfC\tH: %4.1lf%%\tP: %6.1lfmb", ctime(&rdata.rtime), rdata.temperature, rdata.humidity, rdata.pressure);
}

/** Prints setpoints
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param struct setpoints sdata
 * @return void
 */
void GhDisplaySetpoints(setpoint_s sdata)
{
	fprintf(stdout,"\nSetpoints       Temperature: %3.1lfC       Humidity: %3.0lf%\n",sdata.temperature, sdata.humidity);
}

/** Prints heater and humidifier on or off status
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param struct controls ctrl
 * @return void
 */
void GhDisplayControls(control_s ctrl)
{
	fprintf(stdout,"Controls        Heater: %d       Humidifier: %d\n", ctrl.heater,ctrl.humidifier);
}

/** Gets temperature value
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param void
 * @return double temperature data
 */
double GhGetTemperature(void)
{
#if SIMTEMPERATURE
	return GhGetRandom(USTEMP - LSTEMP) + LSTEMP;
#else
	ht221sData_s ct = {0};

	ct = ShGetHT221SData();
	return ct.temperature;
#endif
}

/** Gets humidity value
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param void
 * @return double humidity data
 */
double GhGetHumidity(void)
{
#if SIMHUMIDITY
	return GhGetRandom(USHUMID - LSHUMID) + LSHUMID;
#else
	ht221sData_s ch = {0};

	ch = ShGetHT221SData();
	return ch.humidity;
#endif
}

/** Gets pressure value
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param void
 * @return double pressure data
 */
double GhGetPressure(void)
{
#if SIMPRESSURE
	return GhGetRandom(USPRESS - LSPRESS) + LSPRESS;
#else
	lps25hData_s cp = {0};

	cp = ShGetLPS25HData();
	return cp.pressure;
#endif
}

// Structure that gets temperature, humidity and pressure readings
reading_s GhGetReadings(void)
{
	reading_s now = {0};
	now.rtime = time(NULL);
	now.temperature = GhGetTemperature();
	now.humidity = GhGetHumidity();
	now.pressure = GhGetPressure();
	return now;
}

/** Logs data into file
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param fname string with ghdata
 * @param struct readings ghdata
 * @return 0 or 1
 */
int GhLogData(char *fname, reading_s ghdata)
{
	FILE *fp;
	char ltime[25];
	fp = fopen(fname,"w");
	if (fp == NULL)
	{
		printf("\nCan't open file. data not retrived!\n");
		return 0;
	}
	strcpy(ltime,ctime(&ghdata.rtime));
	ltime[3] = ',';
	ltime[7] = ',';
	ltime[10] = ',';
	ltime[19] = ',';
	fprintf(fp,"\n%.24s,%3.1lf,%3.1lf,%5.1lf",ltime,ghdata.temperature,ghdata.humidity,ghdata.pressure);
	fclose(fp);
	return 1;
}

/** Saves setpoints data in file
 * @version 09ARP2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param fnmae string
 * @param struct setpoints spts
 * @return 0 or 1
 */
int GhSaveSetpoints(char *fname, setpoint_s spts)
{
	FILE *fp;
	fp = fopen(fname,"w");
		if (fp == NULL)
		{	
			return 0;
		}
		else
		{
			fwrite(&spts,sizeof(setpoint_s),1,fp);
			fclose(fp);
			return 1;
		}
}

//Structure that retrieves data from file
setpoint_s GhRetrieveSetpoints(char *fname)
{
	setpoint_s spts = {0.0};
	FILE *fp;
	fp = fopen(fname,"r");
		if (fp == NULL)
		{
			return spts;
		}
		else
		{
			fread(&spts,sizeof(setpoint_s),1,fp);
			fclose(fp);
			return spts;
		}
}

/** Displays bar graph on pisensehat
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param struct readings rd
 * @param struct setpoints sd
 * @return void
 */
void GhDisplayAll(reading_s rd, setpoint_s sd)
{
	int rv = 0;
	int sv = 0;
	int avh = 0;
        int avl = 0;
   	fbpixel_s pxc = {0};
	ShClearMatrix();

	rv = (8.0 * ((rd.temperature / (USTEMP - LSTEMP)) + 0.05)) - 1.0;
	sv = (8.0 * ((rd.temperature / (USTEMP - LSTEMP)) + 0.05)) - 1.0;
	pxc.red = 0x00;
	pxc.green = 0xFF;
	pxc.blue = 0x00;
	ShSetVerticalBar(TBAR,pxc,rv);
	pxc.red = 0x0F;
	pxc.green = 0x0F;
	pxc.blue = 0x00;
	ShSetPixel(TBAR,sv,pxc);

	rv = (8.0 * ((rd.humidity / (USHUMID - LSHUMID)) + 0.05)) - 1.0;
	sv = (8.0 * ((rd.humidity / (USHUMID - LSHUMID)) + 0.05)) - 1.0;
	pxc.red = 0x00;
	pxc.green = 0xFF;
	pxc.blue = 0x00;
	ShSetVerticalBar(HBAR,pxc,rv);
	pxc.red = 0x0F;
	pxc.green = 0x0F;
	pxc.blue = 0x00;
	ShSetPixel(HBAR,sv,pxc);

	rv = (8.0 * ((rd.pressure / (USPRESS - LSPRESS)) + 0.05)) - 1.0;
	pxc.red = 0x00;
	pxc.green = 0xFF;
	pxc.blue = 0x00;
	ShSetVerticalBar(PBAR,pxc,rv);
}

// Structure that sets alarm limit
alarmlimit_s GhSetAlarmLimits(void)
{
	alarmlimit_s calarm = {0};
	calarm.hight = UPPERATEMP;
	calarm.lowt = LOWERATEMP;
	calarm.highh = UPPERAHUMID;
	calarm.lowh = LOWERAHUMID;
	calarm.highp = UPPERAPRESS;
	calarm.lowp = LOWERAPRESS;
        return calarm;
}

/** Sets alarms
 * @version 09APR2020
 * @author Paul Moggach
 * @author Douglas Chinwuba
 * @param array alarm_s calarm[NALARM]
 * @param struct alarmlimits alarmpt
 * @param struct readings rdata
 * @return void
 */
void GhSetAlarms(alarm_s calarm[NALARMS], alarmlimit_s alarmpt, reading_s rdata)
{
	int i;
	for(i = 0;i < 7;i++)
	{
		calarm[i].code = NOALARM;
	}

	if(rdata.temperature >= alarmpt.hight)
	{
		calarm[HTEMP].code = HTEMP;
		calarm[HTEMP].atime = rdata.rtime;
		calarm[HTEMP].value = rdata.temperature;
	}

	if(rdata.temperature <= alarmpt.lowt)
	{
		calarm[LTEMP].code = LTEMP;
		calarm[LTEMP].atime = rdata.rtime;
		calarm[LTEMP].value = rdata.temperature;
	}

	if(rdata.humidity >= alarmpt.highh)
	{
		calarm[HHUMID].code = HHUMID;
		calarm[HHUMID].atime = rdata.rtime;
		calarm[HHUMID].value = rdata.humidity;
	}
	
	if(rdata.humidity <= alarmpt.lowh)
	{
		calarm[LHUMID].code = LHUMID;
		calarm[LHUMID].atime = rdata.rtime;
		calarm[LHUMID].value = rdata.humidity;
	}

	if(rdata.pressure >= alarmpt.highp)
	{	
		calarm[HPRESS].code = HPRESS;
		calarm[HPRESS].atime = rdata.rtime;
		calarm[HPRESS].value = rdata.pressure;
	}

	if(rdata.pressure <= alarmpt.lowp)
	{	
		calarm[LPRESS].code = LPRESS;
		calarm[LPRESS].atime = rdata.rtime;
		calarm[LPRESS].value = rdata.pressure;
	}
}

/** Displays alarms
 * @version 09APR2020
 * @author Paul Maggach
 * @author Douglas Chinwuba
 * @param array alarm_s alrm[NALARMS]
 * @return void
 */
void GhDisplayAlarms(alarm_s alrm[NALARMS])
{
	fprintf(stdout,"Alarms\n");
	for(int i = 1;i < 7;i++)
	{
		if(alrm[i].code != NOALARM)
		{
			fprintf(stdout,"%s Alarm  %s",alarmnames[i],ctime(&alrm[i].atime));
		}	
	}
}
