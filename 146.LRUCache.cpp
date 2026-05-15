/*
 * 146. LRU Cache
 * https://leetcode.com/problems/lru-cache/
 *
 * 请你设计并实现一个满足 LRU (最近最少使用) 缓存约束的数据结构。
 * 实现 LRUCache 类：
 *   - LRUCache(int capacity)：以正整数作为容量 capacity 初始化 LRU 缓存
 *   - int get(int key)：如果 key 存在于缓存中，返回其值，否则返回 -1
 *   - void put(int key, int value)：如果 key 已存在，则变更其值；
 *     如果不存在，则插入。如果插入导致数量超过 capacity，逐出最久未使用的关键字。
 *   - get 和 put 必须以 O(1) 的平均时间复杂度运行。
 *
 * ──────────────────────────────────────────────
 * 数据结构选择分析
 * ──────────────────────────────────────────────
 *
 *   LRU 缓存需要满足两个核心操作，且都要 O(1)：
 *
 *   1. 快速查找：给定 key，O(1) 找到对应的 value
 *      → 哈希表（unordered_map）
 *
 *   2. 维护使用顺序：记录"谁最近被使用"、"谁最久未使用"
 *      - 每次 get/put 时，被访问的元素要移到"最近使用"的位置
 *      - 容量满时，要删除"最久未使用"的元素
 *      → 需要一个支持 O(1) 插入、删除、移动的有序结构
 *      → 双向链表！
 *
 *   最终方案：哈希表 + 双向链表
 *
 *     ┌─────────────────────────────────────────────────────┐
 *     │  HashMap: key → 链表节点指针                         │
 *     │                                                     │
 *     │  双向链表：head ⇄ [最近使用] ⇄ ... ⇄ [最久未使用] ⇄ tail │
 *     │           (dummy)                              (dummy)│
 *     └─────────────────────────────────────────────────────┘
 *
 * ──────────────────────────────────────────────
 * 解法一：哈希表 + 手写双向链表
 * ──────────────────────────────────────────────
 *
 * 一、数据结构设计
 *
 *   双向链表节点：
 *     struct DLinkedNode {
 *         int key, value;
 *         DLinkedNode *prev, *next;
 *     };
 *
 *   LRUCache：
 *     - unordered_map<int, DLinkedNode*> cache  // key → 节点指针
 *     - DLinkedNode *head, *tail               // dummy 头尾哨兵
 *     - int capacity, size                     // 容量和当前大小
 *
 *   约定：
 *     head->next = 最近使用的节点（MRU）
 *     tail->prev = 最久未使用的节点（LRU）
 *
 * 二、核心操作图解
 *
 *   初始状态（capacity = 3）：
 *     head ⇄ tail    （空链表，只有哨兵）
 *
 *   put(1, 1)：
 *     head ⇄ [1:1] ⇄ tail
 *
 *   put(2, 2)：
 *     head ⇄ [2:2] ⇄ [1:1] ⇄ tail
 *              ↑ 最近使用         ↑ 最久未使用
 *
 *   put(3, 3)：
 *     head ⇄ [3:3] ⇄ [2:2] ⇄ [1:1] ⇄ tail
 *
 *   get(1)：访问 key=1，将其移到头部
 *     head ⇄ [1:1] ⇄ [3:3] ⇄ [2:2] ⇄ tail
 *              ↑ 刚被访问         ↑ 最久未使用（变成了 2）
 *
 *   put(4, 4)：容量已满，逐出 tail->prev = [2:2]，插入新节点到头部
 *     head ⇄ [4:4] ⇄ [1:1] ⇄ [3:3] ⇄ tail
 *              ↑ 新插入                   ↑ 最久未使用
 *
 * 三、各操作的实现
 *
 *   get(key)：
 *     1. 在 map 中查找 key
 *     2. 找不到 → 返回 -1
 *     3. 找到 → 将节点移到链表头部（moveToHead），返回 value
 *
 *   put(key, value)：
 *     1. 在 map 中查找 key
 *     2. 存在 → 更新 value，移到头部
 *     3. 不存在 → 创建新节点，插入头部，放入 map
 *        如果 size > capacity → 删除尾部节点，从 map 中移除
 *
 *   moveToHead(node)：将节点移到头部
 *     = removeNode(node) + addToHead(node)
 *
 *   removeNode(node)：从链表中删除节点（O(1)，双向链表优势）
 *     node->prev->next = node->next
 *     node->next->prev = node->prev
 *
 *   addToHead(node)：插入到 head 之后
 *     node->prev = head
 *     node->next = head->next
 *     head->next->prev = node
 *     head->next = node
 *
 *   removeTail()：删除尾部节点（最久未使用）
 *     node = tail->prev
 *     removeNode(node)
 *     return node（用于从 map 中删除对应 key）
 *
 * 四、为什么用 dummy 头尾哨兵？
 *
 *   没有哨兵时，链表的头尾插入/删除需要特判空链表、只有一个节点等情况。
 *   有了 head 和 tail 两个哨兵，链表永远非空，操作统一，无需特判。
 *
 *   空链表：head ⇄ tail（不是 nullptr）
 *   一个节点：head ⇄ [node] ⇄ tail
 *
 * 五、为什么节点要存 key？
 *
 *   当容量满需要逐出尾部节点时，我们要从 map 中删除对应的条目。
 *   删除需要 key，而我们只有节点指针，所以节点里必须存 key。
 *
 * 时间复杂度：get O(1)，put O(1)
 * 空间复杂度：O(capacity)  —— 哈希表 + 链表存储
 */
