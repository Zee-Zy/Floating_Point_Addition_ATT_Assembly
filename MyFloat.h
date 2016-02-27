#ifndef MY_FLOAT_H

  #define MY_FLOAT_H

  
  #include <iostream>
 
  using namespace std;

  
  class MyFloat{

    public:

    	//constructors

      MyFloat();

      MyFloat(float f);

      MyFloat(const MyFloat & rhs);

      virtual ~MyFloat() {};

      
      //output

      friend ostream& operator<<(std::ostream& strm, const MyFloat& f);

      
      //comparisons

      bool smallerExponent(const MyFloat& rhs) const;

      bool smallerMantissa(const MyFloat& rhs) const;
 
      bool operator==(const MyFloat& rhs) const;

      bool operator==(const float rhs) const;

      
      
      //addition

      MyFloat& operator+=(const MyFloat& rhs);

      MyFloat operator+(const MyFloat& rhs) const;
 
      
      //subtraction

      MyFloat& operator-=(const MyFloat& rhs);

      MyFloat operator-(const MyFloat& rhs) const;
 
    
    private:

		  unsigned int sign;

		  unsigned int exponent;

		  unsigned int mantissa;

		  
		  void normalize();

		  
		  void unpackFloat(float f);

		  float packFloat() const;

};

  
  bool operator==(const float lhs, const MyFloat& rhs);

  
#endif
