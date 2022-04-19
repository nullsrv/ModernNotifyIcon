#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace ImmersiveContextMenu {

enum class Style
{
    Auto,

    ClassicLight,
    ClassicDark,

    ImmersiveLight,
    ImmersiveDark,
};


extern "C" auto ImmersiveTrackPopupMenu (HMENU hMenu, UINT uFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptp, Style style, bool ppi) -> UINT;

} // namespace ImmersiveContextMenu
