	lw	0	1	zero
	lw	0	2	mcand
	lw	0	3	mplier
	lw	0	4	one
	lw	0	5	last
loop	beq	4	5	end
	nor	3	3	6
	nor	4	4	7
	nor	6	7	6
	beq	6	0	shift
	add	1	2	1
shift	add	4	4	4
	add	2	2	2
	beq	0	0	loop
end	halt
mcand	.fill	6203
mplier	.fill	1429
one	.fill	1
last	.fill	65536
zero	.fill	0
