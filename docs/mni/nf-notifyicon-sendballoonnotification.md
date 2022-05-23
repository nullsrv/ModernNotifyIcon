# SendBalloonNotification function

Send balloon notification.

## Syntax

```cpp
auto SendBalloonNotification(
    [in] std::wstring_view title,
    [in] std::wstring_view text,
    [in] BalloonIconType   iconType,
    [in] HICON             hIcon,
    [in] BalloonFlags      flags
) -> HRESULT;
```

## Parameters

`[in] title`</br>
Type: **std::wstring_view**

Notification title.

`[in] text`</br>
Type: **std::wstring_view**

Notification text.

`[in] iconType`</br>
Type: [BalloonIconType](ne-balloonicontype.md)

Balloon icon type.

`[in] hIcon`</br>
Type: HICON

Handle to icon. Only used when **iconType** is set to **BalloonIconType::Custom**.

`[in] flags`</br>
Type: [BalloonFlags](ne-balloonflags.md)

Flags.

## Return value

This function returns one of the following values.

Return code                        | Type    | Description
---------------------------------- | ------- | -----------------------------------------------------------------------------
MNI_SUCCESS                        | Success | The function successfully show balloon notification.
MNI_ICON_NOT_CREATED               | Error   | Icon is not created in Notification Area.
MNI_FAILED_TO_COPY_BALLOON_TITLE   | Error   | Failed to copy balloon title.
MNI_FAILED_TO_COPY_BALLOON_TEXT    | Error   | Failed to copy balloon text.
MNI_FAILED_TO_SHOW_BALLOON         | Error   | Call to Shell_NotifyIconW(NIM_MODIFY, ...) failed.
