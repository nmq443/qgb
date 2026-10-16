#include "core/Emulator.h"
#include "core/PPU.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

int main(int, char *[])
{
    using namespace qgb;

    Emulator emu;
    emu.load("../roms/lyc/lyc.gb");
    emu.setSerialCallback([](char c)
                          { std::cout << c << std::flush; });

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    constexpr int WIDTH = PPU::kLcdWidth;
    constexpr int HEIGHT = PPU::kLcdHeight;
    constexpr int SCALE = 8;

    std::array<uint32_t, WIDTH * HEIGHT> pixels; // converted ARGB
    constexpr uint32_t SHADES[4] = {0xFFE0F8D0, 0xFF88C070, 0xFF346856, 0xFF081820};

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer("Buffer", WIDTH * SCALE, HEIGHT * SCALE,
                                     0, &window, &renderer))
    {
        SDL_Log("Create failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Texture *texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST); // crisp pixels

    bool running = true;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        while (!emu.getPPU().isFrameReady())
        {
            emu.step();
        }

        const auto &frameBuffer = emu.getPPU().getFrameBuffer();
        for (size_t i = 0; i < frameBuffer.size(); ++i)
        {
            pixels[i] = SHADES[frameBuffer[i]];
        }

        SDL_UpdateTexture(texture, nullptr, pixels.data(),
                          WIDTH * sizeof(uint32_t)); // pitch in bytes

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr); // stretches to window
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}