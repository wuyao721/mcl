mcl(My C Library)是一个C语言库，主要目的是实现C语言各种常规数据结构，并侧重于提高程序性能。

为什么要用这个库？
首先，在使用C语言使用场景中，mcl提供常见的数据结构和算法；其次，程序性能是该库的首要目标，mcl适合对性能有高要求的项目使用

mcl有哪些缺陷？
首先是通用性，mcl的目标不是成为一个通用的C语言库（像STL那样），mcl对应用场景是有要求的，所以用户得先了解自己需要什么样的数据结构；
其次是易用性，用户可能需要根据自己的需求直接修改库的代码，可以把mcl当作一个基础代码进行二次开发出自己需要的库；
然后是耦合性，在这方面考虑还有欠缺。

非共享内存的数据结构
mcl_list

基于共享内存的数据结构，支持多个进程/线程同时读写
mcl_hash是一个哈希表，特点是可删除，确定是需要提前确定总数的最大值
mcl_array_hash是一个哈希表，特点是可删除，确定是需要提前确定总数的最大值
mcl_array_hash_d是一个哈希表，特点是可删除，确定是需要提前确定总数的最大值
mcl_array_hash_s是一个哈希表，特点是可删除，确定是需要提前确定总数的最大值
mcl_ring是一个环
mcl_array是一个数据

参考
https://en.wikipedia.org/wiki/Hash_table
