/**
 * @file   object.c
 *
 * @author Dalian University of Technology
 *
 * @section LICENSE
 *
 * Copyright (C) 2010 Dalian University of Technology
 *
 * This file is part of EDUGUI.
 *
 * EDUGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * EDUGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EDUGUI; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * All rights reserved.
**/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <base_type.h>
# include <log.h>

# include "object.h"

void *object_init(si_t id)
{
    struct object * addr;

    /* 分配存储空间 */
    addr = (struct object *)malloc(sizeof(struct object));

    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("failed to malloc");
    }
    else
    {
        /* 默认父节点 */
        addr->parent = NULL;
        /* 默认左子节点 */
        addr->lchild = NULL;
        /* 默认右子节点 */
        addr->rchild = NULL;
        /* 名称 */
        addr->name = "struct object";
        /* ID */
        addr->id = id;
    }

    return addr;
}

si_t object_exit(struct object * obj)
{
    /* 清理 */
    free(obj);

    return 0;
}

si_t object_tree_free(struct object * root)
{
    /* 释放左子树 */
    if(root->lchild != NULL)
    {
        /* root->lchild 左子节点的地址 */
        object_tree_free(root->lchild);
    }

    /* 释放右子树 */
    if(root->rchild != NULL)
    {
        /* root->rchild 右子节点的地址 */
        object_tree_free(root->rchild);
    }

    /* root->node.data 与 root 的值一样 */
    free(root);

    return 0;
}

void *object_tree_l_most_node(struct object * node)
{
    while(node->lchild != NULL)
    {
        node = node->lchild;
    }

    return node;
}

void *object_tree_r_most_node(struct object * node)
{
    while(node->rchild != NULL)
    {
        node = node->rchild;
    }

    return node;
}

void *object_tree_iterator_increment(struct object * tree, struct object * node)
{
    /* 找出父节点 */
    struct object * temp = node->parent;

    /* 有右子节点 */
    if(node->rchild != NULL)
    {
        /* 就向右走 */
        node = node->rchild;

        /* 以右子节点为根节点的子树的最左节点 */
        /* 一直往左走 */
        while(node->lchild != NULL)
        {
            node = node->lchild;
        }
    }
    /* 没有右子节点 */
    else
    {
        /* 节点是右子节点 */
        while(node == temp->rchild)
        {
            /* 一直上溯，直到不为右子节点。 */
            node = temp;
            temp = temp->parent;
        }

        /* 找根节点的下一个节点恰巧根节点没有右子节点 */
        if(node->rchild != temp)
        {
            node = temp;
        }
    }

    /* 下一个节点是 object_tree 的节点 */
    /* 找的是最右边节点的下一个节点 */
    if(tree == node)
    {
        return NULL;
    }
    else
    {
        return node;
    }
}

void *object_tree_iterator_decrement(struct object * tree, struct object * node)
{
    /* 找出父节点 */
    struct object * temp = node->parent;

    /* 有左子节点 */
    if(node->lchild != NULL)
    {
        /* 就向左走 */
        node = node->lchild;

        /* 以左子节点为根节点的子树的最右节点 */
        /* 一直往右走 */
        while(node->rchild != NULL)
        {
            node = node->rchild;
        }
    }
    /* 没有左子节点 */
    else
    {
        /* 节点是左子节点 */
        while(node == temp->lchild)
        {
            /* 一直上溯，直到不为左子节点。 */
            node = temp;
            temp = temp->parent;
        }

        /* 找根节点的上一个节点恰巧根节点没有左子节点 */
        if(node->lchild != temp)
        {
            node = temp;
        }
    }

    /* 上一个节点是 object_tree 的节点 */
    /* 找的是最左边节点的上一个节点 */
    if(tree == node)
    {
        return NULL;
    }
    else
    {
        return node;
    }
}

/*
    总是将孩子节点加在子节点链的最左边

    如果 obj 在对象树中
    那么完成添加工作后一定要更新 root->parent 的 lchild 成员和 rchild 成员
    这是为了保证遍历的正确
*/
si_t object_attach_child(struct object * obj, struct object * child)
{
    struct object * tree;

    /* parent 没有子对象 */
    if(obj->rchild == NULL)
    {
        /* 作为上层对象的右子节点 */
        obj->rchild = child;
        child->parent = obj;
    }
    else
    {
        obj = obj->rchild;

        /* 一直往左走 */
        while(obj->lchild != NULL)
        {
            obj = obj->lchild;
        }

        /* 作为同层对象的左子节点 */
        obj->lchild = child;
        child->parent = obj;
    }

    /* 找到这棵树根节点的父节点 */
    /* 没有的话 tree == NULL */
	tree = object_get_root(obj)->parent;

    /* obj 在对象树中 */
    /* 必须更新 root->parent 节点的 lchild 成员和 rchild 成员 */
    /* 此时 tree == root->parent */
    if(tree != NULL)
    {
        /* 更新最左边节点 */
        tree->lchild = object_tree_l_most_node(tree->parent);
        /* 更新最右边节点 */
        tree->rchild = object_tree_r_most_node(tree->parent);
    }

    return 0;
}

