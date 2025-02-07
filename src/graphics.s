.segment "CODE"

.export _setTextColor
_setTextColor:
	sta $0286	; text color
	rts

.export _setFrameColor
_setFrameColor:
	sta $D020	; frame color
	rts

.export _setBackgroundColor
_setBackgroundColor:
	sta $D021	; background color
	rts

.export _clearScreen
_clearScreen:
	lda #32		; SPACE
	ldx #0
	clearScreenLoop:

		; Screen is divided into 4 parts of 256 bytes,
		; so we update a byte in every part.
		sta $0400, x
		sta $0500, x
		sta $0600, x
		sta $0700, x

		inx
		bne clearScreenLoop
	
	rts
