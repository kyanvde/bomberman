#include "Observer.h"

namespace core {

Observer::~Observer() = default;

void Observer::render(AbstractRenderer&) const {}

} // namespace core
