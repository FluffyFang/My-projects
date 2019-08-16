$Window = WinGetPos("[ApplicationName]")
Global $FieldLoc[2] = [0, 0] ;Manually set upper left location of board relative to window
Global $BoardSizeX = 8
Global $BoardSizeY = 8
Global $boardColors[$BoardSizeX][$BoardSizeY]
Global $NumberOfColors = 7 ;Max number of colors
Global $TileSize = 99 ;Assumes tiles are squares
Global $DefaultProximity = 50

Func RMC($mouseX, $mouseY) ;relative mouseclick
   MouseClick("primary", $mouseX + $Window[0], $mouseY + $Window[1], 1, 3)
EndFunc

Func BoardSelect($x, $y) ;Select a piece
   RMC($FieldLoc[0] + ($x + .5) * $TileSize, $FieldLoc[1] + ($y + .5) * $TileSize)
EndFunc

Func Swap ($x1, $y1, $x2, $y2) ;Swap 2 pieces
   BoardSelect($x1, $y1)
   Sleep(200)
   BoardSelect($x1 + $x2, $y1 + $y2)
EndFunc

Func HexToRGB($val)
   Local $color[3] = [BitAND(BitShift($val, 16), 255), BitAND(BitShift($val, 8), 255), BitAND($val, 255)]
   Return $color
EndFunc

Func Proximity ($color1, $color2, $distance = $DefaultProximity) ;Compare two colors stored in [$R,$G,$B] format
   Return $distance > Sqrt(($color1[0] - $color2[0]) ^ 2 + ($color1[1] - $color2[1]) ^ 2 + ($color1[2] - $color2[2]) ^ 2)
EndFunc

Func CheckForPixel($x, $y, $color1, $distance = $DefaultProximity) ;Look for a color at coordinate. Leniancy of match increases with distance.
   $val = PixelGetColor($Window[0] + $x, $Window[1] + $y)
   Local $color2[3] = [BitAND(BitShift($val, 16), 255), BitAND(BitShift($val, 8), 255), BitAND($val, 255)]
   Return Proximity($color1, $color2, $distance)
EndFunc

Func PrintBoard() ;Print the contents of boardColors to console. Used for troubleshooting
   For $i = 0 To 7
	  For $j = 0 To 7
		ConsoleWrite($boardColors[$j][$i] & " ")
	  Next
	  ConsoleWrite(@CRLF)
   Next
EndFunc

Func AnalyzeBoard() ;Assign each color a number and populate boardColors with them
   Local $pixelColors[$BoardSizeX][$BoardSizeY]

   ;Get pixel values from the board
   For $i = 1 To BoardSizeX
	  For $j = 1 To BoardSizeY
		 $val = PixelGetColor($Window[0] + $FieldLoc[0] + ($i - .5) * $TileSize, $Window[1] + $FieldLoc[1] + ($j - .5) * $TileSize)
		 $pixelColors[$i][$j] = HexToRGB($val)
	  Next
   Next

   $movingBoard = True
   While $movingBoard ;Wait for field to stop moving. Skip this step if tiles are animated (and thus never stop moving)
	  $movingBoard = False
	  For $i = 1 To BoardSizeX
		 For $j = 1 To BoardSizeY
			$val = PixelGetColor($Window[0] + $FieldLoc[0] + ($i - .5) * $TileSize, $Window[1] + $FieldLoc[1] + ($j - .5) * $TileSize)
			Local $temp[3] = HexToRGB($val)

			For $k = 0 To 2
			   If Not $temp[$k] = ($pixelColors[$i][$j])[$k] Then
				  $pixelColors[$i][$j] = $temp
				  $movingBoard = True
				  ExitLoop
			   EndIf
			Next
		 Next
	  Next
   WEnd

   $colors[$NumberOfColors]
   $foundColors = 0
   For $i = 0 To BoardSizeX - 1
	  For $j = 0 To BoardSizeY - 1
		 $newColor = True
		 For $k in $colors
			If Proximity($pixelColors[$i][$j], $k) Then
			   $boardColors[$i][$j] = $foundColors
			   $newColor = False
			   ExitLoop
			EndIf
		 Next

		 If $newColor Then
			$colors[$foundColors] = $pixelColors[$i][$j]
			foundColors += 1
		 EndIf
	  Next
   Next

   ;PrintBoard()
EndFunc

