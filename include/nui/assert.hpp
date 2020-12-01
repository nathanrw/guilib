#ifndef NUI_ASSERT_H
#define NUI_ASSERT_H

#include <cassert>

#define NUI_ASSERT(EXPR, MSG) assert((EXPR) && MSG)

#endif