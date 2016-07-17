一个带冲突率检查、超时机制的哈希表

相关特性：
 - 开链法解决冲突问题
 - 哈希函数和比较函数通过接口方式提供
 - 支持哈希表查询、插入、删除等操作
 - 哈希表是最大容量的7倍，冲突率较低（4%以下）
 - 超时机制，删除失效元素
 - 统计冲突率，方便调试
 - 动态分配内存，释放的内存被添加空闲列表，降低malloc函数调用次数

用法：
首先创建哈希表，
htable_t* htable_create(uint32_t payload_size, uint32_t cnt_used_max,
                        uint32_t cnt_idle_init, uint32_t cnt_idle_max,
                        uint32_t cnt_timeout, uint32_t (*hash)(const void*),
                        int (*equal)(const void*, const void*));

payload_size为每个数据的大小，cnt_used_max是评估最大容量，cnt_idle_init是预分配的空闲容量，cnt_idle_max是空闲容量的最大值
cnt_timeout是超时时间，如果为0，则不进行超时检查，hash为哈希函数，equal为比较函数。

然后就可以进行插入、查询、删除等操作，
htable_item_t* htable_find(htable_t *table, void *key, void *payload);
htable_item_t* htable_insert(htable_t *table, void *key, void *payload);
int htable_remove(htable_t *table, htable_item_t *item);

另外还可以进行定期删除超时失效的数据项，定期更新当前时间，
void htable_remove_timeout(htable_t *table);
void htable_update_now(htable_t *table, time_t now);

定期打印统计信息
void htable_print_stat(htable_t *table);