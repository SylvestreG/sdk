//
// Created by syl on 1/20/20.
//

#ifndef SDK__UUID_H_
#define SDK__UUID_H_

#include <cstdint>
#include <string>

constexpr std::size_t uuid_str_len = 36;

class uuid {
public:
  uuid();
  uuid(uuid const &uuid);
  uuid const &operator=(uuid const &uuid);

  bool operator==(uuid const &uuid) const;

  std::string to_string() const;

private:
  uint32_t _time_low;
  uint16_t _time_mid;
  uint16_t _time_hi_and_version;
  uint8_t _clock_seq_hi_and_reserved;
  uint8_t _clock_seq_low;
  uint8_t _node[6];
};

#endif // SDK__UUID_H_
