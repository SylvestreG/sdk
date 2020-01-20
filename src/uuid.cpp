//
// Created by syl on 1/20/20.
//
#include "uuid.h"
#include <cstring>
#include <random>

uuid::uuid() {
  std::random_device rd;

  std::uniform_int_distribution<uint32_t> dist32(0, UINT32_MAX);
  std::uniform_int_distribution<uint16_t> dist16(0, UINT16_MAX);
  std::uniform_int_distribution<uint8_t> dist8(0, UINT8_MAX);

  _time_low = dist32(rd);
  _time_mid = dist16(rd);
  _time_hi_and_version = dist16(rd);
  _clock_seq_hi_and_reserved = dist8(rd);
  _clock_seq_low = dist8(rd);
  for (int i = 0; i < 6; ++i)
    _node[i] = dist8(rd);

  _clock_seq_hi_and_reserved &= ~(1 << 6);
  _clock_seq_hi_and_reserved |= (1 << 7);
  _time_hi_and_version &= ~(1 << 12);
  _time_hi_and_version &= ~(1 << 13);
  _time_hi_and_version |= (1 << 14);
}

uuid::uuid(uuid const &uuid) { operator=(uuid); }

uuid const &uuid::operator=(uuid const &uuid) {
  if (this != &uuid) {
    _time_low = uuid._time_low;
    _time_mid = uuid._time_mid;
    _time_hi_and_version = uuid._time_hi_and_version;
    _clock_seq_hi_and_reserved = uuid._clock_seq_hi_and_reserved;
    _clock_seq_low = uuid._clock_seq_low;

    memcpy(&_node, uuid._node, sizeof(_node));
  }
  return *this;
}

#define DIFF_RETURN(a, b, field)                                               \
  do {                                                                         \
    if ((a).field != (b).field)                                                \
      return false;                                                            \
  } while (0)

bool uuid::operator==(uuid const &uuid) const {
  int res;

  /* Deal with NULL or equal pointers. */
  /* We have to compare the hard way. */
  DIFF_RETURN(*this, uuid, _time_low);
  DIFF_RETURN(*this, uuid, _time_mid);
  DIFF_RETURN(*this, uuid, _time_hi_and_version);
  DIFF_RETURN(*this, uuid, _clock_seq_hi_and_reserved);
  DIFF_RETURN(*this, uuid, _clock_seq_low);

  res = memcmp(_node, uuid._node, sizeof(_node));
  if (res)
    return false;
  return true;
}

std::string uuid::to_string() const {
  char uuid[37];
  int c;

  c = snprintf(uuid, 37, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
               _time_low, _time_mid, _time_hi_and_version,
               _clock_seq_hi_and_reserved, _clock_seq_low, _node[0], _node[1],
               _node[2], _node[3], _node[4], _node[5]);

  if (c < 0)
    return std::string();

  return std::string(uuid);
}
