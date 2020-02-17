#ifndef NUI_DATA_H
#define NUI_DATA_H

typedef bool nui_data_get_child_node_by_name_func(
  struct nui_data* node,
  nui_utf8 key,
  struct nui_data* out
);
// get_child_node_by_index
// get_value_string
// get_value_int
// get_value_real
// set_value_string
// set_value_int
// set_value_real
// get_num_children
// get_last_error
// log_error
// acquire_reference
// release_reference

struct nui_data_vtable {
   nui_data_get_child_node_by_name_func get_child_node_by_name;
   // ...
};

struct nui_data {
  struct nui_data_vtable* vtable;
  void* handle;
};

#endif
