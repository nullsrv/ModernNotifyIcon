# List of Callbacks

[OnCreate](#OnCreate)</br>
[OnDestroy](#OnDestroy)</br>
[OnShow](#OnShow)</br>
[OnHide](#OnHide)</br>
[OnKeySelect](#OnKeySelect)</br>
[OnLmbClick](#OnLmbClick)</br>
[OnLmbDoubleClick](#OnLmbDoubleClick)</br>
[OnMmbClick](#OnMmbClick)</br>
[OnContextMenuOpen](#OnContextMenuOpen)</br>
[OnContextMenuSelect](#OnContextMenuSelect)</br>
[OnContextMenuClose](#OnContextMenuClose)</br>
[OnBalloonShow](#OnBalloonShow)</br>
[OnBalloonHide](#OnBalloonHide)</br>
[OnBalloonTimeout](#OnBalloonTimeout)</br>
[OnBalloonUserClick](#OnBalloonUserClick)</br>
[OnRichPopupOpen](#OnRichPopupOpen)</br>
[OnRichPopupClose](#OnRichPopupClose)</br>
[OnDpiChange](#OnDpiChange)</br>
[OnThemeChange](#OnThemeChange)</br>
[OnTaskbarCreated](#OnTaskbarCreated)</br>
[OnTimer](#OnTimer)</br>
[OnCustomMessage](#OnCustomMessage)
[OnSystemMessage](#OnSystemMessage)

## OnCreate

When window is created.

Prototype:</br>

```cpp
std::function<void ()> OnCreate;
```

## OnDestroy

When window is destroyed.

Prototype:</br>

```cpp
std::function<void ()> OnDestroy();
```

## OnShow

When icon in Notification Area is shown.

Prototype:</br>

```cpp
std::function<void ()> OnShow;
```

## OnHide

When icon in Notification Area is hidden.

Prototype:</br>

```cpp
std::function<void ()> OnHide;
```

## OnKeySelect

When icon is selected with keyboard and **Space** or **Enter** is pressed.

Prototype:</br>

```cpp
std::function<void (int, int)> OnKeySelect;
```

Arguments:</br>
`int` - x position of the icon</br>
`int` - y position of the icon

## OnLmbClick

When icon is clicked with left mouse button.

Prototype:</br>

```cpp
std::function<void (int, int)> OnLmbClick;
```

Arguments:</br>
`int` - x position where icon was clicked</br>
`int` - y position where icon was clicked

## OnLmbDoubleClick

When icon is double clicked with left mouse button.

Prototype:</br>

```cpp
std::function<void (int, int)> OnLmbDoubleClick;
```

Arguments:</br>
`int` - x position where icon was clicked</br>
`int` - y position where icon was clicked

## OnMmbClick

When icon is clicked with middle mouse button.

Prototype:</br>

```cpp
std::function<void (int, int)> OnMmbClick;
```

Arguments:</br>
`int` - x position where icon was clicked</br>
`int` - y position where icon was clicked

## OnContextMenuOpen

Called before Context Menu is about to be shown.
This is trigger by either right clicking on icon or using keyboard with Shift+F10.

Prototype:</br>

```cpp
std::function<void ()> OnContextMenuOpen;
```

## OnContextMenuSelect

When user selects item on Context Menu.
This callback is not called if user cancel context menu.

Prototype:</br>

```cpp
std::function<void (int)> OnContextMenuSelect;
```

Arguments:</br>
`int` - id of item that was selected

## OnContextMenuClose

Called after Context Menu is closed.

Prototype:</br>

```cpp
std::function<void ()> OnContextMenuClose;
```

## OnBalloonShow

When Balloon Notification is shown.

Prototype:</br>

```cpp
std::function<void ()> OnBalloonShow;
```

## OnBalloonHide

When the balloon disappears. For example, when the icon is deleted. This message is not sent
if the balloon is dismissed because of a timeout or if the user clicks the mouse.

Prototype:</br>

```cpp
std::function<void ()> OnBalloonHide;
```

## OnBalloonTimeout

When the balloon is dismissed because of a timeout.

Prototype:</br>

```cpp
std::function<void ()> OnBalloonTimeout;
```

## OnBalloonUserClick

When the balloon is dismissed because the user clicked the mouse.

Prototype:</br>

```cpp
std::function<void ()> OnBalloonUserClick;
```

## OnRichPopupOpen

Sent when the user hovers the cursor over an icon to indicate that the
richer pop-up UI should be used in place of a standard textual tooltip.

Prototype:</br>

```cpp
std::function<void (int, int)> OnRichPopupOpen;
```

Arguments:</br>
`int` - x position of mouse cursor where it's hovered over icon</br>
`int` - y position of mouse cursor where it's hovered over icon

## OnRichPopupClose

Sent when a cursor no longer hovers over an icon to indicate that the
rich pop-up UI should be closed.

Prototype:</br>

```cpp
std::function<void ()> OnRichPopupClose;
```

## OnDpiChange

When system dpi was changed.</br>
This requires minimum OS of Windows 8.1.

Prototype:</br>

```cpp
std::function<void (int)> OnDpiChange;
```

Arguments:</br>
`int` - dpi value

## OnThemeChange

When system theme is changed.

Prototype:</br>

```cpp
std::function<void (ThemeInfo)> OnThemeChange;
```

Arguments:</br>
[ThemeInfo](nc-themeinfo.md) - information about new theme

## OnTaskbarCreated

When taskbar is created.

Prototype:</br>

```cpp
std::function<void ()> OnTaskbarCreated;
```

## OnTimer

When custom timer timeout.

Prototype:</br>

```cpp
std::function<void (UINT)> OnTimer;
```

Arguments:</br>
`UINT` - id of timer

## OnCustomMessage

When custom message is send.
To send custom message use [SendCustomMessage](nf-notifyicon-sendcustommessage.md).

Prototype:</br>

```cpp
std::function<void (UINT, WPARAM, LPARAM)> OncustomMessage;
```

Arguments:</br>
`UINT` - message id</br>
`WPARAM` - message extra information, depends on message</br>
`LPARAM` - message extra information, depends on message

## OnSystemMessage

When system message is send.
If you DON'T handle the message return false.

**Use with caution!**

If you don't handle this callback properly it can make initialization fail.

Prototype:</br>

```cpp
std::function<bool (UINT, WPARAM, LPARAM)> OnSystemMessage;
```

Arguments:</br>
`UINT` - message id</br>
`WPARAM` - message extra information, depends on message</br>
`LPARAM` - message extra information, depends on message
