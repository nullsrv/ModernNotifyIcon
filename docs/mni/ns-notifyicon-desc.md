# NotifyIcon::Desc structure

Description used to construct NotifyIcon.

## Syntax

```cpp
struct Desc {
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
```

## Members

`icon`</br>
Type: **HICON**

Handle to icon used in notification area.

`menu`</br>
Type: **HICON**

Handle to menu used for context menu.

`useStandardTip`</br>
Type: **bool**

Determines whenever standard text tip is used instead of rich popup.

`tip`</br>
Type: **std::wstring**

Tip text. Used only if **useStandardTip** is set to true.

`useGuid`</br>
Type: **bool**

Determines whenever NotifyIcon should use GUID to identify icon.
More information [here](https://docs.microsoft.com/en-us/windows/win32/api/shellapi/ns-shellapi-notifyicondataa#troubleshooting).

`guid`</br>
Type: **GUID**

NotifyIcon guid. Used only if **useGuid** is set to true.

`instance`</br>
Type: **HINSTANCE**

Handle to hInstance of application. If **instance** is null, GetModuleHandle(NULL) would be used to obtain instance.

`windowTitle`</br>
Type: **std::wstring_view**

Title of invisible window.

`className`</br>
Type: **std::wstring_view**

Class name of invisible window.

`resourceDestructionPolicy`</br>
Type: [ResourceDestructionPolicy](ne-notifyicon-resourcedestructionpolicy.md)

Determines how NotifyIcon should handle resource destruction.

`useImmersiveContextMenu`</br>
Type: **bool**

Class name of invisible window.

`contextMenuStyle`</br>
Type: [icm::Style](ne-icm-style.md)

Style of context menu.
