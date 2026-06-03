# EasyWindow

A simple C++ library for creating windows with Windows.h. No extra dependencies, just easy to use.

## Quick example

```cpp
#include "EasyWindow.h"

class MyWindow : public EasyWindow {
    void OnDraw() override {
        Clear(30, 30, 50);
        DrawCircle(400, 300, 100, 255, 0, 0);
        DrawText("Hello!", 380, 50, 255, 255, 255);
    }
};

int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int show) {
    MyWindow window;
    window.Create("My Window", 800, 600);
    window.Show();
    window.Run();
    return 0;
}
