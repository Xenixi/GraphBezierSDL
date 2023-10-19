#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <stdbool.h>
extern "C"
{
#include <PTTransform.h>
}

int main(int argc, char **argv)
{

    std::cout << "3-PT Bezier Interpolation - Graph Generator\n<x1>, <y1>, <x2>, <y2>, <x3>, <y3>, <OPTIONAL-numPts>\nEnter Params: ";
    std::string input;

    std::cin >> input;

    std::stringstream ss(input);

    std::string token;

    double values[6];
    int i;
    int numPts;
    for (i = 0; i < 6 && std::getline(ss, token, ','); i++)
    {

        try
        {
            double d = std::stod(token);
            std::cout << "Recieved data '" << d << "' for index " << i << ". Value set.\n";
            values[i] = d;
        }
        catch (std::exception &err)
        {
            std::cout << "failed to parse double at index " << i << ". Defaulting to 0\n";
            values[i] = 0;
        }
    }

    if (std::getline(ss, token, ','))
    {
        try
        {
            numPts = std::stoi(token);
            std::cout << "Set number of generated points to " << numPts << ".\n";
        }
        catch (std::exception &err)
        {
            std::cout << "Invalid input for <numPts>. Defaulting to 10.\n";
            numPts = 10;
        }
    }
    else
    {
        numPts = 10;
    }

    for (; i < 6; i++)
    {
        std::cout << "No value received for index " << i << ". Defaulting to 0\n";
        values[i] = 0;
    }

    for (int i = 0; i < 6; i += 2)
    {
        std::cout << "Setting point " << i / 2 << " as (" << values[i] << "," << values[i + 1] << ").\n";
    }

    genPointSet(values[0], values[1], values[2], values[3], values[4], values[5]);

    std::cout << "\nData Set.\n Generating curve with " << numPts << " points.\n";


    constexpr double L = 0.5; 

    interpolate2DCurve(&getGenPts()[0], &getGenPts()[1], &getGenPts()[2], L, numPts);

    SDL_Window *wnd;
    SDL_Renderer *rnd;

    SDL_CreateWindowAndRenderer(800, 800, SDL_WindowFlags::SDL_WINDOW_SHOWN, &wnd, &rnd);

    

    bool wndAlive = true;

    SDL_Event evt;

    int wndX;
    int wndY;

    SDL_GetWindowSize(wnd, &wndX, &wndY);

    double wndOrigX, wndOrigY;

    wndOrigX = wndX / 2.0;
    wndOrigY = wndX / 2.0;

    // Render setup
    SDL_SetRenderDrawColor(rnd, 0, 0, 0, 0);
    SDL_RenderClear(rnd);

    SDL_SetRenderDrawColor(rnd, 255, 0, 0, 0);

    SDL_FRect rect;
    rect.h = 0.8;
    rect.h = 0.8;

    for (int i = 0; i < numPts; i++)
    {
        Point2D *pt = &getCalculated2DCurve()[i];
        // SDL_RenderDrawPointF(rnd, pt->x + wndOrigX, -(pt->y) + wndOrigY);
        rect.x = pt->x + wndOrigX;  
        rect.y = -(pt->y) + wndOrigY;
        SDL_RenderDrawRectF(rnd, &rect);
        SDL_RenderFillRectF(rnd, &rect);
    }

    SDL_SetWindowTitle(wnd, (char*)"Graph1");
    //

    while (wndAlive)
    {

        while (SDL_PollEvent(&evt))
        {
            if (evt.window.event == SDL_WINDOWEVENT_CLOSE || evt.key.keysym.sym == SDL_GetKeyFromName((char *)"escape"))
            {
                wndAlive = false;
            }
        }

        SDL_RenderPresent(rnd);
    }

    freeAll();

    SDL_DestroyWindow(wnd);
    SDL_DestroyRenderer(rnd);
    SDL_Quit();

    return 0;
}