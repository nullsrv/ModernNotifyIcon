# ResourceDestructionPolicy enumeration

Style of context menu.

## Syntax

```cpp
enum class ResourceDestructionPolicy : bool {
    Auto,
    Manual
};
```

## Constants

| |
|-|
`Auto`</br>Calls destroy for icons and menus handles whenever you set new values.
`Manual`</br>Leave menu and icon handles for user to manually destroy.
