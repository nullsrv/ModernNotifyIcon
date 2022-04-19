
# Docs

Welcome to ModernNotifyIcon docs.

Note: documentation might be incomplete!

# NotifyIcon

Represents icon in notification area.

## Structures

|   |
| - |
[NotifyIcon::Desc](mni/ns-notifyicon-desc.md.md)</br>Description used in constructor

## Enumerations

|   |
| - |
[NotifyIcon::ResourceDestructionPolicy](mni/ne-notifyicon-resourcedestructionpolicy.md.md.md)</br>Resource destruction policy

## Callbacks

|   |
| - |
[List of callbacks](mni/nm-notifyicon-callbacks.md)

## Methods

|   |
| - |
[NotifyIcon::NotifyIcon](mni/nf-notifyicon-constructor.md)</br>Constructor
[NotifyIcon::~NotifyIcon](mni/nf-notifyicon-destructor.mdor.md)</br>Destructor
[NotifyIcon::Init](mni/nf-notifyicon-init.md)</br>Initialize NotifyIcon
[NotifyIcon::MainLoop](mni/nf-notifyicon-mainloop.md)</br>Handle window messages
[NotifyIcon::Quit](mni/nf-notifyicon-quit.md)</br>Post Quit to message queue
[NotifyIcon::Show](mni/nf-notifyicon-show.md)</br>Show icon in Notification Area
[NotifyIcon::Hide](mni/nf-notifyicon-hide.md)</br>Hide icon from Notification Area
[NotifyIcon::ShowContextMenu](mni/nf-notifyicon-showcontextmenu.md)</br>Manually show context menu
[NotifyIcon::SetIcon](mni/nf-notifyicon-seticon.md)</br>Set icon
[NotifyIcon::SetMenu](mni/nf-notifyicon-setmenu.md)</br>Set menu
[NotifyIcon::SetTip](mni/nf-notifyicon-settip.md)</br>Set tip
[NotifyIcon::GetIcon](mni/nf-notifyicon-geticon.md)</br>Get current icon handle
[NotifyIcon::GetMenu](mni/nf-notifyicon-getmenu.md)</br>Get current menu handle
[NotifyIcon::GetTip](mni/nf-notifyicon-gettip.md)</br>Get current tip text
[NotifyIcon::UseStandardTip](mni/nf-notifyicon-usestandardtip.md)</br>Use standard tip
[NotifyIcon::UseRichPopupTip](mni/nf-notifyicon-userichpopuptip.md)</br>Use rich popup tip
[NotifyIcon::IsStandardTip](mni/nf-notifyicon-isstandardtip.md)</br>Check if standard tip is used
[NotifyIcon::IsRichPopupTip](mni/nf-notifyicon-isrichpopuptip.md.md)</br>Check if rich popup tip is used
[NotifyIcon::SendBalloonNotification](mni/nf-notifyicon-sendballoonnotification.md)</br>Send balloon notification
[NotifyIcon::RemoveBalloonNotification](mni/nf-notifyicon-removeballoonnotification.md)</br>Remove balloon notification
[NotifyIcon::StartTimer](mni/nf-notifyicon-starttimer.md)</br>Start timer
[NotifyIcon::StopTimer](mni/nf-notifyicon-stoptimer.md)</br>Stop timer
[NotifyIcon::SendCustomMessage](mni/nf-notifyicon-sendcustommessage.md)</br>Send custom message
[NotifyIcon::SetResourceDestructionPolicy](mni/nf-notifyicon-setresourcedestructionpolicy.md)</br>Set destruction policy
[NotifyIcon::UseClassicMenu](mni/nf-notifyicon-useclassicmenu.md)</br>Use classic context menu
[NotifyIcon::UseImmersiveMenu](mni/nf-notifyicon-useimmersivemenu.md)</br>Use immersive context menu
[NotifyIcon::IsClassicMenu](mni/nf-notifyicon-isclassicmenu.md)</br>Check if classic menu is used
[NotifyIcon::IsImmersiveMenu](mni/nf-notifyicon-isimmersivestyle.md)</br>Check if immersive menu is used
[NotifyIcon::SetImmersiveStyle](mni/nf-notifyicon-setimmersivestyle.md)</br>Set style of context menu

# Other

|   |
| - |
[mni::BalloonFlags](mni/ne-balloonflags.md)</br>Balloon flags
[mni::BalloonIconType](mni/ne-balloonicontype.md)</br>Balloon icon type
[icm::Style](mni/ne-icm-style.md)</br>Context menu style
[mni::ThemeInfo](mni/nc-themeinfo.md)</br>Theme info
