#pragma once

#include <Parsers/ternarylogic.h>

#include <string>
#include <unordered_map>

namespace Simulators
{
namespace REBEL6
{
class Registry
{
 public:
   Registry(int size);

   void reset(int size);

   void store(const std::string& regName, Tint regValue);
   Tint load(const std::string& regName);

   std::int32_t getAccumulatedTritFlips() const;

   void printRegistry();

   static std::int16_t getIntegerValue(const std::string& regName);

 private:
   std::unordered_map<std::string, Tint> m_registry;
   std::uint32_t m_numTritFlips;
};

}
}
