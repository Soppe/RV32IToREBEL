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
   logger.h
   Assemblers/directivehelper.h
   Assemblers/REBEL6/assembler.h
   Assemblers/REBEL6/assemblerutils.h
   Assemblers/REBEL6/executableprogram.h
   Assemblers/RV32I/assembler.h
   Assemblers/RV32I/assemblerutils.h
   Assemblers/RV32I/executableprogram.h
   Converters/converter.h
   Converters/instructionconverterbase.h
   Converters/rv32ipseudotorv32ibase.h
   Converters/rv32itorebel2.h
   Converters/rv32itorebel6.h
   Expressions/all_expressions.h
   Expressions/directive.h
   Expressions/expression.h
   Expressions/instruction.h
   Expressions/label.h
   Parsers/expressionparser.h
   Parsers/lexer.h
   Parsers/parser.h
   Parsers/parseutils.h
   Parsers/ternarylogic.h
   Parsers/token.h
   Simulators/REBEL6/Cpu/cpu.h
   Simulators/REBEL6/Cpu/instructionexecutor.h
   Simulators/REBEL6/Cpu/registry.h   
   Simulators/REBEL6/simulator.h
   Simulators/RV32I/Cpu/cpu.h
   Simulators/RV32I/Cpu/instructionexecutor.h
   Simulators/RV32I/Cpu/registry.h   
   Simulators/RV32I/simulator.h
)

# Source files
set(SOURCES
   main.cpp
   Assemblers/directivehelper.cpp
   Assemblers/REBEL6/assembler.cpp
   Assemblers/REBEL6/assemblerutils.cpp
   Assemblers/REBEL6/executableprogram.cpp
   Assemblers/RV32I/assembler.cpp
   Assemblers/RV32I/assemblerutils.cpp
   Assemblers/RV32I/executableprogram.cpp
   Converters/converter.cpp
   Converters/instructionconverterbase.cpp
   Converters/rv32ipseudotorv32ibase.cpp
   Converters/rv32itorebel2.cpp
   Converters/rv32itorebel6.cpp
   Expressions/directive.cpp
   Expressions/instruction.cpp
   Expressions/label.cpp
   Parsers/expressionparser.cpp
   Parsers/lexer.cpp
   Parsers/parser.cpp
   Parsers/parseutils.cpp
   Parsers/ternarylogic.cpp
   Simulators/REBEL6/Cpu/cpu.cpp
   Simulators/REBEL6/Cpu/instructionexecutor.cpp
   Simulators/REBEL6/Cpu/registry.cpp   
   Simulators/REBEL6/simulator.cpp
   Simulators/RV32I/Cpu/cpu.cpp
   Simulators/RV32I/Cpu/instructionexecutor.cpp
   Simulators/RV32I/Cpu/registry.cpp   
   Simulators/RV32I/simulator.cpp

)

# Add the executable
add_executable(RISCVToRebelTranslator ${SOURCES} ${HEADERS})
