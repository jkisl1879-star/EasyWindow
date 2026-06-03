#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <functional>

class EasyWin;

// Button class
class Button {
private:
    HWND hwnd;
    EasyWin* parent;
    std::string text;
    int x, y, width, height;
    int id;
    std::function<void()> onClick;

public:
    Button();
    ~Button();

    void Create(EasyWin* parentWindow, const std::string& buttonText,
        int x, int y, int w, int h, int buttonId);
    void SetText(const std::string& newText);
    std::string GetText();
    void Show();
    void Hide();
    void Enable();
    void Disable();
    void SetClickHandler(std::function<void()> handler);
    HWND GetHWND() { return hwnd; }
    int GetId() { return id; }
};

// Main window class
class EasyWin {
private:
    HWND hwnd;
    std::string title;
    int width;
    int height;
    bool isRunning;
    HDC currentDC;

    // Double buffering
    HDC memDC;
    HBITMAP memBitmap;
    int backBufferWidth;
    int backBufferHeight;

    // Buttons
    std::vector<Button*> buttons;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    EasyWin();
    virtual ~EasyWin();

    // Core functions
    void Create(const std::string& windowTitle, int w, int h);
    void Show();
    void Run();
    void Close();
    void Redraw();

    // Drawing functions
    void Clear(int r, int g, int b);
    void DrawRect(int x, int y, int w, int h, int r, int g, int b);
    void DrawCircle(int x, int y, int radius, int r, int g, int b);
    void DrawText(const std::string& text, int x, int y, int r, int g, int b);
    void DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b);

    // Button management
    Button* AddButton(const std::string& text, int x, int y, int w, int h);
    void RemoveButton(Button* button);
    void RemoveButton(int index);

    // Events (override these)
    virtual void OnDraw() {}
    virtual void OnKeyDown(int key) {}
    virtual void OnMouseMove(int x, int y) {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnResize(int width, int height) {}
    virtual void OnButtonClick(int buttonId) {}

    // Utility functions
    void Message(const std::string& text, const std::string& caption);
    int GetWidth() { return width; }
    int GetHeight() { return height; }
    HWND GetHWND() { return hwnd; }

    // Friend classes
    friend class Button;
};
