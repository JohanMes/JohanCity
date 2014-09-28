##################################################################
# we gaan de standaard moderne template gebruiken

!include "MUI2.nsh"

##################################################################
# namen en andere gegevens

!define PRODUCT_NAME "DirectX Engine"
!define PRODUCT_VERSION "1.4.2.7069"
!define PRODUCT_PUBLISHER "J.C. Mes Productions"
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"

##################################################################
# maak de installer

OutFile "${PRODUCT_NAME} ${PRODUCT_VERSION} Setup.exe"

##################################################################
# laat altijd de vooruitgang zien

ShowInstDetails show
ShowUnInstDetails show

##################################################################
# installerpagina's

!define MUI_ABORTWARNING
!define MUI_LANGDLL_ALLLANGUAGES

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "License.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

##################################################################
# uninstallerpagina's

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_LICENSE "License.txt"
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

##################################################################
# de talen

!insertmacro MUI_LANGUAGE "Afrikaans"
!insertmacro MUI_LANGUAGE "Albanian"
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Basque"
!insertmacro MUI_LANGUAGE "Belarusian"
!insertmacro MUI_LANGUAGE "Bosnian"
!insertmacro MUI_LANGUAGE "Breton"
#!insertmacro MUI_LANGUAGE "Bulgarian" KAPUTT
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "Croatian"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Estonian"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "Galician"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Hebrew"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Icelandic"
!insertmacro MUI_LANGUAGE "Indonesian"
!insertmacro MUI_LANGUAGE "Irish"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Kurdish"
!insertmacro MUI_LANGUAGE "Latvian"
!insertmacro MUI_LANGUAGE "Lithuanian"
!insertmacro MUI_LANGUAGE "Luxembourgish"
!insertmacro MUI_LANGUAGE "Macedonian"
!insertmacro MUI_LANGUAGE "Malay"
!insertmacro MUI_LANGUAGE "Mongolian"
!insertmacro MUI_LANGUAGE "Norwegian"
!insertmacro MUI_LANGUAGE "NorwegianNynorsk"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Serbian"
!insertmacro MUI_LANGUAGE "SerbianLatin"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "Slovak"
!insertmacro MUI_LANGUAGE "Slovenian"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SpanishInternational"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Thai"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_LANGUAGE "Uzbek"
!insertmacro MUI_LANGUAGE "Welsh"

##################################################################
# standaarddir

installDir $DESKTOP

##################################################################
# selecteer uw taal-functies

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd
Function un.onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

##################################################################
# de eigenlijke uitgevoerde code, begin

section

##################################################################
# currentdir in installfolder

SetOutPath "$INSTDIR\DirectX\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\*.*"
SetOutPath "$INSTDIR\DirectX\Tools\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Tools\*.*"
SetOutPath "$INSTDIR\DirectX\Data\Materials\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Data\Materials\*.*"
SetOutPath "$INSTDIR\DirectX\Data\Materials\sponza\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Data\Materials\sponza\*.*"
SetOutPath "$INSTDIR\DirectX\Data\Models\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Data\Models\*.*"
SetOutPath "$INSTDIR\DirectX\Data\Models\sponza\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Data\Models\sponza\*.*"
SetOutPath "$INSTDIR\DirectX\Data\Scenes\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Data\Scenes\*.*"
SetOutPath "$INSTDIR\DirectX\Data\Shaders\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Data\Shaders\*.*"
SetOutPath "$INSTDIR\DirectX\Data\Source\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Data\Source\*.*"
SetOutPath "$INSTDIR\DirectX\Data\Source\lib\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Data\Source\lib\*.*"
SetOutPath "$INSTDIR\DirectX\Data\Textures\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Data\Textures\*.*"
SetOutPath "$INSTDIR\DirectX\Data\Textures\sponza\"
File /nonfatal /x *.bak /x *.orig /x *.lnk "..\Data\Textures\sponza\*.*"

##################################################################
# maak uiteraard een uninstaller

writeUninstaller "$INSTDIR\DirectX Engine Uninstaller.exe"

sectionEnd

section "Uninstall"

RMdir /r "$INSTDIR\DirectX\"
delete "$INSTDIR\DirectX Engine Uninstaller.exe"

sectionEnd
