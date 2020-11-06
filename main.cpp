#include <SDL.h>
#include <vector>
#include <random>
#include <iostream>
//#include <SFML/Graphics.hpp>
#undef main
//constexpr auto WINSIZEX = 2560;
//constexpr auto WINSIZEY = 1080;
constexpr auto WINSIZEX = 900;
constexpr auto WINSIZEY = 900;

struct Vector2
{
	float x;
	float y;
};

struct Vector2i
{
	int x;
	int y;
};

struct Color
{
	int r;
	int g;
	int b;
	int a;
};

std::random_device rng;
std::mt19937_64 dev(rng());

std::vector<Vector2> pos;
std::vector<Vector2> vel;
std::vector<Color> cs;

float size = 50000;

SDL_Window* window;
SDL_Renderer* renderer;

Vector2i mpos;

float zoom;

bool shouldClose;

bool mpressed;
bool rpressed;

float sqrt2(Vector2 dif)
{
	return sqrt((dif.x * dif.x) + (dif.y * dif.y));
}

Vector2 normalise(Vector2 dif)
{
	float sqroot = sqrt2(dif);
	Vector2 vec;
	vec.x = dif.x / sqroot;
	vec.y = dif.y / sqroot;
	return vec;
}

void update(unsigned int t)
{
	int tsize = size / 4;
	for (unsigned int i = tsize * (t - 1); i < tsize * t; i++)
	{
		if (mpressed)
		{
			Vector2 dif;
			dif.x = pos[i].x - mpos.x;
			dif.y = pos[i].y - mpos.y;
			Vector2 normalised = normalise(dif);
			vel[i].x -= normalised.x / 3;
			vel[i].y -= normalised.y / 3;
		}
		/*
		if (rpressed)
		{
			Vector2 dif;
			dif.x = pos[i].x - mpos.x;
			dif.y = pos[i].y - mpos.y;
			Vector2 normalised = normalise(dif);
			vel[i].x += normalised.x / 5;
			vel[i].y += normalised.y / 5;
		}
		*/
		float curdif = abs(vel[i].x) + abs(vel[i].y);
		cs[i].b = 255;
		cs[i].g = 0;
		if (curdif * 10 + 50 > 255)
		{
			cs[i].r = 255;
		}
		else
		{
			cs[i].r = curdif * 10 + 50;
		}
		vel[i].x *= 0.99;
		vel[i].y *= 0.99;
		pos[i].x += vel[i].x * 3;
		pos[i].y += vel[i].y * 3;
		SDL_SetRenderDrawColor(renderer, cs[i].r, cs[i].g, cs[i].b, 255);
		SDL_RenderDrawPoint(renderer, pos[i].x * zoom, pos[i].y * zoom);
		//SDL_RenderDrawLineF
	}
}

int main()
{
	zoom = 1;
	for (unsigned int i = 0; i < size; i++)
	{
		std::uniform_int_distribution<int> x(0, WINSIZEX);
		std::uniform_int_distribution<int> y(0, WINSIZEY);
		Vector2 curpos;
		Vector2 curvel;
		curvel.x = 0;
		curvel.y = 0;
		curpos.x = x(rng);
		curpos.y = y(rng);
		pos.push_back(curpos);
		vel.push_back(curvel);
		Color curc;
		cs.push_back(curc);
	}
	shouldClose = 0;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINSIZEX, WINSIZEY, 0, &window, &renderer);
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	//sf::Thread t1(&update, 1);
	//sf::Thread t2(&update, 2);
	//sf::Thread t3(&update, 3);
	//sf::Thread t4(&update, 4);
	while (!shouldClose)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				shouldClose = 1;
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				SDL_GetMouseState(&mpos.x, &mpos.y);
				mpos.x = mpos.x / zoom;
				mpos.y = mpos.y / zoom;
			}
			if (event.type == SDL_MOUSEWHEEL)
			{
				zoom += event.wheel.y * 0.05;
				SDL_GetMouseState(&mpos.x, &mpos.y);
				mpos.x = mpos.x / zoom;
				mpos.y = mpos.y / zoom;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				mpressed = true;
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				mpressed = false;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		rpressed = false;
		/*
		t1.launch();
		t1.wait();
		t2.launch();
		t2.wait();
		t3.launch();
		t3.wait();
		t4.launch();
		t4.wait();
		*/
		/*
		t1.wait();
		t2.wait();
		t3.wait();
		t4.wait();
		*/
		update(1);
		update(2);
		update(3);
		update(4);
		/*
		for (unsigned int i = 0; i < pos.size(); i++)
		{
			SDL_SetRenderDrawColor(renderer, cs[i].r, cs[i].g, cs[i].b, 255);
			SDL_RenderDrawPoint(renderer, pos[i].x * zoom, pos[i].y * zoom);
			//std::cout << "ok" << std::endl;
		}
		*/
		SDL_RenderPresent(renderer);

		//sf::sleep(sf::milliseconds(1));
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}