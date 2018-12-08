;=======================================================Introduction==========================================
; Author: 			Le Ngoc Huy - UIT K12
; Contact:
;	Email : 		huykingsofm@gmail.com
;	Edu Email: 		17520074@gm.uit.edu.vn
;	Github: 		https://github.com/huykingsofm/WarGame.git
;
; File Name: 		utilities.au3
; Language:			AutoIT
; Modified Date:	Dec 29 2018
; Purpose:			Support some functions to create Metro-UI and mining data
;
; PUBPLIC FUNCTIONS:
;	__eventOnCover(controlID, cursor, curColor, px, py, w, h, fontsize, changeFlag, ByRef fOver)
;	__eventOnCoverPic(PicID, cursor, curPic, newPic, px, py, w, h, ByRef fOver)
;	__controlOnClick(cursor)
;
;	__extractLog(data, flag)
;	__seperate(binary, sep)
;	__fade(hWnd, flag)
;	__readAnElementJson($hFile)
;	__fallColor(controlID, curColor, newColor, speed)
;	__breakText($text)
;	__getParentDir(myDir)
;
;	Functions copied from public sources
;   _ColorGradient(hInitialColor, hFinalColor, iReturnSize)
;	_hBmpToPicControl(iCID, hBmp, iFlag = 0)
;	_GetWHI(sImage)
;
; PUBPLIC CONSTANTS:
;	See detail in below......
;=============================================================================================================

#Region GLOBAL CONSTANT
   Const $SOFT_TIME = 250
   Const $W_SCREEN  = 1366
   Const $H_SCREEN  = 750

#EndRegion

#include-once
#include <String.au3>
#include <GDIPlus.au3>
;============================================================================
; Func __eventOnCover(controlID, cursor, curColor, px, py, w, h, fontsize, changeFlag, ByRef fOver)
; Purpose: Change status of control if cursor covers it
;
; Parameters:
;	+ controlID: 		handler of control
;	+ cursor:			value is returned by GUIGetCursorInfo()
;	+ curColor:			current color of control
;	+ px, py, w, h:		position (left, top, witdh, height) of control
;	+ fontsize:			font size of text of control
;	+ changeFlag:		if Flag is off(False), the control change only color
;	+ fOver:			a flag check whether cursor is on control or not
; Return:
;	+ no return
;=============================================================================
Func __eventOnCover($controlID, $cursor, $curColor, $px, $py, $w, $h, $fontsize, $changeFlag, ByRef $fOver)
   $newColor = $curColor + 0x111111
   If $cursor[4] = $ControlID Then
	  If $fOver = False Then
		 If $changeFlag Then
			GUICtrlSetPos($controlID, $px -2, $py - 2, $w + 4, $h + 4)
			GUICtrlSetFont($controlID, $fontsize + 1, 400, 0, "Arial Rounded MT Bold")
		 EndIf
		 GUICtrlSetBkColor($controlID, $newColor)
		 $fOver = True
	  EndIf
   Else
	  If $fOver = True Then
		 If $changeFlag Then
			GUICtrlSetPos($controlID, $px, $py, $w, $h)
			GUICtrlSetFont($controlID, $fontsize, 400, 0, "Arial Rounded MT Bold")
		 EndIf
		 GUICtrlSetBkColor($controlID, $curColor)
		 $fOver = False
	  EndIf
   EndIf
EndFunc

