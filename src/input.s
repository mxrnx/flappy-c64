.segment "CODE"

; Returns 1 if space or the shoot button of joy 1 is down, 0 otherwise.
.export _isSpaceOrShootDown
_isSpaceOrShootDown:
	ldx #0				; Set higher byte of return value to avoid returning garbage

	lda #$7F  ;%01111111
	sta $DC00
	lda $DC01 
	and #$10  ;mask %00010000	; Matches both shoot on the first joystick and space
	bne noPress 

        lda #1
	rts
noPress:
        lda #0
	rts
