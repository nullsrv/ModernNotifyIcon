# BalloonIconType enumeration

Type of icon used in notification.

## Syntax

```cpp
enum class BalloonIconType : unsigned char {
    NoIcon        = 0,
    SystemInfo    = 1,
    SystemWarning = 2,
    SystemError   = 3,
    Custom        = 4
};
```

## Constants

| |
|-|
`NoIcon`</br>Don't use icon in notification
`SystemInfo`</br>Use system Info icon
`SystemWarning`</br>Use system Warning icon
`SystemError`</br>Use system Error icon
`Custom`</br>Use icon from function argument
