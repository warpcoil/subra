---
title: "v7_stack_stat_clean()"
decl_name: "v7_stack_stat_clean"
symbol_kind: "func"
signature: |
  void v7_stack_stat_clean(struct v7 *v7);
---

Available if only `V7_ENABLE_STACK_TRACKING` is defined.

Clean all stack statistics gathered so far. See `v7_stack_stat()` 

