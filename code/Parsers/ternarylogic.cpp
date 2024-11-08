#include "ternarylogic.h"

#include <math.h>
#include <iostream>

// Code taken from https://rosettacode.org/wiki/Balanced_ternary#C.2B.2B and slightly modified
void TernaryLogic::tintToTrits(Tint in, Trits& out)
{
   if (in == 0)
   {
      return;
   }

   bool neg = in < 0;
   if (neg)
   {
      in = -in;
   }

   Trit val = 0;
   while (in != 0)
   {
      Tint r = in % 3;
      if(r == 0)
      {
         val = 0;
      }
      else if (r == 1)
      {
         val = 1;
      }
      else
      {
         val = -1;
         ++in;
      }

      if(neg)
      {
         val = -val;
      }

      in /= 3;

      out.push_back(val);
   }
}

void TernaryLogic::tritsToTint(const Trits& in, Tint& out)
{
   out = 0;
   for(char i = 0; i < in.size(); ++i)
   {
      out += in[i] * std::pow(3, i);
   }
}

void TernaryLogic::parseImmediate(uint8_t immediateSizeTrits, const Tint& in, Tint& out)
{
   Trits trits;
   tintToTrits(in, trits);
   trits.resize(immediateSizeTrits, 0);
   tritsToTint(trits, out);
}
