/*
** $Id: lapi.h,v 2.9.1.1 2017/04/19 17:20:42 roberto Exp $
** Auxiliary functions from Lua API
** See Copyright Notice in lua.h
*/

#ifndef lapi_h  /* 防止重复包含头文件 */
#define lapi_h


#include "llimits.h"  /* 包含Lua限制相关的定义 */
#include "lstate.h"   /* 包含Lua状态相关的定义 */

/* 增加栈顶指针,并检查是否发生栈溢出 */
#define api_incr_top(L)   {L->top++; api_check(L, L->top <= L->ci->top, \
				"stack overflow");}

/* 调整返回结果数量,如果是多返回值且栈顶超过调用信息的栈顶,则更新调用信息的栈顶 */
#define adjustresults(L,nres) \
    { if ((nres) == LUA_MULTRET && L->ci->top < L->top) L->ci->top = L->top; }

/* 检查栈中是否有足够的元素 */
#define api_checknelems(L,n)	api_check(L, (n) < (L->top - L->ci->func), \
				  "not enough elements in the stack")


#endif
