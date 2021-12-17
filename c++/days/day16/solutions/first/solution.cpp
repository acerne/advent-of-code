

#include "solution.hpp"

#include <bitset>
#include <fstream>
#include <map>
#include <sstream>

namespace
{

enum class PacketType : uint8_t {
  SUM = 0,
  PRODUCT = 1,
  MINIMUM = 2,
  MAXIMUM = 3,
  VALUE = 4,
  GREATER = 5,
  LESS = 6,
  EQUALS = 7
};

static const std::map<char, std::string> hex_to_bin_{
  {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"}, {'5', "0101"},
  {'6', "0110"}, {'7', "0111"}, {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
  {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"},
};

static const std::map<PacketType, std::string> type_id_symbol_{
  {PacketType::SUM, "+"},       {PacketType::PRODUCT, "*"}, {PacketType::MINIMUM, "min"},
  {PacketType::MAXIMUM, "max"}, {PacketType::VALUE, "#"},   {PacketType::GREATER, ">"},
  {PacketType::LESS, "<"},      {PacketType::EQUALS, "=="}};

class BitBuffer
{
public:
  BitBuffer(const std::string& source)
    : m_source(source)
    , m_index(0) {}

  std::string next(size_t n) {
    std::string sequenece = m_source.substr(m_index, n);
    m_index += n;
    return sequenece;
  }

  PacketType next_type() {
    return static_cast<PacketType>(std::stoi(next(3), nullptr, 2));
  }

  uint64_t next_int(size_t n) {
    return std::stoul(next(n), nullptr, 2);
  }

  bool next_bool() {
    return m_source.at(m_index++) != '0';
  }

  void round_up() {
    m_index = ((m_index + bit_stride) / bit_stride) * bit_stride;
  }

  size_t any_left() const {
    return m_index < (m_source.size() - 1);
  }

  size_t index() const {
    return m_index;
  }

private:
  const std::string m_source;
  const size_t bit_stride = 8;
  size_t m_index;
};

struct Packet {
  uint8_t version;
  PacketType type_id;
  uint64_t value;
  std::vector<Packet> subpackets;
};

Packet recursive_decoder(BitBuffer& buffer) {
  Packet packet;
  packet.version = buffer.next_int(3);
  packet.type_id = buffer.next_type();
  if (packet.type_id == PacketType::VALUE) { // literal value packet
    bool last = false;
    std::string temp{};
    while (!last) {
      last = !buffer.next_bool();
      temp += buffer.next(4);
    }
    packet.value = BitBuffer(temp).next_int(temp.size());
  } else { // operator packet
    if (!buffer.next_bool()) {
      const uint16_t length = buffer.next_int(15);
      const auto current_index = buffer.index();
      do {
        const auto subpacket = recursive_decoder(buffer);
        packet.subpackets.push_back(std::move(subpacket));
      } while (buffer.index() < current_index + length);
    } else {
      const uint16_t count = buffer.next_int(11);
      for (int i = 0; i < count; i++) {
        const auto subpacket = recursive_decoder(buffer);
        packet.subpackets.push_back(std::move(subpacket));
      }
    }
    switch (packet.type_id) {
    case PacketType::SUM: {
      packet.value = 0;
      for (const auto& subpacket : packet.subpackets) {
        packet.value += subpacket.value;
      }
    } break;
    case PacketType::PRODUCT: {
      packet.value = 1;
      for (const auto& subpacket : packet.subpackets) {
        packet.value *= subpacket.value;
      }
    } break;
    case PacketType::MINIMUM: {
      auto min = std::numeric_limits<uint64_t>::max();
      for (const auto& subpacket : packet.subpackets) {
        min = std::min(min, subpacket.value);
      }
      packet.value = min;
    } break;
    case PacketType::MAXIMUM: {
      auto max = std::numeric_limits<uint64_t>::min();
      for (const auto& subpacket : packet.subpackets) {
        max = std::max(max, subpacket.value);
      }
      packet.value = max;
    } break;
    case PacketType::GREATER: {
      packet.value = 0;
      if (packet.subpackets.at(0).value > packet.subpackets.at(1).value) {
        packet.value = 1;
      }
    } break;
    case PacketType::LESS: {
      packet.value = 0;
      if (packet.subpackets.at(0).value < packet.subpackets.at(1).value) {
        packet.value = 1;
      }
    } break;
    case PacketType::EQUALS: {
      packet.value = 0;
      if (packet.subpackets.at(0).value == packet.subpackets.at(1).value) {
        packet.value = 1;
      }
    } break;
    default:
      throw std::runtime_error("Unsupported packet");
    }
  }
  return packet;
} // namespace

size_t recursive_version_sum(const Packet& packet) {
  size_t sum = packet.version;
  for (const auto& subpacket : packet.subpackets) {
    sum += recursive_version_sum(subpacket);
  }
  return sum;
}

std::vector<Packet> decode(BitBuffer& buffer) {
  std::vector<Packet> packets;
  while (buffer.any_left()) {
    const auto packet = recursive_decoder(buffer);
    packets.push_back(std::move(packet));
    buffer.round_up();
  }
  return packets;
}

void recursive_draw(const Packet& packet, size_t depth) {
  std::ostringstream oss;
  oss << std::string(depth * 3, ' ') << "─";
  oss << '[' << int(packet.version) << "] ";
  oss << type_id_symbol_.at(packet.type_id) << ' ';
  oss << packet.value << ' ';

  std::cout << oss.str() << std::endl;

  for (const auto& subpacket : packet.subpackets) {
    recursive_draw(subpacket, depth + 1);
  }
}

void draw(const std::vector<Packet>& packets) {
  for (const auto& packet : packets) {
    recursive_draw(packet, 0);
  }
  std::cout << std::endl;
}

} // namespace

Input Solution::read(const std::string& file_path) {
  Input inputs;
  std::ifstream line_stream(file_path);
  std::string line;
  while (std::getline(line_stream, line)) {
    std::string bits;
    for (const auto& c : line) {
      bits += hex_to_bin_.at(c);
    }
    inputs.push_back(bits);
  }
  return inputs;
}

Result Solution::Part1::process(const Input& input) {
  Result result;
  for (const auto& line : input) {
    BitBuffer buffer(line);
    const auto packets = decode(buffer);
    size_t sum = 0;
    for (const auto packet : packets) {
      sum += recursive_version_sum(packet);
    }
    result.push_back(sum);
  }
  return result;
}

Result Solution::Part2::process(const Input& input) {
  Result result;
  for (const auto& line : input) {
    BitBuffer buffer(line);
    const auto packets = decode(buffer);
    result.push_back(packets.front().value);
    // draw(packets);
  }
  return result;
}

extern "C" Solution* create_solution() {
  return new Solution();
}

extern "C" void destroy_solution(Solution* object) {
  delete object;
}