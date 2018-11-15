#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "cstr.h"
#include "cs_malloc.h"

// gcc -o cstr cstr.c -g -I/usr/lib/gcc/x86_64-redhat-linux/4.4.4/include/ -std=c99

Cstr * cstr_new_str(const char * str)
{
	Cstr * cstr = NULL;
	if(NULL == str)
	{
		cstr = (Cstr *)malloc(_CSTR_MIN_SIZE);
		if(NULL == cstr) return NULL;
		cstr->cap = _CSTR_MIN_SIZE - (CSTR_HDR_SIZE + 1);
		cstr->data[0] = 0;//set last byte to '\0' just like a normal c string
		cstr->used = 0;
		CSTR_SET_END(cstr);
	}
	else
	{
		uint32_t size = strlen(str);
		// printf("init str=%s,size=%u\n", str,size);
		cstr = cstr_new_size(size);
		memcpy(cstr->data,str,size);
		cstr->used = size;
		CSTR_SET_END(cstr);
	}
	return cstr;
}

Cstr * cstr_new_size(uint32_t cap)
{
	cap = round_up_power_2(cap+CSTR_HDR_SIZE+1);
	if(0 == cap || cap > _CSTR_MAX_SIZE)
	{
		return NULL;
	}
	if(cap < _CSTR_MIN_SIZE)
	{
		cap = _CSTR_MIN_SIZE;
	}
	Cstr * cstr = (Cstr *)malloc(cap);
	if(NULL == cstr) return NULL;
	cstr->cap = cap - (CSTR_HDR_SIZE + 1);
	cstr->used = 0;
	CSTR_SET_END(cstr);
	return cstr;
}

/*
copy a new cstr just exactly the same as 'cstr'
*/
Cstr * cstr_new(const Cstr * cstr)
{
	if(NULL != cstr) 
		assert(NULL != cstr->data);
	if(NULL == cstr)
	{
		Cstr * copy = (Cstr *)malloc(_CSTR_MIN_SIZE);
		if(NULL == copy) return NULL;
		copy->used = 0;
		copy->cap = _CSTR_MIN_SIZE - (CSTR_HDR_SIZE + 1);
		memset(copy->data,0,copy->cap+1);
		return copy;
	}
	Cstr * copy = (Cstr *)malloc(cstr->cap + CSTR_HDR_SIZE + 1);
	if(NULL == copy)
		return NULL;
	copy->cap = cstr->cap;
	copy->used = cstr->used;
	memcpy(copy->data,cstr->data,cstr->cap+1);
	CSTR_SET_END(copy);
	return copy;
}
/*
neighter 'cstr1' nor 'cstr2' can be NULL,or program will core,
binary compare till one 'used' size of the two cstr reached
*/
int cstr_compare(const Cstr * cstr1,const Cstr * cstr2)
{
	assert(cstr_is_valid(cstr1) && cstr_is_valid(cstr2));
	uint32_t i = 0;
	for(;i < cstr1->used && i < cstr2->used; ++i)
	{
		if(cstr1->data[i] != cstr2->data[i]) 
			break;
	}
	if(i == cstr1->used && i == cstr2->used)
	{
		return 0;
	}
	else if(i == cstr1->used)
	{
		return -1;
	}
	else if(i == cstr2->used)
	{
		return 1;
	}
	return cstr1->data[i] - cstr2->data[i];
}
int cstr_find_first(const Cstr * cstr,const char * str,int begin)
{
	return sunday_search_first(cstr,str,begin);
}
/*
neighter 'cstr' nor 'str' can be NULL,or program will core
binary compare just like strcmp
*/
int cstr_compare_str(const Cstr * cstr, const char * str)
{
	assert(cstr_is_valid(cstr) && NULL != str);
	return strcmp(cstr->data,str);
}

bool cstr_is_start_with(const Cstr * cstr,const char * str)
{
	assert(cstr_is_valid(cstr) && NULL != str);
	uint32_t len = strlen(str);
	if(len > cstr->used)
		return false;
	if(0 == len) return true;
	for(uint32_t i = 0; i < len; ++i)
	{
		if(cstr->data[i] != str[i])
			return false;
	}
	return true;
}

