

#include <string.h>

#include "list.h"
#include "rbtree.h"

#include "d_schedule.h"
#include "d_mutex.h"
#include "d_thread.h"
#include "d_semaphore.h"
#include "d_time.h"


typedef struct 
{
    char name[64];
    TIMER_PROC_FUNC_t proc_func;
    uint32_t nInterval;
    void *para;
    uint32_t expire_time;
}f_timer_inst_info_t;

typedef struct
{
    f_timer_inst_info_t inst_info;
    struct list_head node;
}d_sched_list_node_t;

typedef struct 
{
    uint32_t expire_time;
    d_sched_list_node_t head;
    struct rb_node rb;
}d_sched_rb_node_t;

typedef struct 
{
    DSem_t sem;
    DMutex_t mtx;
    DThread_t thread_id;
    struct rb_root task_root;
    int time2wait;
}f_timer_ctrl_t;

static f_timer_ctrl_t timer_ctx;

int _rb_insert(struct rb_root *task_root, d_sched_rb_node_t *val)
{
    struct rb_node **new_node = &task_root->rb_node, *parent = NULL;
    while(*new_node)
    {
        parent = *new_node;
        if (val->expire_time < rb_entry(parent, d_sched_rb_node_t, rb)->expire_time)
        {
            new_node = &parent->rb_left;
        }
        else if (val->expire_time > rb_entry(parent, d_sched_rb_node_t, rb)->expire_time)
        {
            new_node = &parent->rb_right;
        }
        else
        {
            return 0;
        }
    }
    rb_link_node(&val->rb, parent, new_node);
    rb_insert_color(&val->rb, task_root);
    return 1;
}

d_sched_rb_node_t * _rb_find(struct rb_root *task_root, uint32_t key)
{
    d_sched_rb_node_t *entry_ptr = NULL;
    struct rb_node *rb_node = task_root->rb_node;
    while(rb_node)
    {
        entry_ptr = rb_entry(rb_node, d_sched_rb_node_t, rb);
        if (key < entry_ptr->expire_time)
        {
            rb_node = rb_node->rb_left;
        }
        else if (key > entry_ptr->expire_time)
        {
            rb_node = rb_node->rb_right;
        }
        else
        {
            return entry_ptr;
        }
    }
    return NULL;
}

void _rb_erase(struct rb_root *task_root, d_sched_rb_node_t *val)
{
    rb_erase(&val->rb, task_root);
}

d_sched_rb_node_t * _rb_first_entry(struct rb_root *task_root)
{
    if (task_root)
    {
        struct rb_node *rb_node = rb_first(task_root);
        if (rb_node)
        {
            return rb_entry(rb_node, d_sched_rb_node_t, rb);
        }
    }
    return NULL;
}

int _insert_task(struct rb_root *task_rb_root, uint32_t expire_time, d_sched_list_node_t *list_entry_ptr)
{
    d_sched_rb_node_t *next_rb_node = NULL;
    next_rb_node =_rb_find(task_rb_root, expire_time);
    if (NULL == next_rb_node)
    {
        next_rb_node = (d_sched_rb_node_t *)malloc(sizeof(d_sched_rb_node_t));
        next_rb_node->expire_time = expire_time;
        INIT_LIST_HEAD(&next_rb_node->head.node);   
        list_add_tail(&list_entry_ptr->node, &next_rb_node->head.node);

        rb_init_node(&next_rb_node->rb);
        _rb_insert(task_rb_root, next_rb_node);
    }
    else 
    {
        list_add_tail(&list_entry_ptr->node, &next_rb_node->head.node);
    }
    return 0;
}

int _find_task(struct rb_root *task_rb_root, const char *name, d_sched_rb_node_t **rb_entry, d_sched_list_node_t **list_entry)
{
    int is_found = 0;
    struct rb_root *rbnode = NULL;
    d_sched_rb_node_t *rb_entry_ptr = NULL;
    d_sched_list_node_t *list_entry_ptr = NULL;
    for (rbnode = rb_first(&timer_ctx.task_root);rbnode;rbnode = rb_next(rbnode))
    {
        list_entry_ptr = NULL;
        rb_entry_ptr = rb_entry(rbnode, d_sched_rb_node_t, rb);
        list_for_each_entry(list_entry_ptr, &rb_entry_ptr->head.node, node)
        {
            if (!strcmp(name, list_entry_ptr->inst_info.name) 
                && strlen(name) == strlen(list_entry_ptr->inst_info.name))
            {
                is_found = 1;
                break;
            }
        }

        if (is_found)
        {
            if (rb_entry)
            {
                *rb_entry = rb_entry_ptr;
            }
            if(list_entry)
            {
                *list_entry = list_entry_ptr;
            }
            return 1;
        }
    }

    return 0;
}

