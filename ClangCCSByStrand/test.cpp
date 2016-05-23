#include <iostream>
#include <type_traits>
#include <string>
#include <sstream>
#include <stdexcept>

class Value {
  public:
    Value(std::string str) : str_(str) {}

    template<typename T,
             typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    operator T() {
        T t;
        if (std::istringstream(str_) >> t) {
            return t;
        } else {
            throw std::runtime_error("woops");
        }
    }

    operator const char*() {
        std::cout  << "In here" << std::endl;
        return str_.c_str(); }

private:
    std::string str_;
};


int main(int argc, char *argv[])
{
    Value v("0");

    bool b = (bool)v;
    const char* c = (const char*)v;

    std::cout << b << std::endl;
    return 0;
}
