/** Constants, structures, function prototype for Gh control code
 * @version ghcontrol.h
 */

#ifndef GHCONTROL_H
#define GHCONTROL_H
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <wiringPi.h>
#include "pisensehat.h"
#define GHUPDATE 2000
#define SENSORS 3
#define TEMPERATURE 0
#define HUMIDITY 1
#define PRESSURE 2
#define SIMULATE 0
#define USTEMP 50
#define LSTEMP -10
#define USHUMID 100
#define LSHUMID 0
#define USPRESS 1016
#define LSPRESS 985
#define STEMP 25.0
#define SHUMID 55.0
#define ON 1
#define OFF 0
#define SIMTEMPERATURE 0
#define SIMHUMIDITY 0
#define SIMPRESSURE 0
#define NUMBERS 8
#define NUMPTS 8.0
#define TBAR 7
#define HBAR 5
#define PBAR 3
#define SENSEHAT 1
#define NALARMS 7
#define LOWERATEMP 10
#define UPPERATEMP 30
#define LOWERAHUMID 25
#define UPPERAHUMID 70
#define LOWERAPRESS 985
#define UPPERAPRESS 1016
#define ALARMNMSZ 18
//CONSTANTS

//Enumerated Types
typedef enum{NOALARM,HTEMP,LTEMP,HHUMID,LHUMID,HPRESS,LPRESS}alarm_e;

//Structures
typedef struct readings
{
	time_t rtime;
	double temperature;
	double humidity;
	double pressure;
}reading_s;

typedef struct setpoints
{
	double temperature;
	double humidity;
}setpoint_s;

typedef struct controls
{
	int heater;
	int humidifier;
}control_s;

typedef struct alarmlimits
{
	double hight;
	double lowt;
	double highh;
	double lowh;
	double highp;
	double lowp;
}alarmlimit_s;

typedef struct alarms
{
	alarm_e code;
	time_t atime;
	double value;
}alarm_s;

//Function Prototypes
///@cond INTERNAL
void GhDisplayHeader(const char *sname);
int GhGetRandom(int range);
void GhDelay(int milliseconds);
void GhControllerInit(void);
void GhGetControls(void);
control_s GhSetControls(setpoint_s target, reading_s rdata);
void GhGetSetpoints(void);
setpoint_s GhSetSetpoints(void);
void GhDisplayReadings(reading_s rdata);
void GhDisplaySetpoints(setpoint_s sdata);
void GhDisplayControls(control_s ctrl);
double GhGetTemperature(void);
double GhGetHumidity(void);
double GhGetPressure(void);
reading_s GhGetReadings(void);
int GhLogData(char *fname, reading_s ghdata);
int GhSaveSetpoints(char *fname, setpoint_s spts);
setpoint_s GhRetrieveSetpoints(char *fname);
void GhDisplayAll(reading_s rd, setpoint_s sd);
alarmlimit_s GhSetAlarmLimits(void);
void GhSetAlarms(alarm_s calarm[NALARMS],alarmlimit_s alarmpt,reading_s rdata);
void GhDisplayAlarms(alarm_s calarm[NALARMS]);
///@endcond
#endif
