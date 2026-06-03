# EasyWindow

**EasyWindow** - простая библиотека для создания окон на C++ через Win32 API.

## Быстрый старт

```cpp
#include "EasyWindow.h"

class MyWindow : public EasyWindow {
    void OnDraw() override {
        Clear(40, 40, 80);
        DrawText("Hello!", 380, 300, 255, 255, 255);
    }
};

int main() {
    MyWindow window;
    window.Create("Моё окно", 800, 600);
    window.Show();
    window.Run();
    return 0;
}
