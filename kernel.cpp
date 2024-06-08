void printf(char* str) {
	unsigned short* videoMemBaseAddr = (unsigned short*)0xb8000;

	for(int i = 0; str[i] != '\0'; ++i) {
		videoMemBaseAddr[i] = (videoMemBaseAddr[i] & 0xFF00) | str[i];
	}
}

extern "C" void kernelMain(void* multibootStruct, unsigned int magicNumber) {
	printf("Hello World! This is Aman, your creator.");

	/*
	* We dont want the kernel to ever stop.
	* So, let's dive into an infinite loop.
	*/
	while(1);
}