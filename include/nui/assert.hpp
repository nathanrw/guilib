#ifndef NUI_ASSERT_H
#define NUI_ASSERT_H

#ifndef NUI_ASSERT
#define NUI_ASSERT(EXPR, MSG) \
    if (!(EXPR)) \
        nui::handle_assertion(\
            #EXPR, \
            __FILE__, \
            __FUNCTION__, \
            __LINE__, \
            MSG \
        );
#endif
#define NUI_PRE(EXPR, MSG) NUI_ASSERT(EXPR, MSG)
#define NUI_POST(EXPR, MSG) NUI_POST(EXPR, MSG)

namespace nui {
    void handle_assertion(
        const char* expr,
        const char* filename,
        const char* function,
        int line,
        const char* msg
    );
}

#endif
