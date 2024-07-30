#ifndef ASSEMBLERUTILS_H
#define ASSEMBLERUTILS_H

#include <string>

namespace Simulators
{
namespace RV32I
{

class ExecutableProgram;

class AssemblerUtils
{
public:

   static void generateAssemblyFileForMRCS(const ExecutableProgram& program, const std::string& fileName);
};

}
}
#endif // ASSEMBLERUTILS_H