/*
    总是将孩子节点加在子节点链的最右边

    如果 obj 在对象树中
    那么完成添加工作后一定要更新 root->parent 的 lchild 成员和 rchild 成员
    这是为了保证遍历的正确
*/
si_t object_insert_child(struct object* parent, struct object* child)
{
	struct object* tree = NULL;

	if(NULL == parent)
	{
		return 0;
	}

	/* 父节点有右子节点 */
	if(parent->rchild != NULL)
	{
		parent->rchild->parent = child;

		/* 父节点的右子节点作为自己的左子节点 */
		child->lchild = parent->rchild;
	}

	child->parent = parent;
	parent->rchild = child;

	/* 找到这棵树根节点的父节点 */
	tree = object_get_root(parent)->parent;

	/* child 在对象树中 */
	/* 必须更新 root->parent 节点的 lchild 成员和 rchild 成员 */
	/* 更新最左边节点 */
	tree->lchild = object_tree_l_most_node(tree->parent);
	/* 更新最右边节点 */
	tree->rchild = object_tree_r_most_node(tree->parent);

	return 0;
}

/*
    如果 obj 有左子节点
    那么这个左子节点并不是 obj 子对象
    所以在析构以 obj 为根的树之前一定要要将 obj->lchild 成员清空

    如果 obj 在对象树中
    那么完成删除工作后一定要更新 root->parent 的 lchild 成员和 rchild 成员
    这是为了保证遍历的正确
*/
si_t object_remove(struct object * obj)
{
    struct object * tree;

    /* obj 有父对象 */
    if(obj->parent != NULL)
    {
        /* 找到这棵树根节点的父节点 */
        /* 没有的话 tree == NULL */
		tree = object_get_root(obj)->parent;

        /* obj 是由父节点右子节点 */
        if(obj == obj->parent->rchild)
        {
            /* obj 没有左子节点 */
            /* parent 的下一层对象只有 obj */
            if(obj->lchild == NULL)
            {
                /* 清除父节点中的指针 */
                obj->parent->rchild = NULL;
            }
            /* obj 有左子节点 */
            /* parent 的下一层有多个对象 */
            else
            {
                /* 处理 obj 同层的对象 */
                obj->parent->rchild = obj->lchild;
                obj->lchild->parent = obj->parent;

                /* 清空 obj->lchild */
                obj->lchild = NULL;
            }
        }
        /* obj 是由父节点左子节点 */
        /* 此时 parent 的下一层有多个对象 */
        else if(obj == obj->parent->lchild)
        {
            /* obj 没有左子节点 */
            /* obj 处于末端 */
            if(obj->lchild == NULL)
            {
                /* 清除父节点中的指针 */
                obj->parent->lchild = NULL;
            }
            /* obj 有左子节点 */
            /* obj 处于中间 */
            else
            {
                /* 处理 obj 同层的对象 */
                obj->parent->lchild = obj->lchild;
                obj->lchild->parent = obj->parent;

                /* 清空 obj->lchild */
                obj->lchild = NULL;
            }
        }

        /* 删除 obj 以及它的所有子对象 */
        object_tree_free(obj);

        /* obj 在对象树中 */
        /* 必须更新 root->parent 节点的 lchild 成员和 rchild 成员 */
        /* 此时 tree == root->parent */
        if(tree != NULL)
        {
            /* 更新最左边节点 */
            tree->lchild = object_tree_l_most_node(tree->parent);
            /* 更新最右边节点 */
            tree->rchild = object_tree_r_most_node(tree->parent);
        }
    }
    /* 没有父对象 */
    else
    {
        /* 清空 obj->lchild */
        obj->lchild = NULL;

        /* 删除 obj 以及它的所有子对象 */
        object_tree_free(obj);
    }

    return 0;
}

