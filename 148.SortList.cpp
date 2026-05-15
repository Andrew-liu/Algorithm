/*
 * 148. Sort List
 * https://leetcode.com/problems/sort-list/
 *
 * 给你链表的头结点 head，请将其按升序排列并返回排序后的链表。
 *
 * 进阶：你可以在 O(n log n) 时间复杂度和 O(1) 空间复杂度下排序链表吗？
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
 * 为什么链表排序首选归并排序？
 * ──────────────────────────────────────────────
 *
 *   - 快速排序：需要随机访问（partition 时），链表不支持，效率低
 *   - 堆排序：需要随机访问，不适合链表
 *   - 归并排序：天然适合链表！
 *     - 找中点：快慢指针 O(n)
 *     - 合并有序链表：21 题的经典操作，O(1) 额外空间
 *     - 不需要随机访问
 *
 * ──────────────────────────────────────────────
 * 解法一：自顶向下归并排序（递归）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   经典的分治法：
 *     1. 找到链表中点，将链表分成两半
 *     2. 递归排序左半部分和右半部分
 *     3. 合并两个有序链表（复用 21 题）
 *
 * 二、图解
 *
 *   链表：4 → 2 → 1 → 3 → null
 *
 *   第一层分割（快慢指针找中点）：
 *     4 → 2 → 1 → 3
 *          ↑ slow 停在 2，从 2 之后断开
 *     左半：4 → 2 → null
 *     右半：1 → 3 → null
 *
 *   第二层分割：
 *     左半 4 → 2：分成 [4] 和 [2]
 *     右半 1 → 3：分成 [1] 和 [3]
 *
 *   递归到底（单个节点天然有序），开始合并：
 *
 *     合并 [4] 和 [2] → 2 → 4
 *     合并 [1] 和 [3] → 1 → 3
 *     合并 [2→4] 和 [1→3] → 1 → 2 → 3 → 4 ✅
 *
 * 三、递归树
 *
 *              [4, 2, 1, 3]
 *              /            \
 *        [4, 2]            [1, 3]
 *        /    \            /    \
 *      [4]   [2]        [1]   [3]
 *        \    /            \    /
 *       [2, 4]            [1, 3]
 *              \          /
 *            [1, 2, 3, 4]
 *
 * 四、找中点的细节
 *
 *   使用快慢指针找中点（与 234 题相同）：
 *     slow 每次走 1 步，fast 每次走 2 步
 *     循环条件：fast->next && fast->next->next
 *     结束时 slow 在前半部分的最后一个节点
 *
 *   断开链表：
 *     mid = slow
 *     rightHead = mid->next
 *     mid->next = nullptr   // 切断！
 *
 *   注意：对于只有 2 个节点的链表 [a, b]：
 *     slow 停在 a，rightHead = b，正确分成 [a] 和 [b]
 *
 * 时间复杂度：O(n log n)  —— 每层 O(n)，共 log n 层
 * 空间复杂度：O(log n)    —— 递归调用栈深度
 */
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        // base case：空链表 或 单个节点
        if (head == nullptr || head->next == nullptr)
        {
            return head;
        }

        // 步骤 1：快慢指针找中点
        ListNode *slow = head;
        ListNode *fast = head;
        while (fast->next != nullptr && fast->next->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        // 从中点断开
        ListNode *rightHead = slow->next;
        slow->next = nullptr;

        // 步骤 2：递归排序左右两半
        ListNode *left = sortList(head);
        ListNode *right = sortList(rightHead);

        // 步骤 3：合并两个有序链表
        return merge(left, right);
    }

