#ifndef REGISTRY_H
#define REGISTRY_H

#include <unordered_map>
#include <string>

namespace Simulators
{
namespace RV32I
{


class Registry
{
public:
   Registry();

   void store(const std::string& regName, int regValue);
   int load(const std::string& regName);

   int getAccumulatedBitFlips() const;

   void printRegistry();

   static int getIntegerValue(const std::string& regName);

private:
   std::unordered_map<std::string, int> m_registry;
   int m_numBitFlips;
};

}
}

#endif // REGISTRY_H
