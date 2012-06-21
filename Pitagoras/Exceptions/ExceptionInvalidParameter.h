#ifndef EXCEPTION_INVALID_PARAMETER
#define EXCEPTION_INVALID_PARAMETER

#include "Exception.h"

class CExceptionInvalidParameter:
	public CException
{
public:
	string m_sParameterName;

	CExceptionInvalidParameter(const string& sParameterName):
		m_sParameterName(sParameterName)
	{
	}
};

#endif
