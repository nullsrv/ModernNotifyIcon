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

#pragma once

#include <functional>
#include <string>
#include <string_view>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#if defined(ENABLE_FEATURE_IMMERSIVE_CONTEXT_MENU)
    #define FEATURE_IMMERSIVE_CONTEXT_MENU_ENABLED
    #include "../../deps/ImmersiveContextMenu/include/ImmersiveContextMenu.hpp"
    
    namespace icm = ImmersiveContextMenu;
#endif

#pragma region "Custom Codes"

namespace {
    constexpr auto MNI_ERROR_CODE = 1;
}

constexpr auto MNI_SUCCESS                      = S_OK;

// Errors
constexpr auto MNI_UNSUPPORTED_VERSION          = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 0);
constexpr auto MNI_FAILED_TO_ADD_ICON           = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 1);
constexpr auto MNI_FAILED_TO_DELETE_ICON        = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 2);
constexpr auto MNI_FAILED_TO_SHOW_ICON          = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 3);
constexpr auto MNI_FAILED_TO_HIDE_ICON          = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 4);
constexpr auto MNI_FAILED_TO_CHANGE_ICON        = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 5);
constexpr auto MNI_FAILED_TO_CHANGE_TIP         = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 6);
constexpr auto MNI_FAILED_TO_COPY_TIP           = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 7);
constexpr auto MNI_FAILED_TO_COPY_BALLOON_TITLE = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 8);
constexpr auto MNI_FAILED_TO_COPY_BALLOON_TEXT  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 9);
constexpr auto MNI_FAILED_TO_SHOW_BALLOON       = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 10);
constexpr auto MNI_FAILED_TO_REMOVE_BALLOON     = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 11);
constexpr auto MNI_FAILED_TO_REGISTER_WNDCLASS  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 12);
constexpr auto MNI_FAILED_TO_CREATE_WINDOW      = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 13);
constexpr auto MNI_INVALID_INSTANCE             = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 14);
constexpr auto MNI_INVALID_WINDOW_HANDLE        = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 15);
constexpr auto MNI_INVALID_ICON                 = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 16);
constexpr auto MNI_INVALID_MENU                 = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 17);
constexpr auto MNI_ICON_NOT_CREATED             = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, MNI_ERROR_CODE + 18);

// Warnings
constexpr auto MNI_WINDOW_ALREADY_CREATED       = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, MNI_ERROR_CODE + 0);
constexpr auto MNI_ICON_ALREADY_CREATED         = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, MNI_ERROR_CODE + 1);
constexpr auto MNI_ICON_ALREADY_SHOWN           = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, MNI_ERROR_CODE + 2);
constexpr auto MNI_ICON_ALREADY_HIDDEN          = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, MNI_ERROR_CODE + 3);
constexpr auto MNI_ARGUMENT_ICON_IS_NULL        = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, MNI_ERROR_CODE + 4);
constexpr auto MNI_ARGUMENT_MENU_IS_NULL        = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, MNI_ERROR_CODE + 5);
constexpr auto MNI_ARGUMENT_TIP_IS_NULL         = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, MNI_ERROR_CODE + 6);

// User defined message ids.
constexpr auto MNI_USER_MESSAGE_ID = UINT{WM_APP};

// User defined timer ids.
constexpr auto MNI_USER_TIMER_ID   = UINT{1000};

#pragma endregion

namespace mni {

#pragma region "ThemeInfo"

class ThemeInfo final
{
public:
    enum class Theme : unsigned char
    {
        Dark,
        Light,
        HighContrast
    };

private:
    Theme mTheme      = Theme::Dark;
    UINT  mForeground = 0x00FFFFFF;  // BGR
    UINT  mBackground = 0x00000000;

public:
    ThemeInfo () = delete;
    constexpr ThemeInfo (const Theme theme, UINT foreground, UINT background)
        : mTheme      (theme)
        , mForeground (foreground)
        , mBackground (background)
    {
    }

    auto IsDark         () { return mTheme == Theme::Dark; }
    auto IsLight        () { return mTheme == Theme::Light; }
    auto IsHighContrast () { return mTheme == Theme::HighContrast; }

    auto GetTheme           () { return mTheme; }
    auto GetForegroundColor () { return mForeground; }
    auto GetBackgroundColor () { return mBackground; }

    auto operator== (ThemeInfo& other) -> bool
    {
        return mTheme == other.mTheme && mForeground == other.mForeground && mBackground == other.mBackground;
    }
    auto operator!= (ThemeInfo& other) -> bool
    {
        return mTheme != other.mTheme || mForeground != other.mForeground || mBackground != other.mBackground;
    }

