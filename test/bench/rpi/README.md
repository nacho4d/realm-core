# Benchmarking on Raspberry Pi

_Hypothesis:_ Benchmarking on an array of Raspberry Pi's is "close enough" to
benchmarking on an array of real devices.

## Todo

- [x] Cross-compile core using [Linaro
  binaries](http://releases.linaro.org/components/toolchain/binaries/). (The
[raspberrypi/tools](https://github.com/raspberrypi/tools) cross-compiler is
from 2014, and doesn't support C++14.)
- [ ] Create a Docker image for cross-compiling core.
- [ ] Figure out a repository for cross-compiled versions of core. There's no reason to cross-compile a given version more than once for a given platform.
- [ ] Replace glibc with Bionic on the rpi.

## Our Pi's

We have a number Raspberry Pi 3 Model B lying around.

Here is some information about their CPUs:

    model name  : ARMv7 Processor rev 4 (v7l)
    ...
    Features  : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32
    ...
    Hardware  : BCM2709

## Cross-Compilation

_Hypothesis:_ Compiling core on a Raspberry Pi directly is too slow.

Cross-compiling a static version of core and copying it to the device is a
viable solution if this hypothesis is confirmed.