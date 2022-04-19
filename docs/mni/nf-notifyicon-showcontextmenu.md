# ShowContextMenu function

Shows Context Menu at specified [x, y] position.

## Syntax

```cpp
auto ShowContextMenu(
    [in] int x,
    [in] int y
) -> HRESULT;
```

## Parameters

`[in] x`</br>
Type: **int**

Position X.

`[in] y`</br>
Type: **int**

Position Y.

## Return value

This function returns one of the following values.

Return code                        | Type    | Description
---------------------------------- | ------- | -----------------------------------------------------------------------------
MNI_SUCCESS                        | Success | The function successfully show the icon.
MNI_INVALID_WINDOW_HANDLE          | Error   | Window handle is NULL.
MNI_ICON_NOT_CREATED               | Error   | Icon is not created.
