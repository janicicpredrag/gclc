#pragma once

#include "stdinc.h"
#include "Log.h"

/*************************************************************
*
* Object.h
*
* Base class for all objects in the project.
* Provides object reference count methods.
*
**************************************************************/

class Object
{
protected:
	int _refCount;
	int _objectId;

public:

	Object();
	virtual ~Object();

	void AddRef();
	int GetRefCount() const;
	virtual void Dispose();

	int GetObjectId() const;
	void SetObjectId(int objId);

	static void PrintREAL(REAL f);
};
