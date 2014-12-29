#ifndef TWIST_EXCEPTIONS_HPP
#define TWIST_EXCEPTIONS_HPP

namespace twist {
  struct invalid_json_string_sequence {};
  struct invalid_unicode_character {};
  struct invalid_utf16_sequence {};
  struct invalid_utf8_sequence {};
  struct invalid_json {};
  struct not_ready {};
  struct unexpected_end {};
}

#endif

