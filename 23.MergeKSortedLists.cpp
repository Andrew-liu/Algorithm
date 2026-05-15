/*
 * 23. Merge k Sorted Lists
 * https://leetcode.com/problems/merge-k-sorted-lists/
 *
 * 给你一个链表数组，每个链表都已经按升序排列。
 * 请你将所有链表合并到一个升序链表中，返回合并后的链表。
 *
 *   链表结构：
 *     struct ListNode {
 *         int val;
 *         ListNode *next;
 *         ListNode() : val(0), next(nullptr) {}
 *         ListNode(int x) : val(x), next(nullptr) {}
 *         ListNode(int x, ListNode *next) : val(x), next(next) {}
 *     };
 *
 * ──────────────────────────────────────────────
 * 解法一：逐一合并
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   最直观的方法：将 k 个链表逐一合并。
 *   先合并前两个，结果再和第三个合并，依此类推。
 *   每次合并复用 21 题的"合并两个有序链表"。
 *
 * 二、图解
 *
 *   lists = [l1, l2, l3, l4]
 *
 *   第 1 步：merge(l1, l2) → m1
 *   第 2 步：merge(m1, l3) → m2
 *   第 3 步：merge(m2, l4) → m3（最终结果）
 *
 * 三、时间分析（为什么不够优？）
 *
 *   设 k 个链表，总节点数为 N，每条链表平均长度 N/k。
 *
 *   第 1 次合并：处理 N/k + N/k = 2N/k 个节点
 *   第 2 次合并：处理 2N/k + N/k = 3N/k 个节点
 *   第 3 次合并：处理 3N/k + N/k = 4N/k 个节点
 *   ...
 *   第 k-1 次：处理 N 个节点
 *
 *   总计：(2+3+...+k) × N/k ≈ O(kN)
 *
 *   当 k 很大时效率不高，因为前面合并的结果被反复遍历。
 *
 * 时间复杂度：O(kN)  —— k 个链表，N 为总节点数
 * 空间复杂度：O(1)   —— 原地合并
 */
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;

        ListNode *result = lists[0];
        for (int i = 1; i < lists.size(); ++i)
        {
            result = mergeTwoLists(result, lists[i]);
        }
        return result;
    }

private:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy;
        ListNode *tail = &dummy;

        while (l1 != nullptr && l2 != nullptr)
        {
            if (l1->val <= l2->val)
            {
                tail->next = l1;
                l1 = l1->next;
            }
            else
            {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }

        tail->next = (l1 != nullptr) ? l1 : l2;
        return dummy.next;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：分治合并（Divide and Conquer）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   将 k 个链表两两配对合并，得到 k/2 个链表；
 *   再两两配对合并，得到 k/4 个链表；
 *   重复直到只剩 1 个链表。
 *
 *   类似归并排序的分治思想，避免了逐一合并中前面结果被反复遍历的问题。
 *
 * 二、图解
 *
 *   lists = [l1, l2, l3, l4, l5, l6]
 *
 *   第 1 轮（两两合并，k=6 → 3）：
 *     merge(l1, l2) → m1
 *     merge(l3, l4) → m2
 *     merge(l5, l6) → m3
 *
 *   第 2 轮（k=3 → 2）：
 *     merge(m1, m2) → m4
 *     m3 落单，直接保留
 *
 *   第 3 轮（k=2 → 1）：
 *     merge(m4, m3) → 最终结果
 *
 *   分治树：
 *          [l1, l2, l3, l4, l5, l6]
 *          /                       \
 *     [l1, l2, l3]            [l4, l5, l6]
 *      /       \                /       \
 *   [l1,l2]   [l3]          [l4,l5]   [l6]
 *      |        |              |        |
 *    merge    (单独)         merge    (单独)
 *      \       /                \       /
 *      merge                    merge
 *          \                    /
 *              最终 merge
 *
 * 三、为什么比逐一合并快？
 *
 *   逐一合并：每个节点最多被遍历 k-1 次 → O(kN)
 *   分治合并：每个节点最多被遍历 log k 次 → O(N log k)
 *
 *   直觉：分治让每条链表参与合并的次数从 O(k) 降到 O(log k)
 *
 * 时间复杂度：O(N log k)  —— 每层合并 O(N)，共 log k 层
 * 空间复杂度：O(log k)    —— 递归调用栈深度
 */
class Solution2 {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        return divideAndConquer(lists, 0, lists.size() - 1);
    }

private:
    ListNode* divideAndConquer(vector<ListNode*>& lists, int left, int right) {
        // 只有一个链表，直接返回
        if (left == right)
        {
            return lists[left];
        }

        // 分成两半
        int mid = left + (right - left) / 2;
        ListNode *l1 = divideAndConquer(lists, left, mid);
        ListNode *l2 = divideAndConquer(lists, mid + 1, right);

        // 合并两个有序链表
        return mergeTwoLists(l1, l2);
    }

    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy;
        ListNode *tail = &dummy;

        while (l1 != nullptr && l2 != nullptr)
        {
            if (l1->val <= l2->val)
            {
                tail->next = l1;
                l1 = l1->next;
            }
            else
            {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }

        tail->next = (l1 != nullptr) ? l1 : l2;
        return dummy.next;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：优先队列（最小堆）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   维护一个大小为 k 的最小堆，堆中存放每个链表当前的头节点。
 *   每次从堆中取出最小的节点，接到结果链表后面。
 *   然后将该节点的 next（如果存在）放入堆中。
 *
 *   本质：用堆来高效地在 k 个候选节点中找最小值。
 *
 * 二、图解
 *
 *   lists = [1→4→5, 1→3→4, 2→6]
 *
 *   初始化堆（放入每个链表的头）：
 *     heap = [1(l1), 1(l2), 2(l3)]（最小堆）
 *
 *   第 1 步：弹出 1(l1)，result: 1，把 4 入堆
 *     heap = [1(l2), 2(l3), 4(l1)]
 *
 *   第 2 步：弹出 1(l2)，result: 1→1，把 3 入堆
 *     heap = [2(l3), 4(l1), 3(l2)]
 *
 *   第 3 步：弹出 2(l3)，result: 1→1→2，把 6 入堆
 *     heap = [3(l2), 4(l1), 6(l3)]
 *
 *   第 4 步：弹出 3(l2)，result: 1→1→2→3，把 4 入堆
 *     heap = [4(l1), 6(l3), 4(l2)]
 *
 *   第 5 步：弹出 4(l1)，result: 1→1→2→3→4，把 5 入堆
 *     heap = [4(l2), 6(l3), 5(l1)]
 *
 *   第 6 步：弹出 4(l2)，result: 1→1→2→3→4→4，l2 无 next
 *     heap = [5(l1), 6(l3)]
 *
 *   第 7 步：弹出 5(l1)，result: 1→1→2→3→4→4→5，l1 无 next
 *     heap = [6(l3)]
 *
 *   第 8 步：弹出 6(l3)，result: 1→1→2→3→4→4→5→6，l3 无 next
 *     heap = []（空），结束
 *
 *   结果：1→1→2→3→4→4→5→6 ✅
 *
 * 三、为什么是 O(N log k)？
 *
 *   堆的大小始终 ≤ k（每个链表最多一个节点在堆中）
 *   每个节点入堆一次、出堆一次，每次操作 O(log k)
 *   总共 N 个节点 → O(N log k)
 *
 * 时间复杂度：O(N log k)  —— N 个节点，每次堆操作 O(log k)
 * 空间复杂度：O(k)        —— 堆中最多 k 个元素
 */
class Solution3 {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // 自定义比较器：小顶堆
        auto cmp = [](ListNode* a, ListNode* b) {
            return a->val > b->val; // 注意：greater 表示小顶堆
        };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);

        // 将每个链表的头节点放入堆
        for (ListNode* head : lists)
        {
            if (head != nullptr)
            {
                pq.push(head);
            }
        }

        ListNode dummy;
        ListNode *tail = &dummy;

        while (!pq.empty())
        {
            // 取出最小节点
            ListNode *node = pq.top();
            pq.pop();

            // 接到结果链表
            tail->next = node;
            tail = tail->next;

            // 该节点的下一个入堆
            if (node->next != nullptr)
            {
                pq.push(node->next);
            }
        }

        return dummy.next;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法          时间复杂度    空间复杂度    特点
 *   ────          ────────      ────────      ────
 *   逐一合并      O(kN)         O(1)          最直观，但效率最低
 *   分治合并      O(N log k)    O(log k)      递归分治，高效
 *   优先队列      O(N log k)    O(k)          用堆维护 k 路最小值
 *
 *   N = 所有节点总数，k = 链表个数
 *
 *   面试建议：
 *   - 解法二和解法三都是最优时间复杂度 O(N log k)
 *   - 分治合并：代码量少，复用"合并两个有序链表"，推荐先写
 *   - 优先队列：思路直观（"k 路归并"），适合解释清楚
 *   - 逐一合并：作为 baseline 提及，说明为什么需要优化
 *
 * ──────────────────────────────────────────────
 * 思路演进（面试时的表达）
 * ──────────────────────────────────────────────
 *
 *   1. "最简单的方法是逐一合并，但时间是 O(kN)，前面的结果被反复遍历"
 *   2. "优化方向一：分治，两两合并，每个节点只被遍历 log k 次 → O(N log k)"
 *   3. "优化方向二：用最小堆维护 k 个候选头节点，每次 O(log k) 取最小 → O(N log k)"
 *   4. "两种优化时间复杂度一样，分治空间更优 O(log k) vs O(k)"
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   21. Merge Two Sorted Lists  —— 本题的核心子过程
 *   148. Sort List              —— 链表归并排序（分治 + 合并）
 *   264. Ugly Number II         —— 多路归并思想
 *   378. Kth Smallest in Matrix —— 优先队列多路归并
 */
