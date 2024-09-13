#include "ternarylogic.h"

#include <math.h>
#include <iostream>

// Code taken from https://tfetimes.com/c-balanced-ternary/
void TernaryLogic::TintToTrits(Tint in, Trits& out)
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

void TernaryLogic::TritsToTint(const Trits& in, Tint& out)
{
   out = 0;
   for(char i = 0; i < in.size(); ++i)
   {
      out += in[i] * std::pow(3, i);
   }
}

void TernaryLogic::ParseImmediate(uint8_t immediateSizeTrits, const Tint& in, Tint& out)
{
   // This can be solved by calculating std::pow(3, immediateSize) to get min/max and then add some overflow/underflow logic, but this way is more fun and "correct".
   Trits trits;
   TintToTrits(in, trits);
   trits.resize(immediateSizeTrits, 0);
   TritsToTint(trits, out);
}
