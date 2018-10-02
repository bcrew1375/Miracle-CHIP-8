#include <windows.h>
#include <stdlib.h>
#include <time.h>

unsigned char opcode1, opcode2, opcode3, opcode4;
unsigned short int PC, SP, Index;
unsigned char V[0x10];
unsigned short int stack[0x10];
unsigned char delayTimer, soundTimer;
unsigned char keyStatus[0x10];

extern unsigned char *romBuffer;
extern unsigned char screenData[];

extern CheckSDLEvents();
extern UpdateDisplay();
extern StartTimer();
extern RemoveTimer();

void (*InstructionTable[0x10])();

void Reset();
void GetOpcode();
void Interpret();
void ClearScreen();
void ReturnFromSub();
void JumpToAddress();
void JumpToSub();
void SkipIfRegEqual();
void SkipIfRegNotEqual();
void SkipIfRegEqualReg();
void LoadRegWithConstant();
void AddConstantToReg();
void LoadRegWithReg();
void OrRegWithReg();
void AndRegWithReg();
void XorRegWithReg();
void AddRegToReg();
void AddConstantToReg();
void LoadRegWithReg();
void OrRegWithReg();
void AndRegWithReg();
void XorRegWithReg();
void AddRegToReg();
void SubRegFromReg();
void ShiftRegRight();
void RSubRegFromReg();
void ShiftRegLeft();
void SkipIfRegNotEqualReg();
void LoadIndexWithConstant();
void JumpToAddressPlusReg0();
void CreateRandomNumber();
void DrawSprite();
void SkipIfKeyPressed();
void SkipIfKeyNotPressed();
void GetDelayTimerIntoReg();
void WaitForKeyPress();
void SetDelayTimer();
void SetSoundTimer();
void AddRegToIndex();
void PointIndexToFont();
void StoreBCD();
void StoreRegsAtIndex();
void LoadRegsFromIndex();
void LoadFont();

void Reset()
{
	PC = 0x200;
	SP = 0xF;

	delayTimer = 0;
	soundTimer = 0;

	srand((unsigned)time(NULL));

	LoadFont();

	ClearScreen();

	RemoveTimer();

	StartTimer();
}

void GetOpcode()
{
	opcode1 = (*(romBuffer + PC) & 0xF0) >> 4;
	opcode2 = *(romBuffer + PC) & 0x0F;
	opcode3 = (*(romBuffer + (PC + 1)) & 0xF0) >> 4;
	opcode4 = *(romBuffer + (PC + 1)) & 0x0F;

	PC += 2;
}

int UpdateTimers(unsigned long int interval, void *param)
{
	if (delayTimer > 0)
		delayTimer--;
	if (soundTimer > 0)
		soundTimer--;

	return interval;
}

void Interpret()
{
	for (;;)
	{
		CheckSDLEvents();
		GetOpcode();

		switch (opcode1)
		{
		case 0x0:
		{
			switch (opcode3)
			{
			case 0xE:
			{
				switch (opcode4)
				{
				case 0x0:
				{
					// Clear the screen
					ClearScreen();
				}
				break;
				case 0xE:
				{
					// Return from subroutine
					ReturnFromSub();
				}
				break;
				}
			}
			break;
			}
		}
		break;
		case 0x1:
		{
			JumpToAddress();
		}
		break;
		case 0x2:
		{
			JumpToSub();
		}
		break;
		case 0x3:
		{
			SkipIfRegEqual();
		}
		break;
		case 0x4:
		{
			SkipIfRegNotEqual();
		}
		break;
		case 0x5:
		{
			SkipIfRegEqualReg();
		}
		break;
		case 0x6:
		{
			LoadRegWithConstant();
		}
		break;
		case 0x7:
		{
			AddConstantToReg();
		}
		break;
		case 0x8:
		{
			switch (opcode4)
			{
			case 0x0:
			{
				LoadRegWithReg();
			}
			break;
			case 0x1:
			{
				OrRegWithReg();
			}
			break;
			case 0x2:
			{
				AndRegWithReg();
			}
			break;
			case 0x3:
			{
				XorRegWithReg();
			}
			break;
			case 0x4:
			{
				AddRegToReg();
			}
			break;
			case 0x5:
			{
				SubRegFromReg();
			}
			break;
			case 0x6:
			{
				ShiftRegRight();
			}
			break;
			case 0x7:
			{
				RSubRegFromReg();
			}
			break;
			case 0xE:
			{
				ShiftRegLeft();
			}
			break;
			}
		}
		break;
		case 0x9:
		{
			SkipIfRegNotEqualReg();
		}
		break;
		case 0xA:
		{
			LoadIndexWithConstant();
		}
		break;
		case 0xB:
		{
			JumpToAddressPlusReg0();
		}
		break;
		case 0xC:
		{
			CreateRandomNumber();
		}
		break;
		case 0xD:
		{
			DrawSprite();
			UpdateDisplay();
		}
		break;
		case 0xE:
		{
			switch (opcode3)
			{
			case 0x9:
			{
				SkipIfKeyPressed();
			}
			break;
			case 0xA:
			{
				SkipIfKeyNotPressed();
			}
			break;
			}
		}
		break;
		case 0xF:
		{
			switch (opcode3)
			{
			case 0x0:
			{
				switch (opcode4)
				{
				case 0x7:
				{
					GetDelayTimerIntoReg();
				}
				break;
				case 0xA:
				{
					WaitForKeyPress();
				}
				break;
				}
			}
			break;
			case 0x1:
			{
				switch (opcode4)
				{
				case 0x5:
				{
					SetDelayTimer();
				}
				break;
				case 0x8:
				{
					SetSoundTimer();
				}
				break;
				case 0xE:
				{
					AddRegToIndex();
				}
				break;
				}
			}
			break;
			case 0x2:
			{
				PointIndexToFont();
			}
			break;
			case 0x3:
			{
				StoreBCD();
			}
			break;
			case 0x5:
			{
				StoreRegsAtIndex();
			}
			break;
			case 0x6:
			{
				LoadRegsFromIndex();
			}
			break;
			}
		}
		break;
		}
	}
}

