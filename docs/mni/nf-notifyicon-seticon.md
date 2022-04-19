# SetIcon function

Set icon of NotifyIcon.

## Syntax

This function comes in two variants:

```cpp
auto SetIcon(
    [in] HICON icon
) -> HRESULT;
```

```cpp
auto SetIcon(
    [in] HICON                     icon,
    [in] ResourceDestructionPolicy rdp
) -> HRESULT;
```

## Parameters

`[in] icon`</br>
Type: **HICON**

Handle to an icon.

`[in] rdp`</br>
Type: [ResourceDestructionPolicy](ne-notifyicon-resourcedestructionpolicy.md)

Destruction policy.
If **ResourceDestructionPolicy** is set to **Auto** this function calls DestroyIcon to free 
memory of previous icon.

## Return value

This function returns one of the following values.

Return code                        | Type    | Description
---------------------------------- | ------- | -----------------------------------------------------------------------------
MNI_SUCCESS                        | Success | The function successfully changed icon.
MNI_ARGUMENT_ICON_IS_NULL          | Warning | Icon was set to NULL.
MNI_FAILED_TO_CHANGE_ICON          | Error   | Call to Shell_NotifyIconW(NIM_MODIFY, ...) failed.
