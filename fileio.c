#include <stdio.h>
#include <windows.h>

FILE *fileHandle;
unsigned long bytesRead;

extern unsigned char *romBuffer;

int LoadRomFile(char filename[])
{	
	// Try to open the file in Read-Only binary mode, if it fails,
	// return with an error.
	if((fileHandle = fopen(filename, "rb")) == NULL)
		return -1;
	
	// Clear the ROM buffer to 0.
	memset(romBuffer, 0, 4096);

	// Read up to the max size of a Chip8 ROM.
	bytesRead = fread(romBuffer + 0x200, 1, 4096, fileHandle);

	// Close the file.
	fclose(fileHandle);

	// See if there were any problems reading the file and make sure
	// the file size is not 0.
	if ((!bytesRead) || (bytesRead == 0))
		return -2;
	
	// Return with no errors.
	return 0;
}

