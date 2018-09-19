#include "../Common.h"

#ifndef MB7092_H
#define MB7092_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void MB7092_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

uint16_t MB7092_measureDistance();

#endif // MB7092_H