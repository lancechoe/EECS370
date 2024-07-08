	lw	0	1	19 010 000 001 0000000000010011
	lw	0	2	15 010 000 010 0000000000001111
	lw	0	3	16
	lw	0	4	17
	lw	0	5	18
loop	beq	4	5	14
	nor	3	3	6
	nor	4	4	7
	nor	6	7	6
	beq	6	0	1
	add	1	2	1
shift	add	4	4	4
	add	2	2	2
	beq	0	0	-9 1111 1111 1111 0111
end	halt
mcand	.fill	6203
mplier	.fill	1429
one	.fill	1
last	.fill	65536
zero	.fill	0