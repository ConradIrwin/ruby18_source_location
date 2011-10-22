// Include the Ruby headers and goodies
#include "ruby.h"

typedef struct RNode {
    unsigned long flags;
    char *nd_file;
    union {
        struct RNode *node;
        ID id;
        VALUE value;
        VALUE (*cfunc)(ANYARGS);
        ID *tbl;
    } u1;
    union {
        struct RNode *node;
        ID id;
        long argc;
        VALUE value;
    } u2;
    union {
        struct RNode *node;
        ID id;
        long state;
        struct global_entry *entry;
        long cnt;
        VALUE value;
    } u3;
} NODE;

struct FRAME {
    VALUE self;
    int argc;
    ID last_func;
    ID orig_func;
    VALUE last_class;
    struct FRAME *prev;
    struct FRAME *tmp;
    struct RNode *node;
    int iter;
    int flags;
    unsigned long uniq;
} *ruby_frame;

struct METHOD {
    VALUE klass, rklass;
    VALUE recv;
    ID id, oid;
    int safe_level;
    NODE *body;
};

struct BLOCK {
    NODE *var;
    NODE *body;
    VALUE self;
    struct FRAME frame;
    struct SCOPE *scope;
    VALUE klass;
    NODE *cref;
    int iter;
    int vmode;
    int flags;
    int uniq;
    struct RVarmap *dyna_vars;
    VALUE orig_thread;
    VALUE wrapper;
    VALUE block_obj;
    struct BLOCK *outer;
    struct BLOCK *prev;
};

static VALUE
source_pair(const char *filename, int lineno)
{
    VALUE array = rb_ary_new();
    rb_array_push(array, rb_str_new2(filename));
    rb_array_push(array, INT2FIX(lineno));

    return array;
}

static VALUE
node_source_location(NODE *node)
{
    const char *filename = node->nd_file;
    int lineno = nd_line(node);

    if (filename && lineno)
        return source_pair(filename, lineno);

    return Qnil;
}

/*
 * call-seq:
 *    meth.source_location  => [String, Fixnum]
 *
 * returns a pair of the Filename and line number on which the method is defined
 *
 * returns nil if method has no associated ruby source code
 */
static VALUE
method_source_location(VALUE method)
{
    struct METHOD *data;
    NODE *node;

    Data_Get_Struct(method, struct METHOD, data);
    if (node = data->body) {
        return node_source_location(node);
    }
    return Qnil;
}

/*
 * call-seq:
 *    meth.source_location  => [String, Fixnum]
 *
 * returns a pair of the Filename and line number on which the method is defined
 *
 * returns nil if method has no associated ruby source code
 */
static VALUE
proc_source_location(VALUE block)
{
    struct BLOCK *data;
    NODE *node;

    Data_Get_Struct(block, struct BLOCK, data);
    if ((node = data->frame.node) || (node = data->body)) {
        return node_source_location(node);
    }
    return Qnil;
}

void
Init_ruby18_source_location()
{
    rb_define_method(rb_cUnboundMethod, "source_location", method_source_location, 0);
    rb_define_method(rb_cMethod, "source_location", method_source_location, 0);
    rb_define_method(rb_cProc, "source_location", proc_source_location, 0);
}
