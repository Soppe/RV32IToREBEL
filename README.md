# RISCVToRebelTranslator

A lightweight translator to translate and simulate binary RISCV RV32I assembly to ternary REBEL6 assembly. The RV32I assembly code is generated using the GCC Toolchain. Some GCC features and syntax is therefor supported, but far from everything.

## GCC Toolchain setup

The setup and installation of the [GCC Toolchain](https://pages.github.com/) has been tested using Windows Subsystem for Linux (WSL) as OS, targeting the Newlib cross-compiler to avoid interference from any OS when generating RV32I assembly. Although the GIT page for the toolchain contains a walkthrough of how to set up and install the toolchain, we none the less describe the setup that worked for us.

As the walkthrough dictates, certain prerequisites need to be installed.

```
$ sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev
```

A handful of environments are created to make it easier to run the installation commands and to give a better overview. These can be added to the .bashrc file.

```
export RISCV_TOOL_BASE=/riscv                                   // Base dir for RISCV tools.
export RISCV_TOOLCHAIN=${RISCV_TOOL_BASE}/toolchain             // Toolchain dir
export RISCV_TOOLCHAIN_INSTALL=${RISCV_TOOLCHAIN}/installed     // Dir for all installed toolchain-related binaries and programs
```

The PATH variable need to be updated before initiating installation.

```
export PATH="${RISCV_TOOLCHAIN_INSTALL}/bin:$PATH"
```

Once configured, the only thing remaining is to setup and install the toolchain itself, targeting the RV32I architecture.

```
mkdir -p $RISCV_TOOLCHAIN_INSTALL
cd $RISCV_TOOLCHAIN
git clone https://github.com/riscv/riscv-gnu-toolchain
cd riscv-gnu-toolchain
mkdir build
cd build
../configure --prefix=$RISCV_TOOLCHAIN_INSTALL --with-arch=rv32i --with-abi=ilp32
make -j$(nproc)
```
