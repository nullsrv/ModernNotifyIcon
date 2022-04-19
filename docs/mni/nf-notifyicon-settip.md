# SetTip function

Set tip of NotifyIcon.

## Syntax

This function comes in two variants:

```cpp
auto SetTip(
    [in] std::wstring_view tip
) -> HRESULT;
```

```cpp
auto SetTip(
    [in] std::wstring_view tip     tip,
    [in] ResourceDestructionPolicy rdp
) -> HRESULT;
```

## Parameters

`[in] tip`</br>
Type: **std::wstring**

Text of the tip.

`[in] rdp`</br>
Type: [ResourceDestructionPolicy](ne-notifyicon-resourcedestructionpolicy.md)

Destruction policy. This value has no effect in this function.


## Return value

This function returns one of the following values.

Return code                        | Type    | Description
---------------------------------- | ------- | -----------------------------------------------------------------------------
MNI_SUCCESS                        | Success | The function successfully changed tip.
MNI_ARGUMENT_TIP_IS_NULL           | Warning | Tip text was set to empty text.
MNI_FAILED_TO_COPY_TIP             | Error   | Failed to copy tip data.
MNI_FAILED_TO_CHANGE_TIP           | Error   | Call to Shell_NotifyIconW(NIM_MODIFY, ...) failed.
