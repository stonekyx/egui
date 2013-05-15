# ifndef _VECTOR_H_
# define _VECTOR_H_ 1

# include "base_type.h"

/**
 * 动态数组节点
**/
struct vector_node
{
    /**
     * 数据
    **/
    void * data;
};

/**
 * 动态数组
**/
struct vector
{
    /**
     * 存储区域的开始地址
    **/
    struct vector_node * head;
    /**
     * 当前动态数组的末尾地址
    **/
    struct vector_node * curr;
    /**
     * 存储区域的结束地址
    **/
    struct vector_node * tail;
};

/**
 * 申请动态数组
 *
 * @param v 动态数组指针
 *
 * @return 0
**/
extern
si_t
vector_init
(struct vector * v);

/**
 * 释放动态数组
 *
 * @param v 动态数组指针
 *
 * @return 0
**/
extern
si_t
vector_exit
(struct vector * v);

/**
 * 获得动态数组中元素的个数
 *
 * @param v 动态数组指针
 *
 * @return 动态数组中元素的个数
**/
extern
ui_t
vector_size
(struct vector * v);

/**
 * 判断动态数组是否空
 *
 * @param v 动态数组指针
 *
 * @return 空返回 1，否则返回 0。
**/
extern
si_t
vector_empty
(struct vector * v);

/**
 * 获得动态数组头部节点地址
 *
 * @param v 动态数组指针
 *
 * @return 动态数组头部节点地址
**/
extern
void *
vector_begin
(struct vector * v);

/**
 * 获得动态数组尾部节点地址
 *
 * @param v 动态数组指针
 *
 * @return 动态数组尾部节点地址
**/
extern
void *
vector_end
(struct vector * v);

/**
 * 获得动态数组头部的元素
 *
 * @param v 动态数组指针
 *
 * @return 返回动态数组头部元素的地址，如果没有返回 NULL。
**/
extern
void *
vector_front
(struct vector * v);

/**
 * 获得动态数组尾部的元素
 *
 * @param v 动态数组指针
 *
 * @return 返回动态数组尾部元素的地址，如果没有返回 NULL。
**/
extern
void *
vector_back
(struct vector * v);

/**
 * 通过下标获得动态数组的元素
 *
 * @param v 动态数组指针
 * @param i 下标
 *
 * @return 返回动态数组第 index + 1 个元素的地址，如果没有返回 NULL。
**/
extern
void *
vector_at
(struct vector * v,
 si_t i);

/**
 * 将元素移动到头部
 *
 * @param v 动态数组指针
 * @param i 下标
 *
 * @return 0
**/
extern
si_t
vector_move_front
(struct vector * v,
 si_t i);


/**
 * 将元素移动到尾部
 *
 * @param v 动态数组指针
 * @param i 下标
 *
 * @return 0
**/
extern
si_t
vector_move_back
(struct vector * v,
 si_t i);

/**
 * 对动态数组中满足 func 的元素进行计数
 *
 * @param v 动态数组指针
 * @param func 比较函数
 *
 * @return 满足 func 的元素个数
**/
extern
ui_t
vector_count
(struct vector * v,
 si_t (* func)(void *));

/**
 * 在动态数组中查找元素
 *
 * 在 v 所指动态数组中查找第一个满足 comp 的元素
 *
 * @param v 动态数组指针
 * @param data 可能用到的参数
 * @param comp 元素比较函数指针
 *
 * @return 返回动态数组中第一个满足 comp 的元素所在节点的下标，如果没有返回 -1。
**/
extern
si_t
vector_find
(struct vector * v,
 void * data,
 si_t (* comp)(void *, void *));

/**
 * 在动态数组中查找可以插入某个元素的第一个位置
 *
 * 查找可以插入 data 的第一个位置
 * 也就是第一个大于或者等于 data 的元素所在位置
 *
 * @param v 动态数组指针
 * @param data 元素指针
 * @param comp 元素比较函数指针
 *
 * @return 返回动态数组中可以插入 data 的第一个位置
**/
extern
si_t
vector_lower_bound
(struct vector * v,
 void * data,
 si_t (* comp)(void *,  void *));

/**
 * 在动态数组中查找可以插入某个元素的最后一个位置
 *
 * 查找可以插入 data 的最后一个位置
 * 也就是第一个大于 data 的元素所在位置
 *
 * @param v 动态数组指针
 * @param data 元素指针
 * @param comp 元素比较函数指针
 *
 * @return 返回动态数组中可以插入 data 的最后一个位置
**/
extern
si_t
vector_upper_bound
(struct vector * v,
 void * data,
 si_t (* comp)(void *, void *));

/**
 * 向动态数组尾部插入元素
 *
 * @param v 动态数组指针
 * @param data 元素地址
 * @param datasize 元素大小
 *
 * @return 0
**/
extern
si_t
vector_push_back
(struct vector * v,
 void * data,
 ui_t datasize);

/**
 * 从动态数组尾部删除元素
 *
 * @param v 动态数组指针
 *
 * @return 0
**/
extern
si_t
vector_pop_back
(struct vector * v);

/**
 * 向动态数组插入第 index + 1 个元素
 *
 * @param v 动态数组指针
 * @param i 下标
 * @param data 元素地址
 * @param datasize 元素大小
 *
 * @return 0
**/
extern
si_t
vector_insert
(struct vector * v,
 ui_t i,
 void * data,
 ui_t datasize);

/**
 * 从动态数组删除第 index + 1 个元素
 *
 * @param v 动态数组指针
 * @param i 下标
 *
 * @return 0
**/
extern
si_t
vector_erase
(struct vector * v,
 ui_t i);

/**
 * 清空动态数组
 *
 * @param v 动态数组指针
 *
 * @return 0
**/
extern
si_t
vector_clear
(struct vector * v);

/**
 * 对动态数组中的每个元素执行操作
 *
 * @param v 动态数组指针
 * @param data 可能用到的数据指针
 * @param function 操作元素的函数指针
 *
 * @return 0
**/
extern
si_t
vector_for_each
(struct vector * v,
 void * data,
 si_t (* function)(void *, void *));

/**
 * 动态数组排序
 *
 * @param v 动态数组指针
 * @param comp 元素比较函数指针
 *
 * @return 0
**/
extern
si_t
vector_sort
(struct vector * v,
 si_t (* comp)(void *, void *));

# endif

