#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    DUMMY_CODE(data, index, eof);
}

size_t StreamReassembler::unassembled_bytes() const { return {}; }

bool StreamReassembler::empty() const { return {}; }

StreamReassembler::SubString::SubString() : _data(), _index(0), _eof(false) {}

StreamReassembler::SubString::SubString(const std::string &data, const uint64_t index, const bool eof)
    : _data(data), _index(index), _eof(eof) {}

bool StreamReassembler::SubString::overlap_with(const SubString &other) const {
    uint64_t a1 = _index;
    uint64_t b1 = a1 + _data.size() - 1;
    uint64_t a2 = other._index;
    uint64_t b2 = a2 + other._data.size() - 1;

    return (a1 <= a2 && a2 <= b1) || (a2 <= a1 && a1 <= b2);
}

bool StreamReassembler::SubString::operator<(const SubString &other) const { return _index < other._index; }

StreamReassembler::SubString StreamReassembler::SubString::merge_substrings(const SubString &str1,
                                                                            const SubString &str2) {
    uint64_t a1 = str1._index;
    uint64_t b1 = a1 + str1._data.size() - 1;
    uint64_t a2 = str2._index;
    uint64_t b2 = a2 + str2._data.size() - 1;

    std::string result;
    uint64_t index = 0;
    bool eof = false;

    if (a1 <= a2 && a2 <= b1) {
        result += str1._data;
        index = str1._index;
        if (b2 > b1) {
            result += str2._data.substr(b1 - a2 + 1);
            eof = str2._eof;
        } else {
            eof = str1._eof;
        }
    } else if (a2 <= a1 && a1 <= b2) {
        result += str2._data;
        index = str2._index;
        if (b2 < b1) {
            result += str1._data.substr(b2 - a1 + 1);
            eof = str1._eof;
        } else {
            eof = str2._eof;
        }
    }

    return SubString(result, index, eof);
}