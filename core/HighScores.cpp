#include "HighScores.h"

#include <algorithm>
#include <fstream>
#include <functional>

namespace core {
namespace {
constexpr std::size_t maxEntries = 5;
} // namespace

HighScores::HighScores(std::string path) : path(std::move(path)) {
    std::ifstream file(this->path);
    if (!file.is_open()) {
        return; // no file yet (e.g. a fresh checkout) -- start with an empty list, not an error
    }

    std::string line;
    while (std::getline(file, line)) {
        try {
            scores.push_back(std::stoi(line));
        } catch (const std::exception&) {
            continue; // a corrupt or non-numeric line is skipped rather than treated as fatal
        }
    }

    sortAndTrim();
}

void HighScores::sortAndTrim() {
    std::sort(scores.begin(), scores.end(), std::greater<>());
    if (scores.size() > maxEntries) {
        scores.resize(maxEntries);
    }
}

void HighScores::record(const int score) {
    scores.push_back(score);
    sortAndTrim();

    std::ofstream file(path, std::ios::trunc);
    if (!file.is_open()) {
        return; // best-effort: the in-memory list is still updated even if the save fails
    }

    for (const int value : scores) {
        file << value << '\n';
    }
}

} // namespace core
