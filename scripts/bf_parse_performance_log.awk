#!/usr/bin/awk -f
{ sum[$3] += $2 } END { for (key in sum) print sum[key], key  }