;============================================================================
; Func __eventOnCoverPic(PicID, cursor, curPic, newPic, px, py, w, h, ByRef fOver)
; Purpose: Change status of control if cursor cover it
;
; Purpose: Change status of Pic if cursor cover it
;
; Parameters:
;	+ PicID: 			handler of Pic
;	+ cursor:			value is returned by GUIGetCursorInfo()
;	+ curPic:			hBITMAP is return by _GetWHI()
;	+ newColor:			hBITMAP is return by _GetWHI()
;	+ px, py, w, h:		position (left, top, witdh, height) of Pic
;	+ fOver:			a flag check whether cursor is on control or not
; Return:
;	+ no return
;=============================================================================
Func __eventOnCoverPic($PicID, $cursor, $curPic, $newPic, $px, $py, $w, $h, ByRef $fOver)
   If $cursor[4] = $PicID Then
	  If $fOver = False Then
		 _hBmpToPicControl($PicID, $newPic)
		 GUICtrlSetPos($PicID, $px, $py, $w, $h)
		 $fOver = True
	  EndIf
   Else
	  If $fOver = True Then
		 _hBmpToPicControl($PicID, $curPic)
		 GUICtrlSetPos($PicID, $px, $py, $w, $h)
		 $fOver = False
	  EndIf
   EndIf
EndFunc


;============================================================================
; Func __controlOnClick(cursor)
; Purpose: Return control which cursor is on
;
; Parameters:
;	+ cursor:			value is returned by GUIGetCursorInfo()
; Return:
;	+ If cursor is on a control, return handler of it
;	+ If cursor is not on any control, return -1
;=============================================================================
Func __controlOnClick($cursor)
   If $cursor[2] = 1 Then
	  Return $cursor[4]
   Else
	  Return -1
   EndIf
EndFunc


;============================================================================
; Func __extractLog(data, flag)
; Purpose: get some nessessary fraction of data
;
; Parameters:
;	+ data:			value is returned by ALUCall("ALU") - a function of ALU.au3
;	+ flag:			one of constants:
;					EXTRACT_ERROR: 	get error of data
;					EXTRACT_MUL3:	get log of 3-register-multiple
;					EXTRACT_MUL2:	get log of 2-register-multiple
;					EXTRACT_DIV3:	get log of 3-register-division
; Return:
;	+ If success, return fraction data responding flag
;	+ If failure, return "error"
;=============================================================================
Const $EXTRACT_ERROR = 1
Const $EXTRACT_MUL3 = 2
Const $EXTRACT_MUL2 = 4
Const $EXTRACT_DIV3 = 8

Func __extractLog($data, $flag)

   $log = StringSplit($data, "|")

   $res = "error"

   If BitAND($flag, $EXTRACT_ERROR) Then
	  Return $log[1]
   EndIf

   If BitAND($flag, $EXTRACT_MUL3) Then
	  ;Return $log[2]
	  Return StringSplit($log[2], ",")
   EndIf

   If BitAND($flag, $EXTRACT_MUL2) Then
	  Return StringSplit($log[3], ",")
   EndIf

   If BitAND($flag, $EXTRACT_DIV3) Then
	  Return StringSplit($log[4], ",")
   EndIf

   Return $res
EndFunc


;============================================================================
; Func __seperate(binary, sep)
; Purpose: seperate binary number string into many part to make more readable
;
; Parameters:
;	+ binary:			binary string
;	+ sep:				the width of each fraction
; Return:
;	return the binary string after seperating
;=============================================================================
Func __seperate($binary, $sep)
   $n = StringLen($binary)

   For $i = $n - $sep To 0 Step -$sep
	  $binary = _StringInsert($binary, " ", $i)
   Next

   Return $binary
EndFunc


;==============================================================================
; Func __fade(hWnd, flag)
; Purpose: Provide a Smooth starting and ending GUI
;
; Parameters:
;	+ hWnd: handle of GUI
;	+ flag = 1, starting GUI
;	  flag = 0, ending GUI
;
; Return:
;	no-return
;================================================================================
Func __fade($hWnd, $flag)
   $b = 255
   $e = 0
   $s = -15

   If $flag == 1 Then
	  $b = 0
	  $e = 252
	  $s = 12
   EndIf

   For $i = $b To $e Step $s
	  WinSetTrans($hWnd, "", $i)
	  Sleep(1)
   Next
EndFunc

