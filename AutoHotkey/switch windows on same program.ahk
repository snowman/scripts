#SingleInstance Force

;; This AutoHotkey script is to switch between open Windows of the same
;; type and same App (.exe)
;; The "type" checking is based on the App's Title convention that
;; stipulates that the App name should be at the end of the Window title
;; (Eg: New Document - Word )
;; It works well with regular Window Apps, Chrome Shortcuts and Chrome Apps
;;

ExtractAppTitle(FullTitle)
{
  ;; Get the last field of empty-separated string
  AppTitle := SubStr(FullTitle, InStr(FullTitle, " ", false, -1) + 1)
  Return AppTitle
}

;; Activate NEXT Window of same type (title checking) of
;; the current APP with "Alt + `"
!`::
WinGet, ActiveProcess, ProcessName, A
WinGet, OpenWindowsAmount, Count, ahk_exe %ActiveProcess%

If OpenWindowsAmount = 1
  Return
Else
{
  WinGetTitle, FullTitle, A
  AppTitle := ExtractAppTitle(FullTitle)

  ;; The command below create varaible starts with WindowsWithSameTitleList
  ;; For example, the below is get from "SciTE4AutoHotkey"
  ;; WindowsWithSameTitleList = 2 ;; two window
  ;; WindowsWithSameTitleList1 = 0xHEX_VALUE;
  ;; WindowsWithSameTitleList2 = 0xHEX_VALUE;
  SetTitleMatchMode, 2
  WinGet, WindowsWithSameTitleList, List, %AppTitle%

  If WindowsWithSameTitleList > 1
  {
     WinActivate, % "ahk_id " WindowsWithSameTitleList%WindowsWithSameTitleList%
  }
}
Return
