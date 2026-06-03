# 🪟 EasyWindow

**EasyWindow** is a lightweight C++ wrapper around the Win32 API that lets you create windows and draw graphics in just a few lines of code. Perfect for beginners, students, and rapid prototyping.

## ✨ Features

- 🚀 **Simple API** – Create a window in 3 lines of code
- 🎨 **Built-in drawing** – Rectangles, circles, lines, and text
- 🖱️ **Event system** – Keyboard, mouse, and button events
- 🔘 **Native Windows buttons** – Real Windows controls
- ⚡ **Double buffering** – No flickering, smooth animations
- 📦 **No dependencies** – Only standard Windows libraries

## 🚀 Quick Start

```cpp
#include "EasyWindow.h"

class MyWindow : public EasyWindow {
    void OnDraw() override {
        Clear(30, 30, 50);                        // Dark blue background
        DrawCircle(400, 300, 100, 255, 0, 0);     // Red circle
        DrawText("Hello EasyWindow!", 350, 50, 255, 255, 255);
    }
    
    void OnKeyDown(int key) override {
        if (key == VK_ESCAPE) Close();
    }
};

int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int show) {
    MyWindow window;
    window.Create("My First Window", 800, 600);
    window.Show();
    window.Run();
    return 0;
}
