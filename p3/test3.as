	lw	0	1	data1	$1= mem[data1]
	add	1	1	2
	nor	1	2	3
	lw	0	2	data1
	add	1	1	3
	sw	1	3	2
	add	3	3	3
	lw	2	3	data1
	add	1	2	3
	halt
data1	.fill	12345
