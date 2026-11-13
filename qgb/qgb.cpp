#include "core/Emulator.h"
#include "core/PPU.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <optional>

static std::optional<std::filesystem::path> gPendingRom;
static std::mutex gRomMutex;

static const SDL_DialogFileFilter kRomFilters[] = {
    {"Game Boy ROMs", "gb;gbc"},
    {"All files", "*"}};

static void onRomSelected(void *userdata, const char *const *filelist, int filter)
{
    if (!filelist)
    {
        SDL_Log("File dialog error: %s", SDL_GetError());
        return;
    }

    if (!*filelist)
    {
        return;
    }

    std::lock_guard lock(gRomMutex);
    gPendingRom = filelist[0];
}

static void openDialog(SDL_Window *window)
{
    SDL_ShowOpenFileDialog(onRomSelected, nullptr, window, kRomFilters, SDL_arraysize(kRomFilters), nullptr, false);
}

int main(int argc, char **argv)
{
    using namespace qgb;
    constexpr int kScale = 5;

    Emulator emu;
    // emu.load(romPath);
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

    if (!SDL_CreateWindowAndRenderer("Buffer", PPU::kLcdWidth * kScale, PPU::kLcdHeight * kScale,
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

            if (e.type == SDL_EVENT_KEY_DOWN and e.key.key == SDLK_O and (e.key.mod & SDL_KMOD_CTRL) and !emu.hasRom())
            {
                openDialog(window);
            }

            if (e.type == SDL_EVENT_DROP_FILE and !emu.hasRom())
            {
                emu.load(e.drop.data);
            }
        }

        {
            std::lock_guard lock(gRomMutex);
            if (gPendingRom and !emu.hasRom())
            {
                emu.load(*gPendingRom);
                gPendingRom.reset();
            }
        }

        if (emu.hasRom())
        {
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
                SDL_RenderClear(renderer);
                SDL_UpdateTexture(texture, nullptr, pixels.data(),
                                  PPU::kLcdWidth * sizeof(uint32_t)); // pitch in bytes
                SDL_FRect dst{0.0f, 0.0f, static_cast<float>(PPU::kLcdWidth * kScale), static_cast<float>(PPU::kLcdHeight * kScale)};
                SDL_RenderTexture(renderer, texture, nullptr, &dst);

                emu.getPPU().setFrameReady(false);
                SDL_RenderPresent(renderer);
            }
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(16); // don't spin at 100% CPU while idle
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
