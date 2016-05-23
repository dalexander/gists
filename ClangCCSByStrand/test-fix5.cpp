#include <iostream>
#include <type_traits>
#include <string>
#include <sstream>
#include <stdexcept>

namespace {

const char* stringConvert(std::string s)
{
    std::cout << "String" << std::endl;
    return "";
}

template <typename T>
T integralConvert(std::string s)
{
    std::cout << "Integral" << std::endl;
    return T();
}

template <typename T>
T floatingPointConvert(std::string s)
{
    std::cout << "Floating" << std::endl;
    return T();
}

} // namespace


class Value {
  public:
    Value(std::string str) : str_(str) {}

    operator bool()         { return integralConvert<bool>(str_); }
    operator float()        { return floatingPointConvert<float>(str_); }
    operator double()       { return floatingPointConvert<double>(str_); }
    operator int()          { return integralConvert<int>(str_); }
    operator unsigned int() { return integralConvert<unsigned int>(str_); }
    operator const char*()  { return stringConvert(str_); }

private:
    std::string str_;
};



int main(int argc, char *argv[])
{
    Value v("0");

    bool b = (bool)v;
    const char* c = (const char*)v;
    float f = (float)v;

    std::cout << b << std::endl;


    return 0;
}
