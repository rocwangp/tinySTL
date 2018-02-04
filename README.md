# tinySTL

## 完成进度：
* STL基本组件
  * 内存相关
    * 空间配置器
  * 迭代器相关
    * iterator，常见的五种迭代器类型(input, output, forward, bidirectional, random)
    * reverse_iterator，用于容器的rbegin()/rend()操作
    * iterator_traits，用于萃取迭代器中的内嵌类型定义
  * 容器相关
    * vector
    * list
    * deque，双端队列
    * queue，容器适配器，由deque实现
    * stack，容器适配器，由deque实现
    * avl_tree，高度平衡二叉搜索树，通过旋转维持平衡
    * skiplist，跳表，代替红黑树实现map和set
    * map/set，由skiplist实现
    * hashtable，哈希表，开链法解决冲突，必要时进行rehash
    * unordered_map/unordered_set，由哈希表实现
* STL算法
  * 不修改序列的操作
    * all_of/any_of/none_of
    * for_each
    * count/count_of
    * mismatch
    * find/find_if/find_if_not
    * find_end
    * find_first_of
    * adjacent_find
    * search
  * 修改序列操作
    * copy/copy_if
    * copy_backward
    * fill
    * fill_n
    * generate
    * generate_n
    * reverse
  * 排序操作
    * is_sorted
    * sort
  * 堆操作
    * is_heap
    * make_heap
    * push_heap
    * pop_heap
    * sort_heap
  * 最大/最小操作
    * max
    * min
  * 比较操作
    * equal
  * 排列操作
    * is_permutation
  * 未初始化内存上的操作
    * uninitialized_copy
    * uninitialized_copy_n
    * uninitialized_fill
    * uninitialized_fill_n
    * construct
    * destroy





## 性能测试：

>测试环境：操作系统ubuntu，编译器gcc 5.4.0，编辑器vim


```c
//std::vector<int> vec;
tinystl::vector<int> vec;
ProfilerInstance::start();
for(int i = 0; i != count; ++i)
	vec.push_back(value);
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
```

|      container       | operation | quantity(次) | time(ms) |
| :------------------: | :-------: | :---------: | :------: |
| tinystl::vector<int> | push_back |     10万     |   1.5    |
| tinystl::vector<int> | push_back |    100万     |    10    |
| tinystl::vector<int> | push_back |    1000万    |    62    |
|   std::vector<int>   | push_back |     10万     |    2     |
|   std::vector<int>   | push_back |    100万     |    12    |
|   std::vector<int>   | push_back |    1000万    |    72    |



```c
//std::vector<std::string> vec;
tinystl::vector<std::string> vec;
ProfilerInstance::start();
for(int i = 0; i != count; ++i)
	vec.push_back(value);
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
```


|           container            | operation | quantity(次) | time(ms) |
| :----------------------------: | :-------: | :---------: | :------: |
| tinystl::vector\<std::stirng\> | push_back |     10万     |    10    |
| tinystl::vector\<std::stirng\> | push_back |    100万     |    49    |
| tinystl::vector\<std::stirng\> | push_back |    1000万    |   426    |
| tinystl::vector\<std::stirng\> | push_back |     10万     |    10    |
| tinystl::vector\<std::stirng\> | push_back |    100万     |    47    |
| tinystl::vector\<std::stirng\> | push_back |    1000万    |   400    |



```c
//std::string str;
tinystl::string str;
ProfilerInstance::start();
for(int i = 0; i != count; ++i)
	str.push_back(value);
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
```

|    container    | operation | quantity(次) | time(ms) |
| :-------------: | :-------: | :---------: | :------: |
| tinystl::string | push_back |    100万     |    7     |
| tinystl::string | push_back |    1000万    |    37    |
| tinystl::string | push_back |   10000万    |   217    |
|   std::string   | push_back |    100万     |    9     |
|   std::string   | push_back |    1000万    |    40    |
|   std::string   | push_back |   10000万    |   260    |