;================================================================================
; Func _GetWHI(sImage)
;
; Purpose : Get infomation of image
;
; Parameter:
; 	+ sImage = Path to your image
; Returns
;	+ Array[3]
;         Array[0] = Width
;         Array[1] = Height
;         Array[2] = handle to a HBITMAP
;================================================================================
Func _GetWHI($sImage)
    Local $hImage, $aRet[3]
    _GDIPlus_Startup()
    $hImage = _GDIPlus_ImageLoadFromFile($sImage)
    $aRet[0] = _GDIPlus_ImageGetWidth($hImage)
    $aRet[1] = _GDIPlus_ImageGetHeight($hImage)
    $aRet[2] = _GDIPlus_BitmapCreateHBITMAPFromBitmap($hImage)
    _GDIPlus_ImageDispose($hImage)
    _GDIPlus_Shutdown()
    Return $aRet
 EndFunc   ;==>_GetWHI

;=================================================================================
; Func _hBmpToPicControl(iCID, hBmp, iFlag = 0)
;
; Purpose : Insert BITMAP image into ControlPic
;
; Parameters:
; 	+ iCID = Control ID as returned from GUICtrlCreatePic()
; 	+ hBmp = HBITMAP as returned by _GetWHI()
; 	+ iFlag = Set to 1 will delete the $hBmp object after setting it the pic control
;=================================================================================
Func _hBmpToPicControl($iCID, $hBmp, $iFlag = 0)
   Local Const $STM_SETIMAGE = 0x0172
   Local Const $IMAGE_BITMAP = 0
   Local $hOldBmp
   $hOldBmp = GUICtrlSendMsg($iCID, $STM_SETIMAGE, $IMAGE_BITMAP, $hBmp)
   If $hOldBmp Then _WinAPI_DeleteObject($hOldBmp)
   If $iFlag Then _WinAPI_DeleteObject($hBmp)
EndFunc   ;==>_hBmpToPicControl



