#ifndef __NON_COPYABLE_H__
#define __NON_COPYABLE_H__

class NonCopyable
{
protected:
	NonCopyable() = default;
	virtual ~NonCopyable() = default;

	NonCopyable(const NonCopyable &) = delete;
	NonCopyable & operator=(const NonCopyable &) = delete;
};

#endif