private:
    // 合并两个有序链表（21 题）
    ListNode* merge(ListNode* l1, ListNode* l2) {
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
 * 解法二：自底向上归并排序（迭代）—— O(1) 空间
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   自顶向下用递归拆分，需要 O(log n) 栈空间。
 *   自底向上用迭代模拟归并过程，真正做到 O(1) 空间。
 *
 *   思路：
 *     - 先以步长 1 合并相邻的每对节点
 *     - 再以步长 2 合并相邻的每对有序段
 *     - 步长 4, 8, 16... 直到步长 >= 链表长度
 *
 * 二、图解
 *
 *   链表：4 → 2 → 1 → 3 → null (len = 4)
 *
 *   ─── 步长 size = 1 ───
 *   将链表按 size=1 切分，每两段一组合并：
 *
 *     第 1 组：[4] 和 [2] → 合并 → 2 → 4
 *     第 2 组：[1] 和 [3] → 合并 → 1 → 3
 *
 *     本轮结果：2 → 4 → 1 → 3
 *
 *   ─── 步长 size = 2 ───
 *   将链表按 size=2 切分，每两段一组合并：
 *
 *     第 1 组：[2→4] 和 [1→3] → 合并 → 1 → 2 → 3 → 4
 *
 *     本轮结果：1 → 2 → 3 → 4
 *
 *   size = 4 >= len = 4，结束 ✅
 *
 * 三、更长的示例
 *
 *   链表：5 → 3 → 8 → 1 → 6 → 2 → 7 → 4 (len = 8)
 *
 *   size=1: [5][3] → [3,5]  [8][1] → [1,8]  [6][2] → [2,6]  [7][4] → [4,7]
 *           结果：3→5→1→8→2→6→4→7
 *
 *   size=2: [3,5][1,8] → [1,3,5,8]  [2,6][4,7] → [2,4,6,7]
 *           结果：1→3→5→8→2→4→6→7
 *
 *   size=4: [1,3,5,8][2,4,6,7] → [1,2,3,4,5,6,7,8]
 *           结果：1→2→3→4→5→6→7→8 ✅
 *
 * 四、关键操作：切割前 n 个节点
 *
 *   split(head, n)：从 head 开始切出前 n 个节点，返回剩余部分的头。
 *
 *   例：链表 2→4→1→3, split(head, 2)
 *     切出 [2→4]，断开，返回 1（剩余部分的头）
 *
 * 五、每轮合并的流程
 *
 *   对于当前步长 size：
 *     while 链表还有节点未处理:
 *       1. 切出左半段（size 个节点）
 *       2. 切出右半段（size 个节点）
 *       3. 合并左右两段
 *       4. 接到结果链表的尾部
 *
 * 时间复杂度：O(n log n)  —— 外层 log n 轮，每轮合并 O(n)
 * 空间复杂度：O(1)        —— 只用几个指针，无递归
 */
class Solution2 {
public:
    ListNode* sortList(ListNode* head) {
        if (head == nullptr || head->next == nullptr)
        {
            return head;
        }

        // 计算链表长度
        int len = 0;
        ListNode *cur = head;
        while (cur != nullptr)
        {
            len++;
            cur = cur->next;
        }

        ListNode dummy(0, head);

        // 外层循环：步长从 1 开始，每次翻倍
        for (int size = 1; size < len; size *= 2)
        {
            ListNode *prev = &dummy;   // 上一段合并结果的尾
            ListNode *curr = dummy.next; // 当前待处理的头

            while (curr != nullptr)
            {
                // 切出左半段（size 个节点）
                ListNode *left = curr;
                ListNode *right = split(left, size);

                // 切出右半段（size 个节点）
                curr = split(right, size); // curr 指向下一组的起始

                // 合并左右两段，接到 prev 后面
                prev->next = merge(left, right);

                // prev 移动到合并结果的末尾
                while (prev->next != nullptr)
                {
                    prev = prev->next;
                }
            }
        }

        return dummy.next;
    }

private:
    // 从 head 开始切出前 n 个节点，返回剩余部分的头
    ListNode* split(ListNode* head, int n) {
        if (head == nullptr) return nullptr;

        ListNode *cur = head;
        // 走 n-1 步到第 n 个节点
        for (int i = 1; i < n && cur->next != nullptr; ++i)
        {
            cur = cur->next;
        }

        ListNode *rest = cur->next;
        cur->next = nullptr; // 断开
        return rest;
    }

    // 合并两个有序链表
    ListNode* merge(ListNode* l1, ListNode* l2) {
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
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法          时间复杂度    空间复杂度    特点
 *   ────          ────────      ────────      ────
 *   自顶向下      O(n log n)    O(log n)      经典分治，代码清晰
 *   自底向上      O(n log n)    O(1)          进阶要求，真正原地排序
 *
 *   面试建议：
 *   - 先写自顶向下（递归），逻辑清晰不容易出错
 *   - 如果面试官要求 O(1) 空间，再写自底向上（迭代）
 *   - 两种解法都复用了"合并两个有序链表"（21 题）作为子过程
 *   - 自底向上的难点在于 split 操作和指针管理，画图辅助
 *
 * ──────────────────────────────────────────────
 * 知识点总结
 * ──────────────────────────────────────────────
 *
 *   本题综合了多个链表基础操作：
 *     - 快慢指针找中点（234 题）
 *     - 合并两个有序链表（21 题）
 *     - 虚拟头节点技巧
 *     - 链表的断开与拼接
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   21. Merge Two Sorted Lists   —— 本题的核心子过程
 *   23. Merge k Sorted Lists     —— 多路归并
 *   876. Middle of Linked List   —— 快慢指针找中点
 *   912. Sort an Array           —— 数组排序（对比链表排序的差异）
 */
