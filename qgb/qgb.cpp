#include "core/Emulator.h"
#include "core/PPU.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <algorithm>
#include <iostream>

int main(int argc, char **argv)
{
    using namespace qgb;
    int scale = 5; // default scale

    std::filesystem::path romPath;
    if (argc < 2)
    {
        romPath = "../roms/dmg-acid2.gb"; // default to this rom
    }
    else
    {
        romPath = argv[1];
        scale = std::stoi(argv[2]);
    }

    Emulator emu;
    emu.load(romPath);
    emu.setSerialCallback([](char c)
                          { std::cout << c << std::flush; });

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    std::array<uint32_t, PPU::kLcdWidth * PPU::kLcdHeight> pixels; // converted ARGB
    constexpr uint32_t SHADES[4] = {0xFFE0F8D0, 0xFF88C070, 0xFF346856, 0xFF081820};

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer("Buffer", PPU::kLcdWidth * scale, PPU::kLcdHeight * scale,
                                     0, &window, &renderer))
    {
        SDL_Log("Create failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Texture *texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        PPU::kLcdWidth, PPU::kLcdHeight);
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

        emu.step();

        if (emu.getPPU().isFrameReady())
        {
            const bool *keystate = SDL_GetKeyboardState(nullptr);
            emu.setJoypadState({keystate[SDL_SCANCODE_X],
                                keystate[SDL_SCANCODE_Z],
                                keystate[SDL_SCANCODE_RETURN],
                                keystate[SDL_SCANCODE_RSHIFT],
                                keystate[SDL_SCANCODE_LEFT],
                                keystate[SDL_SCANCODE_RIGHT],
                                keystate[SDL_SCANCODE_UP],
                                keystate[SDL_SCANCODE_DOWN]});

            const auto &frameBuffer = emu.getPPU().getFrameBuffer();
            std::transform(frameBuffer.begin(), frameBuffer.end(), pixels.begin(), [&SHADES](auto pixel)
                           { return SHADES[pixel]; });

            SDL_UpdateTexture(texture, nullptr, pixels.data(),
                              PPU::kLcdWidth * sizeof(uint32_t)); // pitch in bytes
            SDL_RenderClear(renderer);
            SDL_FRect dst{0.0f, 0.0f, static_cast<float>(PPU::kLcdWidth * scale), static_cast<float>(PPU::kLcdHeight * scale)};
            SDL_RenderTexture(renderer, texture, nullptr, &dst);
            SDL_RenderPresent(renderer);

            emu.getPPU().setFrameReady(false);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}