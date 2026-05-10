#pragma once

#include <string>

namespace Aefera {
namespace UI {

    class WindowManager {
    public:
        static WindowManager& getInstance();
        
        void init();
        void showBootAnimation();
        void renderDesktop();
        void update();
        
    private:
        WindowManager() = default;
        ~WindowManager() = default;
    };

} // namespace UI
} // namespace Aefera
