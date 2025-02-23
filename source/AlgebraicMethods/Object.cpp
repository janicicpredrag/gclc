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
