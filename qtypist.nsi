!include "MUI.nsh"
!define APPNAME "qtypist"
!define VERSION "0.1.0"
!define DESCRIPTION "練習打字的小程式"
!define UNINSTALLER "uninstall.exe"
!define LICENSE "license.txt"
!define CHANGELOG "changelog.txt"

Name "${APPNAME}"
OutFile "qtypist_${VERSION}-setup.exe"
InstallDir $PROGRAMFILES\qtypist

RequestExecutionLevel admin

var StartMenuFolder

!insertmacro MUI_PAGE_LICENSE ${LICENSE}
!insertmacro MUI_PAGE_DIRECTORY

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\qtypist"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchProgram"
!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
!define MUI_FINISHPAGE_SHOWREADME $INSTDIR\${CHANGELOG}
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;Languages

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "TradChinese"

Section

	SetOutPath $INSTDIR

	# Program Files
	File *.exe
	File audio-sources.txt
	File *.dll
	File ${LICENSE}
	File ${CHANGELOG}

	CreateDirectory translations
	CreateDirectory lists

	SetOutPath $INSTDIR\translations
	File translations\*.qm
	
	SetOutPath $INSTDIR\lists
	File lists\*.txt

	# Create Uninstaller
	WriteUninstaller "$INSTDIR\${UNINSTALLER}"

	# Create Desktop Shortcut
	CreateShortcut "$DESKTOP\qtypist.lnk" "$INSTDIR\qtypist.exe"

	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
	# Start Menu Shortcuts
	CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
	CreateShortcut "$SMPROGRAMS\$StartMenuFolder\qtypist.lnk" "$INSTDIR\qtypist.exe"
	CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall qtypist.lnk" "$INSTDIR\${UNINSTALLER}"
	!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Function LaunchProgram
	ExecShell "" "$INSTDIR\qtypist.exe"
FunctionEnd

# Uninstaller

Section "un.Uninstaller"

	Delete $INSTDIR\${UNINSTALLER}

	Delete $INSTDIR\*.exe
	Delete $INSTDIR\audio-sources.txt
	Delete $INSTDIR\*.dll
	Delete $INSTDIR\${LICENSE}
	Delete $INSTDIR\${CHANGELOG}
	Delete $INSTDIR\lists\*
	RmDir  $INSTDIR\lists
	Delete $INSTDIR\translations\*
	RmDir  $INSTDIR\translations
	RmDir  $INSTDIR    # Remove the installation directory if it's empty.

	Delete "$DESKTOP\qtypist.lnk"
	!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
	Delete "$SMPROGRAMS\$StartMenuFolder\*.lnk"
	RmDir  "$SMPROGRAMS\$StartMenuFolder"
	DeleteRegKey /ifempty HKLM "Software\qtypist"

SectionEnd
