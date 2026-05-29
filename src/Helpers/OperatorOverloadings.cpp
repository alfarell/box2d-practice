#include "OperatorOverloadings.hpp"

bool operator==(const b2WorldId& lhs, const b2WorldId& rhs) {
    return (lhs.index1 == rhs.index1) && (lhs.generation == rhs.generation);
}

bool operator!=(const b2WorldId& lhs, const b2WorldId& rhs) {
    return !(lhs == rhs);
}

bool operator==(const b2BodyId& lhs, const b2BodyId& rhs) {
    return (lhs.index1 == rhs.index1) && (lhs.world0 == rhs.world0) &&
           (lhs.generation == rhs.generation);
}

bool operator!=(const b2BodyId& lhs, const b2BodyId& rhs) {
    return !(lhs == rhs);
}

bool operator==(const b2ShapeId& lhs, const b2ShapeId& rhs) {
    return (lhs.index1 == rhs.index1) && (lhs.world0 == rhs.world0) &&
           (lhs.generation == rhs.generation);
}

bool operator!=(const b2ShapeId& lhs, const b2ShapeId& rhs) {
    return !(lhs == rhs);
}