Cstr * cstr_assign(Cstr * dst,const Cstr * src)
{
	assert(cstr_is_valid(src) && cstr_is_valid(dst));
	uint32_t new_cap = cal_fit_cap(CSTR_TOTAL_SIZE(dst),CSTR_TOTAL_USED(src));
	printf("new_cap:%u\n", new_cap);
	if(new_cap < _CSTR_MIN_SIZE || new_cap > _CSTR_MAX_SIZE) 
		return NULL;
	if(new_cap != CSTR_TOTAL_SIZE(dst))
	{
		dst = (Cstr *)realloc(dst,new_cap);
		if(NULL == dst) 
			return NULL;
	}
	dst->cap = new_cap - (CSTR_HDR_SIZE + 1);
	dst->used = src->used;
	memcpy(dst->data,src->data,src->used);
	memset((dst->data + dst->used),0,(dst->cap - dst->used));
	CSTR_SET_END(dst);
	return dst;
}
/*
src len should not over 512M,otherwise the behavior is undefined,
return value -1 indicate dst is NULL or src is over 512M
if src is NULL,nothing will happen
*/
Cstr * cstr_assign_str(Cstr * dst,const char * src)
{
	assert(cstr_is_valid(dst));
	if(src == NULL) return 0;

	uint32_t src_len = strlen(src);
	uint32_t total_used = src_len + CSTR_HDR_SIZE + 1;
	uint32_t new_cap = cal_fit_cap(CSTR_TOTAL_SIZE(dst),total_used);
	if(new_cap < _CSTR_MIN_SIZE || new_cap > _CSTR_MAX_SIZE) 
		return NULL;
	if(new_cap != CSTR_TOTAL_SIZE(dst))
	{
		dst = (Cstr *)realloc(dst,new_cap);
		if(NULL == dst) 
			return NULL;
	}
	dst->cap = new_cap - (CSTR_HDR_SIZE + 1);
	dst->used = src_len;
	memcpy(dst->data,src,src_len);
	memset((dst->data + dst->used),0,(dst->cap - dst->used));
	CSTR_SET_END(dst);
	return dst;
}

Cstr * cstr_copy(Cstr * dst,const Cstr * src,uint32_t begin, uint32_t len)
{
	assert(cstr_is_valid(dst) && cstr_is_valid(src));
	if(len == 0 || len > src->used) len = src->used;
	uint32_t total_used = len + CSTR_HDR_SIZE + 1;
	uint32_t new_cap = cal_fit_cap(CSTR_TOTAL_SIZE(dst),total_used);
	if(new_cap < _CSTR_MIN_SIZE || new_cap > _CSTR_MAX_SIZE)
		return NULL;
	if(new_cap != CSTR_TOTAL_SIZE(dst))
	{
		dst = (Cstr *)realloc(dst,new_cap);
		if(NULL == dst) 
			return NULL;
	}
	dst->cap = new_cap - (CSTR_HDR_SIZE + 1);
	dst->used = len > dst->used ? len : dst->used;
	memcpy(dst->data,(src->data + begin),len);
	CSTR_SET_END(dst);
	return dst;
}

Cstr * cstr_copy_str(Cstr * dst,const char * src,uint32_t begin, uint32_t len)
{
	assert(cstr_is_valid(dst));
	if(NULL == src)
		return dst;
	uint32_t src_len = strlen(src);
	if(src_len == 0)
		return dst;
	if(len == 0 || len > src_len) len = src_len;
	uint32_t total_used = len + CSTR_HDR_SIZE + 1;
	uint32_t new_cap = cal_fit_cap(CSTR_TOTAL_SIZE(dst),total_used);
	if(new_cap < _CSTR_MIN_SIZE || new_cap > _CSTR_MAX_SIZE)
		return NULL;
	if(new_cap != CSTR_TOTAL_SIZE(dst))
	{
		dst = (Cstr *)realloc(dst,new_cap);
		if(NULL == dst) 
			return NULL;
	}
	dst->cap = new_cap - (CSTR_HDR_SIZE + 1);
	dst->used = len > dst->used ? len : dst->used;
	memcpy(dst->data,src+begin,len);
	return dst;
}

