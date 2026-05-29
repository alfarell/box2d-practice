#pragma once

#include <box2d/box2d.h>

bool operator==(const b2WorldId& lhs, const b2WorldId& rhs);
bool operator!=(const b2WorldId& lhs, const b2WorldId& rhs);

bool operator==(const b2BodyId& lhs, const b2BodyId& rhs);
bool operator!=(const b2BodyId& lhs, const b2BodyId& rhs);

bool operator==(const b2ShapeId& lhs, const b2ShapeId& rhs);
bool operator!=(const b2ShapeId& lhs, const b2ShapeId& rhs);
