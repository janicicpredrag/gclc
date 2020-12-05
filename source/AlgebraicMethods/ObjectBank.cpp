#include "ObjectBank.h"
#include "Power.h"
#include "UTerm.h"
#include "TermStorageAvl.h"

#ifdef USE_OBJECT_BANKS
const int ObjectsArray::_size1 = 1024;
const int ObjectsArray::_step = 64;

ObjectsArray::ObjectsArray()
{
	_size = _step;
	_objects = (Object***) malloc (_size * sizeof(Object**));

	for (int ii = 0; ii < _size; ii++)
	{
		_objects[ii] = new Object*[_size1];

		for (int jj = 0; jj < _size1; jj++)
		{
			_objects[ii][jj] = NULL;
		}
	}
}

ObjectsArray::~ObjectsArray()
{
#if 0
	for (int ii = 0; ii < _size; ii++)
	{
		delete [] _objects[ii];
	}
	delete [] _objects;
#endif
}

Object* ObjectsArray::GetObject(int pos) const
{
	return _objects[pos / _size1][pos % _size1];
}

void ObjectsArray::SetObject(Object* object, int pos)
{
	if (object)
	{
		object->SetObjectId(pos);
	}
	_objects[pos / _size1][pos % _size1] = object;
}

void ObjectsArray::SwapObjects(int pos1, int pos2)
{
	Object* t = GetObject(pos1);
	SetObject(GetObject(pos2), pos1);
	SetObject(t, pos2);
}

int ObjectsArray::GetCount() const
{
	return _size1 * _size;
}

void ObjectsArray::Resize()
{
	//Log::PrintLogF(0, "Object bank is full (size %d)!\n", _size * _size1);

	_size += _step;

	_objects = (Object***) realloc(_objects, _size * sizeof(Object**));

	// swap last block with first block
	for (int ii = 0; ii < _step; ii++)
	{
		_objects[_size - _step + ii] = new Object*[_size1];

		// swap row (ii) with row (_size - _step + ii)
		for (int jj = 0; jj < _size1; jj++)
		{
			this->SetObject(_objects[ii][jj], (_size - _step + ii) * _size1 + jj);
			this->SetObject(NULL, ii * _size1 + jj);
		}
	}

	//Log::PrintLogF(0, "Object bank resized (new size %d)!\n", _size * _size1);
}


ObjectBank::ObjectBank()
{
	// no reusable objects
	_lastFreeIndex = -1;

	// no used objects
	_firstUsedIndex = _objectsArray.GetCount();
}

ObjectBank::~ObjectBank()
{
#if 0
	// dispose everything
	for (int ii = 0; ii <= _lastFreeIndex; ii++)
	{
		delete _objectsArray.GetObject(ii);
	}

	for (int ii = _firstUsedIndex; ii < _objectsArray.GetCount(); ii++)
	{
		delete _objectsArray.GetObject(ii);
	}
#endif
}

Object* ObjectBank::AcquireObject()
{
	// is there free object
	if (_lastFreeIndex >= 0)
	{
		// replace last free with last empty
		_objectsArray.SwapObjects(_lastFreeIndex--, --_firstUsedIndex);

		if (_lastFreeIndex + 1 >= _firstUsedIndex)
		{
			// last free index is -1
			// first used index is 0

			// object bank is full!
			// must resize it
			//Log::PrintLogF(0, "_lastFreeIndex = %d, _firstUsedIndex = %d", _lastFreeIndex, _firstUsedIndex);

			// first used index is size difference
			int diff = _objectsArray.GetCount();
			_objectsArray.Resize();
			_firstUsedIndex = _objectsArray.GetCount() - diff;
		}

		// return first used object
		return _objectsArray.GetObject(_firstUsedIndex);
	}

	// must create new object
	return NULL;
}

void ObjectBank::ReleaseObject(Object* obj)
{
	// increase reference count
	obj->AddRef();

	// replace with first used
	_objectsArray.SwapObjects(obj->GetObjectId(), _firstUsedIndex);

	// replace with first empty
	_objectsArray.SwapObjects(_firstUsedIndex++, ++_lastFreeIndex);
}

void ObjectBank::ReusableObject(Object* obj)
{
	// set to the first empty
	_objectsArray.SetObject(obj, ++_lastFreeIndex);
}

//
// Powers Bank
//

PowerBank PowerBank::PowerFactory;

Power* PowerBank::AcquirePower(int index, int degree, VAR_TYPE vt)
{
	Power* pw = (Power*)this->AcquireObject();

	if (pw == NULL)
	{
		pw = new Power(index, degree, vt);
		this->ReusableObject(pw);
		pw = (Power*)this->AcquireObject();
	}
	else
	{
		pw->Construct(index, degree, vt);
	}

	return pw;
}

//
// UTerms Bank
//
UTermsBank UTermsBank::UTermsFactory;

UTerm* UTermsBank::AcquireUTerm(REAL coeff)
{
	UTerm* ut = (UTerm*)this->AcquireObject();

	if (ut == NULL)
	{
		ut = new UTerm(coeff);
		this->ReusableObject(ut);
		ut = (UTerm*)this->AcquireObject();
	}
	else
	{
		ut->Construct(coeff);
	}

	return ut;
}

//
// XTerms Bank
//

//
// AvlNode Bank
//
AvlNodeBank AvlNodeBank::AvlNodeFactory;
AvlNode* AvlNodeBank::AcquireAvlNode(Term* t)
{	
	AvlNode* an = (AvlNode*)this->AcquireObject();

	if (an == NULL)
	{
		an = new AvlNode(t);
		this->ReusableObject(an);
		an = (AvlNode*)this->AcquireObject();
	}
	else
	{
		an->Construct(t);
	}

	return an;
}

//
// AvlTree Bank
//
AvlTreeBank AvlTreeBank::AvlTreeFactory;
TermStorageAvlTree* AvlTreeBank::AcquireAvlTree()
{	
	TermStorageAvlTree* at = (TermStorageAvlTree*)this->AcquireObject();

	if (at == NULL)
	{
		at = new TermStorageAvlTree();
		this->ReusableObject(at);
		at = (TermStorageAvlTree*)this->AcquireObject();
	}
	else
	{
		at->Construct();
	}

	return at;
}
#endif