Func FindMatches($match4 = True) ;Brute force checking to see if a potential swap creates a match. If parameter is true, makes the first 4+ match found.

   For $i = 0 To BoardSizeX - 1
	  For $j = 0 To BoardSizeY - 1
		 $matchesFound = 0
		 ;Easy reference variables to make things less messy
		 Local $l2 = -1, $l1 = -1, $r2 = -1, $r1 = -1, $u2 = -1, $u1 = -1, $d2 = -1, $d1 = -1
		 If $i >= 2 Then $l2 = $boardColors[$i-2][$j]
		 If $i >= 1 Then $l1 = $boardColors[$i-1][$j]
		 If $i <= 5 Then $r2 = $boardColors[$i+2][$j]
		 If $i <= 6 Then $r1 = $boardColors[$i+1][$j]
		 If $j >= 2 Then $u2 = $boardColors[$i][$j-2]
		 If $j >= 1 Then $u1 = $boardColors[$i][$j-1]
		 If $j <= 5 Then $d2 = $boardColors[$i][$j+2]
		 If $j <= 6 Then $d1 = $boardColors[$i][$j+1]

		 ;Swap from left
		 If $i >= 1 Then
			$currentColor = $l1
			If $r2 = $currentColor And $r1 = $currentColor Then $matchesFound += 1 ;matched right
			If $u2 = $currentColor And $u1 = $currentColor Then $matchesFound += 1 ;matched up
			If $u1 = $currentColor And $d1 = $currentColor Then $matchesFound += 1 ;matched center
			If $d1 = $currentColor And $d2 = $currentColor Then $matchesFound += 1 ;matched down
			If $matchesFound >= 2 And $match4 Then
			   GemSwap ($i, $j, -1, 0)
			   Return True
			ElseIf $matchesFound = 1 And $currentColor <= $winningColor Then
			   Local $tempAry[4] = [$i, $j, -1, 0]
			   $availableMatch = $tempAry
			EndIf
			$matchesFound = 0
		 EndIf

		 ;Swap from right
		 If $i <= 6 Then
			$currentColor = $r1
			If $l2 = $currentColor And $l1 = $currentColor Then $matchesFound += 1 ;matched left
			If $u2 = $currentColor And $u1 = $currentColor Then $matchesFound += 1 ;matched up
			If $u1 = $currentColor And $d1 = $currentColor Then $matchesFound += 1 ;matched center
			If $d1 = $currentColor And $d2 = $currentColor Then $matchesFound += 1 ;matched down
			If $matchesFound >= 2 And $match4 Then
			   GemSwap ($i, $j, 1, 0)
			   Return True
			ElseIf $matchesFound = 1 And $currentColor <= $winningColor Then
			   Local $tempAry[4] = [$i, $j, 1, 0]
			   $availableMatch = $tempAry
			EndIf
			$matchesFound = 0
		 EndIf

		 ;Swap from top
		 If $j >= 1 Then
			$currentColor = $u1
			If $d2 = $currentColor And $d1 = $currentColor Then $matchesFound += 1 ;matched down
			If $r2 = $currentColor And $r1 = $currentColor Then $matchesFound += 1 ;matched right
			If $r1 = $currentColor And $l1 = $currentColor Then $matchesFound += 1 ;matched center
			If $l1 = $currentColor And $l2 = $currentColor Then $matchesFound += 1 ;matched left
			If $matchesFound >= 2 And $match4 Then
			   GemSwap ($i, $j, 0, -1)
			   Return True
			ElseIf $matchesFound = 1 And $currentColor <= $winningColor Then
			   Local $tempAry[4] = [$i, $j, 0, -1]
			   $availableMatch = $tempAry
			EndIf
			$matchesFound = 0
		 EndIf

		 ;Swap from bottom
		 If $j <= 6 Then
			$currentColor = $d1
			If $u2 = $currentColor And $u1 = $currentColor Then $matchesFound += 1 ;matched up
			If $r2 = $currentColor And $r1 = $currentColor Then $matchesFound += 1 ;matched right
			If $r1 = $currentColor And $l1 = $currentColor Then $matchesFound += 1 ;matched center
			If $l1 = $currentColor And $l2 = $currentColor Then $matchesFound += 1 ;matched left
			If $matchesFound >= 2 And $match4 Then
			   GemSwap ($i, $j, 0, 1)
			   Return True
			ElseIf $matchesFound = 1 And $currentColor <= $winningColor Then
			   Local $tempAry[4] = [$i, $j, 0, 1]
			   $availableMatch = $tempAry
			EndIf
			$matchesFound = 0
		 EndIf
	  Next
   Next
   Return False
EndFunc

AnalyzeBoard()
GemSwap($availableMatch[0], $availableMatch[1], $availableMatch[2], $availableMatch[3])