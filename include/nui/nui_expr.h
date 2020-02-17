#ifndef NUI_EXPR_H
#define NUI_EXPR_H

// how to associate a widget with a value in the program?

// label foo {
//   expr: "bar.qux"
// }


// expr factory func
typedef bool nui_compile_expr_func(
  struct nui_gui*,
  nui_utf8 expr,
  struct nui_compiled_expr* out
);

typedef nui_bool nui_compiled_expr_evaluate_func(
  struct nui_compiled_expr* expr,
  struct nui_gui* gui,
  struct nui_data* out
);

typedef void nui_compiled_expr_acquire_reference(
   struct nui_compiled_expr* expr
);

typedef void nui_compiled_expr_release_reference(
  struct nui_compiled_expr* expr
);

struct nui_compiled_expr_vtable {
  nui_compiled_expr_evaluate_func evaluate;
  nui_compiled_expr_acquire_reference_func acquire_reference;
  nui_compiled_expr_acquire_reference_func release_reference;
};

struct nui_compiled_expr {
  struct nui_compiled_expr_vtable* vtable;
  void* handle;
};

#endif
