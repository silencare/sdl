#include <iostream>
#include <SDL.h>
#include <fstream>
#include <string>
#include "drawingOOP.h"
using namespace std;

SDL_Renderer* renderer;
void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}
void RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
	int offsetx, offsety, d;
	int status;

	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;

	while (offsety >= offsetx) {

		status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
			x + offsety, y + offsetx);
		status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
			x + offsetx, y + offsety);
		status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
			x + offsetx, y - offsety);
		status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
			x + offsety, y - offsetx);

		if (status < 0) {
			status = -1;
			break;
		}

		if (d >= 2 * offsetx) {
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}
}
void Circle::Draw()
{
    cout << "Drawing " << m_name << endl;
	SDL_SetRenderDrawColor(renderer, m_color.red, m_color.green, m_color.blue, m_color.alpha);
	DrawCircle(renderer, m_x, m_y, m_radius);
	RenderFillCircle(renderer, m_x, m_y, m_radius);
}


void Rectangle::Draw()
{
    cout << "Drawing " << m_name << endl;
    SDL_Rect r;
    r.x = m_x;
    r.y = m_y;
    r.w = m_width;
    r.h = m_height;
    SDL_SetRenderDrawColor(renderer, m_color.red, m_color.green, m_color.blue, m_color.alpha);
    SDL_RenderFillRect(renderer, &r);

}

int main(int argc, char** argv)
{
    bool leftMouseButtonDown = false;
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
    Uint32* pixels = new Uint32[800 * 600];
    memset(pixels, 0xff000000, 800 * 600 * sizeof(Uint32));
    
    
    
    
    
    while (!quit)
    {
        
        SDL_UpdateTexture(texture, NULL, pixels, 800 * sizeof(Uint32));

        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                leftMouseButtonDown = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                leftMouseButtonDown = true;
        case SDL_MOUSEMOTION:
            if (leftMouseButtonDown)
            {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                //pixels[mouseY * 640 + mouseX] = 0;
            }
            break;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Clear winow
        SDL_RenderClear(renderer);
        Color color = Color{ 1,2,3,4 };
        vector<Shape*> items;
        Rectangle* rect1 = new Rectangle("First Rectangle", 100, 100, 100, 100, Color{0,0,0,255});
        Rectangle* rect2 = new Rectangle("Second Rectangle", 230, 100, 100, 100, Color{ 0,0,0,255 });
        Rectangle* rect3 = new Rectangle("Third Rectangle", 360, 100, 100, 100, Color{ 0,0,0,255 });
        Rectangle* rect4 = new Rectangle("Fourth Rectangle", 490, 100, 100, 100, Color{ 0,0,0,255 });
		Circle* circle1 = new Circle("Fst Circle", 200, 400, 150, Color{ 0,0,0,255 });


        
        items.push_back(rect1);
        items.push_back(rect2);
        items.push_back(rect3);
        items.push_back(rect4);
        items.push_back(circle1);
        for (Shape* it : items)
        {
            it->Draw();
            delete it;
        }

        items.clear();
        //SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}