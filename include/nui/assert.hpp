#ifndef NUI_ASSERT_H
#define NUI_ASSERT_H

#include <cassert>

#define NUI_ASSERT(EXPR, MSG) assert((EXPR) && MSG)
#define NUI_PRE(EXPR, MSG) NUI_ASSERT(EXPR, MSG)
#define NUI_POST(EXPR, MSG) NUI_POST(EXPR, MSG)

#endif
