#include <GUIConstantsEx.au3>
#include <StaticConstants.au3>
#include <WindowsConstants.au3>
#include <AutoItConstants.au3>
#include "utilities.au3"


; GENERAL GUI
; This is general gui, used for all stage of game, include
;	+ auction  (Dau gia)
;	+ Embattle (Dan tran)
;	+ Fight	   (Chien dau)
;
; Each stage has different GUI, but all of them base on this general gui
; General GUI has many elements, include:
;	+ Title : The big words on head of GUI is a title of Game : Vung dat bung no
;	+ Team Name : Name of each team, team 1 has red color, team 2 has blue color
;	+ Team Move : depending on stage, if it's stage1(Auction), TeamScore is remain money, if it;s stage 3(Fight), it's a move of each player
;	+ [timer]   : timer sometimes appears on GUI, it will count down time to wait for Program of player running
;
; RULE OF CONST AND VARIABLE
;	W_Control is width of Control
;	H_Control is heigth of Control
; 	X_Control is horizon position of Control
;   Y_Control is vertical position of Control


; Setting up some attribute which configure the game
; FOLDER player
$hFile = FileOpen("config.txt")
$player1 = FileReadLine($hFile)
$player2 = FileReadLine($hFile)
$game = FileReadLine($hFile)
$log = FileReadLine($hFile)
$transmissFile = FileReadLine($hFile)


