void test_strings(void) {
	char * cs, ct;
	cs = "Hello, world!";

	/* testing strcmp */

	if (!strcmp(cs, "Hello, world!"))
		printf("String is \'Hello, world!\'\n");
	else
		printf("String is not \'Hello, world!\'\n");

	cs = "Hello, ";
	ct = "world!\n"; 
	
	/* testing strcat */

	printf(strcat(cs,ct));

	/* testing strcpy */

	strcpy(cs, "Hello, world!\n");
	printf(cs);

	/* testing strlen & printf */

	cs = "Hello, world!\n";
	printf("The length of the string is: %d\n", strlen(cs));

	/* testing memset */

	cs = 0x0;
	printf("cs before memset: %s\n", cs);

	cs = memset(cs, 'H', 14);
	printf("cs after memset: %s\n", cs);
}
