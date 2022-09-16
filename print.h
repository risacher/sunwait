//
// sunwait - print.h
//
// 08-12-2014  IFC  0.6  No changes from 0.5
// 02-05-2015  IFC  0.7  No changes from 0.5, still
// 2015-05-27  IFC  0.8  Resolve 'dodgy day' and cleanup
// 2022-09-16  DRR  0.91 Convert to C courtesy mstilkerich 
//

#ifndef PRINT_H
#define PRINT_H

#include "sunwait.h"
#include "sunriset.h"

void generate_report(const runStruct *pRun);

void print_list (const runStruct *pRun);

double diurnalArcWithOffset (const runStruct *pRun, const targetStruct *pTarget);
double getOffsetRiseHourUTC (const runStruct *pRun, const targetStruct *pTarget);
double getOffsetSetHourUTC  (const runStruct *pRun, const targetStruct *pTarget);


#endif
