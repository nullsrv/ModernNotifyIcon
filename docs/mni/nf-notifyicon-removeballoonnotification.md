# RemoveBalloonNotification function

Remove previously shown notification from system.

## Syntax

```cpp
auto RemoveBalloonNotification() -> HRESULT;
```

## Return value

This function returns one of the following values.

Return code                        | Type    | Description
---------------------------------- | ------- | -----------------------------------------------------------------------------
MNI_SUCCESS                        | Success | The function successfully removed notification
MNI_ICON_NOT_CREATED               | Error   | NotifyIcon is not created
MNI_FAILED_TO_REMOVE_BALLOON       | Error   | Call to Shell_NotifyIconW(NIM_MODIFY, ...) failed