void ClearScreen()
{
	int i, j;

	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 64; i++)
		{
			screenData[j * 64 + i] = 0;
		}
	}
}

void ReturnFromSub()
{
	SP++;
	PC = stack[SP];
}

void JumpToAddress()
{
	PC = (opcode2 << 8) + (opcode3 << 4) + opcode4;
}

void JumpToSub()
{
	stack[SP] = PC;
	SP--;

	PC = (opcode2 << 8) + (opcode3 << 4) + opcode4;
}

void SkipIfRegEqual()
{
	if (V[opcode2] == ((opcode3 << 4) + opcode4))
		PC += 2;
}

void SkipIfRegNotEqual()
{
	if (V[opcode2] != ((opcode3 << 4) + opcode4))
		PC += 2;
}

void SkipIfRegEqualReg()
{
	if (V[opcode2] == V[opcode3])
		PC += 2;
}

void LoadRegWithConstant()
{
	V[opcode2] = (opcode3 << 4) + opcode4;
}

void AddConstantToReg()
{
	V[opcode2] += (opcode3 << 4) + opcode4;
}

void LoadRegWithReg()
{
	V[opcode2] = V[opcode3];
}

void OrRegWithReg()
{
	V[opcode2] |= V[opcode3];
}

void AndRegWithReg()
{
	V[opcode2] &= V[opcode3];
}

void XorRegWithReg()
{
	V[opcode2] ^= V[opcode3];
}

void AddRegToReg()
{
	if(V[opcode2] && V[opcode3])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[opcode2] += V[opcode3];
}

// Borrow check needs fixed.
void SubRegFromReg()
{
	if (V[opcode2] >= V[opcode3])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[opcode2] -= V[opcode3];
}

void ShiftRegRight()
{
	if (V[opcode2] && 128)
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[opcode2] >>= 1;
}

// Borrow check needs fixed.
void RSubRegFromReg()
{
	if (V[opcode3] >= V[opcode2])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[opcode2] = V[opcode3] - V[opcode2];
}

void ShiftRegLeft()
{
	if (V[opcode2] && 1)
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[opcode2] <<= 1;
}

void SkipIfRegNotEqualReg()
{
	if (V[opcode2] != V[opcode3])
		PC += 2;
}

void LoadIndexWithConstant()
{
	Index = (opcode2 << 8) + (opcode3 << 4) + opcode4;
}

void JumpToAddressPlusReg0()
{
	PC = V[0x0] + (opcode2 << 8) + (opcode3 << 4) + opcode4;
}

void CreateRandomNumber()
{
	V[opcode2] = (rand() & ((opcode3 << 4) + opcode4));
}

void DrawSprite()
{
	int j;
	int i;
	int plotCoordinates;
	unsigned char data;

	// If no collision, default VF to 0.
	V[0xF] = 0;
	for(j = 0; j < opcode4; j++)
	{
		data = romBuffer[Index + j];

		for (i = 0; i < 8; i++)
		{
			plotCoordinates = (((V[opcode3] + j) % 32) * 64) + ((V[opcode2] + i) % 64);

			if ((data & (0x80 >> i)) != 0)
			{
				if (screenData[plotCoordinates] == 1)
					V[0xF] = 1;

				screenData[plotCoordinates] ^= 1;
			}
		}
	}
}

void SkipIfKeyPressed()
{
	if (keyStatus[V[opcode2]] == 1)
		PC += 2;
}

void SkipIfKeyNotPressed()
{
	if (keyStatus[V[opcode2]] == 0)
		PC += 2;
}

