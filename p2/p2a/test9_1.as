subOne	lw	0	2	neg1
subG	lw	0	4	Glocal
	add	1	2	1
	jalr	7	6
neg1	.fill	-1
SubAdr	.fill	subOne
Glocal	.fill	8
	.fill	Glocal
