\# 🪟 EasyWin - Simple Windows GUI Library for C++



\*\*EasyWin\*\* is a lightweight wrapper around the Win32 API that lets you create windows and draw graphics in just a few lines of code. Perfect for beginners, students, and rapid prototyping.



\## ✨ Features



\- 🚀 \*\*Simple API\*\* - Create a window in 3 lines of code

\- 🎨 \*\*Built-in drawing\*\* - Rectangles, circles, lines, and text

\- 🖱️ \*\*Event system\*\* - Keyboard, mouse, and button events

\- 🔘 \*\*Native Windows buttons\*\* - Real Windows buttons with click handling

\- ⚡ \*\*Double buffering\*\* - No flickering, smooth animations

\- 📦 \*\*No dependencies\*\* - Only standard Windows libraries

\- 💯 \*\*Lightweight\*\* - Single header + implementation file



\## 📸 Example



```cpp

\#include "EasyWin.h"



class MyWindow : public EasyWin {

&#x20;   void OnDraw() override {

&#x20;       Clear(30, 30, 50);                        // Dark blue background

&#x20;       DrawCircle(400, 300, 100, 255, 0, 0);     // Red circle

&#x20;       DrawText("Hello EasyWin!", 350, 50, 255, 255, 255);  // White text

&#x20;   }

&#x20;   

&#x20;   void OnKeyDown(int key) override {

&#x20;       if (key == VK\_ESCAPE) Close();             // Exit on ESC

&#x20;   }

};



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 

&#x20;                  LPSTR lpCmdLine, int nCmdShow) {

&#x20;   MyWindow window;

&#x20;   window.Create("My First Window", 800, 600);

&#x20;   window.Show();

&#x20;   window.Run();

&#x20;   return 0;

}

