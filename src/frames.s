.export _setupRasterInterrupt
.import _frames

.segment "CODE"

_setupRasterInterrupt:
	sei                  ; disable interrupts for now, so we're not interrupted while setting up the interrupts
	lda #%01111111       ; switch off interrupt signals from CIA-1
	sta $DC0D

	and $D011            ; clear most significant bit of VIC's raster register
	sta $D011

	sta $DC0D            ; acknowledge pending interrupts from CIA-1
	sta $DD0D            ; acknowledge pending interrupts from CIA-2

	lda #210             ; set rasterline
	sta $D012

	lda #<irq_handler    ; set interrupt vectors, pointing to interrupt service routine below
	sta $0314
	lda #>irq_handler
	sta $0315

	lda #%00000001       ; enable raster interrupt signals from VIC
	sta $D01A

	cli                  ; enabled interrupts again
	rts

irq_handler:
	inc _frames          ; Increment frame counter
	asl $D019            ; acknowledge the interrupt by clearing the VIC's interrupt flag
	jmp $EA31            ; jump into KERNAL's standard interrupt service routine to handle keyboard scan, cursor display etc.
