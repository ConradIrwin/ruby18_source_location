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

/*
 * call-seq:
 *    meth.__file__  => String  
 *
 * returns the filename containing this method's definition
 *
 * raises ArgumentError if method has no associated ruby source code
 *
 * <i>Only available when MBARI_API extentions are enabled at build time</i>
 */
 
static VALUE
method_source_file_name(VALUE method)
{
    struct METHOD *data;
    NODE *node;

    Data_Get_Struct(method, struct METHOD, data);   
    if (node = data->body) {
      const char *filename = node->nd_file;
      if (filename)
        return rb_str_new2(filename);
    }
    rb_raise(rb_eArgError, "native Method");
}

/*
 * call-seq:
 *    meth.__line__  => Fixnum  
 *
 * returns the starting line number of this method
 *
 * raises ArgumentError if method has no associated ruby source code
 *
 * <i>Only available when MBARI_API extentions are enabled at build time</i>
 */
 
static VALUE
method_source_line(VALUE method)
{
    struct METHOD *data;
    NODE *node;

    Data_Get_Struct(method, struct METHOD, data);
    if (node = data->body) {
      int lineno = nd_line(node);
      if (lineno)
        return INT2FIX(nd_line(node));
    }
    rb_raise(rb_eArgError, "native Method");
}


/*
 * call-seq:
 *    prc.__file__  => String  
 *
 * returns the filename where this proc is defined
 *
 * raises ArgumentError if proc has no associated ruby source
 *
 * <i>Only available when MBARI_API extentions are enabled at build time</i>
 */
 
static VALUE
proc_source_file_name(VALUE block)
{
    struct BLOCK *data;
    const char *filename;
    NODE *node;

    Data_Get_Struct(block, struct BLOCK, data);
    if ((node = data->frame.node) || (node = data->body))
      return rb_str_new2(node->nd_file);
    rb_raise(rb_eArgError, "native Proc");
}


/*
 * call-seq:
 *    prc.__line__  => Fixnum  
 *
 * returns the starting line number of this proc
 *
 * raises ArgumentError if proc has no associated ruby source code
 *
 * <i>Only available when MBARI_API extentions are enabled at build time</i>
 */
 
static VALUE
proc_source_line(VALUE block)
{
    struct BLOCK *data;
    NODE *node;

    Data_Get_Struct(block, struct BLOCK, data);
    if ((node = data->frame.node) || (node = data->body))
      return INT2FIX( nd_line(node) );
    rb_raise(rb_eArgError, "native Proc");
}
void
Init_ruby18_source_location()
{
    rb_define_method(rb_cUnboundMethod, "__file__", method_source_file_name, 0);
    rb_define_method(rb_cUnboundMethod, "__line__", method_source_line, 0);
    rb_define_method(rb_cProc, "__file__", proc_source_file_name, 0);
    rb_define_method(rb_cProc, "__line__", proc_source_line, 0);
    rb_define_method(rb_cMethod, "__file__", method_source_file_name, 0);
    rb_define_method(rb_cMethod, "__line__", method_source_line, 0);
}
