#include "Power.h"
#include "ObjectBank.h"

Power::Power(uint index, uint degree, VAR_TYPE varType)
: _index(index), _degree(degree), _vType(varType)
{
	COSTR("power");
}

void Power::Construct(uint index, uint degree, VAR_TYPE varType)
{
	_index = index;
	_degree = degree;
	_vType = varType;

	COSTR("reused power");
}


Power::~Power()
{
	DESTR("power");
}

Power* Power::Clone()
{
#ifdef POWER_BANK
	return PowerBank::PowerFactory.AcquirePower(_index, _degree, _vType);
#else
	return new Power(_index, _degree, _vType);
#endif
}

void Power::Dispose()
{
	_refCount --;

	if (_refCount == 0)
	{
#ifdef POWER_BANK
		PowerBank::PowerFactory.ReleaseObject(this);
#else
		delete this;
#endif
	}
}

uint Power::GetIndex() const
{
	return _index;
}

uint Power::GetDegree() const
{
	return _degree;
}

void Power::SetDegree(uint degree)
{
	_degree = degree;
}

int Power::Compare(Power* pow) const
{
	int retValue = 0;
	if (this->GetIndex() < pow->GetIndex())
	{
		retValue = -1;
	}
	else if (this->GetIndex() > pow->GetIndex())
	{
		retValue = 1;
	}
	else if (this->GetDegree() < pow->GetDegree())
	{
		retValue = -1;
	}
	else if (this->GetDegree() > pow->GetDegree())
	{
		retValue = 1;
	}

	return retValue;
}

// multiplication
void Power::Mul(Power* pow)
{
	_degree += pow->GetDegree();
}

// printing

//
// {index, degree}
void Power::PrettyPrint() const
{
	Log::PrintLogF(0, "{%d, %d}", _index, _degree);
}

//
// superscript of subscript
//
void Power::PrintLatex(StringBuilder* sb) const
{
#if 1
	// safe latex output
	// but output is larger than optimal
	sb->AddChar(_vType == VAR_TYPE_U ? 'u' : 'x');
	sb->AddString("_{", 2);
	sb->AddInt(_index);
	sb->AddChar('}');

	if (_degree > 1)
	{
		sb->AddString("^{", 2);
		sb->AddInt(_degree);
		sb->AddChar('}');
	}
#else
	sb->AddChar(_var);
	sb->AddChar('_');
	sb->AddInt(_index);

	if (_degree > 1)
	{
		sb->AddChar('^');
		sb->AddInt(_degree);
	}
#endif
}