Cstr * cstr_sub_cstr(Cstr * substr,const Cstr * cstr,const uint32_t begin, uint32_t len)
{
	assert(cstr_is_valid(substr) && cstr_is_valid(cstr));
	if(begin > cstr->used-1 || len == 0 || len > cstr->used)
		return NULL;
	if(len + begin > cstr->used)
	{
		len = cstr->used - begin;
	}
	uint32_t total_used = len + CSTR_HDR_SIZE + 1;
	uint32_t new_cap = cal_fit_cap(CSTR_TOTAL_SIZE(substr),total_used);
	if(new_cap < _CSTR_MIN_SIZE || new_cap > _CSTR_MAX_SIZE)
		return NULL;
	if(new_cap != CSTR_TOTAL_SIZE(substr))
	{
		substr = (Cstr *)realloc(substr,new_cap);
		if(NULL == substr) 
			return NULL;
	}
	substr->cap = new_cap - (CSTR_HDR_SIZE + 1);
	substr->used = len;
	memcpy(substr->data,cstr->data+begin,len);
	CSTR_SET_END(substr);
	return substr;
}

Cstr * cstr_sub_str(Cstr * substr,const char * str,const uint32_t begin,uint32_t len)
{
	assert(cstr_is_valid(substr) && NULL != str);
	uint32_t src_len = strlen(str);
	if(begin > src_len-1 || len == 0 || len > src_len)
		return NULL;
	if(len + begin > src_len)
	{
		len = src_len - begin;
	}
	uint32_t total_used = len + CSTR_HDR_SIZE + 1;
	uint32_t new_cap = cal_fit_cap(CSTR_TOTAL_SIZE(substr),total_used);
	if(new_cap < _CSTR_MIN_SIZE || new_cap > _CSTR_MAX_SIZE)
		return NULL;
	if(new_cap != CSTR_TOTAL_SIZE(substr))
	{
		substr = (Cstr *)realloc(substr,new_cap);
		if(NULL == substr) 
			return NULL;
	}
	substr->cap = new_cap - (CSTR_HDR_SIZE + 1);
	substr->used = len;
	memcpy(substr->data,str+begin,len);
	CSTR_SET_END(substr);
	return substr;
}

Cstr * cstr_insert(Cstr * dst,const uint32_t pos,const Cstr * istr,const uint32_t begin,uint32_t len)
{
	assert(cstr_is_valid(dst) && cstr_is_valid(istr));
	if(begin > istr->used-1)
		return dst;
	if(len == 0)
		len = istr->used;
	if(begin + len > istr->used)
	{
		len = istr->used - begin;
	}
	uint32_t total_used = dst->used + len;
	uint32_t old_used = dst->used;
	if(total_used > dst->cap)
	{
		uint32_t new_cap = cal_fit_cap(CSTR_TOTAL_SIZE(dst),total_used+CSTR_HDR_SIZE+1);
		if(new_cap < _CSTR_MIN_SIZE)
			return NULL;
		if(new_cap != dst->cap)
		{
			dst = (Cstr *)realloc(dst,new_cap);
			if(dst == NULL)
				return NULL;
		}
		dst->cap = new_cap - (CSTR_HDR_SIZE + 1);
	}
	dst->used = total_used;
	memmove((dst->data + pos + len),(dst->data + pos),old_used-pos);
	memcpy((dst->data + pos),(istr->data + begin),len);
	CSTR_SET_END(dst);
	return dst;
}

Cstr * cstr_insert_str(Cstr * dst,const uint32_t pos,const char * str,const uint32_t begin,uint32_t len)
{
	assert(cstr_is_valid(dst));
	if(NULL == str)
		return dst;

	uint32_t src_len = strlen(str);
	if(begin > src_len-1)
		return dst;
	if(len == 0)
		len = src_len;
	if(begin + len > src_len)
	{
		len = src_len - begin;
	}

	uint32_t total_used = dst->used + len;
	uint32_t old_used = dst->used;
	if(total_used > dst->cap)
	{
		uint32_t new_cap = cal_fit_cap(CSTR_TOTAL_SIZE(dst),total_used+CSTR_HDR_SIZE+1);
		if(new_cap < _CSTR_MIN_SIZE)
				return NULL;
		if(new_cap != dst->cap)
		{
			dst = (Cstr *)realloc(dst,new_cap);
			if(dst == NULL)
				return NULL;
		}
		dst->cap = new_cap - (CSTR_HDR_SIZE + 1);
	}
	dst->used = dst->used + len;
	memmove((dst->data + pos + len),(dst->data + pos),old_used-pos);
	memcpy((dst->data + pos),(str + begin),len);
	CSTR_SET_END(dst);
	return dst;
}

