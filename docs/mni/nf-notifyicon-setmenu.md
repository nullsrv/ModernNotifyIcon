# SetMenu function

Set context menu of NotifyIcon.

## Syntax

This function comes in two variants:

```cpp
auto SetMenu(
    [in] HMENU menu
) -> HRESULT;
```

```cpp
auto SetMenu(
    [in] HMENU                     menu,
    [in] ResourceDestructionPolicy rdp
) -> HRESULT;
```

## Parameters

`[in] menu`</br>
Type: **HMENU**

Handle to menu.

`[in] rdp`</br>
Type: [ResourceDestructionPolicy](ne-notifyicon-resourcedestructionpolicy.md)

Destruction policy.
If **ResourceDestructionPolicy** is set to **Auto** this function calls DestroyMenu to free 
memory of previous icon.


## Return value

This function returns one of the following values.

Return code                        | Type    | Description
---------------------------------- | ------- | -----------------------------------------------------------------------------
MNI_SUCCESS                        | Success | The function successfully changed menu.
MNI_ARGUMENT_MENU_IS_NULL          | Warning | Menu was set to NULL.

## Remarks

Changes will make effect when you open context menu on NotifyIcon.
