	lw	0	1	one
	lw	0	5	one
	lw	0	2	two
	beq	1	5	1
	add	1	2	3
	add	3	1	4
	beq	4	1	1
	add	1	5	6
	halt
one	.fill	1
two	.fill	2
