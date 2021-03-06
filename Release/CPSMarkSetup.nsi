; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "CPSMark"
!define PRODUCT_VERSION "1.356"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\CPSMark.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\CPSMark.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "SimpChinese"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup_1.356.assist.exe"
InstallDir "$PROGRAMFILES\CPSMark"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  ReadRegStr $R1 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString"
  StrCmp $R1 "" done

  MessageBox MB_YESNO|MB_ICONEXCLAMATION \
  "${PRODUCT_NAME} is already installed. $\n$\nDo you want to remove \
  the old installation before installing ${PRODUCT_NAME} ." \
  IDNO done

;Run the uninstaller
  ClearErrors
  ExecWait '$R1 _?=$INSTDIR'
done:
FunctionEnd

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite try
  File "AlMD5Verify.dll"
  File "AutoItX3.dll"
  File "CPSMark.exe"
  CreateDirectory "$SMPROGRAMS\CPSMark"
  CreateShortCut "$SMPROGRAMS\CPSMark\CPSMark.lnk" "$INSTDIR\CPSMark.exe"
  CreateShortCut "$DESKTOP\CPSMark.lnk" "$INSTDIR\CPSMark.exe"
  File "DuiLib.dll"
  File "MarkFrame.dll"
  File "vcruntime140.dll"
  File "msvcp140.dll"
  File "Qt5Core.dll"
  File "skin.zip"
  File "translations.zip"
  File "WinRARUse.zip"

  ZipDll::extractall "skin.zip" "$INSTDIR"
  ZipDll::extractall "translations.zip" "$INSTDIR"
  ZipDll::extractall "WinRARUse.zip" "$INSTDIR"

  Delete "$INSTDIR\skin.zip"
  Delete "$INSTDIR\translations.zip"
  Delete "$INSTDIR\WinRARUse.zip"

  CopyFiles "$EXEDIR\Resources.zip" "$INSTDIR"
  ExecWait '"$INSTDIR\WinRARUse\winrar.exe" x "$EXEDIR\AutoSetup.zip" "$EXEDIR"'
  ExecWait '"$EXEDIR\AutoSetupSoftware.exe"'
SectionEnd

Section -AdditionalIcons
  CreateShortCut "$SMPROGRAMS\CPSMark\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\CPSMark.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\CPSMark.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
SectionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\Qt5Core.dll"
  Delete "$INSTDIR\PARAMETER.INI"
  Delete "$INSTDIR\MarkFrame.dll"
  Delete "$INSTDIR\DuiLib.dll"
  Delete "$INSTDIR\CPSMark.exe"
  Delete "$INSTDIR\AutoItX3.dll"
  Delete "$INSTDIR\vcruntime140.dll"
  Delete "$INSTDIR\msvcp140.dll"
  Delete "$INSTDIR\AlMD5Verify.dll"
  Delete "$INSTDIR\Resources.zip"

  Delete "$SMPROGRAMS\CPSMark\Uninstall.lnk"
  Delete "$DESKTOP\CPSMark.lnk"
  Delete "$SMPROGRAMS\CPSMark\CPSMark.lnk"

  RMDir /r "$INSTDIR\WinRARUse"
  RMDir /r "$INSTDIR\translations"
  RMDir /r "$INSTDIR\skin"

  RMDir "$SMPROGRAMS\CPSMark"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
