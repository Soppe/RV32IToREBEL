# MSc thesis

The work in this repository is part of my MSc thesis that can be found [here](https://openarchive.usn.no/usn-xmlui/handle/11250/3169529).

# RV32IToREBEL (R2R)

A lightweight translator to translate and simulate binary RISCV RV32I assembly to ternary REBEL6 assembly. The RV32I assembly code is generated using the GCC Toolchain. Some GCC features and syntax is therefor supported, but far from everything. No optimization has been used when compiling C code to RV32I assembly when using the GCC Toolchain to generate assembly files.

# References

The Lexer, Parser, and Token classes under code/Parsers was copied from [GIT](https://github.com/AZHenley/riscv-parser), a [RISC-V parser created by Austin Henley](https://austinhenley.com/blog/parsingriscv.html), and then adapted to our needs.

## GCC Toolchain setup

The setup and installation of the [GCC Toolchain](https://pages.github.com/) has been tested using Windows Subsystem for Linux (WSL) as OS, targeting the Newlib (bare-metal) cross-compiler to avoid interference from an OS when generating RV32I assembly. Although the GIT page for the toolchain contains a walkthrough of how to set up and install the toolchain, we none the less describe the setup that worked for us.

As the walkthrough dictates, certain prerequisites need to be installed.

```
$ sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev
```

A handful of environments are created to make it easier to run the installation commands and to give a better overview. These can be added to the .bashrc file.

```
// Base directory for RISCV tools.
export RISCV_TOOL_BASE=/riscv
// Toolchain directory
export RISCV_TOOLCHAIN=${RISCV_TOOL_BASE}/toolchain
// Directory for all installed toolchain-related binaries and programs
export RISCV_TOOLCHAIN_INSTALL=${RISCV_TOOLCHAIN}/installed
```

The PATH variable need to be updated before initiating installation. Add the following to the same file as you added the above lines.

```
export PATH="${RISCV_TOOLCHAIN_INSTALL}/bin:$PATH"
```

Once configured, the only thing remaining is to setup and install the toolchain itself, targeting the RV32I architecture.
Do note that this may take a while.

```
sudo mkdir -p $RISCV_TOOLCHAIN_INSTALL
cd $RISCV_TOOLCHAIN
git clone https://github.com/riscv/riscv-gnu-toolchain
cd riscv-gnu-toolchain
sudo mkdir build
cd build
../configure --prefix=$RISCV_TOOLCHAIN_INSTALL --with-arch=rv32i --with-abi=ilp32
make -j$(nproc) // This can take a while
```

## Building the translator

The translator is built with cmake and make using the following commands.

```
BUILD_DIR=<Target directory for build files and the executable>
sudo mkdir ${BUILD_DIR}
cd ${BUILD_DIR}
cmake <Path to the translator's local repository>/code
make
```

## Generating assembly files using the GCC Toolchain

The translator only supports single-file assembly code. This comes from single C-file programs since the GCC Compiler generates one assembly file per C file.
To generate an assembly file from a C file, run `riscv32-unknown-elf-gcc -S <C-coded file>.c -o <RV32I Assembly file>.s`

## Running the translator

The command `./RV32IToREBEL <RV32I Assembly file>.s` is used to run the translator, using the generated RV32I assembly file as input.
