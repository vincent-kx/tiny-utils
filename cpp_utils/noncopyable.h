#ifndef __NON_COPYABLE_H__
#define __NON_COPYABLE_H__

#include "cppversion_macro.h"

#if(SUPPORT_CPP11)

class NonCopyable
{
protected:
	NonCopyable() = default;
	virtual ~NonCopyable() = default;

	NonCopyable(const NonCopyable &) = delete;
	NonCopyable & operator=(const NonCopyable &) = delete;
};

#else

class NonCopyable
{

protected:
	NonCopyable();
	virtual ~NonCopyable();

private:
	NonCopyable(const NonCopyable &){}
	NonCopyable & operator=(const NonCopyable &){}
};
#endif

#endif