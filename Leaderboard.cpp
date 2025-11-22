#include "Leaderboard.hpp"
#include <algorithm>
#include <stdexcept>
#include <queue>

static int partition(std::vector<Player>& arr, int left, int right) {
    Player pivot = arr[right];
    int i = left;
    for (int j = left; j < right; j++) {
        if (arr[j] > pivot) std::swap(arr[i++], arr[j]);
    }
    std::swap(arr[i], arr[right]);
    return i;
}

static void quickSelect(std::vector<Player>& arr, int left, int right, int k) {
    if (left >= right) return;
    int p = partition(arr, left, right);
    if (k == p) return;
    else if (k < p) quickSelect(arr, left, p - 1, k);
    else quickSelect(arr, p + 1, right, k);
}

RankingResult Offline::quickSelectRank(std::vector<Player> players) {
    int n = players.size();
    int topCount = n / 10;
    if (topCount == 0) return {};

    quickSelect(players, 0, n - 1, topCount - 1);

    std::vector<Player> top(players.begin(), players.begin() + topCount);
    std::sort(top.begin(), top.end(), std::greater<Player>());

    return top;
}

RankingResult Offline::heapRank(std::vector<Player> players) {
    int n = players.size();
    int topCount = n / 10;
    if (topCount == 0) return {};

    std::priority_queue<
        Player,
        std::vector<Player>,
        std::greater<Player>
    > minheap;

    for (auto& p : players) {
        if (minheap.size() < (size_t)topCount) minheap.push(p);
        else if (p > minheap.top()) {
            minheap.pop();
            minheap.push(p);
        }
    }

    RankingResult result;
    while (!minheap.empty()) {
        result.push_back(minheap.top());
        minheap.pop();
    }
    std::sort(result.begin(), result.end(), std::greater<Player>());
    return result;
}

void replaceMin(PlayerIterator begin, PlayerIterator end, const Player& newValue) {
    if (begin == end) return;
    *begin = newValue;

    auto parent = begin;
    while (true) {
        auto left = begin + (2 * (parent - begin) + 1);
        auto right = begin + (2 * (parent - begin) + 2);

        auto smallest = parent;
        if (left < end && *left < *smallest) smallest = left;
        if (right < end && *right < *smallest) smallest = right;

        if (smallest == parent) break;
        std::swap(*parent, *smallest);
        parent = smallest;
    }
}

RankingResult rankIncoming(PlayerStream& incoming_players, size_t reporting_interval) {
    std::vector<Player> heap;
    RankingResult results;

    while (incoming_players.remaining() > 0) {
        Player p = incoming_players.nextPlayer();

        if (heap.size() < 11) {
            heap.push_back(p);
            std::push_heap(heap.begin(), heap.end(), std::greater<Player>());
        } else if (p > heap.front()) {
            std::pop_heap(heap.begin(), heap.end(), std::greater<Player>());
            heap.back() = p;
            std::push_heap(heap.begin(), heap.end(), std::greater<Player>());
        }

        if (heap.size() == reporting_interval) {
            RankingResult snapshot = heap;
            std::sort(snapshot.begin(), snapshot.end(), std::greater<Player>());
            results.insert(results.end(), snapshot.begin(), snapshot.end());
        }
    }
    return results;
}