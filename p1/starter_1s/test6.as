	lw	0	1	big
	lw	0	2	big
done	beq	1	2	done
	.fill	5
big	.fill	34566
	.fill	-3
	.fill	0
done	halt
