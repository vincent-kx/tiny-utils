#ifndef __C_STR_H_20181015__
#define __C_STR_H_20181015__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define _CSTR_MIN_SIZE 32 //32bit
#define _CSTR_MAX_SIZE 512*1024*1024 //512M

#define _CSTR_MIDIUM_MAX_SIZE 1024*1024
#define _CSTR_SMALL_MAX_SIZE 2*1024
#define _CSTR_TINY_MAX_SIZE 256

#define _CSTR_INCR_MAX 1024*1024

#define _SEARCH_MAX_LEN 256

/*
an simple binary-safe c string implementation,
any direct operation on 'cap' or 'used' or 'data' can result in undefined behavior,
please use user-api only.
*/
struct __attribute__ ((__packed__)) _cstr_
{
	uint32_t cap; //data capacity
	uint32_t used; //used size
	char data[];
};


typedef struct _cstr_ Cstr;
#define CSTR_HDR_SIZE sizeof(Cstr)
#define CSTR_TOTAL_SIZE(cstr) (cstr->cap + CSTR_HDR_SIZE + 1)
#define CSTR_TOTAL_USED(cstr) (cstr->used + CSTR_HDR_SIZE + 1)
#define CSTR_SET_END(cstr) (cstr->data[cstr->used] = '\0')
#define MAX(a,b) (a) > (b) ? (a) : (b)

struct _cstr_pair_
{
	Cstr * first;
	Cstr * second;	
};

typedef struct _cstr_pair_ CstrPair;
typedef Cstr * CstrArr;

enum CstrSortType
{
	CSTR_SORT_TYPE_ASC = 1,
	CSTR_SORT_TYPE_DESC = 2
};

//user api s

Cstr * cstr_new_str(const char * str);//construct from c raw string
Cstr * cstr_new_size(const uint32_t cap);
Cstr * cstr_new(const Cstr * cstr);//copy constructor

void cstr_free(Cstr * cstr);//destructor
void cstr_clear(Cstr * cstr);
uint32_t cstr_length(const Cstr * cstr);
uint32_t cstr_unused_size(const Cstr * cstr);//get unused size

int cstr_find_first(const Cstr * cstr,const char * str,int begin);
int cstr_find_char(const Cstr * cstr,const char c,int begin,int end);

// // void cstr_split(const Cstr * cstr,const char c,CstrArr result);
// // void cstr_split(const Cstr * cstr,const char * str,CstrArr result);

// //quick sort
// // void cstr_sort(Cstr * cstr,uint32_t sort_type);

// //no rfind,please use reverse and find
uint32_t cstr_count_char(const Cstr * cstr,const char c,uint32_t begin,uint32_t end);
uint32_t cstr_count_str(const Cstr * cstr,const char * str,uint32_t begin);

int cstr_reverse(Cstr * cstr);

/*
cstr1 < cstr2: return -value
cstr1 < cstr2: return +value
cstr1 < cstr2: return 0
the '+/-' flag means 'bigger or smaller',returned 'value' has no meanning
*/
int cstr_compare(const Cstr * cstr1,const Cstr * cstr2);
int cstr_compare_str(const Cstr * cstr, const char * str);
Cstr * cstr_assign(Cstr * dst,const Cstr * src);
Cstr * cstr_assign_str(Cstr * dst,const char * src);
Cstr * cstr_copy(Cstr * dst,const Cstr * src,uint32_t begin, uint32_t len);
Cstr * cstr_copy_str(Cstr * dst,const char * src,uint32_t begin, uint32_t len);

Cstr * cstr_trim(Cstr * cstr);
Cstr * cstr_trim_hdr(Cstr * cstr);
Cstr * cstr_trim_tail(Cstr * cstr);

static inline bool cstr_is_valid(const Cstr * cstr);
bool cstr_is_digit(const Cstr * cstr);
bool cstr_is_hex(const Cstr * cstr);
bool cstr_is_alphabet(const Cstr * cstr);
bool cstr_empty(const Cstr * cstr);
bool cstr_is_start_with(const Cstr * cstr,const char * str);

Cstr * cstr_sub_cstr(Cstr * substr,const Cstr * cstr,const uint32_t begin,uint32_t len);
Cstr * cstr_sub_str(Cstr * substr,const char * str,const uint32_t begin,uint32_t len);

Cstr * cstr_insert(Cstr * dst,const uint32_t pos,const Cstr * istr,const uint32_t begin, uint32_t len);
Cstr * cstr_insert_str(Cstr * dst,const uint32_t pos,const char * str,const uint32_t begin, uint32_t len);

// Cstr * cstr_replace_first(Cstr * dst,const char * toreplace,const char * replace);
// Cstr * cstr_replace_last(Cstr * dst,const char * toreplace,const char * replace);
// Cstr * cstr_replace_all(Cstr * dst,const char * toreplace,const char * replace);

Cstr * cstr_append(Cstr * dst,const Cstr * str);
Cstr * cstr_append_str(Cstr * dst,const char * str);

void cstr_tolower(Cstr * cstr);
void cstr_toupper(Cstr * cstr);

int at(const Cstr * cstr,uint32_t i);
const char * cstr_data(const Cstr * cstr);
void cstr_show(const Cstr * cstr);

int kmp_search_first(const Cstr * cstr,const char * pattern,uint32_t begin);
int boyer_moore_search_first(const Cstr * cstr,const char * str,uint32_t begin);
int sunday_search_first(const Cstr * cstr,const char * str,uint32_t begin);

// //inner api
// //求大于某个数的最小的2的n次方
uint32_t round_up_power_2(uint32_t size);
uint32_t round_down_power_2(uint32_t size);
uint32_t cal_fit_cap(uint32_t total_cap,uint32_t total_used);
void kmp_next_table(const char * pattern,int pat_len,int * next_table);
void bm_bad_char_shift_table(const char * pattern,int pat_len,int shift_table[]);
void bm_suffix(const char * pattern,int pat_len,int suffix[]);
void bm_good_suffix_shift_table(const char * pattern,int pat_len,int shift_table[]);
#endif