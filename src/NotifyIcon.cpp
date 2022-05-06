// The MIT License
//
// Copyright (c) 2022 serverfailure71
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// SPDX-License-Identifier: MIT

#include "mni/NotifyIcon.hpp"

#include <shellapi.h>
#include <windowsx.h>

//#define USE_TRACE

#if defined(_DEBUG) && !defined(TRACE) && defined(USE_TRACE)
    #include <array>
    #include <cstdarg>
    #include <cstdio>
    #include <wchar.h>
    namespace {
        auto wtrace (const wchar_t* format, ...) -> void
        {
            auto buffer = std::array<wchar_t, 1024>();
            buffer.fill(0);

            auto args = va_list();
            va_start(args, format);
            auto size = _vsnwprintf_s(buffer.data(), buffer.size() - 2, buffer.size(), format, args);
            va_end(args);

            for (auto i = 0; i < buffer.size() - 2; i++)
            {
                if (buffer[i] == L'\0')
                {
                    buffer[i] = L'\n';
                    buffer[i+1] = L'\0';
                    break;
                }
            }

            if (size > 0)
            {
                OutputDebugStringW(buffer.data());
            }
        }
    }

    #define TRACE(fmt, ...) do{ wtrace(fmt, __VA_ARGS__); }while(0)
#else
    #define TRACE(...) do{}while(0)
#endif

namespace {
    auto IsLayoutRTL()
    {
        auto rtl = false;

        //if (auto langId = GetUserDefaultUILanguage())
        //{            
        //    //auto lcdata = LOCALESIGNATURE{};
        //    //if (GetLocaleInfoW(langId, LOCALE_FONTSIGNATURE, reinterpret_cast<LPWSTR>(&lcdata), sizeof(lcdata) / sizeof(wchar_t)))
        //    //{
        //    //    if ((lcdata.lsUsb[3] & 0x08000000) != 0)
        //    //    {
        //    //        rtl = true;
        //    //    }
        //    //}
        //}

        auto layout = DWORD{0};
        if (GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_IREADINGLAYOUT, reinterpret_cast<LPWSTR>(&layout), 2))
        {
            // Returns one of the following 4 reading layout values:
            // 0 - Left to right (eg en-US)
            // 1 - Right to left (eg arabic locales)
            // 2 - Vertical top to bottom with columns to the left and also left to right (ja-JP locales)
            // 3 - Vertical top to bottom with columns proceeding to the right
            if ((layout & 0b0011) == 1)
            {
                rtl = true;
            }
        }

        return rtl;
    }

    auto SystemUsesLightTheme () -> bool
    {
        auto data     = DWORD{0};
        auto dataSize = DWORD{sizeof(data)};
        auto status   = ::RegGetValueW(
            HKEY_CURRENT_USER,
            L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
            L"SystemUsesLightTheme",
            RRF_RT_DWORD,
            NULL,
            &data,
            &dataSize
        );

        if (status == ERROR_SUCCESS)
        {
            return static_cast<bool>(data);
        }

        return false;
    }

    auto GetThemeInfo ()
    {
        auto hc = HIGHCONTRASTW{};
        hc.cbSize = sizeof(hc);
        if (SystemParametersInfoW(SPI_GETHIGHCONTRAST, sizeof(hc), &hc, 0))
        {
            // High contrast ON.
            if ((hc.dwFlags & HCF_HIGHCONTRASTON) != 0)
            {
                const auto fc = GetSysColor(COLOR_WINDOWTEXT);
                const auto bc = GetSysColor(COLOR_WINDOW);

                return mni::ThemeInfo(mni::ThemeInfo::Theme::HighContrast, fc, bc);
            }
        }

        if (SystemUsesLightTheme())
        {
            return mni::ThemeInfo(mni::ThemeInfo::Theme::Light, 0x00000000, 0x00FFFFFF);
        }

        return mni::ThemeInfo(mni::ThemeInfo::Theme::Dark, 0x00FFFFFF, 0x00000000) ;
    }
}

namespace mni {
    auto ThemeInfo::Detect () -> ThemeInfo
    {
        return GetThemeInfo();
    }
}

