#include "Prover.h"

void Prover::_ProofInitialize(double timeout)
{
	_maxt = 0;
	ITimeout::SetTimeout(timeout);
	ITimeout::StartSession();
}

void Prover::_ProofDeInitialize() const
{
	ITimeout::EndSession();
}

uint Prover::GetMaxTerms() const
{
	return _maxt;
}