int cstr_reverse(Cstr * cstr)
{
	assert(cstr_is_valid(cstr));

	if(cstr->used == 1)
		return 0;
	int beg = 0,end = cstr->used;
	while(beg < end-1)
	{
		char tmp = cstr->data[beg];
		cstr->data[beg] = cstr->data[end-1];
		cstr->data[end-1] = tmp;
		++beg;
		--end;
	}
	return 0;
}

void cstr_free(Cstr * cstr)
{
	if(NULL == cstr) return;
	free(cstr);
	// cstr = NULL;//cauz param pass by value,no need
}

uint32_t cstr_count_char(const Cstr * cstr,const char c,uint32_t begin,uint32_t end)
{
	assert(cstr_is_valid(cstr));
	uint32_t count = 0;
	if(end == 0) end = cstr->used;
	for(uint32_t i = begin; i < end && i < cstr->used; ++i)
	{
		if(cstr->data[i] == c)
			++count;
	}
	return count;
}

uint32_t cstr_count_str(const Cstr * cstr,const char * str,uint32_t begin)
{
	assert(cstr_is_valid(cstr) && NULL != str);
	uint32_t len = strlen(str);
	if(len > cstr->used - begin || len == 0)
		return 0;
	uint32_t count = 0;
	while(begin < cstr->used && -1 != (begin = cstr_find_first(cstr,str,begin)))
	{
		++count;
		begin = begin + len + 1;
	}
	return count;
}

int cstr_find_char(const Cstr * cstr,const char c,int begin,int end)
{
	assert(cstr_is_valid(cstr));
	if(end == 0) end = cstr->used;
	for(int i = begin; (uint32_t)i < cstr->used && i < end; ++i)
	{
		if(cstr->data[i] == c)
			return i;
	}
	return -1;
}

Cstr * cstr_append(Cstr * dst,const Cstr * str)
{
	assert(cstr_is_valid(dst) && cstr_is_valid(str));
	if(str->used == 0) 
		return dst;
	uint32_t total_cap = CSTR_TOTAL_SIZE(dst) + str->used;
	if(total_cap > _CSTR_MAX_SIZE)
		return NULL;
	uint32_t new_cap = cal_fit_cap(CSTR_TOTAL_SIZE(dst),CSTR_TOTAL_USED(dst) + str->used);
	if(new_cap < _CSTR_MIN_SIZE)
		return NULL;
	if(new_cap - (CSTR_HDR_SIZE + 1) > dst->cap)
	{
		dst = (Cstr *)realloc(dst,new_cap);
		if(NULL == dst)
			return NULL;
	}
	memcpy(dst->data + dst->used,str->data,str->used);
	dst->cap = new_cap - (CSTR_HDR_SIZE + 1);
	dst->used = dst->used + str->used;
	memset(dst->data + dst->used,0,dst->cap - dst->used);
	return dst;
}

Cstr * cstr_append_str(Cstr * dst,const char * str)
{
	if(NULL == str) return 0;
	assert(cstr_is_valid(dst));
	uint32_t append_len = strlen(str);
	if(append_len == 0)
		return dst;
	uint32_t total_cap = CSTR_TOTAL_SIZE(dst) + append_len;
	if(total_cap > _CSTR_MAX_SIZE)
		return NULL;
	uint32_t new_cap = cal_fit_cap(CSTR_TOTAL_SIZE(dst),CSTR_TOTAL_USED(dst)+append_len);
	if(new_cap < _CSTR_MIN_SIZE)
		return NULL;
	if(new_cap - (CSTR_HDR_SIZE + 1) > dst->cap)
	{
		dst = (Cstr *)realloc(dst,new_cap);
		if(NULL == dst)
			return NULL;
	}
	memcpy(dst->data + dst->used,str,append_len);
	dst->cap = new_cap - (CSTR_HDR_SIZE + 1);
	dst->used = dst->used + append_len;
	memset(dst->data + dst->used,0,dst->cap - dst->used);
	return dst;
}

