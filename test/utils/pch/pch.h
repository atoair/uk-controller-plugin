//
// pch.h
// Header for standard system include files.
//
#pragma once

#include "../../src/utils/pch/pch.h"

// Ignore warnings about uninitialised variables in the Gmock headers
#pragma warning( push )
#pragma warning( disable : 26495 26451 28251)
#include "gmock/gmock.h"
#pragma warning( pop )