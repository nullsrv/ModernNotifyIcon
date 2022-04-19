# ThemeInfo class

## Public enumerations

```cpp
enum class Theme : unsigned char {
    Dark,
    Light,
    HighContrast
};
```

## Public methods

```cpp
constexpr ThemeInfo(
    const Theme theme,
    UINT foreground,
    UINT background
);

auto IsDark();
auto IsLight();
auto IsHighContrast();


auto GetTheme();
auto GetForegroundColor();
auto GetBackgroundColor();

auto operator== (ThemeInfo& other) -> bool;
auto operator!= (ThemeInfo& other) -> bool;
```

## Public static methods

### Detect

```cpp
static auto Detect () -> ThemeInfo;
```

Returns information about current system theme.
