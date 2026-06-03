#include "EasyWin.h"

static EasyWin* currentWindow = nullptr;

// ============ Реализация Button ============

Button::Button() : hwnd(nullptr), parent(nullptr), x(0), y(0), width(0), height(0), id(0) {}

Button::~Button() {
    if (hwnd) {
        DestroyWindow(hwnd);
    }
}

void Button::Create(EasyWin* parentWindow, const std::string& buttonText,
    int x, int y, int w, int h, int buttonId) {
    parent = parentWindow;
    text = buttonText;
    this->x = x;
    this->y = y;
    width = w;
    height = h;
    id = buttonId;

    // Конвертируем string в wstring
    std::wstring wtext(buttonText.begin(), buttonText.end());

    hwnd = CreateWindow(
        L"BUTTON",           // Класс кнопки Windows
        wtext.c_str(),       // Текст кнопки
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Стили
        x, y, w, h,         // Позиция и размер
        parentWindow->GetHWND(),  // Родительское окно
        (HMENU)(INT_PTR)buttonId, // ID кнопки
        GetModuleHandle(nullptr),
        nullptr
    );
}

void Button::SetText(const std::string& newText) {
    text = newText;
    std::wstring wtext(newText.begin(), newText.end());
    SetWindowText(hwnd, wtext.c_str());
}

std::string Button::GetText() {
    wchar_t buffer[256];
    GetWindowText(hwnd, buffer, 256);
    std::wstring wtext(buffer);
    return std::string(wtext.begin(), wtext.end());
}

void Button::Show() {
    ShowWindow(hwnd, SW_SHOW);
}

void Button::Hide() {
    ShowWindow(hwnd, SW_HIDE);
}

void Button::Enable() {
    EnableWindow(hwnd, TRUE);
}

void Button::Disable() {
    EnableWindow(hwnd, FALSE);
}

void Button::SetClickHandler(std::function<void()> handler) {
    onClick = handler;
}

// ============ Реализация EasyWin ============

