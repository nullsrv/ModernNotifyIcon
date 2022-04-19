# UseStandardTip function

Tell NotifyIcon to use standard text tip.

## Syntax

```cpp
auto UseStandardTip() -> HRESULT;
```

## Return value

This function returns one of the following values.

Return code                        | Type    | Description
---------------------------------- | ------- | -----------------------------------------------------------------------------
MNI_SUCCESS                        | Success | The function successfully changed tip.
MNI_FAILED_TO_COPY_TIP             | Error   | Failed to copy tip data.
MNI_FAILED_TO_CHANGE_TIP           | Error   | Call to Shell_NotifyIconW(NIM_MODIFY, ...) failed.