uint32_t cstr_length(const Cstr * cstr)
{
	assert(cstr_is_valid(cstr));
	return cstr->used;
}

uint32_t cstr_unused_size(const Cstr * cstr)
{
	assert(cstr_is_valid(cstr));
	return cstr->cap - cstr->used;
}

void cstr_clear(Cstr * cstr)
{
	assert(cstr_is_valid(cstr));
	memset(cstr->data,0,cstr->cap);
	cstr->used = 0;
}

int at(const Cstr * cstr,uint32_t i)
{
	assert(cstr_is_valid(cstr));
	if(i > cstr->used-1)
		return -1;
	return cstr->data[i];
}

Cstr * cstr_trim(Cstr * cstr)
{
	assert(cstr_is_valid(cstr));
	if(cstr->used == 0)
		return cstr;
	uint32_t old_used = cstr->used;
	uint32_t beg = 0,end = cstr->used;
	for(; beg < cstr->used; ++beg)
	{
		if(cstr->data[beg] != '\t' && cstr->data[beg] != ' ')
			break;
	}
	if(beg == end)//all character are blank
	{
		cstr->used = 0;
		memset(cstr->data,0,cstr->used);
		return cstr;
	}
	for(;end > beg; --end)
	{
		if(cstr->data[end-1] != '\t' && cstr->data[end-1] != ' ')
			break;
	}
	if(beg == 0 && end == cstr->used)//no blank prefix or suffix
	{
		return cstr;
	}
	else if(beg == 0 && end < cstr->used)
	{
		cstr->used = end;
		memset((cstr->data + end),0,old_used - end);
	}
	else
	{
		cstr->used = end - beg;
		memmove(cstr->data,(cstr->data + beg),end-beg);
		memset((cstr->data + cstr->used),0,old_used-cstr->used);
	}
	return cstr;
}

Cstr * cstr_trim_hdr(Cstr * cstr)
{
	assert(cstr_is_valid(cstr));
	if(cstr->used == 0)
		return cstr;
	uint32_t old_used = cstr->used;
	uint32_t beg = 0;
	for(; beg < cstr->used; ++beg)
	{
		if(cstr->data[beg] != '\t' && cstr->data[beg] != ' ')
			break;
	}
	if(beg == cstr->used)//all character are blank
	{
		cstr->used = 0;
		memset(cstr->data,0,cstr->used);
	}
	else
	{
		cstr->used = cstr->used - beg;
		memmove(cstr->data,(cstr->data + beg),old_used-beg);
		memset((cstr->data + cstr->used),0,old_used-cstr->used);
	}
	return cstr;
}

Cstr * cstr_trim_tail(Cstr * cstr)
{
	assert(cstr_is_valid(cstr));
	if(cstr->used == 0)
		return cstr;
	int old_used = cstr->used;
	int end = old_used-1;
	for(;end >= 0; --end)
	{
		if(cstr->data[end] != '\t' && cstr->data[end] != ' ')
			break;
	}
	if(end == 0)
	{
		cstr->used = 0;
		memset(cstr->data,0,cstr->used);
	}
	else
	{
		cstr->used = end + 1;
		memset((cstr->data+end+1),0,old_used-end-1);
	}
	return cstr;
}

bool cstr_is_valid(const Cstr * cstr)
{
	return NULL != cstr && NULL != cstr->data;// && cstr->used <= cstr->cap;
}

bool cstr_is_hex(const Cstr * cstr)
{
	if(!cstr_is_valid(cstr))
		return false;
	if(cstr->used == 0)
		return false;
	if(!cstr_is_start_with(cstr,"0x") && !cstr_is_start_with(cstr,"0X"))
		return false;
	for(uint32_t i = 2; i < cstr->used; ++i)
	{
		if(!((cstr->data[i] >= '0' && cstr->data[i] <= '9') 
			|| (cstr->data[i] >= 'a' && cstr->data[i] <= 'f')
			|| (cstr->data[i] >= 'A' && cstr->data[i] <= 'F')))
			return false;
	}
	return true;
}

