// begin = adder.h 

#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

// Define MATHSLIB_API depending on platform and whether we are building or using the DLL
#if defined(_WIN32) || defined(_WIN64)
  #ifdef maths_STATIC
    // Building/using as a static library
    #define MATHSLIB_API
  #else
    #ifdef maths_EXPORTS
      // Building the DLL
      #define MATHSLIB_API __declspec(dllexport)
    #else
      // Using the DLL
      #define MATHSLIB_API __declspec(dllimport)
    #endif
  #endif
#else
  // Non-Windows platforms
  #define MATHSLIB_API
#endif

// function prototype including API keyword
MATHSLIB_API int add(int a, int b);

#endif

// end = adder.h 
