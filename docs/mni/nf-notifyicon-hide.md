# Hide function

Hide icon from Notification Area.

## Syntax

```cpp
auto Hide() -> HRESULT;
```

## Return value

This function returns one of the following values.

Return code                        | Type    | Description
---------------------------------- | ------- | -----------------------------------------------------------------------------
MNI_SUCCESS                        | Success | The function successfully show the icon
MNI_ICON_ALREADY_SHOWN             | Warning | Icon is already shown
MNI_FAILED_TO_ADD_ICON             | Error   | Call to Shell_NotifyIconW(NIM_ADD, ...) failed
MNI_UNSUPPORTED_VERSION            | Error   | Call to Shell_NotifyIconW(NIM_SETVERSION, ...) failed and icon was deleted
MNI_FAILED_TO_DELETE_ICON          | Error   | Call to Shell_NotifyIconW(NIM_SETVERSION, ...) failed and icon wasn't deleted
MNI_FAILED_TO_SHOW_ICON            | Error   | Call to Shell_NotifyIconW(NIM_MODIFY, ...) failed
MNI_FAILED_TO_COPY_TIP             | Error   | Failed to copy tip data
MNI_INVALID_INSTANCE               | Error   | hInstance value is NULL
MNI_FAILED_TO_REGISTER_WNDCLASS    | Error   | Call to RegisterClassExW failed
MNI_FAILED_TO_CREATE_WINDOW        | Error   | Call to CreateWindow failed
