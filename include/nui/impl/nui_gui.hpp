#ifndef NUI_GUI_HPP
#define NUI_GUI_HPP

#include "nui/nui_types.h"
#include <string>

namespace nui {

     class gui {
     public:
          gui();
          virtual ~gui();
     private:
          nui_error_code m_last_error_code;
          std::string m_last_error_message;
     };

}

#endif
