#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

int main() {
  std::vector<std::tuple<boost::uuids::uuid, std::string>> _roomsInfo {
    {boost::uuids::random_generator()(), "Room A"},
    {boost::uuids::random_generator()(), "Room B"},
    {boost::uuids::random_generator()(), "Room C"}
  };

  std::string rooms_info;
  for (int i = 0; i < _roomsInfo.size(); i++) {
    rooms_info += std::get<1>(_roomsInfo[i]) + ":" + boost::lexical_cast<std::string>(std::get<0>(_roomsInfo[i])) + ";";
  }
  rooms_info = "rooms_info;" + rooms_info.substr(0, rooms_info.size() - 1);

  std::cout << rooms_info << std::endl;

  return 0;
}
