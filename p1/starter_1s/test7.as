	lw	0	1	one
	lw	0	2	three
	sw	0	2	one
	lw	0	3	one
	noop
	add	1	3	4
	halt
one	.fill	1
three	.fill	3
