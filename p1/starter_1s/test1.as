	lw	0	1	three
start	lw	0	2	big
	lw	0	6	big
	lw	0	7	big
	sw	0	1	three
	sw	0	7	big
	nor	7	0	0
	nor	0	0	1
	nor	5	1	2
	nor	1	4	3
	nor	1	5	4
	halt
three	.fill	-3
big	.fill	34567
