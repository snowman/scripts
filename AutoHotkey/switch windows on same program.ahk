#SingleInstance Force
#Persistent

DetectHiddenWindows, Off

;; Description
;;
;;   This AutoHotkey script is to switch between different windows of the same process name.
;;
;;   The checking algorithm is based on the app's process basename without path.
;;
;;   It works well with regular Window apps, Explorer, Chrome, etc

;; Usage
;;
;;   Activate NEXT window of same process name
;;   of the current active app with "Alt + `"
!`::
WinGet, activeProcess, ProcessName, A

;; The command below create pseudo array like windowList<index>,
;; and windowList is the count of total window found.
;;
;; windowList = 2              ;; two window
;; windowList1 = 0xHEX_VALUE;  ;; first window
;; windowList2 = 0xHEX_VALUE;  ;; second window
WinGet, windowList, List, ahk_exe %activeProcess%
windowCount := windowList

If windowCount > 1
{
    If (activeProcess = "explorer.exe") {
        Loop, % windowCount {
            rIndex := windowCount + 1 - A_Index

            WinGetClass, sClass, % "ahk_id " windowList%rIndex%

            If (sClass = "CabinetWClass") {
                WinActivate, % "ahk_id " windowList%rIndex%

                break
            }
        }
    } Else {
        WinActivate, % "ahk_id " windowList%windowCount%
    }
}
Return
