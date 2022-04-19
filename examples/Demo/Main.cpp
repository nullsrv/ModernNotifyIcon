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

#include <string>
#include <thread>
#include <vector>

#include <mni/NotifyIcon.hpp>

#define MENU_HELLO        1000
#define MENU_EMPTY        1001
#define MENU_DISABLED     1002
#define MENU_COUNTER      1003
#define MENU_IMG_ITEM     1004
#define MENU_HIDE_SHOW    1005
#define MENU_CLICKME      1006
#define MENU_CHECKME      1007
#define MENU_RADIOCHECK   1008
#define MENU_CUSTOMCHECK  1009
#define MENU_IMG_CHECK    1010
#define MENU_SHOW_BALLOON 1011
#define MENU_EXIT         1012

#define MENU_THEME_AUTO  1100
#define MENU_THEME_LIGHT 1101
#define MENU_THEME_DARK  1102

#define MENU_BALLOON_ICON_NONE     1200
#define MENU_BALLOON_ICON_INFO     1201
#define MENU_BALLOON_ICON_WARN     1202
#define MENU_BALLOON_ICON_ERROR    1203
#define MENU_BALLOON_ICON_USER     1204
#define MENU_BALLOON_PLAY_SOUND    1205
#define MENU_BALLOON_RESPECT_QUIET 1206


auto SetItemBitmap(HMENU hMenu, UINT id, HBITMAP hBmp)
{
    auto referenceMii = MENUITEMINFOW{};
    memset(&referenceMii, 0, sizeof(referenceMii));
    referenceMii.cbSize = sizeof(referenceMii);
    referenceMii.fMask = MIIM_BITMAP;

    const auto bGetRet = GetMenuItemInfoW(hMenu, id, FALSE, &referenceMii);
    if (!bGetRet)
    {
        MessageBoxW(0, L"Failed to get menu item info!", L"Error", MB_OK);
    }
    else
    {
        auto updatedMii = MENUITEMINFOW{};
        memset(&updatedMii, 0, sizeof(updatedMii));
        updatedMii.cbSize = sizeof(updatedMii);
        updatedMii.fMask = MIIM_BITMAP;
        updatedMii.hbmpItem = hBmp;

        const auto bSetRet = SetMenuItemInfoW(hMenu, id, FALSE, &updatedMii);
        if (!bSetRet)
        {
            MessageBoxW(0, L"Failed to set menu item image!", L"Error", MB_OK);
        }
    }
}

auto SetItemText(HMENU hMenu, UINT id, std::wstring text)
{
    auto mii = MENUITEMINFOW{};
    memset(&mii, 0, sizeof(mii));
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_STRING;
    mii.dwTypeData = text.data();
    mii.cch = text.length();

    const auto bSetRet = SetMenuItemInfoW(hMenu, id, FALSE, &mii);
    if (!bSetRet)
    {
        MessageBoxW(0, L"Failed to set menu item text!", L"Error", MB_OK);
    }
}

auto SetItemCheckState(HMENU hMenu, UINT id, bool state)
{
    auto referenceMii = MENUITEMINFOW{};
    memset(&referenceMii, 0, sizeof(referenceMii));
    referenceMii.cbSize = sizeof(referenceMii);
    referenceMii.fMask = MIIM_STATE;

    const auto bGetRet = GetMenuItemInfoW(hMenu, id, FALSE, &referenceMii);
    if (!bGetRet)
    {
        MessageBoxW(0, L"Failed to get menu item info!", L"Error", MB_OK);
    }
    else
    {
        auto updatedMii = MENUITEMINFOW{};
        memset(&updatedMii, 0, sizeof(updatedMii));
        updatedMii.cbSize = sizeof(updatedMii);
        updatedMii.fMask = MIIM_STATE;

        if (state)
        {
            updatedMii.fState = referenceMii.fState | MFS_CHECKED;
        }
        else
        {
            updatedMii.fState = referenceMii.fState & ~MFS_CHECKED;
        }

        const auto bSetRet = SetMenuItemInfoW(hMenu, id, FALSE, &updatedMii);
        if (!bSetRet)
        {
            MessageBoxW(0, L"Failed to set menu item state!", L"Error", MB_OK);
        }
    }
}

