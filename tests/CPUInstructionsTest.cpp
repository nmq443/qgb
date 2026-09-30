#include <core/Emulator.h>
#include <core/Utils.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace qgb;

const std::vector<std::filesystem::path> kROMPaths = {
    "../roms/cpu_instrs/individual/01-special.gb",
    "../roms/cpu_instrs/individual/02-interrupts.gb",
     "../roms/cpu_instrs/individual/03-op sp,hl.gb",
     "../roms/cpu_instrs/individual/04-op r,imm.gb",
     "../roms/cpu_instrs/individual/05-op rp.gb",
     "../roms/cpu_instrs/individual/06-ld r,r.gb",
     "../roms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb",
     "../roms/cpu_instrs/individual/08-misc instrs.gb",
     "../roms/cpu_instrs/individual/09-op r,r.gb",
     "../roms/cpu_instrs/individual/10-bit ops.gb",
     "../roms/cpu_instrs/individual/11-op a,(hl).gb"
};

static std::string runRomUntilResult(const std::filesystem::path &romPath)
{
    Cartridge cartridge;
    EXPECT_NO_THROW(cartridge.load(romPath));
    Timer timer;
    Bus bus;
    bus.init(cartridge, timer);
    CPU cpu;

    std::string output;
    constexpr int maxSteps = 10'000'000;

    for (int i = 0; i < maxSteps; ++i)
    {
        int tCycles = cpu.step(bus);
        timer.tick(tCycles);

        if (bus.read(0xff02) == 0x81)
        {
            char c = static_cast<char>(bus.read(0xff01));
            output += c;

            bus.write(0xFF02, 0x00);

            if (output.find("Passed") != std::string::npos)
            {
                return output;
            }

            if (output.find("Failed") != std::string::npos)
            {
                return output;
            }
        }
    }

    return output;
}

TEST(CPUInstructionsTest, Individuals)
{
    for (const auto &romPath : kROMPaths)
    {
        std::string output = runRomUntilResult(romPath);

        EXPECT_NE(output.find("Passed"), std::string::npos)
            << "ROM: " << romPath << "\nOutput:\n"
            << output;

        std::cout << "Test " << romPath << " passed" << std::endl;
    }
}