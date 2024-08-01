#pragma once

#include <string>

namespace Simulators
{
namespace RV32I
{

class ExecutableProgram;

class AssemblerUtils
{
public:

   AssemblerUtils() = delete;

   static void generateAssemblyFileForMRCS(const ExecutableProgram& program, const std::string& fileName);
};

}
}