int _timer_get_coming_task_recfg(struct rb_root *task_root, uint32_t cur_time, f_timer_inst_info_t *comming_task)
{
    int next_delay = -1;
    struct rb_node *rb_node = NULL;
    d_sched_rb_node_t *rb_first_entry_ptr = NULL;
    d_sched_list_node_t *list_entry_ptr = NULL;

    if (NULL == task_root || NULL == comming_task)
    {
        return 0;
    }
    rb_first_entry_ptr = _rb_first_entry(task_root);
    if (rb_first_entry_ptr)
    {
        next_delay = (rb_first_entry_ptr->expire_time > cur_time) ? (rb_first_entry_ptr->expire_time - cur_time) : 0;
        if (cur_time >=rb_first_entry_ptr->expire_time)
        {
            if (!list_empty(&rb_first_entry_ptr->head.node))
            {
                d_sched_rb_node_t *next_rb_node = NULL;

                list_entry_ptr = list_first_entry(&rb_first_entry_ptr->head.node, d_sched_list_node_t, node);
                *comming_task = list_entry_ptr->inst_info;

                list_del(&list_entry_ptr->node);
                list_entry_ptr->inst_info.expire_time = cur_time + list_entry_ptr->inst_info.nInterval;
                _insert_task(task_root, list_entry_ptr->inst_info.expire_time, list_entry_ptr);
            }

            if (list_empty(&rb_first_entry_ptr->head.node))
            {
                _rb_erase(task_root, rb_first_entry_ptr);
                free(rb_first_entry_ptr);

                rb_first_entry_ptr = _rb_first_entry(task_root);
                if (rb_first_entry_ptr)
                {
                    next_delay = rb_first_entry_ptr->expire_time - cur_time;
                }
                else 
                {
                    next_delay = -1;
                }
            }
        }
    }
    
    return next_delay;
}

void __timer_thread_proc(void *arg)
{
    TIMER_PROC_FUNC_t proc_func = NULL;
    f_timer_inst_info_t inst_info = {0};
    int time2wait;
    uint32_t cur_time = 0;

    d_mutex_lock(timer_ctx.mtx, DWAITFOREVER);
    time2wait = timer_ctx.time2wait;
    d_mutex_unlock(timer_ctx.mtx);

    while(1)
    {
        if (time2wait)
        {
            d_sem_wait(timer_ctx.sem, time2wait);
            cur_time = d_time_stampms();
        }
        inst_info.proc_func = NULL;
        d_mutex_lock(timer_ctx.mtx, -1);
        time2wait =_timer_get_coming_task_recfg(&timer_ctx.task_root, cur_time, &inst_info);
        d_mutex_unlock(timer_ctx.mtx);
        if (inst_info.proc_func)
        {
            inst_info.proc_func(inst_info.para, inst_info.nInterval);
        }
    }
}

int timer_init(void)
{
    timer_ctx.sem = d_sem_new(0);
    timer_ctx.mtx = d_mutex_new();
    timer_ctx.task_root.rb_node = NULL;
    timer_ctx.time2wait = -1;
    
    timer_ctx.thread_id = d_thread_new("Schedule_thread", D_THREAD_PRI_IDLE, D_THREAD_STACKSIZE_256K, __timer_thread_proc, NULL);

    return 0;
}

