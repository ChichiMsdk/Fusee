#ifndef FUSEE_H
#define FUSEE_H

#include "init.h"

void FuseeInit(Fusee* pFusee);
void FuseeDraw(Fusee* pFusee);
void FuseeUpdate(Fusee* pFusee, float gravity);
static inline void FuseeCheck(Fusee* pFusee);

#endif