void GetDelayTimerIntoReg()
{
	V[opcode2] = delayTimer;
}

void WaitForKeyPress()
{
	int i;
	int keyFound = 0;

	while (keyFound == 0)
	{
		CheckSDLEvents();
		for (i = 0; i < 16; i++)
		{
			if (keyStatus[i] == 1)
			{
				V[opcode2] = i;
				keyFound = 1;
			}
		}
	}
}

void SetDelayTimer()
{
	delayTimer = V[opcode2];
}

void SetSoundTimer()
{
	soundTimer = V[opcode2];
}

void AddRegToIndex()
{
	Index += V[opcode2];
}

void PointIndexToFont()
{
	Index = 0x1000 + (V[opcode2] * 0x5);
}

void StoreBCD()
{
	int num = (int)V[opcode2];
	int i;

	for (i = 3; i > 0; i--)
	{
		romBuffer[Index + (i - 1)] = num % 10;

		num /= 10;
	}
}

void StoreRegsAtIndex()
{
	int i;

	for (i = 0; i <= opcode2; i++)
	{
		romBuffer[Index + i] = V[i];
	}
}

void LoadRegsFromIndex()
{
	int i;

	for (i = 0; i <= opcode2; i++)
	{
		V[i] = romBuffer[Index + i];
	}
}

void LoadFont()
{
	romBuffer[0x1000] = 96;
	romBuffer[0x1001] = 144;
	romBuffer[0x1002] = 144;
	romBuffer[0x1003] = 144;
	romBuffer[0x1004] = 96;

	romBuffer[0x1005] = 96;
	romBuffer[0x1006] = 224;
	romBuffer[0x1007] = 96;
	romBuffer[0x1008] = 96;
	romBuffer[0x1009] = 240;

	romBuffer[0x100A] = 96;
	romBuffer[0x100B] = 144;
	romBuffer[0x100C] = 32;
	romBuffer[0x100D] = 64;
	romBuffer[0x100E] = 240;
	
	romBuffer[0x100F] = 224;
	romBuffer[0x1010] = 16;
	romBuffer[0x1011] = 96;
	romBuffer[0x1012] = 16;
	romBuffer[0x1013] = 224;

	romBuffer[0x1014] = 160;
	romBuffer[0x1015] = 160;
	romBuffer[0x1016] = 240;
	romBuffer[0x1017] = 32;
	romBuffer[0x1018] = 32;

	romBuffer[0x1019] = 240;
	romBuffer[0x101A] = 128;
	romBuffer[0x101B] = 240;
	romBuffer[0x101C] = 16;
	romBuffer[0x101D] = 240;
	
	romBuffer[0x101E] = 240;
	romBuffer[0x101F] = 128;
	romBuffer[0x1020] = 240;
	romBuffer[0x1021] = 144;
	romBuffer[0x1022] = 240;

	romBuffer[0x1023] = 240;
	romBuffer[0x1024] = 16;
	romBuffer[0x1025] = 32;
	romBuffer[0x1026] = 64;
	romBuffer[0x1027] = 128;

	romBuffer[0x1028] = 96;
	romBuffer[0x1029] = 144;
	romBuffer[0x102A] = 96;
	romBuffer[0x102B] = 144;
	romBuffer[0x102C] = 96;

	romBuffer[0x102D] = 240;
	romBuffer[0x102E] = 144;
	romBuffer[0x102F] = 240;
	romBuffer[0x1030] = 16;
	romBuffer[0x1031] = 240;

	romBuffer[0x1032] = 96;
	romBuffer[0x1033] = 144;
	romBuffer[0x1034] = 240;
	romBuffer[0x1035] = 144;
	romBuffer[0x1036] = 144;

	romBuffer[0x1037] = 224;
	romBuffer[0x1038] = 144;
	romBuffer[0x1039] = 224;
	romBuffer[0x103A] = 144;
	romBuffer[0x103B] = 224;

	romBuffer[0x103C] = 112;
	romBuffer[0x103D] = 128;
	romBuffer[0x103E] = 128;
	romBuffer[0x103F] = 128;
	romBuffer[0x1040] = 112;

	romBuffer[0x1041] = 224;
	romBuffer[0x1042] = 144;
	romBuffer[0x1043] = 144;
	romBuffer[0x1044] = 144;
	romBuffer[0x1045] = 224;

	romBuffer[0x1046] = 240;
	romBuffer[0x1047] = 128;
	romBuffer[0x1048] = 224;
	romBuffer[0x1049] = 128;
	romBuffer[0x104A] = 240;

	romBuffer[0x104B] = 240;
	romBuffer[0x104C] = 128;
	romBuffer[0x104D] = 224;
	romBuffer[0x104E] = 128;
	romBuffer[0x104F] = 128;
}
