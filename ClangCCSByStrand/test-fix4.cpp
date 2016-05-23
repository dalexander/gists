#include <iostream>
#include <type_traits>
#include <string>
#include <sstream>
#include <stdexcept>

namespace {

template <typename T>
typename std::enable_if<std::is_same<T,const char*>::value, T>::type
convert(std::string s)
{
    std::cout << "String" << std::endl;
    return T();
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
convert(std::string s)
{
    std::cout << "Integral" << std::endl;
    return T();
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
convert(std::string s)
{
    std::cout << "Floating" << std::endl;
    return T();
}

} // namespace


class Value {
  public:
    Value(std::string str) : str_(str) {}

    template<typename T> operator T() { return convert<T>(str_); }

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
