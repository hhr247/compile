.data
a:	.space	4
c:	.space	4
b:	.space	4
i:	.space	4
d:	.space	4
e:	.space	4
.text
main:
	ori	$s0,$0,1
	lw	$t0,c
	add	$t0,$s0,$0
	ori	$s0,$0,0
	lw	$t1,i
	add	$t1,$s0,$0
	ori	$s0,$0,1
	lw	$t2,d
	add	$t2,$s0,$0
	sw	$t0,c
	sw	$t1,i
	sw	$t2,d
WH0:
	lw	$t0,i
	ori	$s1,$0,10
	blt	$t0,$s1,b00
	ori	$t1,$0,0
	b	b01
b00:
	ori	$t1,$0,1
b01:
	beqz	$t1, WE0
	lw	$t2,d
	add	$t0,$t0,$t2
	sw	$t0,i
	sw	$t2,d
	b	WH0
WE0:
	lw	$t0,i
	ori	$s1,$0,2
	mul	$t1,$t0,$s1
	lw	$t2,a
	add	$t2,$t1,$0
	lw	$t3,c
	add	$t4,$t2,$t3
	lw	$t5,b
	add	$t5,$t4,$0
	ori	$s0,$0,0
	add	$t3,$s0,$0
	sw	$t0,i
	addiu	$sp,$sp,-4
	sw	$t1,4($sp)
	sw	$t2,a
	sw	$t3,c
	sw	$t5,b
WH1:
	lw	$t0,a
	lw	$t1,b
	ble	$t0,$t1,b10
	ori	$t2,$0,0
	b	b11
b10:
	ori	$t2,$0,1
b11:
	ori	$s1,$0,40
	ble	$t0,$s1,b20
	ori	$t3,$0,0
	b	b21
b20:
	ori	$t3,$0,1
b21:
	beqz	$t2,b30
	beqz	$t3,b30
	ori	$t4,$0,1
	b	b31
b30:
	ori	$t4,$0,0
b31:
	beqz	$t4, WE1
	ori	$s0,$0,2
	mul	$t0,$t0,$s0
	ori	$s1,$0,2
	mul	$t5,$t1,$s1
	lw	$t6,c
	sub	$t7,$t5,$t6
	add	$t1,$t7,$0
	ori	$s1,$0,10
	blt	$t6,$s1,b40
	ori	$t2,$0,0
	b	b41
b40:
	ori	$t2,$0,1
b41:
	sw	$t0,a
	sw	$t1,b
	sw	$t6,c
	beqz	$t2, EL0
	lw	$t0,c
	addi	$t0,$t0,1
	sw	$t0,c
	b	IE0
EL0:
IE0:
	b	WH1
WE1:
	lw	$t0,b
	ori	$s1,$0,2
	blt	$t0,$s1,b50
	ori	$t1,$0,0
	b	b51
b50:
	ori	$t1,$0,1
b51:
	lw	$t2,a
	ori	$s1,$0,30
	bgt	$t2,$s1,b60
	ori	$t3,$0,0
	b	b61
b60:
	ori	$t3,$0,1
b61:
	bnez	$t1,b70
	bnez	$t3,b70
	ori	$t4,$0,0
	b	b71
b70:
	ori	$t4,$0,1
b71:
	sw	$t0,b
	sw	$t2,a
	beqz	$t4, EL1
	ori	$s0,$0,4
	lw	$t0,b
	add	$t0,$s0,$0
	sw	$t0,b
	b	IE1
EL1:
	lw	$t0,b
	ori	$s1,$0,4
	ble	$t0,$s1,b80
	ori	$t1,$0,0
	b	b81
b80:
	ori	$t1,$0,1
b81:
	sw	$t0,b
	beqz	$t1, EL2
	lw	$t0,c
	lw	$t1,b
	xor	$t2,$t0,$t1
	add	$t1,$t2,$0
	lw	$t3,a
	bgt	$t3,$t1,b90
	ori	$t4,$0,0
	b	b91
b90:
	ori	$t4,$0,1
b91:
	sw	$t0,c
	sw	$t1,b
	sw	$t3,a
	beqz	$t4, EL3
	lw	$t0,a
	lw	$t1,b
	or	$t2,$t0,$t1
	add	$t1,$t2,$0
	sw	$t0,a
	sw	$t1,b
	b	IE3
EL3:
	lw	$t0,b
	lw	$t1,a
	add	$t1,$t0,$0
	sw	$t0,b
	sw	$t1,a
IE3:
	b	IE2
EL2:
IE2:
IE1:
	ori	$s0,$0,4
	lw	$t0,b
	sllv	$t0,$t0,$s0
	lw	$t1,a
	ori	$s1,$0,1
	mul	$t2,$t1,$s1
	ori	$s1,$0,2
	mul	$t3,$t0,$s1
	add	$t4,$t2,$t3
	lw	$t5,c
	ori	$s1,$0,3
	mul	$t6,$t5,$s1
	add	$t7,$t4,$t6
	ori	$s1,$0,20
	sw	$t0,b
	add	$t0,$t7,$s1
	sw	$t1,a
	lw	$t1,4($sp)
	add	$t2,$t0,$t1
	lw	$t3,e
	add	$t3,$t2,$0
	sw	$t3,e
	sw	$t5,c