bool cstr_is_digit(const Cstr * cstr)
{
	if(!cstr_is_valid(cstr))
		return false;
	if(cstr->used == 0)
		return false;
	uint32_t dot_num = 0;
	uint32_t d_num = 0;
	for(uint32_t i = 0; i < cstr->used; ++i)
	{
		if((cstr->data[i] < '0' || cstr->data[i] > '9') && '.' != cstr->data[i])
			return false;
		if('.' == cstr->data[i])
			++dot_num;
		else
			++d_num;
	}
	if(dot_num > 1)
		return false;
	if(dot_num == 1 && d_num == 0)
		return false;
	return true;
}

bool cstr_is_alphabet(const Cstr * cstr)
{
	if(!cstr_is_valid(cstr))
		return false;
	if(cstr->used == 0)
		return false;
	for(uint32_t i = 0; i < cstr->used; ++i)
	{
		if(!((cstr->data[i] >= 'a' && cstr->data[i] <= 'z') 
			|| (cstr->data[i] >= 'A' && cstr->data[i] <= 'Z')))
			return false;
	}
	return true;
}

bool cstr_empty(const Cstr * cstr)
{
	assert(cstr_is_valid(cstr));
	return cstr->used == 0;
}

void cstr_tolower(Cstr * cstr)
{
	assert(cstr_is_valid(cstr));
	uint32_t diff = 'a' - 'A';
	for(uint32_t i = 0; i < cstr->used; ++i)
	{
		if(cstr->data[i] >= 'A' && cstr->data[i] <= 'Z')
		{
			cstr->data[i] = cstr->data[i] + diff;
		}
	}
}

void cstr_toupper(Cstr * cstr)
{
	assert(cstr_is_valid(cstr));
	uint32_t diff = 'a' - 'A';
	for(uint32_t i = 0; i < cstr->used; ++i)
	{
		if(cstr->data[i] >= 'a' && cstr->data[i] <= 'z')
		{
			cstr->data[i] = cstr->data[i] - diff;
		}
	}
}

/*
readonly data return
*/
const char * cstr_data(const Cstr * cstr)
{
	assert(cstr_is_valid(cstr));
	return cstr->data;
}

void cstr_show(const Cstr * cstr)
{
	printf("=============================================================================\n");
	printf("addr:%p,total:%u,capacity:%u,used:%u,data:%s\n",cstr,CSTR_TOTAL_SIZE(cstr),cstr->cap,cstr->used,cstr->data);
}

/*
returned min value is 0x01 and max value is 2^31
otherwise is out of range 
0x0 indicate that size > 2^31
*/
uint32_t round_up_power_2(uint32_t size)
{
	uint32_t power_value = 0x01;
	uint32_t int_max = power_value << 31;
	if(size > int_max) 
		return 0x0;
	while(power_value < size)
	{
		power_value = power_value << 1;
	}
	// printf("power_value=%u\n",power_value);
	return power_value;
}

uint32_t round_down_power_2(uint32_t size)
{
	if(size == 0) return 0x00;
	uint32_t power_value = 0x01; 
	power_value = power_value << 31;
	if(power_value < size)
		return 0x00;
	while(power_value > size)
	{
		power_value = power_value >> 1;
	}
	// printf("power_value=%u\n",power_value);
	return power_value;
}

