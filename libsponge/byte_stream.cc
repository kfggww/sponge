#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _total(capacity), _free(capacity) { _buf.resize(_total); }

size_t ByteStream::write(const string &data) {
    size_t len_data = data.size();
    size_t len = len_data < _free ? len_data : _free;
    string data_buf = data.substr(0, len);

    for (size_t i = 0; i < len; i++) {
        _buf[_wptr] = data_buf[i];
        _wptr = (_wptr + 1) % _total;
    }
    _free -= len;
    _bytes_written += len;

    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t len_used = _total - _free;
    size_t len_result = len < len_used ? len : len_used;

    string result;
    result.resize(len_result);
    for (size_t i = 0, j = 0; i < len_result; i++) {
        j = (_rptr + i) % _total;
        result[i] = _buf[j];
    }

    return result;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t len_used = _total - _free;
    size_t len_pop = len < len_used ? len : len_used;

    _rptr = (_rptr + len_pop) % _total;
    _free += len_pop;
    _bytes_read += len_pop;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string data = peek_output(len);
    pop_output(len);
    return data;
}

void ByteStream::end_input() { _input_ended = true; }

bool ByteStream::input_ended() const { return _input_ended; }

size_t ByteStream::buffer_size() const { return _total - _free; }

bool ByteStream::buffer_empty() const { return _total == _free; }

bool ByteStream::eof() const { return buffer_empty() && _input_ended; }

size_t ByteStream::bytes_written() const { return _bytes_written; }

size_t ByteStream::bytes_read() const { return _bytes_read; }

size_t ByteStream::remaining_capacity() const { return _free; }
