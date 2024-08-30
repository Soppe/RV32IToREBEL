#include "ternarylogic.h"

#include <math.h>

//Code taken from https://tfetimes.com/c-balanced-ternary/
void Simulators::REBEL6::TernaryLogic::TintToTrits(Tint in, Trits& out)
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
      int r = in % 3;
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

void Simulators::REBEL6::TernaryLogic::TritsToTint(const Trits& in, Tint& out)
{
   out = 0;
   for(char i = 0; i < in.size(); ++i)
   {
      out += in[i] * std::pow(3, i);
   }
}
