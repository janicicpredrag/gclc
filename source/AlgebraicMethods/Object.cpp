#include "Object.h"

Object::Object()
{
	_refCount = 1;
	_objectId = 0;
}
Object::~Object()
{
}

void Object::AddRef()
{
	++_refCount;
}

int Object::GetRefCount() const
{
	return _refCount;
}

void Object::Dispose()
{
	--_refCount;

	if (_refCount == 0)
	{
		delete this;
	}
}

int Object::GetObjectId() const
{
	return _objectId;
}

void Object::SetObjectId(int objId)
{
	_objectId = objId;
}

//
// print real number without trailing zeros
//
void Object::PrintREAL(REAL f)
{
	static char dst[37];
	sprintf(dst, "%.20lf", f);

	int len = 0;
	while (dst[len])
	{
		len ++;
	}
	--len;

	while (dst[len] == '0')
	{
		len --;
	}

	if (dst[len] == '.')
	{
		len --;
	}
	dst[len + 1] = 0;

	Log::PrintLogF(0, "%s", dst);
}
