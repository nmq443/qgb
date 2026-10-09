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
    "../roms/cpu_instrs/individual/11-op a,(hl).gb",
    "../roms/instr_timing/instr_timing.gb",
    "../roms/mem_timing/individual/01-read_timing.gb"};

static std::string runRomUntilResult(const std::filesystem::path &romPath)
{
    Cartridge cartridge;
    EXPECT_NO_THROW(cartridge.load(romPath));
    Timer timer;
    Clock clock;
    Bus bus;
    bus.init(cartridge, timer);
    CPU cpu;
    cpu.init(bus, clock);

    std::string output;
    constexpr int maxSteps = 10'000'000;

    for (int i = 0; i < maxSteps; ++i)
    {
        int mCycles = cpu.step();
        timer.tick(mCycles * 4);

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

class RomTest : public ::testing::TestWithParam<std::filesystem::path>
{
};

TEST_P(RomTest, Passes)
{
    const auto &romPath = GetParam();
    std::string output = runRomUntilResult(romPath);

    EXPECT_TRUE(output.contains("Passed"))
        << "ROM: " << romPath << "\nOutput:\n"
        << output;
}

INSTANTIATE_TEST_SUITE_P(
    CpuInstrs,
    RomTest,
    ::testing::ValuesIn(kROMPaths),
    [](const ::testing::TestParamInfo<std::filesystem::path> &info)
    {
        // gtest names may only contain [A-Za-z0-9_]
        std::string name = info.param.stem().string();
        for (char &c : name)
            if (!std::isalnum(static_cast<unsigned char>(c)))
                c = '_';
        return name;
    });