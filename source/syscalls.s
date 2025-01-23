
//most (if not all) of this is taken straight from the WUMS loader source all credit goes to the devs for that
//we reuse the kernel copy data set up by the wums loader 


.global SC_KernelCopyData
SC_KernelCopyData:
	li %r0, 0x2500
	sc
blr

