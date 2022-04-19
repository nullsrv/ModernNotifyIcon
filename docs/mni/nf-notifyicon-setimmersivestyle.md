# SetImmersiveStyle function

Set immersive context menu style.

## Syntax

```cpp
auto SetImmersiveStyle(
    [in] icm::Style style
) -> void;
```

## Parameters

`[in] style`</br>
Type: [icm::Style](ne-icm-style.md)

Style to be set.

## Return value

None

## Remarks

Only available if library is compiled with ```FEATURE_IMMERSIVE_CONTEXT_MENU_ENABLED``` defined.
