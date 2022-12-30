// https://docs.microsoft.com/en-us/windows/win32/menurc/menus
// https://stackoverflow.com/questions/2862883/is-it-possible-to-enumerate-menu-items-of-other-programs-on-windows
// https://blog.csdn.net/greless/article/details/83143465
// https://blog.csdn.net/seele52/article/details/17542265

// Check
// https://stackoverflow.com/questions/18589385/retrieve-list-of-menu-items-in-windows-in-c
// https://blog.csdn.net/liangzhonglin/article/details/5603792
// https://stackoverflow.com/questions/20012990/how-can-i-access-menu-items-of-a-given-program?noredirect=1&lq=1
// http://zetcode.com/gui/winapi/menus/

// idea
//  display tree like `tree` commands, thinking menu as file inside folder.

// Test
// 1. notepad
// 2. irfanview
// 3. sublime text

// TODO: provide query function, and open that menu

#include <stdio.h>
#include <windows.h>

TCHAR _szMenuString[128] = {NULL};

void ErrorExit(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process
    LocalFree(lpMsgBuf);

    printf("Error %s(%d): %s", lpszFunction, dw, lpMsgBuf);

    ExitProcess(dw);
}

void getMenuString(HMENU hMenu, int nPos) {
  GetMenuStringA(hMenu, nPos, _szMenuString, 128, MF_BYPOSITION);
}

void printMenu(HMENU hMenu, int nPos, int level) {
  TCHAR fmt_string[128] = {NULL};
  for (int col=0; col<level*2; col++)
    fmt_string[col] = ' ';

  getMenuString(hMenu, nPos);
  UINT menuId = GetMenuItemID(hMenu, nPos);

  if (menuId != -1) {
    strcpy(fmt_string + level*2, "id, name: %5d, %s\n");
    printf(fmt_string, menuId, _szMenuString);
  } else {
    strcpy(fmt_string + level*2, "menu: %s\n");
    printf(fmt_string, _szMenuString);
  }
}

void printMenuRecursive(HMENU hMenu, int level) {
  int menuCount = GetMenuItemCount(hMenu);
  if (menuCount == -1) {
    // ErrorExit(TEXT("GetMenuItemCount"));
  }

  for (int nPos=0; nPos<menuCount; nPos++) {
    printMenu(hMenu, nPos, level);

    HMENU hSubMenu = GetSubMenu(hMenu, nPos);
    int subMenuItemCount = GetMenuItemCount(hSubMenu);

    if (subMenuItemCount == -1) {
      // ErrorExit(TEXT("GetMenuItemCount"));
    }

    if (subMenuItemCount > 0) {
      printMenuRecursive(hSubMenu, level + 1);
    }
  }
}

int main(int argc, char *argv[]) {
  LPCTSTR lpClassName = argc-1 > 0 ? argv[1] : "notepad";

  HWND hWnd = FindWindowA(lpClassName, NULL);
  printf("hWnd: %d\n", hWnd);

  HMENU hmenuMain = GetMenu(hWnd);
  printf("\nhmenuMain: %d\n", hmenuMain);

  int menuCount = GetMenuItemCount(hmenuMain);
  if (menuCount == -1) {
    ErrorExit(TEXT("GetMenuItemCount"));
  }

  printMenuRecursive(hmenuMain, 0);

  UINT menuId = 65; // Pickup your menuId
  // PostMessage(hWnd, WM_COMMAND, menuId, 0);

  return 0;
}
