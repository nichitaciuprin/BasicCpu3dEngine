#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <array>
using namespace std;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <winuser.h>

#include "Debug.h"
#include "Math.h"
#include "FileHelper.h"
#include "Time.h"
#include "Subgen.h"
#include "Physics.h"
#include "Bitmap.h"
#include "BitmapWindow.h"
#include "BitmapWindow1.h"