namespace mni {

auto NotifyIcon::Dispatch (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CREATE:
        if (Create())
        {
            return 0;
        }
        break;

    case WM_DESTROY:
        if (Destroy())
        {
            return 0;
        }
        break;

    case WM_NOTIFYICON:
        switch (LOWORD(lParam))
        {
        // Left Click.
        //case NIN_SELECT:
        //    if (KeySelect(GET_X_LPARAM(wParam), GET_Y_LPARAM(wParam)))
        //    {
        //        return 0;
        //    }
        //    break;

        // When you select icon with keyboard and press Space or Enter.
        // Pressing Enter triggers this twice.
        case NIN_KEYSELECT:
            if (KeySelect(GET_X_LPARAM(wParam), GET_Y_LPARAM(wParam)))
            {
                return 0;
            }
            break;

        // When you Right Click on icon or Shift+F10 when it's selected with keyboard.
        case WM_CONTEXTMENU:
            if (ContextMenu(GET_X_LPARAM(wParam), GET_Y_LPARAM(wParam)))
            {
                return 0;
            }
            break;

        case WM_LBUTTONUP:
            if (LmbClick(GET_X_LPARAM(wParam), GET_Y_LPARAM(wParam)))
            {
                return 0;
            }
            break;

        case WM_LBUTTONDBLCLK:
            if (LmbDoubleClick(GET_X_LPARAM(wParam), GET_Y_LPARAM(wParam)))
            {
                return 0;
            }
            break;

        case WM_MBUTTONUP:
            if (MmbClick(GET_X_LPARAM(wParam), GET_Y_LPARAM(wParam)))
            {
                return 0;
            }
            break;

        case NIN_BALLOONSHOW:
            if (BalloonShow())
            {                
                return 0;
            }
            break;
        
        case NIN_BALLOONHIDE:
            if (BalloonHide())
            {
                return 0;
            }
            break;

        case NIN_BALLOONTIMEOUT:
            if (BalloonTimeout())
            {
                return 0;
            }
            break;

        case NIN_BALLOONUSERCLICK:
            if (BalloonUserClick())
            {
                return 0;
            }
            break;

        case NIN_POPUPOPEN:
            if (RichPopupOpen(GET_X_LPARAM(wParam), GET_Y_LPARAM(wParam)))
            {
                return 0;
            }
            break;

        case NIN_POPUPCLOSE:
            if (RichPopupClose())
            {
                return 0;
            }
            break;
        }
        break; // WM_NOTIFYICON

    case WM_DPICHANGED:
        // NOTE: delaying dpi change notification, because when it's called
        //       before TaskbarCreated message (which is triggered by dpi change)
        //       changing icons etc. don't work.
        mIsDpiEvent  = true;
        mInternalDpi = LOWORD(wParam);
        return 0;
        //return DpiChange(LOWORD(wParam));

    case WM_SETTINGCHANGE:
        if (wParam == SPI_SETHIGHCONTRAST)
        {
            return ThemeChange();
        }
        else
        {
            if (lParam != NULL && std::wstring_view(reinterpret_cast<wchar_t*>(lParam)) == L"ImmersiveColorSet")
            {
                return ThemeChange();
            }
        }
        break;

    case WM_TIMER:
        if (static_cast<UINT>(wParam) >= MNI_USER_TIMER_ID)
        {
            if (TimerTimeout(static_cast<UINT>(wParam)))
            {
                return 0;
            }
        }
        else
        {
            if (InternalTimerTimeout(static_cast<UINT>(wParam)))
            {
                return 0;
            }
        }
        break;

    case WM_DPICHANGED_DELAYED:
        if (DpiChange(mInternalDpi))
        {
            return 0;
        }
        break;

    case WM_NOTIFYICON_SHOW:
        if (IconShow())
        {
            return 0;
        }
        break;

    case WM_NOTIFYICON_HIDE:
        if (IconHide())
        {
            return 0;
        }
        break;
    }

    if (uMsg == mTaskbarCreatedMsgId)
    {
        return TaskbarCreated();
    }

    // Register message when taskbar is created to get notified when explorer.exe gets restarted.
    if (mTaskbarCreatedMsgId == 0)
    {
        mTaskbarCreatedMsgId = RegisterWindowMessageW(L"TaskbarCreated");
    }

