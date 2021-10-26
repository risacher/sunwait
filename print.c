
/*
** print.c (of sunwait)
**
** Who Ver   When        What
** IFC  0.5  04-12-2014  Fix my 1st release of sunwait for windows and port to linux
** IFC  0.6  08-12-2014  Add timezone for output of timings
** IFC  0.7  30-04-2015  Fix timexone and DST trouble - and problems near dateline
** IFC  0.8  2015-05-27  Resolve 'dodgy day' and cleanup
**
*/

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "sunwait.h"
#include "sunriset.h"
#include "print.h"

static const char* cTo    = " to ";
static const char* cComma = ", ";

#define NO_OFFSET 0.0

// The user-specified offset reduces the diurnal arc, at sunrise AND sunset.
// But make sure dawn aways is before dusk. The offset can mess that up.
double diurnalArcWithOffset1 (const double pDiurnalArc, const double pOffset)
{ double arcWithOffset = pDiurnalArc - pOffset - pOffset;
  if (arcWithOffset >= 24.0) return 24.0;
  if (arcWithOffset <=  0.0) return  0.0;
  return arcWithOffset;
}
// Simpler to use form
double diurnalArcWithOffset (const runStruct *pRun, const targetStruct *pTarget)
{ return diurnalArcWithOffset1 (pTarget->diurnalArc, pRun->offsetHour);
}

// What time, in hours UTC, is the offset sunrise?
double getOffsetRiseHourUTC1 (const double pSouthHourUTC, const double pDiurnalArc, const double pOffsetHour)
{ return pSouthHourUTC - diurnalArcWithOffset1 (pDiurnalArc, pOffsetHour)/2.0;
}
// Simpler to use form
double getOffsetRiseHourUTC (const runStruct *pRun, const targetStruct *pTarget)
{ return getOffsetRiseHourUTC1 (pTarget->southHourUTC, pTarget->diurnalArc, pRun->offsetHour);
}

// What time, in hours UTC, is the offset sunset?
double getOffsetSetHourUTC1 (const double pSouthHourUTC, const double pDiurnalArc, const double pOffsetHour)
{ return pSouthHourUTC + diurnalArcWithOffset1 (pDiurnalArc, pOffsetHour)/2.0;
}
// Simpler to use form
double getOffsetSetHourUTC (const runStruct *pRun, const targetStruct *pTarget)
{ return getOffsetSetHourUTC1 (pTarget->southHourUTC, pTarget->diurnalArc, pRun->offsetHour);
}


void print_a_time
( const OnOff   pGmt_OnOff
, const time_t *pMidnightTimet
, const double  pEventHour
)
{ struct tm tmpTm;
  char tmpBuffer [80];

  // Convert current time to struct tm for UTC or local timezone
  if (pGmt_OnOff == ONOFF_ON)
  { myUtcTime   (pMidnightTimet, &tmpTm);
    tmpTm.tm_min += (int) (pEventHour * 60.0);
    time_t x = mktime (&tmpTm);
    myUtcTime   (&x, &tmpTm);
  }
  else
  { myLocalTime (pMidnightTimet, &tmpTm);
    tmpTm.tm_min += (int) (pEventHour * 60.0);
    mktime (&tmpTm);
  }

  strftime (tmpBuffer, 80, "%H:%M", &tmpTm);
  printf ("%s", tmpBuffer);
}

void print_a_sun_time
( const OnOff   pGmt_OnOff
, const time_t *pMidnightTimet
, const double  pEventHour
, const double  pOffsetDiurnalArc
)
{ // A positive offset reduces the diurnal arc
  if (pOffsetDiurnalArc <=  0.0 || pOffsetDiurnalArc >= 24.0)
    printf ("--:--");
  else
    print_a_time (pGmt_OnOff, pMidnightTimet, pEventHour);
}

void print_times
( const runStruct    *pRun
, const targetStruct *pTarget
, const double  pOffsetHour
, const char   *pSeparator
)
{ double offsetDiurnalArc = diurnalArcWithOffset1 (pTarget->diurnalArc, pOffsetHour);
  double riseHour         = getOffsetRiseHourUTC1 (pTarget->southHourUTC, pTarget->diurnalArc, pOffsetHour);
  double setHour          = getOffsetSetHourUTC1  (pTarget->southHourUTC, pTarget->diurnalArc, pOffsetHour);

  if (pRun->reportSunrise == ONOFF_ON)
    print_a_sun_time (pRun->utc, &pRun->targetTimet, riseHour, offsetDiurnalArc);
  if (pRun->reportSunrise == ONOFF_ON && pRun->reportSunset == ONOFF_ON)
    printf ("%s", pSeparator);
  if (pRun->reportSunset  == ONOFF_ON)
    print_a_sun_time (pRun->utc, &pRun->targetTimet, setHour, offsetDiurnalArc);

       if (offsetDiurnalArc >= 24.0) printf (" (Midnight sun)");
  else if (offsetDiurnalArc <=  0.0) printf (" (Polar night)");

  printf ("\n");
}

