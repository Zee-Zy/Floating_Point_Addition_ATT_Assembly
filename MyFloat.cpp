#include "MyFloat.h"

MyFloat::MyFloat(){
  sign = 0;
  exponent = 0;
  mantissa = 0;
}

MyFloat::MyFloat(float f){
  unpackFloat(f);
}

MyFloat::MyFloat(const MyFloat & rhs){
    sign = rhs.sign;
    exponent = rhs.exponent;
    mantissa = rhs.mantissa;
}

ostream& operator<<(std::ostream &strm, const MyFloat &f){
    strm << f.packFloat();
    return strm;

}

MyFloat MyFloat::operator+(const MyFloat& rhs) const{
    int exponent_difference = 0;
    int borrow = 0;
    int i;
        
    MyFloat float_copy(*this);
    MyFloat rhs_copy(rhs);  
    float_copy.mantissa |= 1<<23; //restoreS leading bit for both mantissas
    rhs_copy.mantissa |= 1<<23; //restore leading bit

    if(float_copy.mantissa == rhs_copy.mantissa)      // case if the number are the same, but the sign is opposite, just return 0 for 7 + -7, return 0;
       if(float_copy.exponent == rhs_copy.exponent)
         if(float_copy.sign != rhs_copy.sign)
          return 0;

    exponent_difference = rhs_copy.exponent - float_copy.exponent; // find difference between exponents
    if (exponent_difference == 0) 
    {

    }
    else if(exponent_difference > 0) //rhs = 5*10^6 this = 32*10^4 = 2
    {
        float_copy.mantissa = float_copy.mantissa >> (exponent_difference -1); // right shift, since rhs is bigger than this
        borrow = float_copy.mantissa & 1;
        float_copy.mantissa = float_copy.mantissa >> 1;
        float_copy.exponent += exponent_difference; //restore the same exponent
    }  
    else //rhs = 32*10^4 this = 5*10^6 = -2
    {
        rhs_copy.mantissa = rhs_copy.mantissa >> ((-1*exponent_difference) - 1); //rhs is smaller than this, so right shift that
        borrow = rhs_copy.mantissa & 1;
        rhs_copy.mantissa = rhs_copy.mantissa >> 1;
        rhs_copy.exponent += -1*(exponent_difference); // restore the same exponent
    }


    if (float_copy.sign == rhs_copy.sign) 
    {
      float_copy.mantissa += rhs_copy.mantissa; //add the mantissas together
    } 
    else
    {
     if(exponent_difference > 0)
        float_copy.sign = rhs_copy.sign;
     if(float_copy.mantissa > rhs_copy.mantissa) 
     {
      float_copy.mantissa -= rhs_copy.mantissa;
      float_copy.mantissa -= borrow;
      }
     else
     {
      float_copy.mantissa = rhs_copy.mantissa - float_copy.mantissa;
      float_copy.mantissa -= borrow;
     }
    }

  //normalize  
  for(i = 31; i >= 0; i--) //start from the most left bit, and decrease while shifting right
  {
  if(((float_copy.mantissa >> i) & 1) == 1) // if the bit at i, anded by 1 ==1, then it is the most significant bit 
     break; // leave out of loop
  }
  
  int num_shift = i - 23;
  if(num_shift > 0) //higher than 23
  {
     float_copy.mantissa = float_copy.mantissa >> num_shift; //right shift
      float_copy.exponent += num_shift; // increase exponent because its a right shift
  }
  else // lower than 23
  {
      float_copy.mantissa = float_copy.mantissa << (-1*num_shift); // left shift
      float_copy.exponent -= (-1*num_shift); // decrease exponent because it is a left shift 
  }  
   float_copy.mantissa = float_copy.mantissa & (0x7fffff); //restore mantissa leading bit

    return float_copy;
}

MyFloat MyFloat::operator-(const MyFloat& rhs) const{
  MyFloat rhs_copy(rhs); //make a copy of rhs
  if(rhs.sign == 1) // switch the signs of the copy, so that it is opposite of rhs
    rhs_copy.sign = 0;
  else  
    rhs_copy.sign = 1;

  return *this + rhs_copy; //add together, will call the addition operator
}

void MyFloat::unpackFloat(float f) {
  __asm__(
  "movl $0x7f800000, %%edi;" // exponent mask
  "movl $0x7fffff, %%esi;" //mantissa mask
  "andl %%eax, %%esi;" // and mantissa mask and f
  "movl %%esi, %%edx;" //mantissa = (f&mantissamask)
  "andl %%eax, %%edi;" //and exponent mask and f
  "shr $23, %%edi;" //(f&exponentmask >> 23)
  "movl %%edi, %%ecx;"
  "shr $31, %%eax;" //(f>>31)
  "movl %%eax, %%ebx"
  :"=b" (sign), "=c" (exponent), "=d" (mantissa) // store float in eax sign in ebx, exponent in ecx, mantissa in edx 
  :"a" (f)
  :"cc"                     
  );
}//unpackFloat

float MyFloat::packFloat() const{
  //returns the floating point number represented by this
  float f = 0;
   __asm__(
      "shl $31, %%ebx;" //sign << 31
      "shl $23, %%ecx;" // exponent <<23
      "orl %%ebx, %%ecx;" // or adds up the shifted exponent and sign
      "orl %%ecx, %%edx;" // or finalizes the floating point number
      "movl %%edx, %%eax" //float has the floating point number
     :"=a" (f)
     :"b" (sign), "c" (exponent), "d" (mantissa)
     :"cc"
    );
  return f;
}//packFloat
//

bool MyFloat::operator==(const float rhs) const{
MyFloat rhs_copy(rhs);
    if(sign == rhs_copy.sign)
           if(mantissa == rhs_copy.mantissa)
             if(exponent == rhs_copy.exponent)
                return true;

          return false;
}// == operator