class LRUCache {
    struct DLinkedNode {
        int key, value;
        DLinkedNode *prev, *next;
        DLinkedNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
        DLinkedNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

public:
    LRUCache(int capacity) : capacity(capacity), size(0) {
        // 创建 dummy 头尾哨兵
        head = new DLinkedNode();
        tail = new DLinkedNode();
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (!cache.count(key))
        {
            return -1; // 不存在
        }

        // 存在：移到头部，返回值
        DLinkedNode *node = cache[key];
        moveToHead(node);
        return node->value;
    }

    void put(int key, int value) {
        if (cache.count(key))
        {
            // key 已存在：更新值，移到头部
            DLinkedNode *node = cache[key];
            node->value = value;
            moveToHead(node);
        }
        else
        {
            // key 不存在：创建新节点
            DLinkedNode *node = new DLinkedNode(key, value);
            cache[key] = node;
            addToHead(node);
            size++;

            // 超出容量：逐出最久未使用的
            if (size > capacity)
            {
                DLinkedNode *removed = removeTail();
                cache.erase(removed->key);
                delete removed;
                size--;
            }
        }
    }

private:
    int capacity;
    int size;
    DLinkedNode *head, *tail; // dummy 哨兵
    unordered_map<int, DLinkedNode*> cache;

    // 将节点添加到头部（head 之后）
    void addToHead(DLinkedNode *node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    // 从链表中移除节点
    void removeNode(DLinkedNode *node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // 将节点移到头部
    void moveToHead(DLinkedNode *node) {
        removeNode(node);
        addToHead(node);
    }

    // 移除尾部节点（最久未使用），返回被移除的节点
    DLinkedNode* removeTail() {
        DLinkedNode *node = tail->prev;
        removeNode(node);
        return node;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：哈希表 + STL list（简洁写法）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   用 STL 的 std::list（双向链表）替代手写双向链表。
 *   利用 list::splice 方法实现 O(1) 移动节点。
 *
 *   list::splice(pos, list, it)：
 *     将 it 指向的元素从原位置移到 pos 之前，O(1) 操作。
 *
 * 二、数据结构
 *
 *   list<pair<int, int>> lru_list;  // 双向链表，存 {key, value}
 *     front = 最近使用（MRU）
 *     back  = 最久未使用（LRU）
 *
 *   unordered_map<int, list<pair<int,int>>::iterator> cache;
 *     key → 链表中对应节点的迭代器
 *
 * 三、相比手写的区别
 *
 *   优点：代码简洁，不需要管理内存（new/delete）
 *   缺点：STL list 的常数因子略大；面试时面试官可能要求手写
 *
 * 时间复杂度：get O(1)，put O(1)
 * 空间复杂度：O(capacity)
 */
class LRUCache2 {
public:
    LRUCache2(int capacity) : capacity(capacity) {}

    int get(int key) {
        if (!cache.count(key))
        {
            return -1;
        }

        // 将访问的节点移到链表头部
        auto it = cache[key];
        lru_list.splice(lru_list.begin(), lru_list, it);
        return it->second;
    }

    void put(int key, int value) {
        if (cache.count(key))
        {
            // key 已存在：更新值，移到头部
            auto it = cache[key];
            it->second = value;
            lru_list.splice(lru_list.begin(), lru_list, it);
        }
        else
        {
            // 容量满：逐出尾部
            if (lru_list.size() == capacity)
            {
                int evict_key = lru_list.back().first;
                lru_list.pop_back();
                cache.erase(evict_key);
            }

            // 插入新节点到头部
            lru_list.emplace_front(key, value);
            cache[key] = lru_list.begin();
        }
    }

private:
    int capacity;
    list<pair<int, int>> lru_list;                             // {key, value}
    unordered_map<int, list<pair<int, int>>::iterator> cache;   // key → iterator
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法              时间复杂度       空间复杂度    特点
 *   ────              ────────         ────────      ────
 *   手写双向链表      get/put O(1)     O(capacity)   面试标准答案，完全掌控
 *   STL list          get/put O(1)     O(capacity)   代码简洁，利用 splice
 *
 *   面试建议：
 *   - 优先手写双向链表（解法一），展示对底层数据结构的掌握
 *   - 如果面试允许用 STL，解法二更快写完、不容易出错
 *   - 关键点：
 *     1. 为什么是"哈希表 + 双向链表"的组合？（O(1) 查找 + O(1) 移动/删除）
 *     2. 为什么需要 dummy 哨兵？（统一操作，无需特判边界）
 *     3. 为什么节点要存 key？（逐出时需要从 map 中删除）
 *
 * ──────────────────────────────────────────────
 * 完整操作示例
 * ──────────────────────────────────────────────
 *
 *   LRUCache cache(2);  // capacity = 2
 *
 *   cache.put(1, 1);    // 链表：[1:1]
 *   cache.put(2, 2);    // 链表：[2:2] ⇄ [1:1]
 *   cache.get(1);       // 返回 1，链表：[1:1] ⇄ [2:2]（1 移到头部）
 *   cache.put(3, 3);    // 容量满，逐出 key=2（最久未使用）
 *                       // 链表：[3:3] ⇄ [1:1]
 *   cache.get(2);       // 返回 -1（已被逐出）
 *   cache.put(4, 4);    // 容量满，逐出 key=1
 *                       // 链表：[4:4] ⇄ [3:3]
 *   cache.get(1);       // 返回 -1（已被逐出）
 *   cache.get(3);       // 返回 3，链表：[3:3] ⇄ [4:4]
 *   cache.get(4);       // 返回 4，链表：[4:4] ⇄ [3:3]
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   460. LFU Cache                —— 最不经常使用缓存（更复杂）
 *   432. All O'one Data Structure —— 全 O(1) 的数据结构
 *   1472. Design Browser History  —— 设计浏览器历史记录
 */
