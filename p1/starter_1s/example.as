	lw	0	2	16
	lw	0	3	17
	lw	0	4	20
	lw	0	7	19
	nor	3	3	5	// 1429 nor 1429 -> r5
	nor	4	4	6	// 1 nor 1 -> r6
	nor	5	6	5	// 1429 nor 과 1 nor 을 nor ?
	beq	5	0	1
	add	1	2	1
	add	2	2	2	// left shift
	lw	0	6	18





	.fill	6203
	.fill	1429

	.fill	15
	.fill	1
