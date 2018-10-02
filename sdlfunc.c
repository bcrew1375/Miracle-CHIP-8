#include <sdl.h>
#include <windows.h>

extern unsigned char keyStatus[];

extern UpdateTimers(Uint32 interval, void *param);

extern HWND hwnd;

SDL_Surface *screen;
SDL_Window *sdlWindow;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Color colors[2];

const short int SCREEN_WIDTH = 640;
const short int SCREEN_HEIGHT = 320;
const short int SCREEN_BPP = 8;

unsigned char color0;
unsigned char color1;

unsigned char screenData[64 * 32];

unsigned char *screenPtr;

SDL_Event event;
SDL_TimerID timerID;

int InitializeSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
	{
		atexit(SDL_Quit);
		return -1;
	}

	return 0;
}

int OpenSDLWindow()
{
	sdlWindow = SDL_CreateWindowFrom(hwnd);
	if (sdlWindow == NULL)
		return 0;
	renderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL)
		return 0;
	SDL_RenderSetLogicalSize(renderer, 64, 32);
	
	screen = SDL_CreateRGBSurfaceFrom(&screenData, 64, 32, 8, 64, 0, 0, 0, 0);

	//screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);
	//if (screen == NULL)
	//	return -1;
	
	//screenPtr = (unsigned char *)screen->pixels;

	colors[0].r = 0x00;
	colors[0].g = 0x00;
	colors[0].b = 0xFF;

	colors[1].r = 0x00;
	colors[1].g = 0xFF;
	colors[1].b = 0xFF;

	SDL_SetPaletteColors(screen->format->palette, colors, 0, 2);
	SDL_SetWindowSize(sdlWindow, SCREEN_WIDTH, SCREEN_HEIGHT);

	//	color0 = SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF);
	//	color1 = SDL_MapRGB(screen->format, 0x00, 0xFF, 0xFF);

	return 0;
}

void CloseSDL()
{
	SDL_Quit();
}

/*int PlotPixel(int x, int y, int color)
{
	if (SDL_MUSTLOCK(screen) == -1)
		if (SDL_LockSurface(screen) == -1)
			return -1;

	*(screenPtr + (y * screen->pitch + x)) = color;
	
	if (SDL_MUSTLOCK(screen) == -1)
		SDL_UnlockSurface(screen);

	return 0;
}*/

int UpdateDisplay()
{
/*	int i;
	int j;

	SDL_Rect plotRectangle;

	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 64; i++)
		{
			plotRectangle.x = i * 9;
			plotRectangle.y = j * 9;
			plotRectangle.w = 9;
			plotRectangle.h = 9;

			switch(screenData[(j * 64) + i])
			{
			case 0:
				{
					SDL_FillRect(screen, &plotRectangle, color0);
				}
			break;
			case 1:
				{
					SDL_FillRect(screen, &plotRectangle, color1);
				}
			break;
			}
		}
	}*/
	SDL_Delay(10);

	SDL_memcpy(screen->pixels, screenData, 0x800);
	texture = SDL_CreateTextureFromSurface(renderer, screen);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, 0, 0);
	SDL_DestroyTexture(texture);
	SDL_RenderPresent(renderer);
	return 0;
}

int CheckSDLEvents()
{
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_KEYDOWN:
			{
				switch(event.key.keysym.sym)
				{
				case SDLK_7:
					{
						keyStatus[0x1] = 1;
					}
				break;
				case SDLK_8:
					{
						keyStatus[0x2] = 1;
					}
				break;
				case SDLK_9:
					{
						keyStatus[0x3] = 1;
					}
				break;
				case SDLK_0:
					{
						keyStatus[0xC] = 1;
					}
				break;
				case SDLK_u:
					{
						keyStatus[0x4] = 1;
					}
				break;
				case SDLK_i:
					{
						keyStatus[0x5] = 1;
					}
				break;
				case SDLK_o:
					{
						keyStatus[0x6] = 1;
					}
				break;
				case SDLK_p:
					{
						keyStatus[0xD] = 1;
					}
				break;
				case SDLK_j:
					{
						keyStatus[0x7] = 1;
					}
				break;
				case SDLK_k:
					{
						keyStatus[0x8] = 1;
					}
				break;
				case SDLK_l:
					{
						keyStatus[0x9] = 1;
					}
				break;
				case SDLK_SEMICOLON:
					{
						keyStatus[0xE] = 1;
					}
				break;
				case SDLK_m:
					{
						keyStatus[0xA] = 1;
					}
				break;
				case SDLK_COMMA:
					{
						keyStatus[0x0] = 1;
					}
				break;
				case SDLK_PERIOD:
					{
						keyStatus[0xB] = 1;
					}
				break;
				case SDLK_SLASH:
					{
						keyStatus[0xF] = 1;
					}
				break;
				}
			}
			break;
		case SDL_KEYUP:
			{
				switch(event.key.keysym.sym)
				{
				case SDLK_7:
					{
						keyStatus[0x1] = 0;
					}
				break;
				case SDLK_8:
					{
						keyStatus[0x2] = 0;
					}
				break;
				case SDLK_9:
					{
						keyStatus[0x3] = 0;
					}
				break;
				case SDLK_0:
					{
						keyStatus[0xC] = 0;
					}
				break;
				case SDLK_u:
					{
						keyStatus[0x4] = 0;
					}
				break;
				case SDLK_i:
					{
						keyStatus[0x5] = 0;
					}
				break;
				case SDLK_o:
					{
						keyStatus[0x6] = 0;
					}
				break;
				case SDLK_p:
					{
						keyStatus[0xD] = 0;
					}
				break;
				case SDLK_j:
					{
						keyStatus[0x7] = 0;
					}
				break;
				case SDLK_k:
					{
						keyStatus[0x8] = 0;
					}
				break;
				case SDLK_l:
					{
						keyStatus[0x9] = 0;
					}
				break;
				case SDLK_SEMICOLON:
					{
						keyStatus[0xE] = 0;
					}
				break;
				case SDLK_m:
					{
						keyStatus[0xA] = 0;
					}
				break;
				case SDLK_COMMA:
					{
						keyStatus[0x0] = 0;
					}
				break;
				case SDLK_PERIOD:
					{
						keyStatus[0xB] = 0;
					}
				break;
				case SDLK_SLASH:
					{
						keyStatus[0xF] = 0;
					}
				break;
				}
			}
		break;
		}
	}
	return 0;
}

void StartTimer()
{
	timerID = SDL_AddTimer((1000 / 30), UpdateTimers, NULL);
}

void RemoveTimer()
{
	if (timerID)
		SDL_RemoveTimer(timerID);
}

void PlayBeep()
{

}