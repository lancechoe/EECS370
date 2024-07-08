	lw	0	1	data1
	add	1	1	2
	lw	0	3	data2
	nor	3	3	4
	lw	0	5	data3
	sw	0	5	data4
	lw	0	1	data5
	beq	0	1	done
	lw	0	0	data4
	lw	0	1	data4
	lw	0	2	data4
done	halt
data1	.fill	1
data2	.fill	-3
data3	.fill	-5
data4	.fill	100
data5	.fill	-100
