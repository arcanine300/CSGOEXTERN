#pragma once

// Stuff we actually need
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <SDKDDKVer.h>
#include <stdio.h>
#include <optional>
#include <Psapi.h>
#include <algorithm>
#include <random>
#include <ctime>

// Offsets
#include "Offsets.hpp"
// Misc stuff
#include "Variables.h"
#include "MemoryManager.h"
#include "Entity.h"
#include "Features.h"
#include "Aim.h"

extern MemoryManager *m;