    if (WM_APP <= uMsg && uMsg <= 0xBFFF)
    {
        if (CustomMessage(uMsg, wParam, lParam))
        {
            return 0;
        }
    }
    else
    {
        if (SystemMessage(uMsg, wParam, lParam))
        {
            return 0;
        }
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

auto NotifyIcon::Create () -> BOOL
{
    TRACE(L"Create()");

    if (OnCreate)
    {
        OnCreate();
    }

    return TRUE;
}

auto NotifyIcon::Destroy () -> BOOL
{
    TRACE(L"Destroy()");
    if (OnDestroy)
    {
        OnDestroy();
    }

    return TRUE;
}

auto NotifyIcon::IconShow () -> BOOL
{
    TRACE(L"IconShow()");
    if (OnShow)
    {
        OnShow();
    }

    return TRUE;
}

auto NotifyIcon::IconHide () -> BOOL
{
    TRACE(L"IconHide()");
    if (OnHide)
    {
        OnHide();
    }

    return TRUE;
}

auto NotifyIcon::KeySelect (int x, int y) -> BOOL
{
    TRACE(L"KeySelect(%d, %d), mDoubleKeySelectPrevention=%d", x, y, mDoubleKeySelectPrevention);

    if (!mDoubleKeySelectPrevention)
    {
        mDoubleKeySelectPrevention = true;
        InternalStartTimer(TM_DOUBLE_KEYSELECT_PREVENTION, 100);

        if (OnKeySelect)
        {
            OnKeySelect(x, y);
        }
    }

    return TRUE;
}

auto NotifyIcon::LmbClick (int x, int y) -> BOOL
{
    TRACE(L"LmbClick(%d, %d)", x, y);
    if (OnLmbClick)
    {
        OnLmbClick(x, y);
    }

    return TRUE;
}

auto NotifyIcon::LmbDoubleClick (int x, int y) -> BOOL
{
    TRACE(L"LmbDoubleClick(%d, %d)", x, y);
    if (OnLmbDoubleClick)
    {
        OnLmbDoubleClick(x, y);
    }

    return TRUE;
}

auto NotifyIcon::MmbClick (int x, int y) -> BOOL
{
    TRACE(L"MmbClick(%d, %d)", x, y);
    if (OnMmbClick)
    {
        OnMmbClick(x, y);
    }

    return TRUE;
}

auto NotifyIcon::ContextMenu (int x, int y) -> BOOL
{
    TRACE(L"ContextMenu(%d, %d)", x, y);
    if (OnContextMenuOpen)
    {
        OnContextMenuOpen();
    }

    //auto cursor = POINT{0};
    //auto result = GetCursorPos(&cursor);
    //if (result != 0) {
    //    position = cursor;
    //}

    auto selectedItem = 0;

    if (mMenu)
    {
        auto hSubMenu = GetSubMenu(mMenu, 0);
        if (hSubMenu)
        {
            // Our window must be foreground before calling TrackPopupMenu
            // or the menu will not disappear when the user clicks away.
            SetForegroundWindow(mWindowHandle);

            // Respect menu drop alignment.
            auto uFlags = UINT{};
            if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0)
            {
                uFlags |= TPM_RIGHTALIGN;
                uFlags = TPM_VERNEGANIMATION | TPM_RETURNCMD | TPM_NONOTIFY | TPM_BOTTOMALIGN | TPM_RIGHTALIGN | TPM_RIGHTBUTTON;
            }
            else
            {
                uFlags |= TPM_LEFTALIGN;
                uFlags = TPM_VERNEGANIMATION | TPM_RETURNCMD | TPM_NONOTIFY | TPM_BOTTOMALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON;
            }

            // HACK This doesn't work.
            //// Check for rtl layout.
            //auto pdwDefaultLayout = DWORD{};
            //if (GetProcessDefaultLayout(&pdwDefaultLayout))
            //{
            //    if (pdwDefaultLayout == LAYOUT_RTL)
            //    {
            //        uFlags = uFlags | TPM_LAYOUTRTL;                    
            //    }
            //}
            if (IsLayoutRTL())
            {
                uFlags = uFlags | TPM_LAYOUTRTL;                
            }

        #if defined(FEATURE_IMMERSIVE_CONTEXT_MENU_ENABLED)
            if (mUseImmersiveContextMenu)
            {
                selectedItem = ImmersiveContextMenu::ImmersiveTrackPopupMenu(hSubMenu, uFlags, x, y, mWindowHandle, nullptr, mImmersiveContextMenuStyle, true);
            }
            else
        #endif
            {       
                selectedItem = TrackPopupMenu(hSubMenu, uFlags, x, y, 0, mWindowHandle, nullptr);
            }
        }
    }

    TRACE(L"\tSelected Item: %d", selectedItem);
    if (OnContextMenuSelect && selectedItem != 0)
    {
        OnContextMenuSelect(selectedItem);
    }
    
    if (OnContextMenuClose)
    {
        OnContextMenuClose();
    }

    return TRUE;
}

auto NotifyIcon::BalloonShow () -> BOOL
{
    TRACE(L"BalloonShow()");
    if (OnBalloonShow)
    {
        OnBalloonShow();
    }

    return TRUE;
}

auto NotifyIcon::BalloonHide () -> BOOL
{
    TRACE(L"BalloonHide()");
    if (OnBalloonHide)
    {
        OnBalloonHide();
    }

    return TRUE;
}

auto NotifyIcon::BalloonTimeout () -> BOOL
{
    TRACE(L"BalloonTimeout()");
    if (OnBalloonTimeout)
    {
        OnBalloonTimeout();
    }

    return TRUE;
}

auto NotifyIcon::BalloonUserClick () -> BOOL
{
    TRACE(L"BalloonUserClick()");
    if (OnBalloonUserClick)
    {
        OnBalloonUserClick();
    }

    return TRUE;
}

auto NotifyIcon::RichPopupOpen (int x, int y) -> BOOL
{
    TRACE(L"RichPopupOpen(%d, %d)", x, y);
    if (OnRichPopupOpen)
    {
        OnRichPopupOpen(x, y);
    }

    return TRUE;
}

auto NotifyIcon::RichPopupClose () -> BOOL
{
    TRACE(L"RichPopupClose()");
    if (OnRichPopupClose)
    {
        OnRichPopupClose();
    }

    return TRUE;
}

auto NotifyIcon::DpiChange (int dpi) -> BOOL
{
    TRACE(L"DpiChange(%d)", dpi);
    if (OnDpiChange)
    {
        OnDpiChange(dpi);
    }

    return TRUE;
}

auto NotifyIcon::ThemeChange () -> BOOL
{
    TRACE(L"ThemeChange()");
    if (OnThemeChange)
    {
        // Don't call callback if nothing changed.
        auto info = GetThemeInfo();
        if (info != mInternalThemeInfo)
        {
            mInternalThemeInfo = info;
            OnThemeChange(info);
        }
    }

    return TRUE;
}

auto NotifyIcon::TaskbarCreated () -> BOOL
{
    TRACE(L"TaskbarCreated(), mIsDpiEvent=%d", mIsDpiEvent);
    // NOTE: This event can be also called from WM_DPICHANGED.
    if (mIsDpiEvent)
    {
        mIsDpiEvent = false;
        SendMessageW(mWindowHandle, WM_DPICHANGED_DELAYED, 0, 0);
    }
    else
    {
        // Icon no longer exists.
        mIconVisible = false;
        mIconCreated = false;

        if (OnTaskbarCreated)
        {
            OnTaskbarCreated();
        }
    }

    return TRUE;
}

auto NotifyIcon::TimerTimeout (UINT id) -> BOOL
{
    TRACE(L"TimerTimeout(id=%d)", id);
    if (id >= MNI_USER_TIMER_ID)
    {
        if (OnTimer)
        {
            OnTimer(id);
        }
    }

    return TRUE;
}

auto NotifyIcon::CustomMessage (UINT uMsg, WPARAM wParam, LPARAM lParam) -> BOOL
{
    TRACE(L"CustomMessage(uMsg=%s, wParam=%lld, lParam=%lld", uMsg, wParam, lParam);
    if (OnCustomMessage)
    {
        OnCustomMessage(uMsg, wParam, lParam);
    }

    return TRUE;
}

auto NotifyIcon::SystemMessage (UINT uMsg, WPARAM wParam, LPARAM lParam) -> BOOL
{
    TRACE(L"SystemMessage(uMsg=%s, wParam=%lld, lParam=%lld", uMsg, wParam, lParam);
    if (OnSystemMessage)
    {
        return OnSystemMessage(uMsg, wParam, lParam);
    }

    return FALSE; // we don't want to handle this message
}

auto NotifyIcon::UpdateIcon (HICON icon) -> HRESULT
{
    TRACE(L"UpdateIcon(%lld), mIconCreated=%d", icon, mIconCreated);
    if (mIconCreated)
    {
        auto nid = NOTIFYICONDATAW{
            .cbSize = sizeof(NOTIFYICONDATAW),
            .hWnd   = mWindowHandle,
            .uID    = 0,
            .uFlags = NIF_ICON,
            .hIcon  = icon,
        };

        if (mUseGuid)
        {
            nid.uFlags |= NIF_GUID;
            nid.guidItem = mGuid;
        }

        if (mUseStandardTip)
        {
            nid.uFlags |= NIF_SHOWTIP;
        }

        if (!Shell_NotifyIconW(NIM_MODIFY, &nid))
        {
            return MNI_FAILED_TO_CHANGE_ICON;
        }
    }

    return MNI_SUCCESS;
}

auto NotifyIcon::UpdateMenu (HMENU menu) -> HRESULT
{
    TRACE(L"UpdateMenu(%lld)", menu);
    return MNI_SUCCESS;
}

auto NotifyIcon::UpdateTip (std::wstring_view tip) -> HRESULT
{
    TRACE(L"UpdateTip(%s)", tip.data());
    if (mIconCreated)
    {
        auto nid = NOTIFYICONDATAW{
            .cbSize = sizeof(NOTIFYICONDATAW),
            .hWnd   = mWindowHandle,
            .uID    = 0,
            .uFlags = NIF_TIP
        };

        if (mUseGuid)
        {
            nid.uFlags |= NIF_GUID;
            nid.guidItem = mGuid;
        }

        if (mUseStandardTip)
        {
            nid.uFlags |= NIF_SHOWTIP;
        
            if (wcscpy_s(nid.szTip, tip.data()) != 0)
            {
                return MNI_FAILED_TO_COPY_TIP;
            }
        }

        if (!Shell_NotifyIconW(NIM_MODIFY, &nid))
        {
            return MNI_FAILED_TO_CHANGE_TIP;
        }
    }

    return MNI_SUCCESS;
}

auto NotifyIcon::InternalCreateWindow  () -> HRESULT
{
    TRACE(
        L"InternalCreateWindow(), mWindowHandle=%lld, mInstance=%lld, mWindowTitle=%s, mClassName=%s",
        mWindowHandle, mInstance, mWindowTitle.c_str(), mClassName.c_str()
    );
    if (mWindowHandle)
    {
        return MNI_WINDOW_ALREADY_CREATED;
    }

    // Get hInstance.
    if (mInstance == nullptr)
    {
        mInstance = GetModuleHandleW(nullptr);
        if (!mInstance)
        {
            return MNI_INVALID_INSTANCE;
        }
    }

    // Init window class.
    {
        const auto wcex = WNDCLASSEXW{
            .cbSize        = sizeof(WNDCLASSEXW),
            .style         = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc   = NotifyIconProc,
            .cbClsExtra    = 0,
            .cbWndExtra    = 0,
            .hInstance     = mInstance,
            .hIcon         = NULL,
            .hCursor       = NULL,
            .hbrBackground = NULL,
            .lpszMenuName  = nullptr,
            .lpszClassName = mClassName.c_str(),
            .hIconSm       = NULL
        };

        if (!RegisterClassExW(&wcex))
        {
            return MNI_FAILED_TO_REGISTER_WNDCLASS;
        }
    }

    // Create invisible window.
    {
        mWindowHandle = CreateWindowW(
            mClassName.c_str(),
            mWindowTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            nullptr,
            nullptr,
            mInstance,
            this        // <- pass this pointer in LPCREATESTRUCT
        );

        if (!mWindowHandle)
        {
            return MNI_FAILED_TO_CREATE_WINDOW;
        }

        UpdateWindow(mWindowHandle);
    }

    // Register message when taskbar is created to get notified when explorer.exe gets restarted.
    if (mTaskbarCreatedMsgId == 0)
    {
        mTaskbarCreatedMsgId = RegisterWindowMessageW(L"TaskbarCreated");
    }

    return MNI_SUCCESS;
}

auto NotifyIcon::InternalCreateNotifyIcon () -> HRESULT
{
    TRACE(L"InternalCreateNotifyIcon(), mIconCreated=%d, mWindowHandle=%lld", mIconCreated, mWindowHandle);
    if (mIconCreated)
    {
        return MNI_ICON_ALREADY_CREATED;
    }

    if (mWindowHandle == NULL)
    {
        return MNI_INVALID_WINDOW_HANDLE;
    }

    auto nid = NOTIFYICONDATAW{
        .cbSize           = sizeof(NOTIFYICONDATAW),
        .hWnd             = mWindowHandle,
        .uID              = 0,
        .uFlags           = NIF_TIP | NIF_ICON | NIF_MESSAGE, // NIF_TIP is required for standard tip and rich popup
        .uCallbackMessage = WM_NOTIFYICON,
        .hIcon            = mIcon,
        .szTip            = 0,
        .dwState          = 0,
        .dwStateMask      = 0,
        .szInfo           = 0,
        .uVersion         = NOTIFYICON_VERSION_4,
        .szInfoTitle      = 0,
        .dwInfoFlags      = 0,
        .guidItem         = 0,
        .hBalloonIcon     = 0,
    };

    if (mUseGuid)
    {
        nid.uFlags |= NIF_GUID;
        nid.guidItem = mGuid;
    }

    if (mUseStandardTip)
    {
        nid.uFlags |= NIF_SHOWTIP;
        
        if (wcscpy_s(nid.szTip, mTip.data()) != 0)
        {
            return MNI_FAILED_TO_COPY_TIP;
        }
    }
    
    if (!Shell_NotifyIconW(NIM_ADD, &nid))
    {
        return MNI_FAILED_TO_ADD_ICON;
    }

    if (!Shell_NotifyIconW(NIM_SETVERSION, &nid))
    {
        if (!Shell_NotifyIconW(NIM_DELETE, &nid))
        {
            return MNI_FAILED_TO_DELETE_ICON;
        }

        return MNI_UNSUPPORTED_VERSION;
    }

    mIconCreated = true;
    mIconVisible = true;

    return MNI_SUCCESS;
}

auto NotifyIcon::InternalDestroyWindow () -> HRESULT
{
    TRACE(L"InternalDestroyWindow(), mWindowHandle=%lld", mWindowHandle);
    if (!mWindowHandle)
    {
        return MNI_INVALID_WINDOW_HANDLE;
    }

    DestroyWindow(mWindowHandle);
    mWindowHandle = NULL;

    UnregisterClassW(mClassName.c_str(), mInstance);

    return MNI_SUCCESS;
}

auto NotifyIcon::InternalDestroyNotifyIcon () -> HRESULT
{
    TRACE(L"InternalDestroyNotifyIcon(), mIconCreated=%d, mWindowHandle=%lld", mIconCreated, mWindowHandle);
    if (!mIconCreated)
    {
        return MNI_ICON_NOT_CREATED;
    }

    auto nid = NOTIFYICONDATAW{
        .cbSize = sizeof(NOTIFYICONDATAW),
        .hWnd   = mWindowHandle,
        .uID    = 0
    };

    if (mUseGuid)
    {
        nid.uFlags |= NIF_GUID;
        nid.guidItem = mGuid;
    }

    if (!Shell_NotifyIconW(NIM_DELETE, &nid))
    {
        return MNI_FAILED_TO_DELETE_ICON;
    }

    mIconCreated = false;
    mIconVisible = false;

    return MNI_SUCCESS;
}

auto NotifyIcon::InternalStartTimer (UINT id, UINT interval) -> BOOL
{
    TRACE(L"InternalStartTimer(id=%d, interval=%d)", id, interval);
    if (id >= MNI_USER_TIMER_ID)
    {
        return FALSE;
    }

    return mWindowHandle ? SetTimer(mWindowHandle, id, interval, NULL) != 0: FALSE;
}

auto NotifyIcon::InternalStopTimer (UINT id) -> BOOL
{
    TRACE(L"InternalStopTimer(id=%d)", id);
    if (id >= MNI_USER_TIMER_ID)
    {
        return FALSE;
    }

    return mWindowHandle ? KillTimer(mWindowHandle, id) : FALSE;
}

auto NotifyIcon::InternalTimerTimeout (UINT id) -> BOOL
{
    TRACE(L"InternalTimerTimeout(id=%d)", id);
    if (id >= MNI_USER_TIMER_ID)
    {
        return FALSE;
    }

    if (id == TM_DOUBLE_KEYSELECT_PREVENTION)
    {
        InternalStopTimer(TM_DOUBLE_KEYSELECT_PREVENTION);
        mDoubleKeySelectPrevention = false;
    }

    return mWindowHandle ? KillTimer(mWindowHandle, id) : FALSE;
}

auto NotifyIcon::NotifyIconProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto notifyIconPtr = static_cast<NotifyIcon*>(nullptr);

