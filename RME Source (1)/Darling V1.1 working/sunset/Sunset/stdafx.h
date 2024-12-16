#pragma once

#include <xtl.h>
#include <xboxmath.h>
#include <stdio.h>
#include <map>
#include <math.h>
#include <xkelib.h>
#include <vector>
#include <array>
#include <ctime>
#include <xhttp.h>
#include <setjmp.h>

using namespace std;

#define DEBUG_BUILD

#ifdef DEBUG_BUILD
#define DUMP_BINS
#endif

#include "MemoryManager.h"
#include "Addresses.h"
#include "Vector.h"
#include "Globals.h"

#include "HvPeekPoke.h"
#include "Utilities.h"
#include "Detour.h"

#include "Arrays.h"

#include "Structs.h"
#include "Definitions.h"
#include "Game.h"
#include "RME.h"
#include "Visuals.h"
#include "Autowall.h"
#include "Spinbot.h"
#include "Aimbot.h"
#include "Render.h"
#include "Visuals.h"
#include "Menu.h"
#include "SimpleIni.h"
#include "INI_Manager.h"
#include "Hooks.h"