    static auto Detect () -> ThemeInfo;
};

#pragma endregion

#pragma region "Balloon Notification"

enum class BalloonFlags : unsigned char
{
    Default          = 0x0,
    Realtime         = 0x1,
    PlaySound        = 0x2,
    RespectQuietTime = 0x4
};
DEFINE_ENUM_FLAG_OPERATORS(BalloonFlags);

enum class BalloonIconType : unsigned char
{
    NoIcon        = 0,
    SystemInfo    = 1,
    SystemWarning = 2,
    SystemError   = 3,
    Custom        = 4
};

#pragma endregion

#pragma region "NotifyIcon"

class NotifyIcon
{
    static constexpr auto WM_NOTIFYICON         = UINT{WM_USER + 0};
    static constexpr auto WM_NOTIFYICON_SHOW    = UINT{WM_USER + 1};
    static constexpr auto WM_NOTIFYICON_HIDE    = UINT{WM_USER + 2};
    static constexpr auto WM_DPICHANGED_DELAYED = UINT{WM_USER + 3};

    //static constexpr auto TM_LMB_DOUBLE_CLICK_CHECK      = UINT{1};
    static constexpr auto TM_DOUBLE_KEYSELECT_PREVENTION = UINT{2};

public:
    enum class ResourceDestructionPolicy : bool
    {
        Auto,
        Manual
    };
    using RDP = ResourceDestructionPolicy;

    struct Desc
    {
        HICON             icon           = nullptr;
        HMENU             menu           = nullptr;

        bool              useStandardTip = true;
        std::wstring      tip            = L"NotifyIcon_Tip";

        bool              useGuid        = false;
        GUID              guid           = GUID{};

        HINSTANCE         instance       = nullptr;
        std::wstring_view windowTitle    = L"NotifyIcon_InvisibleWindow";
        std::wstring_view className      = L"NotifyIcon_WndClass";

        ResourceDestructionPolicy resourceDestructionPolicy = ResourceDestructionPolicy::Auto;