uint32_t cal_fit_cap(uint32_t total_cap,uint32_t total_used)
{
	if(total_used > _CSTR_MAX_SIZE || total_cap > _CSTR_MAX_SIZE) return 0;
	if(total_used == _CSTR_MAX_SIZE) return _CSTR_MAX_SIZE;

	uint32_t new_cap = 0;
	//if total_used(include cstr header and trailing '\0') over 1M than incr 1M per time just like redis sds
	if(total_used >= _CSTR_MIDIUM_MAX_SIZE)
	{
		new_cap = total_used + _CSTR_INCR_MAX;
		if(new_cap > _CSTR_MAX_SIZE)
			new_cap = _CSTR_MAX_SIZE;
	}
	else if(total_used < _CSTR_MIN_SIZE)
	{
		new_cap = _CSTR_MIN_SIZE;
	}
	else//if total_used is less than 1M,then double the capacity
	{
		new_cap = round_up_power_2(total_used);
	}
	if(new_cap < total_cap)
	{
		if(total_cap <= _CSTR_TINY_MAX_SIZE)
		{
			new_cap = total_cap;
		}
	}
	return new_cap;
}
//aaabbbaaa 0 1 2 0 0 0 1 2 3
//0 0 1 2 3 1
//a b a b a a
//k i
//k   i
//  k   i
//    k   i
//      k   i
//  k       i
//k         i
void kmp_next_table(const char * pattern,int pat_len,int * next_table)
{
	next_table[0] = 0;
	int i = 1,k = 0;//k指向当前比较的前缀的最后一位，i指向当前比较的后缀的最后一位
	while(i < pat_len)
	{
		if(pattern[i] == pattern[k])
		{
			next_table[i] = k+1;
			++k;
			++i;
		}
		else
		{
			if(k == 0)
			{
				next_table[i++] = 0;
			}
			else
			{
				k = next_table[k-1];
			}
		}
	}
	for(int n = 0; n < pat_len; ++n)
		printf("%d ", next_table[n]);
	printf("\n");
}

/*
i
0123456789
BACABCDABABC ABCDABD CABDE
         ABCDABD
j
*/

int kmp_search_first(const Cstr * cstr,const char * pattern,uint32_t begin)
{
	assert(cstr_is_valid(cstr));
	if(NULL == pattern)
		return -1;
	if(begin > cstr->used)
		return -1;
	uint32_t pat_len = strlen(pattern);
	if(begin + pat_len > cstr->used || pat_len == 0)
		return -1;
	if(pat_len >= _SEARCH_MAX_LEN) return -1;
	int next_table[_SEARCH_MAX_LEN];
	memset(next_table,0,_SEARCH_MAX_LEN);
	kmp_next_table(pattern,pat_len,next_table);
	int i = begin, j = 0;
	while(i < cstr->used && j < pat_len)
	{
		printf("i=%d,j=%d\n",i,j);
		if(cstr->data[i] == pattern[j])
		{
			if(j == pat_len - 1)
			{
				return i - (pat_len-1);
			}
			++i;
			++j;
		}
		else
		{
			if(j == 0)
				++i;
			else 
				j = next_table[j-1];
		}
	}
	return -1;
}
//0123456789
//BBC ABCDAB ABCDABCDABDE
//ABCDABD
//    ABCDABD
//           ABCDABD
//               ABCDABD

//shift_table lenght shoud >= 256 ,pattern length should <= 256
//坏字符出现在pattern的第i个位置时应该向后移动shift_table[i]位
void bm_bad_char_shift_table(const char * pattern,int pat_len,int shift_table[])
{
	for(uint32_t i = 0; i < _SEARCH_MAX_LEN; ++i)
	{
		shift_table[i] = -1;
	}
	for(uint32_t i = 0; i < pat_len; ++i)
	{
		shift_table[pattern[i]] = i;
	}
}

//suff[i]就是pattern中以i位置字符为后缀和以最后一个字符为后缀的公共后缀串的长度
void bm_suffix(const char * pattern,int pat_len,int suffix[])
{
	int i=0,j=0;
	suffix[pat_len-1] = pat_len;
	for(i = pat_len-2; i >= 0; --i)
	{
		j = i;
		while(j >= 0 && pattern[pat_len-1-i+j] == pattern[j])
			j--;

		suffix[i] = i - j;
	}
}

void bm_good_suffix_shift_table(const char * pattern,int pat_len,int shift_table[])
{
	int suffix[pat_len];
	bm_suffix(pattern,pat_len,suffix);
	//没有好后缀的时候，例如：abcdefg，没有好后缀
	for(int i = 0; i < pat_len; ++i)
		shift_table[i] = pat_len;
	int i = 0,j = 0;
	for(i = pat_len-1; i >= 0; --i)
	{
		if(suffix[i] == i+1)//好后缀出现在pattern的开头，例如：abcxxxabc，此时suffix[2] = 3，好后缀为abc
		{
			for(; j < pat_len-1-i; ++j)
			{
				if(shift_table[j] == pat_len)
				{
					shift_table[j] = pat_len-1-i;
				}
			}
		}
	}

	for(i = 0; i <= pat_len-2; ++i)//好后缀出现在pattern中间的时候,例如：uvwabcxxxabc，此时suffix[5] = 3，好后缀为abc
	{
		shift_table[pat_len-1-suffix[i]] = pat_len - 1 - i;
	}
}

