#include "registry.h"

#include <Simulators/REBEL6/ternarylogic.h>
#include <logger.h>

#include <iostream>
#include <vector>

namespace
{
int tritFlipCostLookup [3][3] = {/*-1, 0, 1*/
                          /* -1 */ {0, 1, 1},
                          /* 0 */  {1, 0, 1},
                          /* 1 */  {1, 1, 0}};

std::uint32_t countTritFlips(Simulators::REBEL6::Tint oldVal, Simulators::REBEL6::Tint newVal)
{
   Simulators::REBEL6::Trits oldTrits;
   Simulators::REBEL6::Trits newTrits;
   std::uint32_t tritFlips = 0;

   Simulators::REBEL6::TernaryLogic::TintToTrits(oldVal, oldTrits);
   Simulators::REBEL6::TernaryLogic::TintToTrits(newVal, newTrits);

   int sizeDiff = oldTrits.size() - newTrits.size();

   if(sizeDiff < 0)
   {
      for(int i = 0; i < -sizeDiff; ++i)
      {
         oldTrits.insert(oldTrits.end(), 0);
      }
   }
   else
   {
      for(int i = 0; i < sizeDiff; ++i)
      {
         newTrits.insert(newTrits.end(), 0);
      }
   }

   for(int i = 0; i < oldTrits.size(); ++i)
   {
      tritFlips += tritFlipCostLookup[oldTrits[i] + 1][newTrits[i] + 1];
   }

   return tritFlips;
}

const std::vector<std::string> RegisterABINames = {"zero", "ra", "sp", "gp", "tp",
                                                    "t0", "t1", "t2",
                                                    "s0", "s1",
                                                    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
                                                    "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
                                                    "t3", "t4", "t5", "t6"};

void convertToABI(const std::string& in, std::string& out)
{
   out.resize(in.length());
   std::transform(in.begin(), in.end(), out.begin(), [](std::uint8_t c){ return std::tolower(c); });

   if(out[0] == 'x')
   {
      try
      {
         std::int32_t index = stoi(out.substr(1, out.length()));
         out = RegisterABINames[index];
      }
      catch(std::exception&)
      {
         //std::cout << __PRETTY_FUNC__ << ": Trying to convert unconvertable x-named register " << in << std::endl;
      }
   }
   else
   {
     // Already happening in the transform
     //out = in;
   }
}
}

namespace Simulators
{
namespace REBEL6
{

Registry::Registry(int size)
{
   reset(size);
}

void Registry::reset(int size)
{
   m_numTritFlips = 0;
   m_registry.clear();

   int negativeStart = size / -2;
   if(size % 2 == 0)
   {
      ++negativeStart;
   }

   std::string reg;
   std::string regABI;
   for(std::int32_t i = negativeStart; i <= size / 2; ++i)
   {
      reg = "x" + std::to_string(i);
      convertToABI(reg, regABI);
      m_registry[regABI] = 0;
   }
}

void Registry::store(const std::string& regName, Tint regValue)
{
   std::string abiName;
   convertToABI(regName, abiName);

   // x0/zero is hardcoded to 0
   if(abiName == "zero")
   {
      return;
   }

   Tint oldVal = 0;

   auto it = m_registry.find(abiName);
   if(it != m_registry.end())
   {
      oldVal = it->second;
      it->second = regValue;
   }
   else
   {
      std::cerr << "Tried to store value " << regValue << " to unsupported registry named " << regName << std::endl;
      abort();
   }

   m_numTritFlips += countTritFlips(oldVal, regValue);
}

Tint Registry::load(const std::string& regName)
{
   std::string abiName;
   convertToABI(regName, abiName);

   Tint retVal = 0;

   auto it = m_registry.find(abiName);
   if(it != m_registry.end())
   {
      retVal = it->second;
   }
   else
   {
      std::cerr << "Tried to load value from unsupported registry named " << regName << std::endl;
      abort();
   }

   return retVal;
}

std::uint8_t Registry::getIntegerValue(const std::string& regName)
{
   std::string name;
   name.resize(regName.length());
   std::transform(regName.begin(), regName.end(), name.begin(), [](std::uint8_t c){ return std::tolower(c); });

   std::uint8_t value = 0;
   if(name[0] == 'x')
   {
      try
      {
         value = stoi(name.substr(1, name.length()));
      }
      catch(const std::exception&)
      {
         std::cerr << __PRETTY_FUNC__ << ": Unable to convert register " << name << " to integer" << std::endl;
         abort();
      }
   }
   else
   {
      auto it = std::find(RegisterABINames.begin(), RegisterABINames.end(), name);
      if(it == RegisterABINames.end())
      {
         std::cerr << __PRETTY_FUNC__ << ": Unable to find register " << name << std::endl;
         abort();
      }
      value = std::distance(RegisterABINames.begin(), it);
   }

   return value;
}

std::int32_t Registry::getAccumulatedTritFlips() const
{
   return m_numTritFlips;
}

void Registry::printRegistry()
{
   std::string abiName;
   std::cout << "x\t" << "abi\t" << "hex\t\t" << "dec" << std::endl;
   std::cout << "------------------------------------" << std::endl;
   for(std::int32_t i = 0; i < 32; ++i)
   {
      const std::string xName = "x" + std::to_string(i);
      convertToABI(xName, abiName);
      std::int32_t val = load(abiName);

      std::cout << xName << "\t" << abiName << "\t0x" << std::hex << val << "\t\t" << std::dec << val << std::endl;
   }
}

}
}
