cmake_minimum_required(VERSION 3.18)

# Project name and version
project(RISCVToRebelTranslator VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR})

# Header files
set(HEADERS
   expressionparser.h
   instructionconverterbase.h
   lexer.h
   parser.h
   parseutils.h
   ternarymapper.h
   ternaryoperandconverter.h
   token.h
   Converters/instructionconverterbase.h
   Converters/rv32ipseudotorv32ibase.h
   Converters/rv32itorebel2.h
   Expressions/all_expressions.h
   Expressions/directive.h
   Expressions/expression.h
   Expressions/instruction.h
   Expressions/label.h
   Simulators/RV32I/Cpus/cpu.h
   Simulators/RV32I/Cpus/registry.h
   Simulators/RV32I/assemblerandlinker.h
   Simulators/RV32I/executableprogram.h
   Simulators/RV32I/simulator.h
   Simulators/RV32I/simulatorutils.h
   Simulators/directivehelper.h
)

# Source files
set(SOURCES
   main.cpp
   expressionparser.cpp
   lexer.cpp
   parser.cpp
   parseutils.cpp
   ternarymapper.cpp
   ternaryoperandconverter.cpp
   Converters/rv32ipseudotorv32ibase.cpp
   Converters/rv32itorebel2.cpp
   Expressions/directive.cpp
   Expressions/expression.cpp
   Expressions/instruction.cpp
   Expressions/label.cpp
   Simulators/RV32I/Cpus/cpu.cpp
   Simulators/RV32I/Cpus/registry.cpp
   Simulators/RV32I/assemblerandlinker.cpp
   Simulators/RV32I/executableprogram.cpp
   Simulators/RV32I/simulator.cpp
   Simulators/RV32I/simulatorutils.cpp
   Simulators/directivehelper.cpp
)

# Add the executable
add_executable(RISCVToRebelTranslator ${SOURCES} ${HEADERS})
