#include "PlayerStream.hpp"

VectorPlayerStream::VectorPlayerStream(const std::vector<Player>& players)
    : data(players), index(0) {}

Player VectorPlayerStream::nextPlayer() {
    if (index >= data.size()) {
        throw std::runtime_error("No players in the stream");
    }
    return data[index++];
}

size_t VectorPlayerStream::remaining() const {
    return data.size() - index;
}