int timer_api_create(const char *name, TIMER_PROC_FUNC_t proc_func,uint32_t ninterval, void *para)
{
    uint32_t cur_time = 0;
    struct rb_node *rbnode = NULL;
    d_sched_rb_node_t *rb_entry_ptr = NULL;
    d_sched_list_node_t *list_entry_ptr = NULL;
    int need_sem_post = 0;

    d_mutex_lock(timer_ctx.mtx, DWAITFOREVER);
    
    if (_find_task(&timer_ctx.task_root, name, NULL, NULL))
    {
        d_mutex_unlock(timer_ctx.mtx);
        return 0;
    }

    cur_time = d_time_stampms();
    ninterval = ninterval > 0 ? ninterval : 1;

    list_entry_ptr = (d_sched_list_node_t *)malloc(sizeof(d_sched_list_node_t));
    strncpy(list_entry_ptr->inst_info.name, name, sizeof(list_entry_ptr->inst_info.name));
    list_entry_ptr->inst_info.nInterval = ninterval;
    list_entry_ptr->inst_info.proc_func = proc_func;
    list_entry_ptr->inst_info.para = para;
    list_entry_ptr->inst_info.expire_time = cur_time + ninterval;

    rb_entry_ptr =_rb_find(&timer_ctx.task_root,  list_entry_ptr->inst_info.expire_time);
    if (rb_entry_ptr)
    {
        list_add_tail(&list_entry_ptr->node, &rb_entry_ptr->head.node);    
    }
    else 
    {
        rb_entry_ptr = (d_sched_rb_node_t *)malloc(sizeof(d_sched_rb_node_t));
        rb_entry_ptr->expire_time = list_entry_ptr->inst_info.expire_time;
        INIT_LIST_HEAD(&rb_entry_ptr->head.node);
        list_add_tail(&list_entry_ptr->node, &rb_entry_ptr->head.node);

        rb_init_node(&rb_entry_ptr->rb);
        _rb_insert(&timer_ctx.task_root, rb_entry_ptr);

        if (_rb_first_entry(&timer_ctx.task_root) == rb_entry_ptr)
        {
            timer_ctx.time2wait = ninterval;
            need_sem_post = 1;
        }
    }
    d_mutex_unlock(timer_ctx.mtx);
    if (need_sem_post)
    {
        d_sem_post(timer_ctx.sem);
    }
    return 0;
}

int timer_api_update_interval(const char *name, uint32_t interval)
{
    struct rb_node *rbnode = NULL;
    d_sched_rb_node_t *rb_entry_ptr = NULL;
    d_sched_list_node_t *list_entry_ptr = NULL;
    int is_found = 0, need_sem_post = 0;
    uint32_t cur_time = 0;

    cur_time = d_time_stampms();
    interval = interval > 0 ? interval : 1;

    d_mutex_lock(timer_ctx.mtx, -1);
    is_found = _find_task(&timer_ctx.task_root, name, NULL, &list_entry_ptr);
    if (is_found)
    {
        if (list_entry_ptr->inst_info.nInterval != interval)
        {    
            list_entry_ptr->inst_info.nInterval = interval;
            list_entry_ptr->inst_info.expire_time = cur_time + interval;
            list_del(&list_entry_ptr->node);
            _insert_task(&timer_ctx.task_root, cur_time + interval, list_entry_ptr);

            rb_entry_ptr = _rb_first_entry(&timer_ctx.task_root);
            if(NULL != rb_entry_ptr && rb_entry_ptr->expire_time > interval)
           {
               timer_ctx.time2wait = interval;
               need_sem_post = 1;
           }
        }
    }
    d_mutex_unlock(timer_ctx.mtx);

    if (need_sem_post)
    {
        d_sem_post(timer_ctx.sem);
    }
    return 0;
}

int timer_api_delete(const char *name)
{
    struct rb_node *rbnode = NULL;
    d_sched_rb_node_t *rb_entry_ptr = NULL;
    d_sched_list_node_t *list_entry_ptr = NULL;
    int is_found = 0, need_sem_post = 0;

    d_mutex_lock(timer_ctx.mtx, -1);
    is_found = _find_task(&timer_ctx.task_root, name, &rb_entry_ptr, &list_entry_ptr);
    if (is_found)
    {
        list_del(&list_entry_ptr->node);
        free(list_entry_ptr);
        if (list_empty(&rb_entry_ptr->head.node))
        {
            if (_rb_first_entry(&timer_ctx.task_root) == rb_entry_ptr)
            {
               need_sem_post = 1;
            }
            _rb_erase(&timer_ctx.task_root, rb_entry_ptr);
            free(rb_entry_ptr);
        }
    }
    d_mutex_unlock(timer_ctx.mtx);

    if (need_sem_post)
    {
        d_sem_post(timer_ctx.sem);
    }
    return is_found;
}
