#ifndef _SHARED_H_
#define _SHARED_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>
#include <zlib.h>

#define MAX_PATH 1024
#define PATH_MAX MAX_PATH

#include "types.h"
#include "macros.h"
#include "cpu/z80.h"
#include "sms.h"
#include "pio.h"
#include "memz80.h"
#include "vdp.h"
#include "render.h"
#include "sound/sn76489.h"
#include "sound/emu2413.h"
#include "sound/ym2413.h"
#include "sound/fmintf.h"
#include "sound/stream.h"
#include "sound/sound.h"
#include "system.h"
#include "error.h"

#include "state.h"
#include "fileio.h"
#include "loadrom.h"
#include "unzip/unzip.h"

#endif /* _SHARED_H_ */