;=================================================================================
; Func __getParentDir(scriptPath)
;
; Purpose : return parent directory of script path
;
; Parameters:
; 	+ scriptPath :  path of script which need to get parent directory
;=================================================================================
Func __getParentDir($scriptPath)
   $ParentDir = StringLeft($scriptPath,StringInStr($scriptPath,"\",0,-1)-1)
   return $ParentDir
EndFunc

;=================================================================================
; Func __readAnElementJson($hFile)
;
; Purpose : read an Json_element from a file. Read at current cursor of this file
;
; Parameters:
; 	+ $hFile :  handle of file which returned by FileOpen()
;
; Return
;	element		array:
;		element[0] 		attribute of element
;		element[1]		id of element (if exitst)
;		element[2]		value of element responding to attribute
;
;=================================================================================
Func __readAnElementJson($hFile)
   $line = FileReadLine($hFile)
   If @error = -1 and StringCompare($line, "") = 0 Then
	  SetError(-1)
	  Return
   EndIf

   If StringCompare($line, "") = 0 Then
	  SetError(-2)
	  Return
   EndIf

   $element = StringSplit($line, ": ", 1)

   If $element[0] <> 2 Then
	  SetError(-1)
	  Return
   EndIf

   $element[1] = StringStripWS($element[1], BitOr(1, 2))
   $element[2] = StringStripWS($element[2], BitOr(1, 2))

   $attribute = StringSplit($element[1], "_")

   $element[0] = $attribute[1]
   If $attribute[0] = 1 Then
	  $element[1] = Default
   Else
	  $element[1] = $attribute[2]
   EndIf

   Return $element
EndFunc

;=================================================================================
; Func _ColorGradient(hInitialColor, hFinalColor, iReturnSize)
;
; Purpose : return a array which color is descend smoothly
;
; Parameters:
; 	+ hInitialColor 	 	int  	start color
;	+ hFinalColor	 		int		end color
;	+ iReturnSize			int		smooth level
;
; Return:
;	array of colors
;=================================================================================
Func _colorGradient($hInitialColor, $hFinalColor, $iReturnSize)
	Local $iNowRed, $iNowBlue, $iNowGreen
	$hInitialColor = Hex($hInitialColor, 6)
	$hFinalColor = Hex($hFinalColor, 6)

	Local $iRed1 = Dec(StringLeft($hInitialColor, 2))
	Local $iGreen1 = Dec(StringMid($hInitialColor, 3, 2))
	Local $iBlue1 = Dec(StringMid($hInitialColor, 5, 2))

	Local $iRed2 = Dec(StringLeft($hFinalColor, 2))
	Local $iGreen2 = Dec(StringMid($hFinalColor, 3, 2))
	Local $iBlue2 = Dec(StringMid($hFinalColor, 5, 2))

	Local $iPlusRed = ($iRed2 - $iRed1) / ($iReturnSize - 1)
	Local $iPlusBlue = ($iBlue2 - $iBlue1) / ($iReturnSize - 1)
	Local $iPlusGreen = ($iGreen2 - $iGreen1) / ($iReturnSize - 1)

	Dim $iColorArray[$iReturnSize]
	For $i = 0 To $iReturnSize - 1
		$iNowRed = Floor($iRed1 + ($iPlusRed * $i))
		$iNowBlue = Floor($iBlue1 + ($iPlusBlue * $i))
		$iNowGreen = Floor($iGreen1 + ($iPlusGreen * $i))
		$iColorArray[$i] = Dec(Hex($iNowRed, 2) & Hex($iNowGreen, 2) & Hex($iNowBlue, 2))
	Next
	Return $iColorArray
EndFunc   ;==>__ColorGradient


;=================================================================================
; Func __fallColor(controlID, curColor, newColor, speed = 10)
;
; Purpose : change color of control smoothly
;
; Parameters:
; 	+ controlID			handler of ControlClick
;	+ curColor			current color of Control
;	+ newColor			new color of Control
;	+ speed				speed of change
;
; Return:
;	no return
;=================================================================================
Func __fallColor($controlID, $curColor, $newColor, $speed = 10)
   $color = _colorGradient($curColor, $newColor, $speed)
   For $i = 0 To $speed - 1
	  GUICtrlSetBkColor($controlID, $color[$i])
	  Sleep(1)
   Next
EndFunc

;=================================================================================
; Func __breakText(text, length)
;
; Purpose : break text into many line
;
; Parameters:
;	+ text 		string 		text
;	+ length	Int			maximum character of each line
;
; Return:
;	Array "breakedText" which
;		breakedText[0]		number of lines
;		breakedText[1]		text line 1
;		breakedText[2]		text line 2
;		....
;=================================================================================
Func __breakText($text, $length)
   $text = StringStripWS($text, BitOr(1,2,4))
   $aText = StringSplit($text, " ")

   For $i = 1 To $aText[0]
	  If StringLen($aText[$i]) > $length Then
		 SetError(1)
		 Return
	  EndIf
   Next

   Local $breakedTextTemp[10]
   $nText = 0;

   $i = 1
   $lastSpacePos = 0
   $curLen = 0
   $curText = ""
   $prePos = 0
   $nPart = 1

   While $i <= $aText[0]
	  $curLen += StringLen($aText[$i])
	  $curLen += $nPart = 1? 0 : 1

	  If $curLen < $length Then
		 $curText &= " " & $aText[$i]
		 $nPart += 1
		 $i += 1
	  ElseIf $curLen = $length Then
		 $breakedTextTemp[$nText] = $curText & " " & $aText[$i]
		 $nText += 1
		 $curText = ""
		 $curLen = 0
		 $nPart = 1
		 $i += 1
	  Else
		 $breakedTextTemp[$nText] = $curText
		 $nText += 1
		 $curLen = 0
		 $curText = ""
		 $nPart = 1
	  EndIf
   WEnd

   If StringCompare($curText, "") <> 0 Then
	  $breakedTextTemp[$nText] = $curText
	  $nText += 1
   EndIf

   Local $breakedText[$nText + 1]
   $breakedText[0] = $nText
   For $i = 1 To $nText
	  $breakedText[$i] = $breakedTextTemp[$i - 1]
   Next
   Return $breakedText
EndFunc