LRESULT CALLBACK EasyWin::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_CREATE) {
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        currentWindow = (EasyWin*)cs->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)currentWindow);
    }
    else {
        currentWindow = (EasyWin*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (currentWindow) {
        switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Получаем размеры клиентской области
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int clientWidth = clientRect.right - clientRect.left;
            int clientHeight = clientRect.bottom - clientRect.top;

            // Создаём временный буфер если его нет или изменился размер
            if (!currentWindow->memDC || clientWidth != currentWindow->backBufferWidth ||
                clientHeight != currentWindow->backBufferHeight) {

                if (currentWindow->memBitmap) {
                    DeleteObject(currentWindow->memBitmap);
                    currentWindow->memBitmap = nullptr;
                }
                if (currentWindow->memDC) {
                    DeleteDC(currentWindow->memDC);
                    currentWindow->memDC = nullptr;
                }

                currentWindow->backBufferWidth = clientWidth;
                currentWindow->backBufferHeight = clientHeight;
                currentWindow->memDC = CreateCompatibleDC(hdc);
                currentWindow->memBitmap = CreateCompatibleBitmap(hdc, clientWidth, clientHeight);
                SelectObject(currentWindow->memDC, currentWindow->memBitmap);
            }

            // Очищаем буфер
            RECT rect = { 0, 0, currentWindow->backBufferWidth, currentWindow->backBufferHeight };
            HBRUSH brush = CreateSolidBrush(RGB(240, 240, 240));
            FillRect(currentWindow->memDC, &rect, brush);
            DeleteObject(brush);

            // Рисуем в временный буфер
            currentWindow->currentDC = currentWindow->memDC;
            currentWindow->OnDraw();

            // Копируем на экран
            BitBlt(hdc, 0, 0, currentWindow->backBufferWidth, currentWindow->backBufferHeight,
                currentWindow->memDC, 0, 0, SRCCOPY);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_SIZE: {
            int newWidth = LOWORD(lParam);
            int newHeight = HIWORD(lParam);
            currentWindow->width = newWidth;
            currentWindow->height = newHeight;
            currentWindow->OnResize(newWidth, newHeight);

            // При изменении размера пересоздадим буфер
            if (currentWindow->memDC) {
                DeleteDC(currentWindow->memDC);
                currentWindow->memDC = nullptr;
            }
            if (currentWindow->memBitmap) {
                DeleteObject(currentWindow->memBitmap);
                currentWindow->memBitmap = nullptr;
            }
            return 0;
        }

        case WM_COMMAND: {
            // Обработка нажатия кнопок
            int buttonId = LOWORD(wParam);
            currentWindow->OnButtonClick(buttonId);

            // Вызываем обработчик кнопки если он есть
            for (auto* btn : currentWindow->buttons) {
                if (btn->GetId() == buttonId) {
                    // Можно вызвать лямбду если установлена
                    break;
                }
            }
            return 0;
        }

        case WM_KEYDOWN:
            currentWindow->OnKeyDown((int)wParam);
            return 0;

        case WM_MOUSEMOVE:
            currentWindow->OnMouseMove((int)LOWORD(lParam), (int)HIWORD(lParam));
            return 0;

        case WM_DESTROY:
            currentWindow->isRunning = false;
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

EasyWin::EasyWin()
    : hwnd(nullptr),
    width(800),
    height(600),
    isRunning(false),
    currentDC(nullptr),
    memDC(nullptr),
    memBitmap(nullptr),
    backBufferWidth(0),
    backBufferHeight(0) {
}

EasyWin::~EasyWin() {
    for (auto* btn : buttons) {
        delete btn;
    }
    buttons.clear();

    if (memBitmap) DeleteObject(memBitmap);
    if (memDC) DeleteDC(memDC);
    Close();
}

void EasyWin::Create(const std::string& windowTitle, int w, int h) {
    title = windowTitle;
    width = w;
    height = h;

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"EasyWinClass";
    wc.hbrBackground = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassEx(&wc);

    std::wstring wtitle(windowTitle.begin(), windowTitle.end());

    hwnd = CreateWindowEx(
        0,
        L"EasyWinClass",
        wtitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr,
        GetModuleHandle(nullptr),
        this
    );
}

void EasyWin::Show() {
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    isRunning = true;
}

void EasyWin::Run() {
    MSG msg = {};

    while (isRunning) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        OnUpdate(0.016f);
        Redraw();
    }
}

void EasyWin::Close() {
    isRunning = false;
    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = nullptr;
    }
}

void EasyWin::Redraw() {
    if (hwnd) {
        InvalidateRect(hwnd, nullptr, FALSE);
    }
}

// Исправленный AddButton
Button* EasyWin::AddButton(const std::string& text, int x, int y, int w, int h) {
    Button* btn = new Button();
    int id = 1000 + (int)buttons.size();
    btn->Create(this, text, x, y, w, h, id);  // ← this теперь правильный
    buttons.push_back(btn);
    return btn;
}

void EasyWin::RemoveButton(Button* button) {
    auto it = std::find(buttons.begin(), buttons.end(), button);
    if (it != buttons.end()) {
        delete* it;
        buttons.erase(it);
    }
}

void EasyWin::RemoveButton(int index) {
    if (index >= 0 && index < (int)buttons.size()) {
        delete buttons[index];
        buttons.erase(buttons.begin() + index);
    }
}

void EasyWin::Clear(int r, int g, int b) {
    if (!currentDC) return;
    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH brush = CreateSolidBrush(RGB(r, g, b));
    FillRect(currentDC, &rect, brush);
    DeleteObject(brush);
}

void EasyWin::DrawRect(int x, int y, int w, int h, int r, int g, int b) {
    if (!currentDC) return;
    HBRUSH brush = CreateSolidBrush(RGB(r, g, b));
    RECT rect = { x, y, x + w, y + h };
    FillRect(currentDC, &rect, brush);
    DeleteObject(brush);
}

void EasyWin::DrawCircle(int x, int y, int radius, int r, int g, int b) {
    if (!currentDC) return;
    HBRUSH brush = CreateSolidBrush(RGB(r, g, b));
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(r, g, b));

    HGDIOBJ oldBrush = SelectObject(currentDC, brush);
    HGDIOBJ oldPen = SelectObject(currentDC, pen);

    Ellipse(currentDC, x - radius, y - radius, x + radius, y + radius);

    SelectObject(currentDC, oldBrush);
    SelectObject(currentDC, oldPen);
    DeleteObject(brush);
    DeleteObject(pen);
}

void EasyWin::DrawText(const std::string& text, int x, int y, int r, int g, int b) {
    if (!currentDC) return;
    SetTextColor(currentDC, RGB(r, g, b));
    SetBkMode(currentDC, TRANSPARENT);
    TextOutA(currentDC, x, y, text.c_str(), (int)text.length());
}

void EasyWin::DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b) {
    if (!currentDC) return;
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(r, g, b));
    HGDIOBJ oldPen = SelectObject(currentDC, pen);

    MoveToEx(currentDC, x1, y1, nullptr);
    LineTo(currentDC, x2, y2);

    SelectObject(currentDC, oldPen);
    DeleteObject(pen);
}

void EasyWin::Message(const std::string& text, const std::string& caption) {
    MessageBoxA(hwnd, text.c_str(), caption.c_str(), MB_OK);
}