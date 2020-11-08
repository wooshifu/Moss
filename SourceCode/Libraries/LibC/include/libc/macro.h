#pragma once

#ifndef __STRINGIFY__
#define __INTERNAL_STRINGIFY_HELPER__(x) #x
#define __STRINGIFY__(x)                 __INTERNAL_STRINGIFY_HELPER__(x)
#endif