auto IsItemChecked(HMENU hMenu, UINT id) -> bool
{
    auto mii = MENUITEMINFOW{};
    memset(&mii, 0, sizeof(mii));
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_STATE;

    const auto bGetRet = GetMenuItemInfoW(hMenu, id, FALSE, &mii);
    if (bGetRet)
    {
        return (mii.fState & MFS_CHECKED) != 0;
    }

    return false;
}

auto UpdateRadioCheckmarks(HMENU hMenu, UINT checked, std::vector<std::pair<UINT, UINT>> items) -> UINT
{
    auto type = 0;

    for (const auto i : items)
    {
        if (checked == i.first)
        {
            type = i.second;
            break;
        }
    }

    for (const auto i : items)
    {
        if (checked == i.first)
        {
            SetItemCheckState(hMenu, i.first, true);
        }
        else
        {
            SetItemCheckState(hMenu, i.first, false);
        }
    }

    return type;
}

auto GetThemeIcon(mni::ThemeInfo theme) -> HICON
{
    auto dark = true;
    if (theme.IsLight())
    {
         dark = false;
    }
    else
    {
        if (theme.IsHighContrast() && theme.GetBackgroundColor() >= 0x00808080)
        {
            dark = false;
        }
    }

    return static_cast<HICON>(
        LoadImageW(0, dark ? L"demo_light.ico" : L"demo_dark.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE)
    );
}

auto WINAPI wWinMain (
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPWSTR    lpCmdLine,
    _In_     int       nShowCmd
) -> int
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    constexpr auto TM_HIDE_FOR_1_SEC_ID = MNI_USER_TIMER_ID;

    static constexpr auto BALLOON_ICON_NONE  = 0;
    static constexpr auto BALLOON_ICON_INFO  = 1;
    static constexpr auto BALLOON_ICON_WARN  = 2;
    static constexpr auto BALLOON_ICON_ERROR = 3;
    static constexpr auto BALLOON_ICON_USER  = 4;

    auto icmStyle         = icm::Style::Auto;
    auto counter          = 0;
    auto balloonIconType  = BALLOON_ICON_NONE;
    auto playSound        = true;
    auto respectQuietTime = true;
    auto theme            = mni::ThemeInfo::Detect();

    const auto themeItems = std::vector<std::pair<UINT, UINT>>{
        std::make_pair(MENU_THEME_AUTO, static_cast<UINT>(icm::Style::Auto)),
        std::make_pair(MENU_THEME_AUTO, static_cast<UINT>(icm::Style::ImmersiveLight)),
        std::make_pair(MENU_THEME_AUTO, static_cast<UINT>(icm::Style::ImmersiveDark))
    };

    const auto balloonItems = std::vector<std::pair<UINT, UINT>>{
        std::make_pair(MENU_BALLOON_ICON_NONE, BALLOON_ICON_NONE),
        std::make_pair(MENU_BALLOON_ICON_INFO, BALLOON_ICON_INFO),
        std::make_pair(MENU_BALLOON_ICON_WARN, BALLOON_ICON_WARN),
        std::make_pair(MENU_BALLOON_ICON_ERROR, BALLOON_ICON_ERROR),
        std::make_pair(MENU_BALLOON_ICON_USER, BALLOON_ICON_USER)
    };

    // If you want transparency you need 32-bit BMP.
    const auto loadFlags = LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE;
    auto bmpCustom    = (HBITMAP)LoadImageW(0, L"item.bmp", IMAGE_BITMAP, 0, 0, loadFlags);
    auto bmpChecked   = (HBITMAP)LoadImageW(0, L"checked.bmp", IMAGE_BITMAP, 0, 0, loadFlags);
    auto bmpUnchecked = (HBITMAP)LoadImageW(0, L"unchecked.bmp", IMAGE_BITMAP, 0, 0, loadFlags);

    auto icoBalloon   = (HICON)LoadImageW(0, L"balloon.ico", IMAGE_ICON, 64, 64, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    auto icoDemoLight = (HICON)LoadImageW(0, L"demo_light.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    auto icoDemoDark  = (HICON)LoadImageW(0, L"demo_dark.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE | LR_DEFAULTSIZE);

    // Create menus.
    auto hMenuTheme   = CreateMenu();
    auto hMenuBalloon = CreateMenu();
    auto hMenu        = CreateMenu();
    auto hPopup       = CreateMenu();

    AppendMenuW(hMenuTheme, MF_CHECKED    | MF_STRING | MFT_RADIOCHECK, MENU_THEME_AUTO, L"Auto");
    AppendMenuW(hMenuTheme, MF_UNCHECKED | MF_STRING | MFT_RADIOCHECK, MENU_THEME_LIGHT, L"Light");
    AppendMenuW(hMenuTheme, MF_UNCHECKED | MF_STRING | MFT_RADIOCHECK, MENU_THEME_DARK, L"Dark");

    AppendMenuW(hMenuBalloon, MF_CHECKED | MF_STRING | MFT_RADIOCHECK, MENU_BALLOON_ICON_NONE, L"None");
    AppendMenuW(hMenuBalloon, MF_STRING | MFT_RADIOCHECK, MENU_BALLOON_ICON_INFO, L"Info");
    AppendMenuW(hMenuBalloon, MF_STRING | MFT_RADIOCHECK, MENU_BALLOON_ICON_WARN, L"Warning");
    AppendMenuW(hMenuBalloon, MF_STRING | MFT_RADIOCHECK, MENU_BALLOON_ICON_ERROR, L"Error");
    AppendMenuW(hMenuBalloon, MF_STRING | MFT_RADIOCHECK, MENU_BALLOON_ICON_USER, L"User");
    AppendMenuW(hMenuBalloon, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hMenuBalloon, MF_STRING | MF_CHECKED, MENU_BALLOON_PLAY_SOUND, L"Play sound");
    AppendMenuW(hMenuBalloon, MF_STRING | MF_CHECKED, MENU_BALLOON_RESPECT_QUIET, L"Respect quiet time");
    
    AppendMenuW(hMenu, MF_STRING, MENU_HELLO, L"Hello");
    AppendMenuW(hMenu, MF_STRING, MENU_EMPTY, L"");
    AppendMenuW(hMenu, MF_STRING | MF_DISABLED, MENU_DISABLED, L"Disabled");
    AppendMenuW(hMenu, MF_STRING | MF_GRAYED , MENU_COUNTER, L"Click count: 0");
    AppendMenuW(hMenu, MF_STRING , MENU_IMG_ITEM, L"Item with image");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hMenu, MF_STRING, MENU_HIDE_SHOW, L"Hide for 1 second");
    AppendMenuW(hMenu, MF_STRING, MENU_CLICKME, L"Click me");
    AppendMenuW(hMenu, MF_STRING, MENU_CHECKME, L"Check me");
    AppendMenuW(hMenu, MF_STRING | MFT_RADIOCHECK, MENU_RADIOCHECK, L"Radio check");
    AppendMenuW(hMenu, MF_STRING, MENU_CUSTOMCHECK, L"Custom Checkmarks");
    AppendMenuW(hMenu, MF_STRING, MENU_IMG_CHECK, L"Image and Checkmark");
    AppendMenuW(hMenu, MF_POPUP | MF_STRING, reinterpret_cast<UINT_PTR>(hMenuTheme), L"Theme");
    AppendMenuW(hMenu, MF_POPUP | MF_STRING, reinterpret_cast<UINT_PTR>(hMenuBalloon), L"Balloon options");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hMenu, MF_STRING, MENU_SHOW_BALLOON, L"Show notification");
    AppendMenuW(hMenu, MF_STRING, MENU_EXIT, L"Exit");
    
    AppendMenuW(hPopup, MF_POPUP, reinterpret_cast<UINT_PTR>(hMenu), L"");

    // Set custom checkmarks.
    SetMenuItemBitmaps(hMenu, MENU_CUSTOMCHECK, MF_BYCOMMAND, bmpUnchecked, bmpChecked);

    // Set item icon. Using SetMenuItemInfoW() is recommended way.
    SetItemBitmap(hMenu, MENU_IMG_ITEM, bmpCustom);
    SetItemBitmap(hMenu, MENU_IMG_CHECK, bmpCustom);

    // Setup notificon descrition.
    auto guid = GUID{0xc527ebdc, 0xec8b, 0x4c1b, 0xbd, 0x76, 0xfd, 0xbf, 0xaa, 0x79, 0xfc, 0xa6};
    auto desc = mni::NotifyIcon::Desc();
    desc.icon = GetThemeIcon(theme);
    desc.menu = hPopup;
    desc.tip  = L"Demo Tip";
    
    desc.useGuid = false;
    desc.guid    = guid;
    
    desc.useImmersiveContextMenu  = true;
    desc.contextMenuStyle         = icmStyle;
    
    
    auto notifyIcon = mni::NotifyIcon(desc);

    // After explorer.exe restart, we want to show icon again.
    notifyIcon.OnTaskbarCreated = [&](){
        notifyIcon.Show();
    };

    // After dpi chane we want to load higher resolution icon (min. Win8.1).
    notifyIcon.OnDpiChange = [&](int dpi){
        //auto size = MulDiv(16, dpi, 96);
        //auto icon = static_cast<HICON>(LoadImageW(0, L"demo.ico", IMAGE_ICON, size, size, LR_LOADFROMFILE));
        //notifyIcon.SetIcon(icon);
        notifyIcon.SetIcon(GetThemeIcon(theme));
    };

    // After theme change we want to change icons.
    notifyIcon.OnThemeChange = [&](mni::ThemeInfo newTheme){
        notifyIcon.SetIcon(GetThemeIcon(newTheme));
        theme = newTheme;
    };

    // It's no recomended to have events for both single and double click,
    // because single click callback will be called twice.
    notifyIcon.OnLmbClick = [&](int x, int y){
        counter++;
        SetItemText(hMenu, MENU_COUNTER, std::wstring(L"Click count: ") + std::to_wstring(counter));
    };

    //notifyIcon.OnLmbDoubleClick = [&](){
    //    MessageBoxW(0, L"DoubleClick", L"Event", MB_OK);
    //};

    
    notifyIcon.OnTimer = [&](UINT id){
        if (id == TM_HIDE_FOR_1_SEC_ID)
        {
            notifyIcon.Show();
            notifyIcon.StopTimer(TM_HIDE_FOR_1_SEC_ID);
        }
    };

    // Handle context menu events.
    notifyIcon.OnContextMenuSelect = [&](int selectedItem){
        switch (selectedItem)
        {
        case MENU_EMPTY:
            SetItemText(hMenu, MENU_EMPTY, L"Text");
            break;

        case MENU_HIDE_SHOW:
            notifyIcon.Hide();
            notifyIcon.StartTimer(TM_HIDE_FOR_1_SEC_ID, 1000);
            break;

        case MENU_CLICKME:
            MessageBoxW(NULL, L"Hello World.", L"Click Me", MB_OK);
            break;

        case MENU_CHECKME:
            SetItemCheckState(hMenu, MENU_CHECKME, !IsItemChecked(hMenu, MENU_CHECKME));
            break;

        case MENU_RADIOCHECK:
            SetItemCheckState(hMenu, MENU_RADIOCHECK, !IsItemChecked(hMenu, MENU_RADIOCHECK));
            break;

        case MENU_CUSTOMCHECK:
            SetItemCheckState(hMenu, MENU_CUSTOMCHECK, !IsItemChecked(hMenu, MENU_CUSTOMCHECK));
            break;

        case MENU_IMG_CHECK:
            SetItemCheckState(hMenu, MENU_IMG_CHECK, !IsItemChecked(hMenu, MENU_IMG_CHECK));
            break;

        case MENU_SHOW_BALLOON:
            {
                auto bi = mni::BalloonIconType::NoIcon;
                switch (balloonIconType)
                {
                case BALLOON_ICON_INFO:
                    bi = mni::BalloonIconType::SystemInfo;
                    break;
                case BALLOON_ICON_WARN:
                    bi = mni::BalloonIconType::SystemWarning;
                    break;
                case BALLOON_ICON_ERROR:
                    bi = mni::BalloonIconType::SystemError;
                    break;
                case BALLOON_ICON_USER:
                    bi = mni::BalloonIconType::Custom;
                    break;
                }

                auto flags = mni::BalloonFlags::Default;
                if (IsItemChecked(hMenuBalloon, MENU_BALLOON_PLAY_SOUND))
                {
                    flags |= mni::BalloonFlags::PlaySound;
                }

                if (IsItemChecked(hMenuBalloon, MENU_BALLOON_RESPECT_QUIET))
                {
                    flags |= mni::BalloonFlags::RespectQuietTime;
                }

                notifyIcon.SendBalloonNotification(L"Title", L"Text", bi, icoBalloon, flags);
            }
            break;


        case MENU_THEME_AUTO:
            if (!IsItemChecked(hMenuTheme, MENU_THEME_AUTO))
            {
                SetItemCheckState(hMenuTheme, MENU_THEME_AUTO, true);
                SetItemCheckState(hMenuTheme, MENU_THEME_LIGHT, false);
                SetItemCheckState(hMenuTheme, MENU_THEME_DARK, false);
                notifyIcon.SetImmersiveStyle(icm::Style::Auto);
            }
            break;
        case MENU_THEME_LIGHT:
            if (!IsItemChecked(hMenuTheme, MENU_THEME_LIGHT))
            {
                SetItemCheckState(hMenuTheme, MENU_THEME_AUTO, false);
                SetItemCheckState(hMenuTheme, MENU_THEME_LIGHT, true);
                SetItemCheckState(hMenuTheme, MENU_THEME_DARK, false);
                notifyIcon.SetImmersiveStyle(icm::Style::ImmersiveLight);
            }
            break;
        case MENU_THEME_DARK:
            if (!IsItemChecked(hMenuTheme, MENU_THEME_DARK))
            {
                SetItemCheckState(hMenuTheme, MENU_THEME_AUTO, false);
                SetItemCheckState(hMenuTheme, MENU_THEME_LIGHT, false);
                SetItemCheckState(hMenuTheme, MENU_THEME_DARK, true);
                notifyIcon.SetImmersiveStyle(icm::Style::ImmersiveDark);
            }
            break;

        case MENU_BALLOON_ICON_NONE:
            balloonIconType = UpdateRadioCheckmarks(hMenuBalloon, MENU_BALLOON_ICON_NONE, balloonItems);
            break;
        case MENU_BALLOON_ICON_INFO:
            balloonIconType = UpdateRadioCheckmarks(hMenuBalloon, MENU_BALLOON_ICON_INFO, balloonItems);
            break;
        case MENU_BALLOON_ICON_WARN:
            balloonIconType = UpdateRadioCheckmarks(hMenuBalloon, MENU_BALLOON_ICON_WARN, balloonItems);
            break;
        case MENU_BALLOON_ICON_ERROR:
            balloonIconType = UpdateRadioCheckmarks(hMenuBalloon, MENU_BALLOON_ICON_ERROR, balloonItems);
            break;
        case MENU_BALLOON_ICON_USER:
            balloonIconType = UpdateRadioCheckmarks(hMenuBalloon, MENU_BALLOON_ICON_USER, balloonItems);
            break;

        case MENU_BALLOON_PLAY_SOUND:
            if (!IsItemChecked(hMenuBalloon, MENU_BALLOON_PLAY_SOUND))
            {
                SetItemCheckState(hMenuBalloon, MENU_BALLOON_PLAY_SOUND, true);
                playSound = true;
            }
            else
            {
                SetItemCheckState(hMenuBalloon, MENU_BALLOON_PLAY_SOUND, false);
                playSound = false;
            }
            break;

        case MENU_BALLOON_RESPECT_QUIET:
            if (!IsItemChecked(hMenuTheme, MENU_BALLOON_RESPECT_QUIET))
            {
                SetItemCheckState(hMenuBalloon, MENU_BALLOON_RESPECT_QUIET, true);
                respectQuietTime = true;
            }
            else
            {
                SetItemCheckState(hMenuBalloon, MENU_BALLOON_RESPECT_QUIET, false);
                respectQuietTime = false;
            }
            break;

        case MENU_EXIT:
            notifyIcon.Quit();
            break;
        }
    };

    //auto mymsg = notifyIcon.GenerateMessageId();
    //std::thread mythread([&]{
    //    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    //    auto id = std::hash<std::thread::id>{}(std::this_thread::get_id());
    //    notifyIcon.SendCustomMessage(mymsg, static_cast<WPARAM>(id), 0);
    //});

    //notifyIcon.OnDestroy = [&]{
    ////    mythread.join();
    //    auto a = notifyIcon.GenerateMessageId();
    //};

    //notifyIcon.OnCustomMessage = [&](UINT msg, WPARAM wParam, LPARAM lParam){
    //    if (msg == mymsg)
    //    {
    //        MessageBoxW(0, std::to_wstring((size_t)wParam).c_str(), L"Custom Message", MB_OK);
    //    }
    //};

    if (FAILED(notifyIcon.Show()))
    {
        MessageBoxW(NULL, L"Failed to show Notify Icon!", L"Error", MB_OK);
        return 1;
    }
    notifyIcon.MainLoop();
        //mythread.join();
    return 0;
    //return notifyIcon.MainLoop();
}
