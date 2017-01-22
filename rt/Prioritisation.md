## Prioritisation

### Number&#95;

All files in this folder get zipped up into "kmods.zip", basically shorthand for zip of kernel modules.

All *.js files get read and executed at boot time as a kernel module and have a prioritisation system.  You'll notice each file starts with a number such as 9&#95; and 1&#95;.

Files with a prefix of 9&#95; are executed first including the base types which are necessary for the drivers which could be given a priority of 8&#95; or lower.

The typical naming convention I have used is priority&#95;file&#95;name.js
