subOne	lw	0	2	neg1
Test	beq	0	4	subOne
	add	1	2	1
	jalr	7	6
neg1	.fill	-1
SubAdr	.fill	subOne
Seven	.fill	7
