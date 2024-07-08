	lw	0	1	five
	lw	0	4	SubAdr
Gtext	add	0	3	3
start	jalr	4	7
	beq	0	1	done
	beq	0	0	start
	beq	0	2	Gtext
done	halt
subOne	lw	0	2	neg1
subG	lw	0	4	Glocal
	add	1	2	1
	jalr	7	6
five	.fill	5
	.fill	Gtext
neg1	.fill	-1
SubAdr	.fill	subOne
Glocal	.fill	8
	.fill	Glocal
