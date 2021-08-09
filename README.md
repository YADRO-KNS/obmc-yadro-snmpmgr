# obmc-yadro-snmpmgr
CLI tool for manage OpenBMC SNMP settings

## Build with OpenBMC SDK
OpenBMC SDK contains toolchain and all dependencies needed for building the
project. See [official
documentation](https://github.com/openbmc/docs/blob/master/development/dev-environment.md#download-and-install-sdk)
for details.

Build steps:
```sh
$ source /path/to/sdk/environment-setup-arm1176jzs-openbmc-linux-gnueabi
$ meson --buildtype plain --optimization s build_dir
$ ninja -C build_dir
```
If build process succeeded, the directory `build_dir` contains executable file
`yadro-snmpmgr`.

