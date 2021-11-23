#include "VolumeCalculator.hpp"

int main(int argc, char* argv[])
{
    const char *path = argv[1];
    TimberControl::VolumeCalculator volumeCalc(path);
    volumeCalc.Calculate();

}
