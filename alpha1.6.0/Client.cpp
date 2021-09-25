#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <bits/stdc++.h>
#include <SDL.h>
#include "MyDefines.h"

using namespace std;

SDL_Window* GameWindow;
SDL_Renderer* GameRender;
map<string, SDL_Texture*> ImageDB;
SDL_Event GameEvent;

SDL_Texture* OpenBMP(string path) {
	SDL_Surface* ImageSurface = SDL_LoadBMP(path.c_str());
	if(ImageSurface == NULL) {
		printf("SDL_LoadBMP(%s) [FAILED] Error->%s\n", path.c_str(), SDL_GetError());
		return NULL;
	}
	printf("SDL_LoadBMP(%s) [DONE]\n", path.c_str());
	SDL_Texture* Image = SDL_CreateTextureFromSurface(GameRender, ImageSurface);
	if(Image == NULL) {
		printf("SDL_CreateTextureFromSurface() [FAILED] Error->%s\n", SDL_GetError());
		return NULL;
	}
	printf("SDL_CreateTextureFromSurface() [DONE]\n");
	SDL_FreeSurface(ImageSurface);
	return Image;
}

void Init() {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "SDL_Init() [FAILED] Error->%s\n", SDL_GetError());
		exit(-1);
	}
	printf("SDL_Init() [DONE]\n");
	
	GameWindow = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HIGH, NULL);
	if(GameWindow == NULL) {
		fprintf(stderr, "SDL_CreateWindow() [FAILED] Error->%s\n", SDL_GetError());
		exit(-1);
	}
	printf("SDL_CreateWindow() [DONE]\n");
	
	GameRender = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);
	if(GameRender == NULL) {
		fprintf(stderr, "SDL_CreateRenderer() [FAILED] Error->%s", SDL_GetError());
		exit(-1);
	}
	printf("SDL_CreateRenderer() [DONE]\n");
	ImageDB["unknow.bmp"] = OpenBMP("img/unknow.bmp");
}

void Quit() {
	for(auto it = ImageDB.begin(); it != ImageDB.end(); it++) {
		SDL_DestroyTexture(it->second);
	}
	printf("SDL_DestroyTexture() for ImageDB [DONE]\n");
	SDL_DestroyWindow(GameWindow);
	printf("SDL_DestroyWindow() [DONE]\n");
	SDL_Quit();
	printf("SDL_Quit() [DONE]\n");
}

bool Loop() {
	SDL_RenderClear(GameRender);
	SDL_RenderCopy(GameRender, ImageDB["unknow.bmp"], NULL, NULL);
	SDL_RenderPresent(GameRender);
	if(SDL_PollEvent(&GameEvent)) {
		switch(GameEvent.type) {
		case SDL_QUIT:
			return true;
		}
	}
	return false;
}

int main(int argc, char *argv[]) {
	Init();
	while(true) {
		int starttime = clock();
		if(Loop()) break;
		int endtime = clock();
		int usedtime = endtime - starttime;
		if(usedtime < 40) {
			SDL_Delay(40 - usedtime);
		}
	}
	Quit();
	
	return 0;
}
