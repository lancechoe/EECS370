	lw	0	1	input
	lw	0	4	SubAdr
	jalr	4	7
	halt
sub4n	lw	0	6	pos1
	sw	5	7	Stack
	add	5	6	5
	sw	5	1	Stack
	add	5	6	5
	add	1	1	1
	add	1	1	3
	lw	0	6	neg1
	add	5	6	5
	lw	5	1	Stack
	add	5	6	5
	lw	5	7	Stack
	jalr	7	4
input	.fill	10
pos1	.fill	1
neg1	.fill	-1
SubAdr	.fill	sub4n
Stack	
