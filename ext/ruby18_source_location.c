#include "ruby.h"
#include "node.h"

// Cargo-culted from ruby internal headers.
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


static VALUE method_source_location(VALUE);
static VALUE proc_source_location(VALUE);

void
Init_ruby18_source_location()
{
    rb_define_method(rb_cUnboundMethod, "source_location", method_source_location, 0);
    rb_define_method(rb_cMethod, "source_location", method_source_location, 0);
    rb_define_method(rb_cProc, "source_location", proc_source_location, 0);
}

static VALUE
node_source_location(NODE *node)
{
    const char *filename = node->nd_file;
    int lineno = nd_line(node);

    if (filename && lineno) {
        VALUE str = rb_str_new2(filename);

        // TODO: is there a better way of telling whether the file should be absolute
        if (Qtrue == rb_funcall(rb_cFile, rb_intern("exist?"), 1, str))
            str = rb_file_expand_path(str, Qnil);

        return rb_assoc_new(str, INT2FIX(lineno));
    }

    rb_raise(rb_eRuntimeError, "source_location: no file for node");
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
    struct BLOCK *block;
    NODE *node;

    Data_Get_Struct(method, struct METHOD, data);

    if (!(node = data->body))
        rb_raise(rb_eRuntimeError, "source_location: no body for method");

    switch (nd_type(node)) {
    // methods defined by C extensions
    case NODE_CFUNC:
        return Qnil;

    // attr_accessor :foo
    case NODE_IVAR:    // method(:foo)
    case NODE_ATTRSET: // method(:foo=)

        // ruby enterpise edition fixes nd_line on these nodes
#ifdef MBARI_API
        return node_source_location(node);
#else
        // FIXME, Ruby-1.8.7 returns a bad nd_line on these.
        return Qnil;
#endif

    // def foo; end
    case NODE_SCOPE:
        return node_source_location((NODE *)node->u3.value);

    // define_method(:foo, method(:bar))
    case NODE_DMETHOD:
        return method_source_location(node->u3.value);

    // define_method(:foo) { }, define_method(:foo, &bar)
    case NODE_BMETHOD:
        return proc_source_location(node->u3.value);

    default:
        rb_raise(rb_eRuntimeError, "source_location: unhandled method type %d", nd_type(node));

    }
}

/*
 * call-seq:
 *    proc.source_location  => [String, Fixnum]
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

    // Proc.new {}, or Proc.new &proc
    if ((node = data->frame.node) && nd_type(node) == NODE_ITER)
        return node_source_location(node);

    // Proc.new &method(:foo)
    if ((node = data->body) && nd_type(node) == NODE_IFUNC)
        return method_source_location(node->u2.value);

    rb_raise(rb_eRuntimeError, "source_location: unhandled proc type");

}