void generate_report (const runStruct *pRun)
{
  /*
  ** Generate and save sunrise and sunset times for target
  */

  targetStruct tmpTarget;
  tmpTarget.twilightAngle = pRun->twilightAngle;
  tmpTarget.daysSince2000 = pRun->target2000;

  sunriset (pRun, &tmpTarget);

  double  twilightAngleTarget  = tmpTarget.twilightAngle;

  /*
  ** Now generate the report
  */

  struct tm nowTm;
  struct tm targetTm;
  char buffer [80];

  if (pRun->utc == ONOFF_ON)
  { myUtcTime (&pRun->nowTimet,    &nowTm);
    myUtcTime (&pRun->targetTimet, &targetTm);
  }
  else
  { myLocalTime (&pRun->nowTimet,    &nowTm);
    myLocalTime (&pRun->targetTimet, &targetTm);
  }

  printf ("\n");

  strftime (buffer, 80, "%d-%b-%Y %H:%M %Z", &nowTm);
  printf
  ("      Current Date and Time: %s\n", buffer);

  printf ("\n\nTarget Information ...\n\n");

  printf
  ("                   Location: %10.6fN, %10.6fE\n"
  , pRun->latitude
  , pRun->longitude
  );

  strftime (buffer, 80, "%d-%b-%Y", &targetTm);
  printf
  ("                       Date: %s\n", buffer);

  strftime (buffer, 80, "%Z", &targetTm);
  printf
  ("                   Timezone: %s\n", buffer);

  printf
  ("   Sun directly north/south: ");
  print_a_time (pRun->utc, &pRun->targetTimet, tmpTarget.southHourUTC);
  printf ("\n");

  if (pRun->offsetHour != NO_OFFSET)
  { printf
    ( "                     Offset: %2.2d:%2.2d hours\n"
    , hours   (pRun->offsetHour)
    , minutes (pRun->offsetHour)
    );
  }

       if (tmpTarget.twilightAngle == TWILIGHT_ANGLE_DAYLIGHT)     printf("             Twilight angle: %5.2f degrees (daylight)\n",     twilightAngleTarget);
  else if (tmpTarget.twilightAngle == TWILIGHT_ANGLE_CIVIL)        printf("             Twilight angle: %5.2f degrees (civil)\n",        twilightAngleTarget);
  else if (tmpTarget.twilightAngle == TWILIGHT_ANGLE_NAUTICAL)     printf("             Twilight angle: %5.2f degrees (nautical)\n",     twilightAngleTarget);
  else if (tmpTarget.twilightAngle == TWILIGHT_ANGLE_ASTRONOMICAL) printf("             Twilight angle: %5.2f degrees (astronomical)\n", twilightAngleTarget);
  else                                                             printf("             Twilight angle: %5.2f degrees (custom angle)\n", twilightAngleTarget);

  printf   ("          Day with twilight: "); print_times (pRun, &tmpTarget, NO_OFFSET, cTo);

  if (pRun->offsetHour != NO_OFFSET)
  { printf (" Day with twilight & offset: "); print_times (pRun, &tmpTarget, pRun->offsetHour, cTo); }

  printf   ("                      It is: %s\n", isDay (pRun) == ONOFF_ON ? "Day (or twilight)" : "Night");

  /*
  ** Generate times for different types of twilight
  */

  targetStruct daylightTarget;
  daylightTarget.twilightAngle = TWILIGHT_ANGLE_DAYLIGHT;
  daylightTarget.daysSince2000 = tmpTarget.daysSince2000;
  sunriset (pRun, &daylightTarget);

  targetStruct civilTarget;
  civilTarget.twilightAngle = TWILIGHT_ANGLE_CIVIL;
  civilTarget.daysSince2000 = tmpTarget.daysSince2000;
  sunriset (pRun, &civilTarget);

  targetStruct nauticalTarget;
  nauticalTarget.twilightAngle = TWILIGHT_ANGLE_NAUTICAL;
  nauticalTarget.daysSince2000 = tmpTarget.daysSince2000;
  sunriset (pRun, &nauticalTarget);

  targetStruct astronomicalTarget;
  astronomicalTarget.twilightAngle = TWILIGHT_ANGLE_ASTRONOMICAL;
  astronomicalTarget.daysSince2000 = tmpTarget.daysSince2000;
  sunriset (pRun, &astronomicalTarget);

  printf ("\nGeneral Information (no offset) ...\n\n");

  printf (" Times ...         Daylight: "); print_times (pRun, &tmpTarget,          NO_OFFSET, cTo);
  printf ("        with Civil twilight: "); print_times (pRun, &civilTarget,        NO_OFFSET, cTo);
  printf ("     with Nautical twilight: "); print_times (pRun, &nauticalTarget,     NO_OFFSET, cTo);
  printf (" with Astronomical twilight: "); print_times (pRun, &astronomicalTarget, NO_OFFSET, cTo);
  printf ("\n");
  printf (" Duration ...    Day length: %2.2d:%2.2d hours\n", hours (    daylightTarget.diurnalArc), minutes (    daylightTarget.diurnalArc));
  printf ("        with civil twilight: %2.2d:%2.2d hours\n", hours (       civilTarget.diurnalArc), minutes (       civilTarget.diurnalArc));
  printf ("     with nautical twilight: %2.2d:%2.2d hours\n", hours (    nauticalTarget.diurnalArc), minutes (    nauticalTarget.diurnalArc));
  printf (" with astronomical twilight: %2.2d:%2.2d hours\n", hours (astronomicalTarget.diurnalArc), minutes (astronomicalTarget.diurnalArc));
  printf ("\n");
}

void print_list (const runStruct *pRun)
{
  targetStruct tmpTarget;

  tmpTarget.daysSince2000 = pRun->target2000;
  tmpTarget.twilightAngle = pRun->twilightAngle;

  for (unsigned int day=0; day < pRun->listDays; day++)
  {
    sunriset (pRun, &tmpTarget);
    print_times
      ( pRun
      , &tmpTarget
      , pRun->offsetHour
      , cComma
      );
    tmpTarget.daysSince2000++;
  }
}