int boyer_moore_search_first(const Cstr * cstr,const char * str,uint32_t begin)
{
	assert(cstr_is_valid(cstr) && NULL != str);
	if(begin > cstr->used)
		return -1;
	int pat_len = strlen(str);
	if(begin + pat_len > cstr->used || pat_len == 0)
		return -1;
	if(pat_len >= _SEARCH_MAX_LEN) return -1;
	int bm_bc[_SEARCH_MAX_LEN];
	bm_bad_char_shift_table(str,pat_len,bm_bc);
	int bm_gs[pat_len];
	bm_good_suffix_shift_table(str,pat_len,bm_gs);
	cstr_show(cstr);
	int ti = begin;//point to text,text index
	while(ti <= cstr->used - pat_len)
	{
		int pi = pat_len-1;//point to pattern,pattern index
		while(pi >= 0 && cstr->data[ti+pi] == str[pi])
		{
			--pi;
			// printf("ti:%d,pi:%d\n",ti,pi);
		}
		if(pi < 0)
		{
			return ti;
		}
		else
		{
			// printf("ti:%d,pi:%d\n",ti,pi);
			ti += MAX(pi-bm_bc[cstr->data[pi+ti]],bm_gs[pi]);
		}
	}
	return -1;
}

int sunday_search_first(const Cstr * cstr,const char * str,uint32_t begin)
{
	assert(cstr_is_valid(cstr) && NULL != str);
	if(begin > cstr->used)
		return -1;
	int pat_len = strlen(str);
	if(begin + pat_len > cstr->used || pat_len == 0)
		return -1;
	if(pat_len >= _SEARCH_MAX_LEN) return -1;

	int shift[_SEARCH_MAX_LEN];
	for(int i = 0; i < _SEARCH_MAX_LEN; ++i)
	{
		shift[i] = pat_len+1;
	}
	for(int i = 0; i < pat_len; ++i)
	{
		shift[str[i]] = pat_len-i;
	}
	int ti = begin;
	while(ti <= cstr->used - pat_len)
	{
		int pi = 0;
		while(pi < pat_len && str[pi] == cstr->data[ti+pi])
		{
			// printf("#ti:%d,pi:%d,tc:%c,pc:%c\n",ti,pi,cstr->data[ti+pi],str[pi]);
			++pi;
		}
		if(pi == pat_len)
		{
			return ti;
		}
		else
		{
			// printf("#ti:%d,pi:%d,tc:%c,pc:%c\n",ti,pi,cstr->data[ti+pi],str[pi]);
			// printf("char:%c,shift:%d\n",cstr->data[ti+pat_len],shift[cstr->data[ti+pat_len]]);
			ti += shift[cstr->data[ti+pat_len]];
		}
	}
	return -1;
}

// Cstr * cstr_replace_first(Cstr * dst,const char * toreplace,const char * replace)
// {
// 	assert(cstr_is_valid(dst) && NULL != toreplace && NULL != NULL);
// }

#define __TEST__

#ifdef __TEST__
int main(int argc, char const *argv[])
{
	Cstr * cstr = cstr_new_str("BACABCDABABC ABCDABD CABDEFABCDABCDAB ABCDABDx");
	// Cstr * cstr = cstr_new_str("hello world,i am linux coder,welcome");
	// Cstr * cstr = cstr_new_str("here-is-a-simple-example");
	// const char * cstr = "hello world,i am linux coder,welcome";
	const char * pattern = "ABCDABDx";
	// Cstr * cstr = cstr_new_str("我不知道这句话说的是什么，你好欢迎你来你好欢迎你来到linux的世界");
	// const char * pattern = "你好欢迎你来你好欢迎你来这";
	int count = cstr_count_str(cstr,pattern,0);
	printf("count=%d\n",count);
	return 0;
}
#endif
