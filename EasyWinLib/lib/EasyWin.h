#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <functional>

class EasyWin;

// Класс кнопки
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

class EasyWin {
private:
    HWND hwnd;
    std::string title;
    int width;
    int height;
    bool isRunning;
    HDC currentDC;

    // Для двойной буферизации
    HDC memDC;
    HBITMAP memBitmap;
    int backBufferWidth;
    int backBufferHeight;

    // Кнопки
    std::vector<Button*> buttons;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    EasyWin();
    virtual ~EasyWin();

    // Основные функции
    void Create(const std::string& windowTitle, int w, int h);
    void Show();
    void Run();
    void Close();
    void Redraw();

    // Функции рисования
    void Clear(int r, int g, int b);
    void DrawRect(int x, int y, int w, int h, int r, int g, int b);
    void DrawCircle(int x, int y, int radius, int r, int g, int b);
    void DrawText(const std::string& text, int x, int y, int r, int g, int b);
    void DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b);

    // Управление кнопками
    Button* AddButton(const std::string& text, int x, int y, int w, int h);
    void RemoveButton(Button* button);
    void RemoveButton(int index);

    // События (переопределяемые)
    virtual void OnDraw() {}
    virtual void OnKeyDown(int key) {}
    virtual void OnMouseMove(int x, int y) {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnResize(int width, int height) {}
    virtual void OnButtonClick(int buttonId) {}

    // Вспомогательные функции
    void Message(const std::string& text, const std::string& caption);
    int GetWidth() { return width; }
    int GetHeight() { return height; }
    HWND GetHWND() { return hwnd; }

    // Дружественные классы
    friend class Button;
};