---
title: Error reporting
---

- `-DV7_DISABLE_CALL_ERROR_CONTEXT` - Unless defined, V7 will try to keep
  track of a couple of things that happen before any function call. It
  helps to improve any error message if the called object is not callable. When
  `V7_DISABLE_CALL_ERROR_CONTEXT` is defined, the error message V7 can give you
  is nothing more than "value is not a function".
- `-DV7_DISABLE_LINE_NUMBERS` - Unless defined, V7 stores line numbers in AST
  and bcode in order to provide nicer stack traces. It currently takes approx.
  7-10% of memory needed to store AST and bcode. We plan to drop the overhead
  to < 1%.
- `-DV7_DISABLE_FILENAMES` - Unless defined, each bcode has a pointer to the
  null-terminated string containing the filename from which the bcode was
  generated. It is used in stack traces only. The overhead is:
  - Filename data itself, one string per file;
  - Pointer in each bcode, i.e. in each live script and function.