si_t object_delete(struct object* object, void(*destructor)(void*))
{
	if(object->parent->parent == object)
	{
		object_tree_for_each(object, destructor);
	}
	else
	{
		/* 找到这棵树根节点的父节点 */
		struct object* tree = object_get_root(object)->parent;

		/* 是父节点的右子节点 */
		if(object == object->parent->rchild)
		{
			/* object 没有左子节点 */
			/* parent 的下一层对象只有 object */
			if(object->lchild == NULL)
			{
				/* 清除父节点中的指针 */
				object->parent->rchild = NULL;
			}
			/* object 有左子节点 */
			/* parent 的下一层有多个对象 */
			else
			{
				/* 处理 object 同层的对象 */
				object->parent->rchild = object->lchild;
				object->lchild->parent = object->parent;

				/* 清空 object->lchild */
				object->lchild = NULL;
			}
		}
		/* 是父节点的左子节点 */
		/* 此时 parent 的下一层有多个对象 */
		else if(object == object->parent->lchild)
		{
			/* object 没有左子节点 */
			/* object 处于末端 */
			if(object->lchild == NULL)
			{
				/* 清除父节点中的指针 */
				object->parent->lchild = NULL;
			}
			/* object 有左子节点 */
			/* object 处于中间 */
			else
			{
				/* 处理 object 同层的对象 */
				object->parent->lchild = object->lchild;
				object->lchild->parent = object->parent;

				/* 清空 object->lchild */
				object->lchild = NULL;
			}
		}

		object_tree_for_each(object, destructor);

		/* 必须更新 root->parent 节点的 lchild 和 rchild */
		/* 此时 tree == root->parent */
		if(tree != NULL)
		{
			/* 更新最左边节点 */
			tree->lchild = object_tree_l_most_node(tree->parent);
			/* 更新最右边节点 */
			tree->rchild = object_tree_r_most_node(tree->parent);
		}

	}
	return 0;
}


void *object_parent(struct object * obj)
{
    struct object * addr;

    /* 没有父对象 */
    if(obj->parent == NULL)
    {
        return NULL;
    }

    addr = obj;
    /* addr 是左子节点 */
    while(addr == addr->parent->lchild)
    {
        addr = addr->parent;
    }

    /* 顶层对象 */
    /* 没有父对象 */
    if(addr->parent->parent == addr)
    {
        return NULL;
    }
    else
    {
        return addr->parent;
    }
}

void *object_find_by_id(struct object * root, si_t id)
{
    if(root->id == id)
    {
        return root;
    }
    else
    {
        return object_find_by_id(root->rchild, id);
        return object_find_by_id(root->lchild, id);
    }
}

/**
 * @brief 递归深度优先遍历树
 *
 * @details
 *
 * @param root 根节点结构体地址
 * @param action 对节点进行的操作
 *
 * @return 0
**/
static si_t object_tree_depth_first_traversal(struct object * root, void(* action)(void *))
{
    action(root);

    if(root->rchild != NULL)
    {
        object_tree_depth_first_traversal(root->rchild, action);
    }

    if(root->lchild != NULL)
    {
        object_tree_depth_first_traversal(root->lchild, action);
    }

    return 0;
}

si_t object_tree_for_each(struct object * root, void(* action)(void *))
{
    if(root->rchild != NULL)
    {
        object_tree_for_each(root->rchild, action);
    }

    if(root->lchild != NULL)
    {
        object_tree_for_each(root->lchild, action);
    }

    action(root);

    return 0;
}

void object_move_first(struct object* obj)
{
	struct object * tree = NULL, * parent = NULL;
	/* 如果活动窗口不是顶层窗口也不是父窗口的右子节点 */
	/* 将活动窗口变成父窗口的右子节点 */
	if(obj == obj->parent->parent || obj == obj->parent->rchild)
	{
		return ;
	}
	/* 找到 win_info_ptr 的父窗口 */
	parent = obj->parent;
	while(parent == parent->parent->lchild)
	{
		parent = parent->parent;
	}
	parent = parent->parent;

	/* obj 有左子节点 */
	/* obj 在末端 */
	if(obj->lchild != NULL)
	{
		obj->lchild->parent = obj->parent;
		obj->parent->lchild = obj->lchild;
	}
	/* obj 没有左子节点 */
	/* obj 在中间*/
	else
	{
		obj->parent->lchild = NULL;
	}

	obj->lchild = parent->rchild;
	parent->rchild->parent =obj;

	obj->parent = parent;
	parent->rchild = obj;

	/* 找到这棵树根节点的父节点 */
	tree = object_get_root(parent)->parent;

	/* 必须更新 root->parent 节点的 lchild 成员和 rchild 成员 */
	/* 此时 tree == root->parent */

	/* 更新最左边节点 */
	tree->lchild = object_tree_l_most_node(tree->parent);
	/* 更新最右边节点 */
	tree->rchild = object_tree_r_most_node(tree->parent);
}


struct object* object_get_root(struct object* obj)
{
	while(obj->parent != NULL && obj->parent->parent != obj)
	{
		obj = obj->parent;
	}
	return obj;
}
