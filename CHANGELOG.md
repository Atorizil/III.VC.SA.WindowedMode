## 2.2
- added support for the Japanese version of GTA VC

## 2.0
- added error message about unsupported game version
- added error message about missing ASI Loader
- added **auto pause** and **auto resume** features
- removed CoordsManager menu
- simplified settings: Alt+Enter now cycles between all available window styles
- window size and style are now stored in the config file and restored on game start
- proper support of multiple monitors
- always show cursor over the inactive game window
- fixed cursor moving out of the window during gameplay in GTA3 and GTA-VC
- fixed flickering window title text
- added aspect ratio information to window title text
- added snapping to known aspec ratios when resizing the window
- fixed missing window icon in GTA3
- fixed issues with audio and animations in the main menu by implementing a frame limiter for it
- fixed freezes when **Grinch's ImGUI** plugin is used

## 1.17
- fixed incorrect positioning of the window in full mode introduced in 1.16
- always show cursor over inactive game window

## 1.16
- setting of initial window position is now done on game start, not postponed until entering main menu
- last window position is now stored in config file and restored on game start

## 1.15
- fixed mouse cursor not showing up in GTA SA
