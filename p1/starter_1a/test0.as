	lw	0	1	data1	$1= mem[data1]
	lw	0	2	data2
	lw	0	3	data3
	lw	0	4	data4
	add	1	1	1
	nor	2	2	2
	sw	0	3	1
	halt
data1	.fill	12345
data2	.fill	1
data3	.fill	2
data4	.fill	3
