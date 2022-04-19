# UseRichPopupTip function

Tell NotifyIcon to use rich popup tip.

## Syntax

```cpp
auto UseRichPopupTip() -> HRESULT;
```

## Return value

This function returns one of the following values.

Return code                        | Type    | Description
---------------------------------- | ------- | --------------------------------------------------
MNI_SUCCESS                        | Success | The function successfully changed tip.
MNI_FAILED_TO_CHANGE_TIP           | Error   | Call to Shell_NotifyIconW(NIM_MODIFY, ...) failed.

## Remarks

If rich popup is used, these two callbacks are available:
**OnRichPopupOpen** and **OnRichPopupClose**.
