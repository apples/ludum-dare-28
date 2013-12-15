#include "entity.hpp"

int UIDCounter::value = 0;

const char* EntityE_DuplicateComponent::what() const noexcept
{
    return "Attempt to add duplicate component to entity.";
}

EntityComponentBase::~EntityComponentBase() = default;