```c
//std::unordered_set<int> ust;
tinystl::unordered_set<int> ust;
ProfilerInstance::start();
for(int i = 0; i != count; ++i)
	ust.insert(i);
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
```

> unordered_set使用hashtable加开链法实现的，初始桶大小是128。在不考虑适时rehash的情况下插入大量元素会很慢，于是在每次insert之后都判断当前的负载因子，如果达到负载因子的上限，就扩大hashtable的桶大小，默认是原大小的5倍。
>
> 另外页可以考虑将rehash的时间消耗分摊到每次的insert中，参数redis

|           container           | operation | quantity(次) | time(ms) |
| :---------------------------: | :-------: | :---------: | :------: |
| tinystl::unordered_set\<int\> |  insert   |     1万      |    3     |
| tinystl::unordered_set\<int\> |  insert   |     10万     |    30    |
| tinystl::unordered_set\<int\> |  insert   |    100万     |   150    |
|   std::unordered_set\<int\>   |  insert   |     1万      |    3     |
|   std::unordered_set\<int\>   |  insert   |     10万     |    15    |
|   std::unordered_set\<int\>   |  insert   |    100万     |    77    |



```c
//std::unordered_set<int> ust;
tinystl::unordered_set<int> ust; 
int insertCnt = std::max(10000, count / 10000);
for(int i = 0; i != insertCnt; ++i)
	ust.insert(i);
ProfilerInstance::start();
for(int i = 0; i != count; ++i)
	ust.count(i);
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
```


|           container           | operation | quantity(次) | time(ms) |
| :---------------------------: | :-------: | :---------: | :------: |
| tinystl::unordered_set\<int\> |   count   |     1千万     |    95    |
| tinystl::unordered_set\<int\> |   count   |     1亿      |   925    |
| tinystl::unordered_set\<int\> |   count   |     10亿     |  12300   |
|   std::unordered_set\<int\>   |   count   |     1千万     |   142    |
|   std::unordered_set\<int\>   |   count   |     1亿      |   1415   |
|   std::unordered_set\<int\>   |   count   |     10亿     |  15616   |



```c
//std::priority_queue<int> pq;
tinystl::priority_queue<int> pq;
ProfilerInstance::start();
for(int i = 0; i != count; ++i)
	pq.push(i);
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
```

|           container            | operation | quantity(次) | time(ms) |
| :----------------------------: | :-------: | :---------: | :------: |
| tinystl::priority_queue\<int\> |   push    |     10万     |    10    |
| tinystl::priority_queue\<int\> |   push    |    100万     |    60    |
| tinystl::priority_queue\<int\> |   push    |    1000万    |   630    |
|   std::priority_queue\<int\>   |   push    |     10万     |    7     |
|   std::priority_queue\<int\>   |   push    |    100万     |    34    |
|   std::priority_queue\<int\>   |   push    |    1000万    |   283    |



```c
tinystl::vector<int> v;
for(int i = 0; i != count; ++i)
	v.push_back(i);
//std::priority_queue<int> pq(v.begin(), v.end());
tinystl::priority_queue<int> pq(v.begin(), v.end()); 
ProfilerInstance::start();
	for(int i = 0; i != count; ++i)
pq.pop();
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
```


|           container            | operation | quantity(次) | time(ms) |
| :----------------------------: | :-------: | :---------: | :------: |
| tinystl::priority_queue\<int\> |    pop    |     10万     |    7     |
| tinystl::priority_queue\<int\> |    pop    |    100万     |    50    |
| tinystl::priority_queue\<int\> |    pop    |    1000万    |   553    |
|   std::priority_queue\<int\>   |    pop    |     10万     |    9     |
|   std::priority_queue\<int\>   |    pop    |    100万     |    72    |
|   std::priority_queue\<int\>   |    pop    |    1000万    |   836    |



```c
//std::deque<int> dq;
tinystl::deque<int> dq;
ProfilerInstance::start();
const int max = count;
for(int i = 0; i != max / 2; ++i)
	dq.push_front(i);
for(int i = 0; i != max; ++i)
	dq.push_back(i);
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
```

