#pragma once

#include <unordered_map>
#include <string>

namespace Simulators
{
namespace RV32I
{


class Registry
{
 public:
   Registry(int size);

   void reset(int size);

   void store(const std::string& regName, std::int32_t regValue);
   std::int32_t load(const std::string& regName);

   std::int32_t getAccumulatedBitFlips() const;

   void printRegistry();

   static std::uint8_t getIntegerValue(const std::string& regName);

 private:
   std::unordered_map<std::string, std::int32_t> m_registry;
   std::uint32_t m_numBitFlips;
};

}
}
