	lw	0	1	five
	lw	0	4	SubAdr
start	jalr	4	7
	beq	0	1	done
	beq	0	0	start
done	halt
subOne	lw	0	2	neg1
Test	beq	0	4	subOne
	add	1	2	1
	jalr	7	6
five	.fill	5
	.fill	Seven
	.fill	Test
neg1	.fill	-1
SubAdr	.fill	subOne
Seven	.fill	7
