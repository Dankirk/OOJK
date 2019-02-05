# OOJK
Excercise project for a university course

The object was to practise object oriented programming with C++ new features.
This project was done using Visual Studio 2017 Community Edition v15.9.6 with C++17 features.

Code comments done using Doxygen format at file, class and function level.
Aditional comments inside functions as needed.

## Tests performed

- Catch2 v2.6.0 standalone .hpp for unit tests and memory leak detection https://github.com/catchorg/Catch2/releases/tag/v2.6.0
  - 74/74 assertations passed.
  - No memory leaks detected. Catch2 itself seems to leak memory when using custom main() 
- OpenCppCoverage addon for VS2017. https://github.com/OpenCppCoverage/OpenCppCoverage
  - Reports 98% unit test code coverage. 
  - The missing 2% come from file reading/writing exceptions I was not able to produce programmatically
- CppCheck v1.85 for static code analysis http://cppcheck.sourceforge.net/
  - Reports std::transform should be used instead of custom loop in Playlist.cpp, but I believe the custom loop conveys meaning better
- VS2017 static code analyze tool with ruleset: C++ Core Check Rules
  - Reports warning C26426 for global static initialized variables for not calling non-constexpr functions, but this is not possible for std::string and std::map
