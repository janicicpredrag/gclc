#include "Log.h"
#include "Power.h"
#include <iostream>
#include <memory>

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

std::shared_ptr<Power> Power::Clone()
{
	return std::make_shared<Power>(_index, _degree, _vType);
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
void Power::PrintLatex(std::ostream &os) const
{
#if 1
	// safe latex output
	// but output is larger than optimal
	os << (_vType == VAR_TYPE_U ? 'u' : 'x') << "_{" << _index << '}';

	if (_degree > 1)
	{
		os << "^{" << _degree << '}';
	}
#else
	os << _var << '_' << _index;

	if (_degree > 1)
	{
		os << '^' << _degree;
	}
#endif
}
