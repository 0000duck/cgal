;===========================
; Copyright 2007, 2008 GeometryFactory (France)
; Author: Andreas Fabri (andreas.fabri@geometryfactrory.com), Fernando Cacciola (fernando.cacciola@geometryfactrory.com)
;============================
; Some portions of this file have been derived from "boost.nsi", the Boost Windows Installer, contributed by www.boost-consulting.org.
;
; Copyright 2006 Daniel Wallin
; Copyright 2006 Eric Niebler
; Distributed under the Boost Software License, Version 1.0. (See
; accompanying file LICENSE_1_0.txt or copy at
; http://www.boost.org/LICENSE_1_0.txt)
;============================


!include "MUI.nsh"
!include "Sections.nsh"
!include "LogicLib.nsh"
!include "Locate.nsh"
!include "StrRep.nsh"
!include "ReplaceInFile.nsh"
!include "WriteEnvStr.nsh"
!include "EnvVarUpdate.nsh"

;!define DebugLog

!ifdef DebugLog
  !include "TextLog.nsh"
!endif

!include "script_cgal_3_4.nsh"

!define CGAL_SRC  "CGAL-3.4"
!define FTP_SRC   "https://cgal.geometryfactory.com/CGAL/precompiled_libs/"
;!define FTP_SRC   "http://cgal.gforge.inria.fr/outgoing/precompiled_libs/"

;--------------------------------
; General
;--------------------------------

  ;Name and file
  Name "CGAL-3.4"
  
  !ifdef FetchLocal
  OutFile "CGAL-3.4-Full-Setup.exe"
  !else
  OutFile "CGAL-3.4-Setup.exe"
  !endif

  ;Default installation folder
  InstallDir "$PROGRAMFILES\CGAL-3.4"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\CGAL-3.4" ""
  
  BrandingText "The CGAL Project and GeometryFactory - Installer created with NSIS."

  VIProductVersion "3.4.0.0"
  VIAddVersionKey "ProductName"     "CGAL Windows Installer"
  VIAddVersionKey "CompanyName"     "The CGAL Project and GeometryFactory"
  VIAddVersionKey "LegalCopyright"  "� The CGAL Project and GeometryFactory"
  VIAddVersionKey "FileDescription" "Windows Installer for CGAL"
  VIAddVersionKey "FileVersion"     "3.4"
  
;--------------------------------
; Variables
;--------------------------------

  Var MUI_TEMP
  Var STARTMENU_FOLDER
  Var SetCGAL_DIR
  Var RegLoc
  Var Add_GMP_LIB_DIR_to_PATH  
  
;--------------------------------
; Interface Settings
;--------------------------------

  !define MUI_ICON  "cgal-16.ico"
  !define MUI_UNICON  "cgal-16.ico"
  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP_NOSTRETCH

  !define MUI_HEADERIMAGE_BITMAP "cgal_very_small_FFFFFF.bmp" ; optional
 
  !define MUI_FINISHPAGE_NOAUTOCLOSE
  
  !define MUI_ABORTWARNING

  !define MUI_WELCOMEFINISHPAGE_BITMAP Zirkel.bmp 
  !define MUI_WELCOMEFINISHPAGE_BITMAP_NOSTRETCH

  !define MUI_COMPONENTSPAGE_SMALLDESC

  !define MUI_WELCOMEPAGE_TEXT "This installs CGAL-3.4 on your machine."

  !define MUI_FINISHPAGE_TITLE "Installation finished"

  !define MUI_FINISHPAGE_TEXT "You have downloaded CGAL successfully. Please continue the installation, reading the installation instructions."

  !define MUI_FINISHPAGE_LINK "Installation instructions"
  
  !define MUI_FINISHPAGE_LINK_LOCATION "file:///$INSTDIR/doc_html/index.html"
  
;--------------------------------
; Pages
;--------------------------------

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "${CGAL_SRC}\LICENSE"
     
  ; A page where the user can specify a default variant configuration (taken from the boost installer)
  Page custom VariantsPage   
  
  !insertmacro MUI_PAGE_COMPONENTS
 
  !insertmacro MUI_PAGE_DIRECTORY
  
  ; A page where the user can check/uncheck the environment variables
  ; used to specify paths in vproj files to be added.
  Page custom envarsPage 
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "CGAL-3.4" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
  
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
; Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
; Sections
;--------------------------------


;--------------------------------
Section "!Main CGAL" MAIN_Idx

!ifndef SkipFiles
  SectionIn RO 
  SetOutPath "$INSTDIR\auxiliary"
  File /nonfatal /r "${CGAL_SRC}\auxiliary\*.*"
  SetOutPath "$INSTDIR\cmake"
  File /r "${CGAL_SRC}\cmake\*.*"
  SetOutPath "$INSTDIR\config"
  File /r "${CGAL_SRC}\config\*.*"
  SetOutPath "$INSTDIR\doc_html"
  File /r "${CGAL_SRC}\doc_html\*.*"
  SetOutPath "$INSTDIR\include"
  File /r "${CGAL_SRC}\include\*.*"
  SetOutPath "$INSTDIR\scripts"
  File /r "${CGAL_SRC}\scripts\*.*"
  SetOutPath "$INSTDIR\src"
  File /r "${CGAL_SRC}\src\*.*"

  SetOutPath "$INSTDIR"
  File "${CGAL_SRC}\AUTHORS"
  File "${CGAL_SRC}\CHANGES"
  File "${CGAL_SRC}\CMakeLists.txt"
  File "${CGAL_SRC}\INSTALL"
  File "${CGAL_SRC}\LICENSE"
  File "${CGAL_SRC}\LICENSE.FREE_USE"
  File "${CGAL_SRC}\LICENSE.LGPL"
  File "${CGAL_SRC}\LICENSE.QPL"
  File "${CGAL_SRC}\VERSION"
  File ".\cgal.ico"
!endif

  !insertmacro MUI_STARTMENU_WRITE_BEGIN  Application  
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"

    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END
    ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd
;--------------------------------

;--------------------------------
Section "CGAL Examples and Demos" SAMPLES_Idx

!ifndef SkipFiles
  SetOutPath "$INSTDIR\examples"
  File /r "${CGAL_SRC}\examples\*.*"
  SetOutPath "$INSTDIR\demo"
  File /r "${CGAL_SRC}\demo\*.*"
!endif
SectionEnd
;--------------------------------

;--------------------------------
; Multi Variant Sections
; Each of the sections below is a group enclosing all the variants for a given set of precomp libraries
; NOTE: The variant selection code uses the trailing "libs" in the group name to identify components.
;       DO NOT change the trailing "libs" in the section name.
;
${MultiVariantSection} "GMP and MPFR precompiled libs"  Install_GMP_MPFR_bin "$Platform" GMP_LIB_Idx

;--------------------------------

;--------------------------------
Section /o "LAPACK and TAUCS precompiled libs" TAUCS_LIB_Idx

  !insertmacro DownloadFile "auxiliary/$Platform/TAUCS-CGAL-3.4/"  "taucs.h.zip"               "$INSTDIR\auxiliary\taucs\include"
  !insertmacro DownloadFile "auxiliary/$Platform/TAUCS-CGAL-3.4/"  "taucs_private.h.zip"       "$INSTDIR\auxiliary\taucs\include"
  !insertmacro DownloadFile "auxiliary/$Platform/TAUCS-CGAL-3.4/"  "taucs_config_tests.h.zip"  "$INSTDIR\auxiliary\taucs\include"
  !insertmacro DownloadFile "auxiliary/$Platform/TAUCS-CGAL-3.4/"  "taucs_config_build.h.zip"  "$INSTDIR\auxiliary\taucs\include"
  !insertmacro DownloadFile "auxiliary/$Platform/TAUCS-CGAL-3.4/"  "blaswrap.h.zip"            "$INSTDIR\auxiliary\taucs\include"

  ${If} "$Platform" == "win32"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libatlas.lib.zip"             "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libcblas.lib.zip"             "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libf77blas.lib.zip"           "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "liblapack.lib.zip"            "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libtstatlas.lib.zip"          "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libmetis-vc71-mt-s.lib.zip"   "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libmetis-vc71-mt.lib.zip"     "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libmetis-vc71-mt-sgd.lib.zip" "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libmetis-vc71-mt-gd.lib.zip"  "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libtaucs-vc71-mt-s.lib.zip"   "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libtaucs-vc71-mt.lib.zip"     "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libtaucs-vc71-mt-sgd.lib.zip" "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libtaucs-vc71-mt-sgd.pdb.zip" "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libtaucs-vc71-mt-gd.lib.zip"  "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "libtaucs-vc71-mt-gd.pdb.zip"  "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "vcf2c-vc71-mt-s.lib.zip"      "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "vcf2c-vc71-mt.lib.zip"        "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "vcf2c-vc71-mt-sgd.lib.zip"    "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/win32/TAUCS-CGAL-3.4/" "vcf2c-vc71-mt-gd.lib.zip"     "$INSTDIR\auxiliary\taucs\lib"
  ${Else}
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "libmetis-vc80-mt-s.lib.zip"   "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "libmetis-vc80-mt.lib.zip"     "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "libmetis-vc80-mt-sgd.lib.zip" "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "libmetis-vc80-mt-gd.lib.zip"  "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "libtaucs-vc80-mt-s.lib.zip"   "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "libtaucs-vc80-mt.lib.zip"     "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "libtaucs-vc80-mt-sgd.lib.zip" "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "libtaucs-vc80-mt-sgd.pdb.zip" "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "libtaucs-vc80-mt-gd.lib.zip"  "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "libtaucs-vc80-mt-gd.pdb.zip"  "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "vcf2c-vc80-mt-s.lib.zip"      "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "vcf2c-vc80-mt.lib.zip"        "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "vcf2c-vc80-mt-sgd.lib.zip"    "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "vcf2c-vc80-mt-gd.lib.zip"     "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "clapack-vc80-mt-s.lib.zip"    "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "clapack-vc80-mt.lib.zip"      "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "clapack-vc80-mt-sgd.lib.zip"  "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "clapack-vc80-mt-gd.lib.zip"   "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "blas-vc80-mt-s.lib.zip"       "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "blas-vc80-mt.lib.zip"         "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "blas-vc80-mt-sgd.lib.zip"     "$INSTDIR\auxiliary\taucs\lib"
  	!insertmacro DownloadFile "auxiliary/x64/TAUCS-CGAL-3.4/" "blas-vc80-mt-gd.lib.zip"      "$INSTDIR\auxiliary\taucs\lib"
  ${Endif}

SectionEnd
;--------------------------------

Section /o "HTML Manuals" DOC_Idx
  !ifndef FetchLocal
    !insertmacro DownloadFileFrom "https://cgal.geometryfactory.com/" "CGAL/3.4/Manual/" "cgal_manual.zip"  "$INSTDIR\doc_html"
  !endif  
SectionEnd

Section "-Unzip"

  ${locate::Open} "$INSTDIR" "/D=0 /X=zip" $0
  ${If} $0 <> 0
    ${Do}
  	  ${locate::Find} $0 $1 $2 $3 $4 $5 $6
      ${If} "$1" != ""
        ZipDLL::extractall $1 $2
    		Pop $7
    		${If} "$7" == "success"
          Delete $1
    		${EndIf}
      ${EndIf}
    ${LoopUntil} "$1" == ""
  ${EndIf}  
  ${locate::Close} $0
  ${locate::Unload}

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir /r "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
    
  Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"
  
  ;Delete empty start menu parent diretories
  StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"
 
  startMenuDeleteLoop:
	ClearErrors
    RMDir $MUI_TEMP
    GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
    
    IfErrors startMenuDeleteLoopDone
  
    StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
  startMenuDeleteLoopDone:

  DeleteRegKey /ifempty HKCU "Software\CGAL-3.4"

SectionEnd




;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_MAIN      ${LANG_ENGLISH} "The main components of the CGAL Library."
  LangString DESC_SAMPLES   ${LANG_ENGLISH} "The CGAL demos and examples, for which you need Qt 4 in order to build them (and Qt 3 for some)."
  LangString DESC_GMP_LIB   ${LANG_ENGLISH} "The precompiled GMP and MPFR libraries (needed for exact constructions)."
  LangString DESC_TAUCS_LIB ${LANG_ENGLISH} "The precompiled LAPACK and TAUCS libraries."
  LangString DESC_DOC       ${LANG_ENGLISH} "The HTML manuals."
  LangString DESC_ENVSET    ${LANG_ENGLISH} "already set"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${MAIN_Idx}      $(DESC_MAIN)
    !insertmacro MUI_DESCRIPTION_TEXT ${SAMPLES_Idx}   $(DESC_SAMPLES)
    !insertmacro MUI_DESCRIPTION_TEXT ${GMP_LIB_Idx}   $(DESC_GMP_LIB)
    !insertmacro MUI_DESCRIPTION_TEXT ${TAUCS_LIB_Idx} $(DESC_TAUCS_LIB)
    !insertmacro MUI_DESCRIPTION_TEXT ${DOC_Idx}       $(DESC_DOC)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END


;--------------------------------
; Functions
;--------------------------------

Function .onInit

  !ifdef DebugLog
  ${LogSetFileName} "CGAL-3.4_install_log.txt"
  ${LogSetOn}
  !endif	
  
  StrCpy $Platform "win32"

  # the plugins dir is automatically deleted when the installer exits
  InitPluginsDir
  File /oname=$PLUGINSDIR\splash.bmp ".\CGAL.bmp" 
  advsplash::show 1000 600 400 -1 $PLUGINSDIR\splash
  
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "variants.ini"
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "environment_variables.ini"
  
FunctionEnd


Function .onInstSuccess

  ${If} $SetCGAL_DIR != ""
    ${WriteEnvStr} "CGAL_DIR"  $SetCGAL_DIR $RegLoc
  ${EndIf}
  
  ${If} $Add_GMP_LIB_DIR_to_PATH = 1
    ${EnvVarUpdate} $0 "PATH" "A" $RegLoc "$INSTDIR\auxiliary\gmp\lib"
  ${EndIf}
  
FunctionEnd

Function VariantsPage

    !insertmacro MUI_HEADER_TEXT "Select Default Variants" "Choose the default variants to autoselect in the next page."
    !insertmacro MUI_INSTALLOPTIONS_INITDIALOG "variants.ini"
    !insertmacro MUI_INSTALLOPTIONS_SHOW

    !insertmacro MUI_INSTALLOPTIONS_READ $0 "variants.ini" "Field 5" "State"
    ${If} $0 = 0
        !insertmacro MUI_INSTALLOPTIONS_READ $0 "variants.ini" "Field 6" "State"
    ${EndIf}

  !insertmacro MUI_INSTALLOPTIONS_READ $0 "variants.ini" "Field 11" "State"
  ${If} $0 = 1
    StrCpy $Platform "win32"
  ${Else}
    StrCpy $Platform "x64"
  ${Endif}

  ;
  ; These lines are TIGHTLY bound to the exact text in the Variants page
  ; (from variants.ini) and the section numbers of the GMP/MPFR components
  ;
  ${MaybeSelectVariant} "VC8.0" "Multithread Debug"                 4
  ${MaybeSelectVariant} "VC8.0" "Multithread"                       5
  ${MaybeSelectVariant} "VC8.0" "Multithread, static runtime"       6
  ${MaybeSelectVariant} "VC8.0" "Multithread Debug, static runtime" 7
  ${MaybeSelectVariant} "VC9.0" "Multithread Debug"                 10
  ${MaybeSelectVariant} "VC9.0" "Multithread"                       11
  ${MaybeSelectVariant} "VC9.0" "Multithread, static runtime"       12
  ${MaybeSelectVariant} "VC9.0" "Multithread Debug, static runtime" 13
  
FunctionEnd

# Disables the env var checkbox # FN and textbox # FN+1
!macro UncheckEnvStrCheckbox FN
  !insertmacro MUI_INSTALLOPTIONS_WRITE "environment_variables.ini" "Field ${FN}" "State" "0"
!macroend

!macro CheckEnvStrCheckbox FN
  !insertmacro MUI_INSTALLOPTIONS_WRITE "environment_variables.ini" "Field ${FN}" "State" "1"
!macroend

!macro DisableEnvStrCheckbox FN
  !insertmacro MUI_INSTALLOPTIONS_WRITE "environment_variables.ini" "Field ${FN}" "Flags" "DISABLED"
!macroend

!macro EnableEnvStrCheckbox FN
  !insertmacro MUI_INSTALLOPTIONS_WRITE "environment_variables.ini" "Field ${FN}" "Flags" ""
!macroend

# Disables the env var checkbox # FN
!macro SetEnvStrValueSlot FN VAL
  !insertmacro MUI_INSTALLOPTIONS_WRITE "environment_variables.ini" "Field ${FN}" "State" "${VAL}"
!macroend

!macro SetEnvStrLabel FN VAL
  !insertmacro MUI_INSTALLOPTIONS_WRITE "environment_variables.ini" "Field ${FN}" "Text" "${VAL}"
!macroend


Function envarsPage

  Push $0
  Push $1
  Push $2
  Push $3
  Push $4
  Push $5

  
  !insertmacro MUI_HEADER_TEXT "Setting Environment Variables" "Choose whether to set or not the following environment variables"
  
  ReadEnvStr $1 "CGAL_DIR"   # $1 = existing value for CGAL_DIR
  
  !insertmacro SetEnvStrValueSlot 7 $INSTDIR
  !insertmacro SetEnvStrValueSlot 7 $INSTDIR
 
  ${If} $1 != ""
      StrCpy $3 "($(DESC_ENVSET):  $1 )"    
      !insertmacro UncheckEnvStrCheckbox 6
      !insertmacro SetEnvStrLabel        8 $3
  ${Endif}

  SectionGetText ${GMP_LIB_Idx} $2 
  
  SectionGetFlags ${GMP_LIB_Idx} $1 
  IntOp $2 $1 & ${SF_PSELECTED}
  
  ${If} $2 == 0
    !insertmacro UncheckEnvStrCheckbox 9
    !insertmacro DisableEnvStrCheckbox 9
  ${Else}
    !insertmacro CheckEnvStrCheckbox  9
    !insertmacro EnableEnvStrCheckbox 9
  ${Endif}
  
  !insertmacro MUI_INSTALLOPTIONS_INITDIALOG "environment_variables.ini"

  !insertmacro MUI_INSTALLOPTIONS_SHOW_RETURN
  Pop $0
  ${If} "$0" = "success"
    # PROCESSING - Installs selected environment variables
    
    !insertmacro MUI_INSTALLOPTIONS_READ $3 "environment_variables.ini" "Field 2" "State" # $3=Is ALL USERS selected

    ${If} $3 == 1
      StrCpy $RegLoc "HKLM"
    ${Else}  
      StrCpy $RegLoc "HKCU"
    ${EndIf}    
    
    !insertmacro MUI_INSTALLOPTIONS_READ $3 "environment_variables.ini" "Field 6" "State" # CGAL_DIR checkbox
    !insertmacro MUI_INSTALLOPTIONS_READ $4 "environment_variables.ini" "Field 7" "State" # CGAL_DIR value
    ${If} $3 == 1 
      StrCpy $SetCGAL_DIR $4
    ${EndIF}

    !insertmacro MUI_INSTALLOPTIONS_READ $5 "environment_variables.ini" "Field 9" "State" # Add to PATH checkbox
    ${If} $5 == 1 
      StrCpy $Add_GMP_LIB_DIR_to_PATH 1
    ${EndIF}
    
  ${EndIf}
  
  Pop $5
  Pop $4
  Pop $3
  Pop $2
  Pop $1
  Pop $0
  
FunctionEnd
