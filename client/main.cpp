#include <iostream>
#include <boost/log/trivial.hpp>

int main() {
    BOOST_LOG_TRIVIAL(info) << "Client app startup";
    std::cout << "Hello, World!" << std::endl;
    return 0;
}