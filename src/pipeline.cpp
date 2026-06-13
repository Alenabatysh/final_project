#include "pipeline.h"
#include <stdexcept>

void Pipeline::add(std::unique_ptr<IFilter> filter) {
    if (!filter) {
        throw std::invalid_argument("Cannot add null filter");
    }
    filters_.push_back(std::move(filter));
}

void Pipeline::apply(Waveform& wav) const {
    for (const auto& filter : filters_) {
        filter->apply(wav);
    }
}

void Pipeline::clear() {
    filters_.clear();
}

bool Pipeline::empty() const {
    return filters_.empty();
}

std::size_t Pipeline::size() const {
    return filters_.size();
}

void Pipeline::reserve(std::size_t capacity) {
    filters_.reserve(capacity);
}