|       container       |    operation    | quantity(次) | time(ms) |
| :-------------------: | :-------------: | :---------: | :------: |
| tinystl::deque\<int\> | push_front/back |     10万     |   2.5    |
| tinystl::deque\<int\> | push_front/back |    100万     |    13    |
| tinystl::deque\<int\> | push_front/back |    1000万    |    75    |
|   std::deque\<int\>   | push_front/back |     10万     |   1.2    |
|   std::deque\<int\>   | push_front/back |    100万     |    10    |
|   std::deque\<int\>   | push_front/back |    1000万    |    40    |



```c
//std::list<int> l;
tinystl::list<int> l;
ProfilerInstance::start();
for(int i = 0; i != count; ++i)
	l.push_back(i);
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
```

|      container       | operation | quantity(次) | time(ms) |
| :------------------: | :-------: | :---------: | :------: |
| tinystl::list\<int\> | push_back |     10万     |    10    |
| tinystl::list\<int\> | push_back |    100万     |    34    |
| tinystl::list\<int\> | push_back |    1000万    |   257    |
|   std::list\<int\>   | push_back |     10万     |    10    |
|   std::list\<int\>   | push_back |    100万     |    36    |
|   std::list\<int\>   | push_back |    1000万    |   280    |



```c
tinystl::list<int> list1;
std::list<int> list2;
std::default_random_engine dre;
std::uniform_int_distribution<int> id;
for(int i = 0; i != count; ++i)
{
    auto n = id(dre);
    list1.push_back(n);
    list2.push_back(n);
}
double cost1 = 0.0, cost2 = 0.0;
for(int i = 0; i != times; ++i)
{
    ProfilerInstance::start();
    list1.sort();
    ProfilerInstance::finish();
    cost1 += ProfilerInstance::millisecond();

    ProfilerInstance::start();
    list2.sort();
    ProfilerInstance::finish();
    cost2 += ProfilerInstance::millisecond();
}
std::cout << "tinystl::list<int>::sort time: " << cost1  << "ms" << std::endl;
std::cout << "std::list<int>::sort time: " << cost2  << "ms" << std::endl;
```

> tinystl::list中的sort最初采用归并排序，但是效率过低，于是改为快排，发现效率仍然不高。最后对快排进行优化，仅采用了插入排序优化和三数取中两种优化方法。由于是双向链表，快排比较好实现，对于单链表的快排还有待学习

|      container       | operation | 数量(个) | quantity(次) | time(ms) |
| :------------------: | :-------: | :---: | :---------: | :------: |
| tinystl::list\<int\> |   sort    |  1万   |    100万     |    60    |
| tinystl::list\<int\> |   sort    |  10万  |     100     |   940    |
| tinystl::list\<int\> |   sort    | 100万  |     100     |  15017   |
|   std::list\<int\>   |   sort    |  1万   |     100     |    53    |
|   std::list\<int\>   |   sort    |  10万  |     100     |   1240   |
|   std::list\<int\>   |   sort    | 100万  |     100     |  34830   |



```c
std::random_device rd;
std::vector<int> v(count);
std::generate(v.begin(), v.end(), [&rd](){ return rd(); });
ProfilerInstance::start();
//std::sort(v.begin(), v.end());
tinystl::sort(v.begin(), v.end());
ProfilerInstance::finish();
ProfilerInstance::dumpDuringTime();
```

>tinystl::algorithm中的sort采用快速排序实现，优化方法有较少元素时采用插入排序，三数去中，将与枢纽pivot相等的值移动到两边


|     container      | operation | quantity(次) | time(ms) |
| :----------------: | :-------: | :---------: | :------: |
| tinystl::algorithm |   sort    |     10万     |    5     |
| tinystl::algorithm |   sort    |    100万     |    70    |
| tinystl::algorithm |   sort    |    1000万    |   815    |
|   std::algorithm   |   sort    |     10万     |    5     |
|   std::algorithm   |   sort    |    100万     |    62    |
|   std::algorithm   |   sort    |    1000万    |   732    |