    #if defined(FEATURE_IMMERSIVE_CONTEXT_MENU_ENABLED)
        bool              useImmersiveContextMenu = true;
        icm::Style        contextMenuStyle        = icm::Style::Auto;
    #endif
    };

private:
    HWND         mWindowHandle              = nullptr;
    HINSTANCE    mInstance                  = nullptr;
    HICON        mIcon                      = nullptr;
    HMENU        mMenu                      = nullptr;
    std::wstring mTip                       = L"NotifyIcon_Tip";
    bool         mUseStandardTip            = true;
    std::wstring mWindowTitle               = L"NotifyIcon_InvisibleWindow";
    std::wstring mClassName                 = L"NotifyIcon_WndClass";
    bool         mUseGuid                   = false;
    GUID         mGuid                      = GUID{};
    bool         mIconCreated               = false;
    bool         mIconVisible               = false;
    UINT         mTaskbarCreatedMsgId       = 0;
    bool         mIsDpiEvent                = false;
    int          mInternalDpi               = 96;
    ThemeInfo    mInternalThemeInfo         = ThemeInfo::Detect();
    RDP          mResourceDestructionPolicy = ResourceDestructionPolicy::Auto;
    bool         mDoubleKeySelectPrevention = false;

#if defined(FEATURE_IMMERSIVE_CONTEXT_MENU_ENABLED)
    bool         mUseImmersiveContextMenu   = true;
    icm::Style   mImmersiveContextMenuStyle = icm::Style::Auto;
#endif

public:
    std::function<void ()>                     OnCreate                  = nullptr;
    std::function<void ()>                     OnDestroy                 = nullptr;
    std::function<void ()>                     OnShow                    = nullptr;
    std::function<void ()>                     OnHide                    = nullptr;
    std::function<void (int, int)>             OnKeySelect               = nullptr; // when you select icon with keyboard and press space or enter
    std::function<void (int, int)>             OnLmbClick                = nullptr;
    std::function<void (int, int)>             OnLmbDoubleClick          = nullptr;
    std::function<void (int, int)>             OnMmbClick                = nullptr;
    std::function<void ()>                     OnContextMenuOpen         = nullptr;
    std::function<void (int)>                  OnContextMenuSelect       = nullptr;
    std::function<void ()>                     OnContextMenuClose        = nullptr;
    std::function<void ()>                     OnBalloonShow             = nullptr;
    std::function<void ()>                     OnBalloonHide             = nullptr;
    std::function<void ()>                     OnBalloonTimeout          = nullptr;
    std::function<void ()>                     OnBalloonUserClick        = nullptr;
    std::function<void (int, int)>             OnRichPopupOpen           = nullptr;
    std::function<void ()>                     OnRichPopupClose          = nullptr;
    std::function<void (int)>                  OnDpiChange               = nullptr; // this event requires minimum os of Win8.1
    std::function<void (ThemeInfo)>            OnThemeChange             = nullptr;
    std::function<void ()>                     OnTaskbarCreated          = nullptr;
    std::function<void (UINT)>                 OnTimer                   = nullptr;
    std::function<void (UINT, WPARAM, LPARAM)> OnCustomMessage           = nullptr;
    std::function<bool (UINT, WPARAM, LPARAM)> OnSystemMessage           = nullptr; // use with caution!

private:
    auto Dispatch (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    // Return TRUE was handled.
    auto Create            ()                     -> BOOL;
    auto Destroy           ()                     -> BOOL;
    auto IconShow          ()                     -> BOOL;
    auto IconHide          ()                     -> BOOL;
    auto KeySelect         (int x, int y)         -> BOOL;
    auto LmbClick          (int x, int y)         -> BOOL;
    auto LmbDoubleClick    (int x, int y)         -> BOOL;
    auto MmbClick          (int x, int y)         -> BOOL;
    auto ContextMenu       (int x, int y)         -> BOOL;
    auto BalloonShow       ()                     -> BOOL;
    auto BalloonHide       ()                     -> BOOL;
    auto BalloonTimeout    ()                     -> BOOL;
    auto BalloonUserClick  ()                     -> BOOL;
    auto RichPopupOpen     (int x, int y)         -> BOOL;
    auto RichPopupClose    ()                     -> BOOL;
    auto DpiChange         (int dpi)              -> BOOL;
    auto ThemeChange       ()                     -> BOOL;
    auto TaskbarCreated    ()                     -> BOOL;
    auto TimerTimeout      (UINT id)              -> BOOL;
    auto CustomMessage     (UINT, WPARAM, LPARAM) -> BOOL;
    auto SystemMessage     (UINT, WPARAM, LPARAM) -> BOOL;

    auto UpdateIcon  (HICON icon)            -> HRESULT;
    auto UpdateMenu  (HMENU menu)            -> HRESULT;
    auto UpdateTip   (std::wstring_view tip) -> HRESULT;

    auto InternalCreateWindow        () -> HRESULT;
    auto InternalCreateNotifyIcon    () -> HRESULT;
    auto InternalDestroyWindow       () -> HRESULT;
    auto InternalDestroyNotifyIcon   () -> HRESULT;

    auto InternalStartTimer   (UINT id, UINT interval) -> BOOL;
    auto InternalStopTimer    (UINT id)                -> BOOL;
    auto InternalTimerTimeout (UINT id)                -> BOOL;

    static auto CALLBACK NotifyIconProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    NotifyIcon     (const NotifyIcon&) = delete;
    auto operator= (const NotifyIcon&) = delete;

    NotifyIcon     (NotifyIcon&&) = delete;
    auto operator= (NotifyIcon&&) = delete;

public:
    NotifyIcon  () = default;
    NotifyIcon  (const Desc desc);
    ~NotifyIcon ();

    auto Init     () -> HRESULT;
    auto MainLoop () -> int;
    auto Quit     () -> void;
    auto Handle   () -> HWND { return mWindowHandle; } // Use with caution!

    auto Show () -> HRESULT;
    auto Hide () -> HRESULT;

    auto ShowContextMenu (int x, int y) -> HRESULT;

    auto SetIcon (HICON icon, ResourceDestructionPolicy rdp)            -> HRESULT;
    auto SetMenu (HMENU menu, ResourceDestructionPolicy rdp)            -> HRESULT;
    auto SetTip  (std::wstring_view tip, ResourceDestructionPolicy rdp) -> HRESULT;

    auto SetIcon (HICON icon)            -> HRESULT;
    auto SetMenu (HMENU menu)            -> HRESULT;
    auto SetTip  (std::wstring_view tip) -> HRESULT;

    auto GetIcon () -> HICON;
    auto GetMenu () -> HMENU;
    auto GetTip  () -> std::wstring;

    auto UseStandardTip  () -> HRESULT;
    auto UseRichPopupTip () -> HRESULT;

    auto IsStandardTip  () -> BOOL;
    auto IsRichPopupTip () -> BOOL;

    auto SendBalloonNotification (
        std::wstring_view title, std::wstring_view text, BalloonIconType iconType, HICON hIcon, BalloonFlags flags
    ) -> HRESULT;
    auto RemoveBalloonNotification () -> HRESULT;

    auto StartTimer (UINT id, UINT interval) -> BOOL;
    auto StopTimer  (UINT id)                -> BOOL;

    auto SendCustomMessage (UINT msg, WPARAM wParam, LPARAM lParam) -> BOOL;

    auto SetResourceDestructionPolicy (ResourceDestructionPolicy rdp) -> void;

#if defined(FEATURE_IMMERSIVE_CONTEXT_MENU_ENABLED)
    auto UseClassicMenu   () { mUseImmersiveContextMenu = false; }
    auto UseImmersiveMenu () { mUseImmersiveContextMenu = true; }

    auto IsClassicMenu   () { return !mUseImmersiveContextMenu; }
    auto IsImmersiveMenu () { return mUseImmersiveContextMenu; }

    auto SetImmersiveStyle (icm::Style style) { mImmersiveContextMenuStyle = style; }
#endif
};

#pragma endregion

} // namespace mni
