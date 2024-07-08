	lw	0	1	five
	lw	0	4	SubAdr
Gtext	add	0	3	3
start	jalr	4	7
	beq	0	1	done
	beq	0	0	start
	beq	0	2	Gtext
done	halt
five	.fill	5
	.fill	Gtext