    if (uMsg == WM_NCCREATE)
    {
        auto lpCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);

        notifyIconPtr = reinterpret_cast<NotifyIcon*>(lpCreateStruct->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(notifyIconPtr));
        
        notifyIconPtr->mWindowHandle = hWnd;
    }
    else
    {
        notifyIconPtr = reinterpret_cast<NotifyIcon*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    }

    if (notifyIconPtr)
    {
        return notifyIconPtr->Dispatch(hWnd, uMsg, wParam, lParam);
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

NotifyIcon::~NotifyIcon ()
{
    InternalDestroyNotifyIcon();
    InternalDestroyWindow();

    if (mIcon && mResourceDestructionPolicy == ResourceDestructionPolicy::Auto)
    {
        DestroyIcon(mIcon);
    }

    if (mMenu && mResourceDestructionPolicy == ResourceDestructionPolicy::Auto)
    {
        DestroyMenu(mMenu);
    }
}

auto NotifyIcon::Init (const Desc& desc) -> HRESULT
{
    mInstance                  = desc.instance;
    mIcon                      = desc.icon;
    mMenu                      = desc.menu;
    mUseStandardTip            = desc.useStandardTip;
    mTip                       = desc.tip;
    mWindowTitle               = desc.windowTitle;
    mClassName                 = desc.className;
    mUseGuid                   = desc.useGuid;
    mGuid                      = desc.guid;
    mIconCreated               = false;
    mIconVisible               = false;
    mTaskbarCreatedMsgId       = 0;
    mIsDpiEvent                = false;
    mInternalDpi               = 96;
    mInternalThemeInfo         = ThemeInfo::Detect();
    mResourceDestructionPolicy = desc.resourceDestructionPolicy;
    mDoubleKeySelectPrevention = false;

#if defined(FEATURE_IMMERSIVE_CONTEXT_MENU_ENABLED)
    mUseImmersiveContextMenu   = desc.useImmersiveContextMenu;
    mImmersiveContextMenuStyle = desc.contextMenuStyle;
#endif

    return InternalCreateWindow();
}

auto NotifyIcon::MainLoop () -> int
{    
    auto msg = MSG{};
    while (auto ret = GetMessageW(&msg, NULL, 0, 0))
    {
        if (ret == -1)
        {
            return -1;
        }
        else
        {
            TranslateMessage(&msg); 
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

auto NotifyIcon::Quit () -> void
{ 
    PostQuitMessage(0);
}

auto NotifyIcon::Show () -> HRESULT
{
    TRACE(L"Show(), mIconVisible=%d, mIconCreated=%d, mWindowHandle=%lld", mIconVisible, mIconCreated, mWindowHandle);
    if (mIconVisible)
    {
        return MNI_ICON_ALREADY_SHOWN;
    }

    if (!mWindowHandle)
    {
        if (const auto result = InternalCreateWindow(); FAILED(result))
        {
            return result;
        }
    }

    if (!mIconCreated)
    {
        if (const auto result = InternalCreateNotifyIcon(); FAILED(result))
        {
            return result;
        }
    }
    else
    {
        auto nid = NOTIFYICONDATAW{
            .cbSize      = sizeof(NOTIFYICONDATAW),
            .hWnd        = mWindowHandle,
            .uID         = 0,
            .uFlags      = NIF_STATE,
            .dwState     = 0,
            .dwStateMask = NIS_HIDDEN
        };

        if (mUseGuid)
        {
            nid.uFlags |= NIF_GUID;
            nid.guidItem = mGuid;
        }

        if (mUseStandardTip)
        {
            nid.uFlags |= NIF_SHOWTIP;
        }

        if (!Shell_NotifyIconW(NIM_MODIFY, &nid))
        {
            return MNI_FAILED_TO_SHOW_ICON;
        }
    }
    
    mIconVisible = true;

    // To supress warning.
    if (mWindowHandle)
    {
        SendMessageW(mWindowHandle, WM_NOTIFYICON_SHOW, 0, 0);
    }

    return MNI_SUCCESS;
}

auto NotifyIcon::Hide () -> HRESULT
{
    TRACE(L"Hide(), mIconVisible=%d, mIconCreated=%d, mWindowHandle=%lld", mIconVisible, mIconCreated, mWindowHandle);
    if (!mIconVisible)
    {
        return MNI_ICON_ALREADY_HIDDEN;
    }

    if (!mIconCreated)
    {
        return MNI_ICON_NOT_CREATED;
    }

    if (!mWindowHandle)
    {
        return MNI_INVALID_WINDOW_HANDLE;
    }

    auto nid = NOTIFYICONDATAW{
        .cbSize      = sizeof(NOTIFYICONDATAW),
        .hWnd        = mWindowHandle,
        .uID         = 0,
        .uFlags      = NIF_STATE,
        .dwState     = NIS_HIDDEN,
        .dwStateMask = NIS_HIDDEN
    };

    if (mUseGuid)
    {
        nid.uFlags |= NIF_GUID;
        nid.guidItem = mGuid;
    }

    if (!Shell_NotifyIconW(NIM_MODIFY, &nid))
    {
        return MNI_FAILED_TO_HIDE_ICON;
    }

    mIconVisible = false;

    SendMessageW(mWindowHandle, WM_NOTIFYICON_HIDE, 0, 0);

    return MNI_SUCCESS;
}

auto NotifyIcon::ShowContextMenu (int x, int y) -> HRESULT
{
    if (!mWindowHandle)
    {
        return MNI_INVALID_WINDOW_HANDLE;
    }

    if (!mIconCreated)
    {
        return MNI_ICON_NOT_CREATED;
    }
    
    ContextMenu(x, y);

    return MNI_SUCCESS;
}

auto NotifyIcon::SetIcon (HICON icon, ResourceDestructionPolicy rdp) -> HRESULT
{
    if (const auto result = UpdateIcon(icon); FAILED(result))
    {
        return result;
    }

    if (mIcon && rdp == ResourceDestructionPolicy::Auto)
    {
        DestroyIcon(mIcon);
    }

    mIcon = icon;

    return mIcon != NULL ? MNI_SUCCESS : MNI_ARGUMENT_ICON_IS_NULL;
}

auto NotifyIcon::SetMenu (HMENU menu, ResourceDestructionPolicy rdp) -> HRESULT
{
    if (const auto result = UpdateMenu(menu); FAILED(result))
    {
        return result;
    }

    if (mMenu && rdp == ResourceDestructionPolicy::Auto)
    {
        DestroyMenu(mMenu);
    }

    mMenu = menu;

    return mMenu != NULL ? MNI_SUCCESS : MNI_ARGUMENT_MENU_IS_NULL;
}

auto NotifyIcon::SetTip (std::wstring_view tip, ResourceDestructionPolicy rdp) -> HRESULT
{
    UNREFERENCED_PARAMETER(rdp);

    if (const auto result = UpdateTip(tip); FAILED(result))
    {
        return result;
    }

    mTip = tip;

    return mTip.length() != 0 ? MNI_SUCCESS : MNI_ARGUMENT_TIP_IS_NULL;
}

auto NotifyIcon::SetIcon (HICON icon) -> HRESULT
{
    return SetIcon(icon, mResourceDestructionPolicy);
}

auto NotifyIcon::SetMenu (HMENU menu) -> HRESULT
{
    return SetMenu(menu, mResourceDestructionPolicy);
}

auto NotifyIcon::SetTip (std::wstring_view tip) -> HRESULT
{
    return SetTip(tip, mResourceDestructionPolicy);
}

auto NotifyIcon::GetIcon () -> HICON
{
    return mIcon;
}

auto NotifyIcon::GetMenu () -> HMENU
{
    return mMenu;
}

auto NotifyIcon::GetTip () -> std::wstring
{
    return mTip;
}

auto NotifyIcon::UseStandardTip () -> HRESULT
{
    // Only update if there is change.
    if (!mUseStandardTip)
    {
        mUseStandardTip = true;
        return UpdateTip(L"");
    }

    return MNI_SUCCESS;
}

auto NotifyIcon::UseRichPopupTip () -> HRESULT
{
    // Only update if there is change.
    if (mUseStandardTip)
    {
        mUseStandardTip = false;
        return UpdateTip(L"");
    }

    return MNI_SUCCESS;
}

auto NotifyIcon::IsStandardTip () -> BOOL
{
    return mUseStandardTip;
}

auto NotifyIcon::IsRichPopupTip () -> BOOL
{
    return !mUseStandardTip;
}

auto NotifyIcon::SendBalloonNotification (
    std::wstring_view title,
    std::wstring_view text,
    BalloonIconType iconType,
    HICON hIcon,
    BalloonFlags flags
) -> HRESULT
{
    TRACE(L"SendBalloonNotification(title=%s, text=%s, icon=%lld, flags=%d), mIconVisible=%d, mIconCreated=%d, mWindowHandle=%lld",
        title.data(), text.data(), static_cast<UINT>(iconType), static_cast<int>(flags), mIconVisible, mIconCreated, mWindowHandle);
    if (!mIconCreated)
    {
        return MNI_ICON_NOT_CREATED;
    }

    auto nid = NOTIFYICONDATAW{
        .cbSize       = sizeof(NOTIFYICONDATAW),
        .hWnd         = mWindowHandle,
        .uID          = 0,
        .uFlags       = NIF_INFO,
        .dwInfoFlags  = NIIF_NONE,
        .hBalloonIcon = NULL
    };

    if (mUseGuid)
    {
        nid.uFlags |= NIF_GUID;
        nid.guidItem = mGuid;
    }

    if (mUseStandardTip)
    {
        nid.uFlags |= NIF_SHOWTIP;
    }

    if ((flags & BalloonFlags::Realtime) == BalloonFlags::Realtime)
    {
        nid.uFlags |= NIF_REALTIME;
    }

    switch (iconType)
    {
    case BalloonIconType::NoIcon:
        break;
    case BalloonIconType::SystemInfo:
        nid.dwInfoFlags = NIIF_INFO | NIIF_LARGE_ICON;
        break;
    case BalloonIconType::SystemWarning:
        nid.dwInfoFlags = NIIF_WARNING | NIIF_LARGE_ICON;
        break;
    case BalloonIconType::SystemError:
        nid.dwInfoFlags = NIIF_ERROR | NIIF_LARGE_ICON;
        break;
    case BalloonIconType::Custom:
        nid.dwInfoFlags  = NIIF_USER | NIIF_LARGE_ICON;
        nid.hBalloonIcon = hIcon;
        break;
    }

    if ((flags & BalloonFlags::PlaySound) != BalloonFlags::PlaySound)
    {
        nid.dwInfoFlags |= NIIF_NOSOUND;
    }

    if ((flags & BalloonFlags::RespectQuietTime) == BalloonFlags::RespectQuietTime)
    {
        nid.dwInfoFlags |= NIIF_RESPECT_QUIET_TIME;
    }
        
    if (wcscpy_s(nid.szInfoTitle, title.data()) != 0)
    {
        return MNI_FAILED_TO_COPY_BALLOON_TITLE;
    }

    if (wcscpy_s(nid.szInfo, text.data()) != 0)
    {
        return MNI_FAILED_TO_COPY_BALLOON_TEXT;
    }

    if (!Shell_NotifyIconW(NIM_MODIFY, &nid))
    {
        return MNI_FAILED_TO_SHOW_BALLOON;
    }

    return MNI_SUCCESS;
}

auto NotifyIcon::RemoveBalloonNotification () -> HRESULT
{
    TRACE(L"RemoveBalloonNotification(), mIconCreated=%d, mWindowHandle=%lld", mIconCreated, mWindowHandle);
    if (!mIconCreated)
    {
        return MNI_ICON_NOT_CREATED;
    }

    auto nid = NOTIFYICONDATAW{
        .cbSize       = sizeof(NOTIFYICONDATAW),
        .hWnd         = mWindowHandle,
        .uID          = 0,
        .uFlags       = NIF_INFO,
        .szInfo       = L"",
        .szInfoTitle  = L"",
        .dwInfoFlags  = NIIF_NONE,
        .hBalloonIcon = NULL,
    };

    if (!Shell_NotifyIconW(NIM_MODIFY, &nid))
    {
        return MNI_FAILED_TO_REMOVE_BALLOON;
    }
    
    return MNI_SUCCESS;
}

auto NotifyIcon::StartTimer (UINT id, UINT interval) -> BOOL
{
    if (id < MNI_USER_TIMER_ID)
    {
        return FALSE;
    }

    return mWindowHandle ? SetTimer(mWindowHandle, id, interval, NULL) != 0 : FALSE;
}

auto NotifyIcon::StopTimer (UINT id) -> BOOL
{
    if (id < MNI_USER_TIMER_ID)
    {
        return FALSE;
    }

    return mWindowHandle ? KillTimer(mWindowHandle, id) : FALSE;
}

auto NotifyIcon::SendCustomMessage (UINT msg, WPARAM wParam, LPARAM lParam) -> BOOL
{
    if (WM_APP <= msg && msg <= 0xBFFF)
    {
        // This returns immediately if calling thread is different than window creation thread. 
        return mWindowHandle ? SendNotifyMessageW(mWindowHandle, msg, wParam, lParam) : FALSE;
    }
    
    return FALSE;
}

auto NotifyIcon::SetResourceDestructionPolicy (ResourceDestructionPolicy rdp) -> void
{
    mResourceDestructionPolicy = rdp;
}

} // namespace mni
