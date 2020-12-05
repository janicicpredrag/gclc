#pragma once

/*************************************************************
*
* ObjectBank.h
*
* Factory for creating and disposing objects that are used
* frequently in the project (like powers, terms, polynomials)
*
**************************************************************/

#include "Object.h"
#include "Power.h"

#ifdef USE_OBJECT_BANKS
//
// Matrix-like array
//
class ObjectsArray
{
private:
	// first dimension
	static const int _size1;

	// step for increasing second dimension
	static const int _step;

	// second dimension
	int _size;

	// matrix
	Object*** _objects;


public:
	ObjectsArray();
	~ObjectsArray();

	Object* GetObject(int pos) const;
	void SetObject(Object* object, int pos);
	void SwapObjects(int pos1, int pos2);

	void Resize();

	int GetCount() const;
};

class ObjectBank
{
protected:
	// separation between used and free objects
	// [ free objects ][ empty objects ][ used objects ]
	int _lastFreeIndex;
	int _firstUsedIndex;

	// objects array
	ObjectsArray _objectsArray;

public:
	ObjectBank();
	~ObjectBank();

	Object* AcquireObject();
	void ReleaseObject(Object* obj);
	void ReusableObject(Object* obj);
};

class PowerBank : public ObjectBank
{
public:
	static PowerBank PowerFactory;
	Power* AcquirePower(int index, int degree, VAR_TYPE vt);
};

class UTerm;
class UTermsBank : public ObjectBank
{
public:
	static UTermsBank UTermsFactory;
	UTerm* AcquireUTerm(REAL coeff = 1);
};

#if 0
class XTermsBank : public ObjectBank
{
public:
	static XTermsBank XTermsFactory;
	XTerm* AcquireXTerm();
};
#endif

class AvlNode;
class Term;
class AvlNodeBank : public ObjectBank
{
public:
	static AvlNodeBank AvlNodeFactory;
	AvlNode* AcquireAvlNode(Term* t = NULL);
};

class TermStorageAvlTree;
class AvlTreeBank : public ObjectBank
{
public:
	static AvlTreeBank AvlTreeFactory;
	TermStorageAvlTree* AcquireAvlTree();
};
#endif
