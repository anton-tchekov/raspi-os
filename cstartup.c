extern int __bss_start__;
extern int __bss_end__;

void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags);

void _cstartup(unsigned int r0, unsigned int r1, unsigned int r2)
{
	/* Clear BSS section */
	int *bss = &__bss_start__;
	int *bss_end = &__bss_end__;
	while(bss < bss_end)
	{
		*bss++ = 0;
	}

	/* Start C Kernel */
	kernel_main(r0, r1, r2);
	for(;;) ;
}
