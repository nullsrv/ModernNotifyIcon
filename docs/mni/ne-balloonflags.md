# BalloonFlags enumeration

Balloon flags.

## Syntax

```cpp
enum class BalloonFlags : unsigned char {
    Default          = 0x0,
    Realtime         = 0x1,
    PlaySound        = 0x2,
    RespectQuietTime = 0x4
};
```

## Constants

| |
|-|
`Default`</br>Default flags
`Realtime`</br>If the balloon notification cannot be displayed immediately, discard it
`PlaySound`</br>Play sound when showing notification
`RespectQuietTime`</br>Do not display the balloon notification if the current user is in "quiet time"