$p1Name = StringSplit($player1, "\")
$p1Name = $p1Name[$p1Name[0]]
$p2Name = StringSplit($player2, "\")
$p2Name = $p2Name[$p2Name[0]]

;$player1 = "badformat"
;$player2 = "player2"
;$game = "game"
$log  = $game &"\log.txt"
$transmissFile = $game & "\.trans"

; This is width,height and colors of each cell on table(chessboard)
Const $W_CELL = 50
Const $H_CELL = 50
Local $C_CELL[2]
$C_CELL[0] = 0xBBBBBB
$C_CELL[1] = 0xDDDDDD

; Distance between 2 controls on horizon and vertical
Const $W_ALIGN = 25
Const $H_ALIGN = 25

; Import some image of tank
Const $FOLDER_TANK = __getParentDir(@ScriptDir) & "/img"

; These are normal tank
Local $hImgTank[2][10][4]
Local $direction[4]
$direction[0] = "_up"
$direction[1] = "_right"
$direction[2] = "_down"
$direction[3] = "_left"

For $i = 1 To 9
   For $j = 0 To 3
	  $aRet = _GetWHI($FOLDER_TANK & "/tank/TankRed/" & $i & $direction[$j] & ".png")
	  $hImgTank[0][$i][$j] = $aRet[2]
   Next
Next
For $i = 1 To 9
   For $j = 0 To 3
	  $aRet = _GetWHI($FOLDER_TANK & "/tank/TankBlue/" & $i & $direction[$j] & ".png")
	  $hImgTank[1][$i][$j] = $aRet[2]
   Next
Next


Local $hImgBullet[2][4]
For $i = 0 To 1
   For $j = 0 To 3
	  $aRet = _GetWHI($FOLDER_TANK & "/Bullet/" & $i + 1 & $direction[$j] & ".png")
	  $hImgBullet[$i][$j] = $aRet[2]
   Next
Next

$aRet = _GetWHI($FOLDER_TANK & "/tank/auction/auction.png")
$hImgAuctionTank = $aRet[2]

Local $hImgGround[6]
For $i = 0 To 5
   $aRet = _GetWHI($FOLDER_TANK & "/ground/" & ($i + 1) &".png")
   $hImgGround[$i] = $aRet[2]
Next

Local $hImgWall[2]
For $i = 0 To 1
   $aRet = _GetWHI($FOLDER_TANK & "/ground/wall" & 2 &".png")
   $hImgWall[$i] = $aRet[2]
Next

Local $hImgExplosion[5]
For $i = 0 To 4
   $aRet = _GetWHI($FOLDER_TANK & "/Explosion/" & ($i + 1) &".png")
   $hImgExplosion[$i] = $aRet[2]
Next

Local $hImgDestroy[5]
For $i = 0 To 4
   $aRet = _GetWHI($FOLDER_TANK & "/Explosion/del_" & ($i + 1) &".png")
   $hImgDestroy[$i] = $aRet[2]
Next


; These are effect tank when move
$aRetTankE0 = _GetWHI($FOLDER_TANK & "/tankE.png")
$aRetTankE1 = _GetWHI($FOLDER_TANK & "/tankE1.png")		; Effect tank of player 1
$aRetTankE2 = _GetWHI($FOLDER_TANK & "/tankE2.png")		; Effect tank of player 2

$aRetEmpty 	 = _GetWHI($FOLDER_TANK & "/no-tank.png") 	; Image of empty cell on chessboards

$aRetPrice 	  = _GetWHI($FOLDER_TANK & "/attribute/money.png") 		; Image of money icon
$aRetArmor 	  = _GetWHI($FOLDER_TANK & "/attribute/armor.png")		; Image of armor icon
$aRetRange 	  = _GetWHI($FOLDER_TANK & "/attribute/range.png")		; Image of range icon
$aRetStrength = _GetWHI($FOLDER_TANK & "/attribute/strength.png")	 	; Image of strength icon

$aRetMiniArmor 	  = _GetWHI($FOLDER_TANK & "/attribute/mini-armor.png")		; Image of mini armor icon
$aRetMiniRange 	  = _GetWHI($FOLDER_TANK & "/attribute/mini-range.png")		; Image of mini range icon
$aRetMiniStrength = _GetWHI($FOLDER_TANK & "/attribute/mini-strength.png")	; Image of mini strength icon

; Creating GUI with
; Size and Position of Window(General GUI) on SCREEN
Const $W_WINDOW = 1000;
Const $H_WINDOW = 720;
Const $C_WINDOW = 0x444444		; This is the color of background of window
Const $X_WINDOW = ($W_SCREEN - $W_WINDOW) / 2
Const $Y_WINDOW = 5

$hBoomLandGUI = GUICreate("BoomLand", $W_WINDOW, $H_WINDOW, $X_WINDOW, $Y_WINDOW, $WS_POPUP)
GUISetBkColor($C_WINDOW)


; Creating Title of GUI
Const $H_TITLE = 100
$hTitle = GUICtrlCreateLabel("Vùng đất bùng nổ", 0, 0, $W_WINDOW, $H_TITLE, BitOR($SS_CENTER,$SS_CENTERIMAGE))
GUICtrlSetColor(-1, 0xEEEEEE)
GUICtrlSetFont(-1, 48, 400, 0, "Arial Bold")


; Creating Stage name of GUI, starting with "DAU GIA"
Const $H_STAGE_NAME = 30
Const $W_STAGE_NAME = 200
Const $X_STAGE_NAME = ($W_WINDOW - $W_STAGE_NAME)/2
Const $Y_STAGE_NAME = $H_TITLE

$hStageName = GUICtrlCreateLabel("Đấu Giá", $X_STAGE_NAME, $Y_STAGE_NAME, $W_STAGE_NAME, $H_STAGE_NAME, BitOR($SS_CENTER,$SS_CENTERIMAGE))
GUICtrlSetColor(-1, 0xEEEEEE)
GUICtrlSetFont(-1, 22, 400, 0, "Arial Bold")


; Creating 2 player name tag on GUI
Const $W_PLAYER  = 200
Const $H_PLAYER  = 30
Const $X_PLAYER1 = $W_WINDOW/4 - $W_PLAYER / 2
Const $X_PLAYER2 = 3 * $W_WINDOW / 4 - $W_PLAYER / 2
Const $Y_PLAYER  = $H_TITLE
Const $C_PLAYER1 = 0xAA0000
Const $C_PLAYER2 = 0x0000AA

$hPlayerName1 = GUICtrlCreateLabel($p1Name, $X_PLAYER1, $Y_PLAYER, $W_PLAYER, $H_PlAYER, BitOR($SS_CENTER,$SS_CENTERIMAGE))
GUICtrlSetColor(-1, 0xEEEEEE)
GUICtrlSetBkColor(-1, $C_PLAYER1)
GUICtrlSetFont(-1, 22, 400, 0, "Arial Bold")

$hPlayerName2 = GUICtrlCreateLabel($p2Name, $X_PLAYER2, $Y_PLAYER, $W_PLAYER, $H_PLAYER, BitOR($SS_CENTER,$SS_CENTERIMAGE))
GUICtrlSetColor(-1, 0xEEEEEE)
GUICtrlSetBkColor(-1, $C_PLAYER2)
GUICtrlSetFont(-1, 22, 400, 0, "Arial Bold")


; Creating move tag of each player
Const $W_MOVE    = $W_PLAYER
Const $H_MOVE    = 40
Const $X_MOVE1   = $X_PLAYER1
Const $X_MOVE2   = $X_PLAYER2
Const $Y_MOVE    = $Y_PLAYER + $H_PLAYER
Const $C_MOVE    = 0xDDDDDD

$hMove1 = GUICtrlCreateLabel("", $X_MOVE1, $Y_MOVE, $W_MOVE, $H_MOVE, BitOR($SS_CENTER,$SS_CENTERIMAGE))
GUICtrlSetColor(-1, 0x000000)
GUICtrlSetBkColor(-1, $C_MOVE)
GUICtrlSetFont(-1, 22, 400, 0, "Arial Bold")

$hMove2 = GUICtrlCreateLabel("", $X_MOVE2, $Y_MOVE, $W_MOVE, $H_MOVE, BitOR($SS_CENTER,$SS_CENTERIMAGE))
GUICtrlSetColor(-1, 0x000000)
GUICtrlSetBkColor(-1, $C_MOVE)
GUICtrlSetFont(-1, 22, 400, 0, "Arial Bold")


; Creating close button of GUI
Const $W_CLOSE = 22
Const $H_CLOSE = 22
Const $X_CLOSE = 5
Const $Y_CLOSE = 5
Const $C_CLOSE = 0x222222

$hClose = GUICtrlCreateLabel("X", $X_CLOSE, $Y_CLOSE, $W_CLOSE, $H_CLOSE, BitOR($SS_CENTER,$SS_CENTERIMAGE))
GUICtrlSetColor(-1, 0xFFFFFF)
GUICtrlSetBkColor(-1, $C_CLOSE)
GUICtrlSetFont(-1, 22, 400, 0, "Arial Rounded MT Bold")

; Caculating some parameters of chessboard(table)
Const $W_TABLE = 8 * $W_CELL
Const $H_TABLE = 8 * $H_CELL
Const $X_TABLE = ($W_WINDOW - $W_TABLE) / 2
Const $Y_TABLE = ($H_TITLE + $H_PLAYER + $H_MOVE + $H_ALIGN) / 2 + ($H_WINDOW - $H_TABLE)/2

; Creating some tank infomation of each player showing on two side of GUI
$W_TANK = $W_CELL
$H_TANK = $H_CELL
$W_TANK_INFO = 25
$H_TANK_INFO = 25

Local $X_TANK[2]
Local $X_TANK_INFO[2]
Local $X_TANK_ARMOR_ICON[2]
Local $X_TANK_ARMOR[2]
Local $X_TANK_RANGE_ICON[2]
Local $X_TANK_RANGE[2]
Local $X_TANK_STRENGTH_ICON[2]
Local $X_TANK_STRENGTH[2]

$X_TANK[0] = $W_WINDOW /8 - $W_TANK/2 - 6 * $W_TANK_INFO / 2
$X_TANK_INFO[0] = $X_TANK[0] + $W_TANK
$X_TANK[1] = 7 * $W_WINDOW/8 - $W_TANK/2 - 6 * $W_TANK_INFO / 2
$X_TANK_INFO[1] = $X_TANK[1] + $W_TANK

$Y_TANK = $H_TITLE + $H_PLAYER + $H_MOVE + $H_ALIGN
$Y_TANK_INFO = $Y_TANK + 12
$C_TANK_INFO = 0xDDDDDD

; Attribute of armor of tank
$X_TANK_ARMOR_ICON[0] = $X_TANK_INFO[0]
$X_TANK_ARMOR[0] = $X_TANK_ARMOR_ICON[0] + $W_TANK_INFO

$X_TANK_ARMOR_ICON[1] = $X_TANK_INFO[1]
$X_TANK_ARMOR[1] = $X_TANK_ARMOR_ICON[1] + $W_TANK_INFO


; Attribute of range of tank
$X_TANK_RANGE_ICON[0] = $X_TANK_ARMOR[0] + $W_TANK_INFO
$X_TANK_RANGE[0] = $X_TANK_RANGE_ICON[0] + $W_TANK_INFO

$X_TANK_RANGE_ICON[1] = $X_TANK_ARMOR[1] + $W_TANK_INFO
$X_TANK_RANGE[1] = $X_TANK_RANGE_ICON[1] + $W_TANK_INFO


; Attribute of strength of tank
$X_TANK_STRENGTH_ICON[0] = $X_TANK_RANGE[0] + $W_TANK_INFO
$X_TANK_STRENGTH[0] = $X_TANK_STRENGTH_ICON[0] + $W_TANK_INFO

$X_TANK_STRENGTH_ICON[1] = $X_TANK_RANGE[1] + $W_TANK_INFO
$X_TANK_STRENGTH[1] = $X_TANK_STRENGTH_ICON[1] + $W_TANK_INFO


Local $hInfo[2][9][7]
For $i = 0 To 1
   For $j = 0 To 8
	  $hInfo[$i][$j][0] =  GUICtrlCreatePic("", 0, 0, 100, 101)
	  _hBmpToPicControl($hInfo[$i][$j][0], $hImgTank[$i][$j + 1][0])
	  GUICtrlSetPos(-1, $X_TANK[$i], $Y_TANK + $H_TANK * $j, $W_TANK, $H_TANK)
	  GUICtrlSetState(-1, $GUI_HIDE)

	  $hInfo[$i][$j][1] = GUICtrlCreatePic("", 0, 0, 100, 101)
	  _hBmpToPicControl($hInfo[$i][$j][1], $aRetMiniArmor[2])
	  GUICtrlSetPos(-1, $X_TANK_ARMOR_ICON[$i], $Y_TANK_INFO + $H_TANK * $j, $W_TANK_INFO, $H_TANK_INFO)
	  GUICtrlSetState(-1, $GUI_HIDE)

	  $hInfo[$i][$j][2] = GUICtrlCreateLabel("", $X_TANK_ARMOR[$i], $Y_TANK_INFO + $H_TANK * $j, $W_TANK_INFO, $H_TANK_INFO, BitOR($SS_CENTER, $SS_CENTERIMAGE))
	  GUICtrlSetBkColor(-1, $C_TANK_INFO)
	  GUICtrlSetFont(-1, 12, 400, 0, "Arial Bold")
	  GUICtrlSetState(-1, $GUI_HIDE)

	  $hInfo[$i][$j][3] = GUICtrlCreatePic("", 0, 0, 100, 101)
	  _hBmpToPicControl($hInfo[$i][$j][3], $aRetMiniRange[2])
	  GUICtrlSetPos(-1, $X_TANK_RANGE_ICON[$i], $Y_TANK_INFO + $H_TANK * $j, $W_TANK_INFO, $H_TANK_INFO)
	  GUICtrlSetState(-1, $GUI_HIDE)

	  $hInfo[$i][$j][4] = GUICtrlCreateLabel("", $X_TANK_RANGE[$i], $Y_TANK_INFO + $H_TANK * $j, $W_TANK_INFO, $H_TANK_INFO, BitOR($SS_CENTER, $SS_CENTERIMAGE))
	  GUICtrlSetBkColor(-1, $C_TANK_INFO)
	  GUICtrlSetFont(-1, 12, 400, 0, "Arial Bold")
	  GUICtrlSetState(-1, $GUI_HIDE)

	  $hInfo[$i][$j][5] = GUICtrlCreatePic("", 0, 0, 100, 101)
	  _hBmpToPicControl($hInfo[$i][$j][5], $aRetMiniStrength[2])
	  GUICtrlSetPos(-1, $X_TANK_STRENGTH_ICON[$i], $Y_TANK_INFO + $H_TANK * $j, $W_TANK_INFO, $H_TANK_INFO)
	  GUICtrlSetState(-1, $GUI_HIDE)

	  $hInfo[$i][$j][6] = GUICtrlCreateLabel("", $X_TANK_STRENGTH[$i], $Y_TANK_INFO + $H_TANK * $j, $W_TANK_INFO, $H_TANK_INFO, BitOR($SS_CENTER, $SS_CENTERIMAGE))
	  GUICtrlSetBkColor(-1, $C_TANK_INFO)
	  GUICtrlSetFont(-1, 12, 400, 0, "Arial Bold")
	  GUICtrlSetState(-1, $GUI_HIDE)
   Next
Next

Local $battlePos[10][10][2]
Local $battleGround[10][10]

For $i = 0 To 9
   For $j = 0 To 9
	  $battlePos[$i][$j][0] = $X_TABLE + $W_CELL * ($j - 1)
	  $battlePos[$i][$j][1] = $Y_TABLE + $H_CELL * ($i - 1)

	  $battleGround[$i][$j] = GUICtrlCreatePic("", $battlePos[$i][$j][0], $battlePos[$i][$j][1], 100, 101)
	  If ($i = 0 Or $j = 0 Or $i = 9 Or $j = 9) Then
		 $iWall = Random(0, 1, 1)
		 _hBmpToPicControl($battleGround[$i][$j], $hImgWall[$iWall])
	  Else
		 $iGround = Random(0, 5, 1)
		 _hBmpToPicControl($battleGround[$i][$j], $hImgGround[$iGround])
	  EndIf
	  GUICtrlSetPos(-1, $battlePos[$i][$j][0], $battlePos[$i][$j][1], $W_CELL, $H_CELL)
	  GUICtrlSetState(-1, $GUI_HIDE)
   Next
Next
; handle of Process BoomLand
Local $hPID
Local $battleTank[10][10]
Local $battle[10][10]

LauchBoomLand($hPID)
;Fight()
Auction()
Embattle()
Fight()

Func Auction()
   ; GUI seems be like:
   ;							TITLE
   ;		P1											P2
   ;		M1											M2
   ;	TankInfo1									TankInfo1
   ;	TankInfo2			AuctioningTank			TankInfo2
   ;		.				InfoOfAuction				.
   ;	TankInfo9		MoneyForTankOfPlayer		TankInfo9
   ;
   ;


   ; Creating a big image of tank to visualize the auction
   $W_AUCTIONING = 240
   $H_AUCTIONING = 240
   $X_AUCTIONING = ($W_WINDOW - $W_AUCTIONING)/2
   $Y_AUCTIONING = ($H_TITLE + $H_PLAYER + $H_MOVE + 2 * $H_ALIGN)/ 2 + ($H_WINDOW - $H_TABLE)/2
   $AuctioningTank =  GUICtrlCreatePic("", $X_AUCTIONING, $Y_AUCTIONING, 100, 101)
   _hBmpToPicControl($AuctioningTank, $hImgAuctionTank)
   GUICtrlSetPos(-1, $X_AUCTIONING, $Y_AUCTIONING, $W_AUCTIONING, $H_AUCTIONING)
   GUICtrlSetState(-1, $GUI_HIDE)

   ; Info of auctioning tank is showed here
   $W_AUCTION_INFO = 30
   $H_AUCTION_INFO = 30
   $Y_AUCTION_INFO = $Y_AUCTIONING + $H_AUCTIONING

   $X_AUCTION_PRICE_ICON = $X_AUCTIONING
   $hAuctionPriceIcon =  GUICtrlCreatePic("", $X_AUCTIONING, $Y_AUCTIONING, 100, 101)
   _hBmpToPicControl($hAuctionPriceIcon, $aRetPrice[2])
   GUICtrlSetPos(-1, $X_AUCTION_PRICE_ICON, $Y_AUCTION_INFO, $W_AUCTION_INFO, $H_AUCTION_INFO)
   GUICtrlSetState(-1, $GUI_HIDE)

   $X_AUCTION_PRICE = $X_AUCTION_PRICE_ICON + $W_AUCTION_INFO
   $hAuctionPrice = GUICtrlCreateLabel("", $X_AUCTION_PRICE, $Y_AUCTION_INFO, $W_AUCTION_INFO, $H_AUCTION_INFO, BitOR($SS_CENTER, $SS_CENTERIMAGE))
   GUICtrlSetBkColor(-1, 0xDDDDDD)
   GUICtrlSetFont(-1, 22, 400, 0, "Arial Bold")
   GUICtrlSetState(-1, $GUI_HIDE)

   $X_AUCTION_ARMOR_ICON = $X_AUCTION_PRICE + $W_AUCTION_INFO
   $hAuctionArmorIcon =  GUICtrlCreatePic("", $X_AUCTIONING, $Y_AUCTIONING, 100, 101)
   _hBmpToPicControl($hAuctionArmorIcon, $aRetArmor[2])
   GUICtrlSetPos(-1, $X_AUCTION_ARMOR_ICON, $Y_AUCTION_INFO, $W_AUCTION_INFO, $H_AUCTION_INFO)
   GUICtrlSetState(-1, $GUI_HIDE)

   $X_AUCTION_ARMOR = $X_AUCTION_ARMOR_ICON + $W_AUCTION_INFO
   $hAuctionArmor = GUICtrlCreateLabel("", $X_AUCTION_ARMOR, $Y_AUCTION_INFO, $W_AUCTION_INFO, $H_AUCTION_INFO, BitOR($SS_CENTER, $SS_CENTERIMAGE))
   GUICtrlSetBkColor(-1, 0xDDDDDD)
   GUICtrlSetFont(-1, 22, 400, 0, "Arial Bold")
   GUICtrlSetState(-1, $GUI_HIDE)

   $X_AUCTION_RANGE_ICON = $X_AUCTION_ARMOR + $W_AUCTION_INFO
   $hAuctionRangeIcon =  GUICtrlCreatePic("", $X_AUCTIONING, $Y_AUCTIONING, 100, 101)
   _hBmpToPicControl($hAuctionRangeIcon, $aRetRange[2])
   GUICtrlSetPos(-1, $X_AUCTION_RANGE_ICON, $Y_AUCTION_INFO, $W_AUCTION_INFO, $H_AUCTION_INFO)
   GUICtrlSetState(-1, $GUI_HIDE)

   $X_AUCTION_RANGE = $X_AUCTION_RANGE_ICON + $W_AUCTION_INFO
   $hAuctionRange = GUICtrlCreateLabel("", $X_AUCTION_RANGE, $Y_AUCTION_INFO, $W_AUCTION_INFO, $H_AUCTION_INFO, BitOR($SS_CENTER, $SS_CENTERIMAGE))
   GUICtrlSetBkColor(-1, 0xDDDDDD)
   GUICtrlSetFont(-1, 22, 400, 0, "Arial Bold")
   GUICtrlSetState(-1, $GUI_HIDE)

   $X_AUCTION_STRENGTH_ICON = $X_AUCTION_RANGE + $W_AUCTION_INFO
   $hAuctionStrengthIcon =  GUICtrlCreatePic("", $X_AUCTIONING, $Y_AUCTIONING, 100, 101)
   _hBmpToPicControl($hAuctionStrengthIcon, $aRetStrength[2])
   GUICtrlSetPos(-1, $X_AUCTION_STRENGTH_ICON, $Y_AUCTION_INFO, $W_AUCTION_INFO, $H_AUCTION_INFO)
   GUICtrlSetState(-1, $GUI_HIDE)

   $X_AUCTION_STRENGTH = $X_AUCTION_STRENGTH_ICON + $W_AUCTION_INFO
   $hAuctionStrength = GUICtrlCreateLabel("", $X_AUCTION_STRENGTH, $Y_AUCTION_INFO, $W_AUCTION_INFO, $H_AUCTION_INFO, BitOR($SS_CENTER, $SS_CENTERIMAGE))
   GUICtrlSetBkColor(-1, 0xDDDDDD)
   GUICtrlSetFont(-1, 22, 400, 0, "Arial Bold")
   GUICtrlSetState(-1, $GUI_HIDE)


   ; Money of each player pay for this tank is showed here
   $W_MONEY_FOR_TANK  = $W_AUCTION_INFO * 8 / 2
   $H_MONEY_FOR_TANK  = 30
   $X_MONEY_FOR_TANK1 = $X_AUCTIONING
   $X_MONEY_FOR_TANK2 = $X_MONEY_FOR_TANK1 + $W_MONEY_FOR_TANK
   $Y_MONEY_FOR_TANK  = $Y_AUCTION_INFO + $H_AUCTION_INFO
   GUICtrlSetState(-1, $GUI_HIDE)

   $hMoneyForTank1 = GUICtrlCreateLabel("", $X_MONEY_FOR_TANK1, $Y_MONEY_FOR_TANK, $W_MONEY_FOR_TANK, $H_MONEY_FOR_TANK, BitOR($SS_CENTER, $SS_CENTERIMAGE, $WS_BORDER))
   GUICtrlSetBkColor(-1, $C_PLAYER1)
   GUICtrlSetColor(-1, 0xFFFFFF)
   GUICtrlSetFont(-1, 16, 400, 0, "Arial Bold")
   GUICtrlSetState(-1, $GUI_HIDE)

   $hMoneyForTank2 = GUICtrlCreateLabel("", $X_MONEY_FOR_TANK2, $Y_MONEY_FOR_TANK, $W_MONEY_FOR_TANK, $H_MONEY_FOR_TANK, BitOR($SS_CENTER, $SS_CENTERIMAGE, $WS_BORDER))
   GUICtrlSetBkColor(-1, $C_PLAYER2)
   GUICtrlSetColor(-1, 0xFFFFFF)
   GUICtrlSetFont(-1, 16, 400, 0, "Arial Bold")
   GUICtrlSetState(-1, $GUI_HIDE)

   $W_TURN = 75
   $H_TURN = 75
   $X_TURN = ($W_WINDOW - $W_TURN)/2
   $Y_TURN = $Y_STAGE_NAME + +$H_STAGE_NAME + $H_ALIGN

   $hTurn = GUICtrlCreateLabel("0", $X_TURN, $Y_TURN, $W_TURN, $H_TURN, BitOR($SS_CENTER, $SS_CENTERIMAGE, $WS_BORDER))
   GUICtrlSetBkColor(-1, 0xDDDDDD)
   GUICtrlSetColor(-1, 0x0)
   GUICtrlSetFont(-1, 16, 400, 0, "Arial Bold")
   GUICtrlSetState(-1, $GUI_HIDE)

   ; A button is used for running to next turn
   $W_NEXT = 100
   $H_NEXT = 50
   $X_NEXT = $X_AUCTION_PRICE_ICON
   $Y_NEXT = $Y_MONEY_FOR_TANK + $H_MONEY_FOR_TANK +$H_ALIGN

   $hNext = GUICtrlCreateLabel("START", $X_NEXT, $Y_NEXT, $W_NEXT, $H_NEXT, BitOR($SS_CENTER, $SS_CENTERIMAGE, $WS_BORDER))
   GUICtrlSetBkColor(-1, 0xDDDDDD)
   GUICtrlSetColor(-1, 0x0)
   GUICtrlSetFont(-1, 16, 400, 0, "Arial Bold")

   $W_AUTO = 100
   $H_AUTO = 50
   $X_AUTO = $X_AUCTION_PRICE_ICON + $W_AUCTION_INFO * 8 - $W_AUTO
   $Y_AUTO = $Y_MONEY_FOR_TANK + $H_MONEY_FOR_TANK +$H_ALIGN
   $C_AUTO = 0xDDDDDD

   $hAuto = GUICtrlCreateLabel("AUTO", $X_AUTO, $Y_AUTO, $W_AUTO, $H_AUTO, BitOR($SS_CENTER, $SS_CENTERIMAGE, $WS_BORDER))
   GUICtrlSetBkColor(-1, $C_AUTO)
   GUICtrlSetColor(-1, 0x0)
   GUICtrlSetFont(-1, 16, 400, 0, "Arial Bold")

   GUISetState(@SW_SHOW)

   $fOverClose 	= False
   $fOverAuto 	= False
   $fOverNext 	= False

   $bAuto	 	= False
   $bFirstTurn 	= True
   $bClosed 	= False

   Local $iTurn 						    ; storing id of current turn
   Local $Attribute	[4]					    ; storing attribute of tank, include the least price, armor, range and strength
   Local $iMoneyForTank1, $iMoneyForTank2	; Money pay for tank of player 1 and 2
   Local $iMoneyOfPlayer1, $iMoneyOfPlayer2 ; Money of player 1 and 2 after this turn
   Local $iPlayerSuccess					; Player which take this tank (0 if no-player take the tank)
   Local $n[2]
   $n[0] = 0
   $n[1] = 0

   $t = 0
   While(1)
	  $cursor = GUIGetCursorInfo($hBoomLandGUI)
	  __eventOnCover($hNext, $cursor, 0xDDDDDD, $X_NEXT, $Y_NEXT, $W_NEXT, $H_NEXT, 16, True, $fOverNext)
	  __eventOnCover($hAuto, $cursor, $C_AUTO, $X_AUTO, $Y_AUTO, $W_AUTO, $H_AUTO, 16, True, $fOverAuto)
	  EffectAll($cursor, $fOverClose)

	  $click = __controlOnClick($cursor)

	  If $bAuto Then
		 $t += 1
		 If Mod($t, 10000) = 0 Then
			$click = $hNext
		 EndIf
	  EndIf

	  Switch($click)
		 Case $hClose
			__fade($hBoomLandGUI, 0)
			GUIDelete($hBoomLandGUI)
			Sleep($SOFT_TIME)
			Exit
		 Case $hNext
			If ($bFirstTurn) Then
			   $bFirstTurn = False
			   LoadAuctionTurn($iTurn, $Attribute, $iMoneyForTank1, $iMoneyForTank2, $iMoneyOfPlayer1, $iMoneyOfPlayer2, $iPlayerSuccess)
			   GUICtrlSetData($hMove1, $iMoneyOfPlayer1)
			   GUICtrlSetData($hMove2, $iMoneyOfPLayer2)
			   GUICtrlSetData($hNext, "NEXT")
			   GUICtrlSetState($AuctioningTank, $GUI_SHOW)
			   GUICtrlSetState($hAuctionPriceIcon, $GUI_SHOW)
			   GUICtrlSetState($hAuctionPrice, $GUI_SHOW)
			   GUICtrlSetState($hAuctionArmorIcon, $GUI_SHOW)
			   GUICtrlSetState($hAuctionArmor, $GUI_SHOW)
			   GUICtrlSetState($hAuctionRangeIcon, $GUI_SHOW)
			   GUICtrlSetState($hAuctionRange, $GUI_SHOW)
			   GUICtrlSetState($hAuctionStrengthIcon, $GUI_SHOW)
			   GUICtrlSetState($hAuctionStrength, $GUI_SHOW)
			   GUICtrlSetState($hMoneyForTank1, $GUI_SHOW)
			   GUICtrlSetState($hMoneyForTank2, $GUI_SHOW)
			   GUICtrlSetState($hTurn, $GUI_SHOW)
			   ;GUICtrlSetState(-1, $GUI_HIDE)
			Else
			   If ($bClosed) Then
				  ExitLoop
			   EndIf
			   GUICtrlSetData($hTurn, $iTurn + 1)
			   $exitCode = LoadAuctionTurn($iTurn, $Attribute, $iMoneyForTank1, $iMoneyForTank2, $iMoneyOfPlayer1, $iMoneyOfPlayer2, $iPlayerSuccess)
			   GUICtrlSetData($hAuctionPrice, $Attribute[0])
			   GUICtrlSetData($hAuctionArmor, $Attribute[1])
			   GUICtrlSetData($hAuctionRange, $Attribute[2])
			   GUICtrlSetData($hAuctionStrength, $Attribute[3])
			   GUICtrlSetData($hMoneyForTank1, $iMoneyForTank1)
			   GUICtrlSetData($hMoneyForTank2, $iMoneyForTank2)
			   Sleep(500)
			   GUICtrlSetData($hMove1, $iMoneyOfPlayer1)
			   GUICtrlSetData($hMove2, $iMoneyOfPLayer2)
			   If ($iPlayerSuccess > 0) Then
				  $i = $iPlayerSuccess - 1

				  GUICtrlSetState($hInfo[$i][$n[$i]][0], $GUI_SHOW)

				  GUICtrlSetData($hInfo[$i][$n[$i]][2], $Attribute[1])
				  GUICtrlSetState($hInfo[$i][$n[$i]][1], $GUI_SHOW)
				  GUICtrlSetState($hInfo[$i][$n[$i]][2], $GUI_SHOW)

				  GUICtrlSetData($hInfo[$i][$n[$i]][4], $Attribute[2])
				  GUICtrlSetState($hInfo[$i][$n[$i]][3], $GUI_SHOW)
				  GUICtrlSetState($hInfo[$i][$n[$i]][4], $GUI_SHOW)

				  GUICtrlSetData($hInfo[$i][$n[$i]][6], $Attribute[3])
				  GUICtrlSetState($hInfo[$i][$n[$i]][5], $GUI_SHOW)
				  GUICtrlSetState($hInfo[$i][$n[$i]][6], $GUI_SHOW)
				  $n[$i] += 1
			   EndIf
			   Sleep($SOFT_TIME)
			   If ($exitCode == 1) Then
				  $bClosed = True
				  GUICtrlSetData($hNext, "CLOSE")
			   EndIf
			EndIf
		 Case $hAuto
			If Not $bAuto Then
			   $bAuto = True
			   $C_AUTO = 0x123456
			   GUICtrlSetColor($hAuto, 0xFFFFFF)
			Else
			   $bAuto = False
			   $C_AUTO = 0xDDDDDD
			   GUICtrlSetColor($hAuto, 0x0)
			EndIf
			GUICtrlSetBkColor($hAuto, $C_AUTO)
			Sleep($SOFT_TIME)
	  EndSwitch
   WEnd

   ; Delete All Control before exit

   GUICtrlDelete($AuctioningTank)
   GUICtrlDelete($hAuctionPriceIcon)
   GUICtrlDelete($hAuctionPrice)
   GUICtrlDelete($hAuctionArmorIcon)
   GUICtrlDelete($hAuctionArmor)
   GUICtrlDelete($hAuctionRangeIcon)
   GUICtrlDelete($hAuctionRange)
   GUICtrlDelete($hAuctionStrengthIcon)
   GUICtrlDelete($hAuctionStrength)
   GUICtrlDelete($hMoneyForTank1)
   GUICtrlDelete($hMoneyForTank2)
   GUICtrlDelete($hNext)
   GUICtrlDelete($hTurn)
   GUICtrlDelete($hAuto)
EndFunc

Func Embattle()
   GUICtrlSetData($hStageName, "Dàn trận")
   Local $n1, $n2
   Local $TankOfPlayer1[10][3], $TankOfPlayer2[10][3]
   LoadStartEmbattle()

   LoadEmbattle($battle)

   For $i = 0 To 9
	  For $j = 0 To 9
		 GUICtrlSetState($battleGround[$i][$j], $GUI_SHOW)
		 If ($i > 0 And $i < 9 And $j > 0 And $j < 9 And $battle[$i][$j] <> 0) Then
			$iPlayer = $battle[$i][$j] > 0 ? 0 : 1;
			$direct  = $battle[$i][$j] > 0 ? 0 : 2;
			$idTank = Abs($battle[$i][$j])
			$BattleTank[$i][$j] = GUICtrlCreatePic("", 0, 0, 100, 101)
			_hBmpToPicControl($BattleTank[$i][$j], $hImgTank[$iPlayer][$idTank][$direct])
			GUICtrlSetPos(-1, $battlePos[$i][$j][0], $battlePos[$i][$j][1], $W_CELL, $H_CELL)
		 EndIf
	  Next
   Next

   $W_NEXT = 75
   $H_NEXT = 25
   $X_NEXT = $W_WINDOW - $W_NEXT - $W_ALIGN
   $Y_NEXT = $H_WINDOW - $H_NEXT - $H_ALIGN

   $hNext = GUICtrlCreateLabel("NEXT", $X_NEXT, $Y_NEXT, $W_NEXT, $H_NEXT, BitOR($SS_CENTER, $SS_CENTERIMAGE, $WS_BORDER))
   GUICtrlSetBkColor(-1, 0xDDDDDD)
   GUICtrlSetColor(-1, 0x0)
   GUICtrlSetFont(-1, 16, 400, 0, "Arial Bold")

   $fOverNext = False
   $fOverClose = False
   GUISetState(@SW_SHOW)

   While(1)
	  $cursor = GUIGetCursorInfo($hBoomLandGUI)

	  EffectAll($cursor, $fOverClose)
	  __eventOnCover($hNext, $cursor, 0xDDDDDD, $X_NEXT, $Y_NEXT, $W_NEXT, $H_NEXT, 16, True, $fOverNext)


	  $click = __controlOnClick($cursor)

	  If $click = $hClose Then
		 __fade($hBoomLandGUI, 0)
		 GUIDelete($hBoomLandGUI)
		 Sleep($SOFT_TIME)
		 Exit
	  ElseIf $click = $hNext Then
		 ExitLoop
	  EndIf
   WEnd
   GUICtrlDelete($hNext)
   Sleep($SOFT_TIME)
EndFunc

Func Fight()
   GUICtrlSetData($hStageName, "Chiến đấu")
   LoadStartFight()
   Local $iTurn
   Local $n1, $n2
   Local $tank1[10][3], $tank2[10][3]
   Local $battle[10][10]
   Local $nevent, $event[10][6]

   $W_AUTO = 75
   $H_AUTO = 25
   $X_AUTO = $W_WINDOW - $W_AUTO - $W_ALIGN
   $Y_AUTO = $H_WINDOW - $H_AUTO - $H_ALIGN
   $C_AUTO = 0xDDDDDD

   $hAuto = GUICtrlCreateLabel("AUTO", $X_AUTO, $Y_AUTO, $W_AUTO, $H_AUTO, BitOR($SS_CENTER, $SS_CENTERIMAGE, $WS_BORDER))
   GUICtrlSetBkColor(-1, $C_AUTO)
   GUICtrlSetColor(-1, 0x0)
   GUICtrlSetFont(-1, 16, 400, 0, "Arial Bold")

   $W_NEXT = 75
   $H_NEXT = 25
   $X_NEXT = $W_WINDOW - $W_NEXT - $W_ALIGN - $W_AUTO - 10
   $Y_NEXT = $H_WINDOW - $H_NEXT - $H_ALIGN

   $hNext = GUICtrlCreateLabel("NEXT", $X_NEXT, $Y_NEXT, $W_NEXT, $H_NEXT, BitOR($SS_CENTER, $SS_CENTERIMAGE, $WS_BORDER))
   GUICtrlSetBkColor(-1, 0xDDDDDD)
   GUICtrlSetColor(-1, 0x0)
   GUICtrlSetFont(-1, 16, 400, 0, "Arial Bold")
   $W_TURN = 50
   $H_TURN = 50
   $X_TURN = ($W_WINDOW - $W_TURN)/2
   $Y_TURN = $Y_STAGE_NAME + +$H_STAGE_NAME + 10

   $hTurn = GUICtrlCreateLabel("0", $X_TURN, $Y_TURN, $W_TURN, $H_TURN, BitOR($SS_CENTER, $SS_CENTERIMAGE, $WS_BORDER))
   GUICtrlSetBkColor(-1, 0xDDDDDD)
   GUICtrlSetColor(-1, 0x0)
   GUICtrlSetFont(-1, 16, 400, 0, "Arial Bold")


   $fOverNext = False
   $fOverAuto = False
   $fOverClose = False
   $bAuto = False
   $exitCode = 0
   $t = 0

   While(1)
	  $cursor = GUIGetCursorInfo($hBoomLandGUI)
	  __eventOnCover($hNext, $cursor, 0xDDDDDD, $X_NEXT, $Y_NEXT, $W_NEXT, $H_NEXT, 16, True, $fOverNext)
	  __eventOnCover($hAuto, $cursor, $C_AUTO, $X_AUTO, $Y_AUTO, $W_AUTO, $H_AUTO, 16, True, $fOverAuto)
	  EffectAll($cursor, $fOverClose)

	  $click = __controlOnClick($cursor)
	  If $bAuto Then
		 $t += 1
		 If Mod($t, 10000) = 0 Then
			$click = $hNext
		 EndIf
	  EndIf


	  Switch $click
		 Case $hClose
			__fade($hBoomLandGUI, 0)
			ExitLoop
		 Case $hNext
			   If $exitCode <> 0 Then
				  ContinueLoop
			   EndIf
			   $exitCode = LoadFight($iTurn, $n1, $n2, $tank1, $tank2, $battle, $event, $nevent)
			   GUICtrlSetData($hTurn, $iTurn)
			   If ($exitCode <> 0 And $iTurn = 1) Then
				  ConsoleWrite($exitCode)
				  ExitLoop
			   EndIf
			   Local $Cevent[4][6]
			   $nCevent = 0
			   For $i = 0 To $nevent -1
				  If ($event[$i][0] = 2 Or $event[$i][0] = 3) Then
					 For $j = 0 To 5
						$Cevent[$nCevent][$j] = $event[$i][$j]
					 Next
					 $nCevent += 1
				  EndIf
			   Next

			   Moving($nCevent, $Cevent)
			   For $i = 0 To $nevent - 1
				  If ($event[$i][0] = 0) Then
					 TankDestroy($event[$i][2], $event[$i][3])
				  ElseIf $event[$i][0] = 1 Then
					 TankDisappear($event[$i][2], $event[$i][3])
				  EndIf
			   Next

			   SetStatusOfTank($n1, $n2, $tank1, $tank2)
		 Case $hAuto
			If Not $bAuto Then
			   $bAuto = True
			   $C_AUTO = 0x123456
			   GUICtrlSetColor($hAuto, 0xFFFFFF)
		    Else
			   $bAuto = False
			   $C_AUTO = 0xDDDDDD
			   GUICtrlSetColor($hAuto, 0x0)
			EndIf
			GUICtrlSetBkColor($hAuto, $C_AUTO)

			Sleep($SOFT_TIME)
	  EndSwitch
   WEnd
   ConsoleWrite($exitcode)
   Sleep(1000)
EndFunc

Func Moving($nevent, $event)
   Local $sequencePos[10][100][2]
   Local $hBullet[4]

   For $i = 0 To $nevent - 1
	  CreatePath($event[$i][2], $event[$i][3], $event[$i][4], $event[$i][5], $sequencePos, $i)

	  $iPlayer = $event[$i][1] > 0 ? 0 : 1
	  $iTank   = Abs($event[$i][1])
	  $oldX = $event[$i][2]
	  $oldY = $event[$i][3]
	  $newX = $event[$i][4]
	  $newY = $event[$i][5]

	  $direct = GetDirection($oldX, $oldY, $newX, $newY)
	  _hBmpToPicControl($battleTank[$oldX][$oldY], $hImgTank[$iPlayer][$iTank][$direct])
	  If ($event[$i][0] = 3) Then
		 $hBullet[$i] = GUICtrlCreatePic("", 0, 0, 100, 101)
		 _hBmpToPicControl($hBullet[$i], $hImgBullet[$iPlayer][$direct])

		 Local $curPos[2]
		 $curPos[0] = $battlePos[$oldX][$oldY][0]
		 $curPos[1] = $battlePos[$oldX][$oldY][1]
		 GUICtrlSetPos($hBullet[$i], $curPos[0], $curPos[1], $W_TANK, $H_TANK)
	  EndIf
	  Sleep(100)
   Next

   For $i = 0 To 99 Step 4
	  For $e = 0 To $nevent - 1
		 If ($event[$e][0] = 2) Then
			$x = $event[$e][2]
			$y = $event[$e][3]
			GUICtrlSetPos($battleTank[$x][$y], $sequencePos[$e][$i][0], $sequencePos[$e][$i][1])
		 Else
			GUICtrlSetPos($hBullet[$e], $sequencePos[$e][$i][0], $sequencePos[$e][$i][1])
		 EndIf
	  Next
	  Sleep(40)
   Next

   For $i = 0 To $nevent - 1
	  If $event[$i][0] = 2 Then
		 GUICtrlDelete($battleTank[$event[$i][4]][$event[$i][5]])
		 $battleTank[$event[$i][4]][$event[$i][5]] = $battleTank[$event[$i][2]][$event[$i][3]]
		 $battleTank[$event[$i][2]][$event[$i][3]] = 0
	  ElseIf $event[$i][0] = 3 Then
		 GUICtrlDelete($hBullet[$i])
		 Explosion($event[$i][4], $event[$i][5])
	  EndIf
   Next

EndFunc

Func SetStatusOfTank($n1, $n2, $tank1, $tank2)
   For $i = 1 To $n1
	  If $tank1[$i][0] = 0 Then
		 For $j = 0 To 6
			GUICtrlSetState($hInfo[0][$i - 1][$j], $GUI_HIDE)
		 Next
	  Else
		 GUICtrlSetData($hInfo[0][$i - 1][2], $tank1[$i][0])
	  EndIf
   Next

   For $i = 1 To $n2
	  If $tank2[$i][0] = 0 Then
		 For $j = 0 To 6
			GUICtrlSetState($hInfo[1][$i - 1][$j], $GUI_HIDE)
		 Next
	  Else
		 GUICtrlSetData($hInfo[1][$i - 1][2], $tank2[$i][0])
	  EndIf
   Next
EndFunc

Func LauchBoomLand(Byref $hPID)
   $Command = __getParentDir(@ScriptDir) & "\BoomLand.exe " & $player1 & " " & $player2 & " " & $game & " " & $log & " " & $transmissFile
   $hPID = Run($Command, __getParentDir(@ScriptDir), @SW_HIDE, $STDIN_CHILD + $STDOUT_CHILD)
EndFunc

Func LoadAuctionTurn(ByRef $iTurn, ByRef $Attribute, ByRef $MoneyForTank1, ByRef $MoneyForTank2, ByRef $Money1, Byref $Money2, ByRef $pSuccess)
   StdinWrite($hPID, "s" & @CRLF)
   ; Wait for program solving
   Sleep(200)
   start_timer(1)

   ; Read file trans
   $transmissFiletemp = $transmissFile

   $hFile 		= FileOpen($transmissFileTemp, $FO_READ)

   $exitCode 	= FileReadLine($hFile)

   $iTurn 		= FileReadLine($hFile)

   $temp 		= FileReadLine($hFile)
   $temp			= StringStripWS($temp, 1+2+4)
   $temp 		= StringSplit($temp, " ")
   $Attribute[0]= Int($temp[1])
   $Attribute[1]= Int($temp[2])
   $Attribute[2]= Int($temp[3])
   $Attribute[3]= Int($temp[4])

   $temp 			= FileReadLine($hFile)
   $temp			= StringStripWS($temp, 1+2+4)
   $temp 			= StringSplit($temp, " ")
   $MoneyForTank1	= Int($temp[1])
   $MoneyForTank2	= Int($temp[2])

   $temp 		= FileReadLine($hFile)
   $temp		= StringStripWS($temp, 1+2+4)
   $temp 		= StringSplit($temp, " ")
   $Money1		= Int($temp[1])
   $Money2 		= Int($temp[2])

   $pSuccess 	= Int(FileReadLine($hFile))

   FileClose($hFile)
   Return $exitCode
EndFunc

Func LoadStartEmbattle()
   StdinWrite($hPID, "s" & @CRLF)
   Sleep(1000)
EndFunc

Func LoadEmbattle(ByRef $battle)
   StdinWrite($hPID, "s" & @CRLF)

   Sleep(200)
   start_timer(1)

   ; Read file trans
   $transmissFiletemp = $transmissFile

   $hFile 		= FileOpen($transmissFileTemp, $FO_READ)

   ;$exitCode = Int(FileReadLine($hFile))

   For $i = 1 To 8
	  $temp = FileReadLine($hFile)
	  $temp = StringStripWS($temp, 1 + 2 + 4)
	  $temp = StringSplit($temp, " ")
	  For $j = 1 To 8
		 $battle[$i][$j] = Int($temp[$j])
	  Next
   Next

EndFunc

Func Start_Timer($time)
   $W_TIMER = 75
   $H_TIMER = 75
   $X_TIMER = $W_WINDOW - $W_TIMER - 5
   $Y_TIMER = 5

   $Timer = GUICtrlCreateLabel($time, $X_TIMER, $Y_TIMER, $W_TIMER, $H_TIMER, BitOR($SS_CENTER, $SS_CENTERIMAGE, $WS_BORDER))
   GUICtrlSetBkColor(-1, 0xDDDDDD)
   GUICtrlSetColor(-1, 0x0)
   GUICtrlSetFont(-1, 22, 400, 0, "Arial Bold")

   For $i = 1 To $time
	  GUICtrlSetData($Timer, $time)
	  Sleep(1000)
	  $time -= 1
   Next
   GUICtrlSetData($Timer, 0)
   Sleep(200)
   GUICtrlDelete($Timer)
EndFunc

Func EffectAll($cursor, ByRef $fOverClose)
   __eventOnCover($hClose, $cursor, $C_CLOSE, $X_CLOSE, $Y_CLOSE, $W_CLOSE, $H_CLOSE, 22, True, $fOverClose)
EndFunc

Func LoadStartFight()
   StdinWrite($hPID, "s" & @CRLF)
   ;Sleep(1000)
EndFunc

Func LoadFight(ByRef $iTurn, ByRef $n1, ByRef $n2, ByRef $tank1, ByRef $tank2, ByRef $battle, Byref $event, ByRef $nevent)
   StdinWrite($hPID, "s" & @CRLF)
   start_timer(3)

   ; Read file trans
   $transmissFiletemp = $transmissFile

   $hFile 		= FileOpen($transmissFileTemp, $FO_READ)

   $exitCode = Int(FileReadLine($hFile))
   $iTurn    = Int(FileReadLine($hFile))

   $temp 		= FileReadLine($hFile)
   $temp		= StringStripWS($temp, 1+2+4)
   $temp 		= StringSplit($temp, " ")
   $n1			= Int($temp[1])
   $n2 			= Int($temp[2])

   For $i = 1 To $n1
	  $temp 		= FileReadLine($hFile)
	  $temp			= StringStripWS($temp, 1+2+4)
	  $temp 		= StringSplit($temp, " ")
	  For $j = 0 To 2
		 $tank1[$i][$j] = Int($temp[$j + 1])
	  Next
   Next

   For $i = 1 To $n2
	  $temp 		= FileReadLine($hFile)
	  $temp			= StringStripWS($temp, 1+2+4)
	  $temp 		= StringSplit($temp, " ")
	  For $j = 0 To 2
		 $tank2[$i][$j] = Int($temp[$j + 1])
	  Next
   Next

   For $i = 1 To 8
	  $temp 		= FileReadLine($hFile)
	  $temp			= StringStripWS($temp, 1+2+4)
	  $temp 		= StringSplit($temp, " ")
	  For $j = 1 To 8
		 $battle[$i][$j] = Int($temp[$j])
	  Next
   Next

   $nevent = Int(FileReadLine($hFile))

   For $i = 0 To $nevent - 1
	  $temp 		= FileReadLine($hFile)
	  $temp			= StringStripWS($temp, 1+2+4)
	  $temp 		= StringSplit($temp, " ")
	  For $j = 0 To 5
		 $event[$i][$j] = Int($temp[$j + 1])
	  Next
   Next

   Return $exitCode
EndFunc


Func TankMoving($idTank, $oldX, $oldY, $newX, $newY)

   $iPlayer = $idTank > 0 ? 0 : 1
   $iTank   = Abs($idTank)

   $direct = GetDirection($oldX, $oldY, $newX, $newY)
   _hBmpToPicControl($battleTank[$oldX][$oldY], $hImgTank[$iPlayer][$iTank][$direct])
   Sleep(100)

   $PosSequence = CreatePath($oldX, $oldY, $newX, $newY)

   For $i = 0 To 99 Step 4
	  GUICtrlSetPos($battleTank[$oldX][$oldY], $PosSequence[$i][0], $PosSequence[$i][1])
	  Sleep(40)
   Next

   GUICtrlDelete($battleTank[$newX][$newY])
   $battleTank[$newX][$newY] = $battleTank[$oldX][$oldY]
   $battleTank[$oldX][$oldY] = 0x0

EndFunc

Func TankDisappear($x, $y)
   GUICtrlDelete($battleTank[$x][$y])
   $battleTank[$x][$y] = 0
EndFunc

Func TankDestroy($x, $y)
   $hEx = GUICtrlCreatePic("", 0, 0, 100, 101)
   _hBmpToPicControl($hEx, $hImgDestroy[0])
   GUICtrlSetPos($hEx, $battlePos[$x][$y][0], $battlePos[$x][$y][1], $W_TANK, $H_TANK)
   For $i = 0 To 4
	  _hBmpToPicControl($hEx, $hImgDestroy[$i])
	  Sleep(100)
   Next
   GUICtrlDelete($hEx)
EndFunc

Func TankShooting($idTank, $curX, $curY, $desX, $desY)
   $iPlayer = $idTank > 0 ? 0 : 1
   FlyingBullet($iPlayer, $curX, $curY, $desX, $desY)
   Explosion($desX, $desY)
EndFunc

Func CreatePath($oldX, $oldY, $newX, $newY, ByRef $sPos, $n)
   Local $oldPos[2]
   $oldPos[0] = $battlePos[$oldX][$oldY][0]
   $oldPos[1] = $battlePos[$oldX][$oldY][1]

   Local $newPos[2]
   $newPos[0] = $battlePos[$newX][$newY][0]
   $newPos[1] = $battlePos[$newX][$newY][1]

   For $i = 0 To 99
	  $sPos[$n][$i][0] = $oldPos[0] + $i * ($newPos[0] - $oldPos[0]) / 99
	  $sPos[$n][$i][1] = $oldPos[1] + $i * ($newPos[1] - $oldPos[1]) / 99
   Next
  ; Return $sPos[$n]
EndFunc

Func Explosion($x, $y)
   $hEx = GUICtrlCreatePic("", 0, 0, 100, 101)
   _hBmpToPicControl($hEx, $hImgExplosion[0])
   GUICtrlSetPos($hEx, $battlePos[$x][$y][0], $battlePos[$x][$y][1], $W_TANK, $H_TANK)
   For $i = 0 To 4
	  _hBmpToPicControl($hEx, $hImgExplosion[$i])
	  Sleep(100)
   Next
   GUICtrlDelete($hEx)
EndFunc

Func FlyingBullet($iPlayer, $curX, $curY, $desX, $desY)

   $PosSequence = CreatePath($curX, $curY, $desX, $desY)
   $direct = GetDirection($curX, $curY, $desX, $desY)

   $hBullet = GUICtrlCreatePic("", 0, 0, 100, 101)
   _hBmpToPicControl($hBullet, $hImgBullet[$iPlayer][$direct])

   Local $curPos[2]
   $curPos[0] = $battlePos[$curX][$curY][0]
   $curPos[1] = $battlePos[$curX][$curY][1]
   GUICtrlSetPos($hBullet, $curPos[0], $curPos[1], $W_TANK, $H_TANK)

   For $i = 0 To 99 Step 4
	  GUICtrlSetPos($hBullet, $PosSequence[$i][0], $PosSequence[$i][1])
	  Sleep(40)
   Next

   GUICtrlDelete($hBullet)
EndFunc

Func GetDirection($oldX, $oldY, $newX, $newY)
   $dx = $newX - $oldX
   $dy = $newY - $oldY

   If ($dy > 0 And Abs($dx) <= Abs($dy)) Then
	  Return 1		; right
   ElseIf ($dy < 0 And Abs($dx) <= Abs($dy)) Then
	  Return 3		; left
   ElseIf ($dx > 0 And Abs($dy) <= Abs($dx)) Then
	  Return 2		; down
   Else
	  Return 0		; up
   EndIf
EndFunc