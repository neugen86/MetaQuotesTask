#include "MyRange.h"

bool operator !=(const MyRange& lhs, const MyRange& rhs)
{
	return (lhs.begin != rhs.begin) || (lhs.end != rhs.end);
}

bool operator ==(const MyRange& lhs, const MyRange& rhs)
{
	return !(lhs != rhs);
}
