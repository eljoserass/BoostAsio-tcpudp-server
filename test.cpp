#include <msgpack.hpp>
#include <iostream>
#include <sstream>
#include <string>

std::string hexToBinary(const std::string& hex) {
    std::string binary;
    binary.reserve(hex.length() / 2);
    for (std::string::size_type i = 0; i < hex.length(); i += 2) {
        int byte = std::stoi(hex.substr(i, 2), nullptr, 16);
        binary.push_back(static_cast<char>(byte));
    }
    return binary;
}

int main() {
    std::string str = "Hello, World!";

    std::stringstream buffer;
    msgpack::pack(buffer, str);
    std::string result;
    std::string binary = buffer.str();

    for (int i = 1; i < binary.size(); i++) {
        char hex[3];
        sprintf(hex, "%02x", (unsigned char)binary[i]);
        result.append(hex);
    }
    std::cout << result << std::endl;

    std::string result2 = hexToBinary(result);
    std::cout << result2 << std::endl;
    
    return 0;
}
