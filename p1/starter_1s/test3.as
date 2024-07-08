	add	0	0	0
	beq	1	2	done
	lw	0	1	one
	lw	0	2	one
	jalr	0	3
one	.fill	1
done	halt
