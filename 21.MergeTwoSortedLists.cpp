/*
 * 21. Merge Two Sorted Lists
 * https://leetcode.com/problems/merge-two-sorted-lists/
 *
 * 将两个升序链表合并为一个新的升序链表并返回。
 * 新链表是通过拼接给定的两个链表的所有节点组成的。
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
 * 解法一：迭代（虚拟头节点）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   创建一个虚拟头节点 dummy，用一个 tail 指针指向新链表的末尾。
 *   每次比较两个链表当前节点的值，把较小的接到 tail 后面。
 *   当一条链表遍历完后，把另一条剩余部分直接接上。
 *
 * 二、为什么用虚拟头节点？
 *
 *   如果不用 dummy，需要单独处理"新链表的第一个节点是谁"的逻辑。
 *   dummy 统一了处理方式：始终往 tail->next 接节点，最后返回 dummy->next。
 *
 * 三、图解
 *
 *   list1: 1 → 2 → 4 → null
 *   list2: 1 → 3 → 4 → null
 *
 *   初始：dummy → null,  tail = dummy
 *
 *   第 1 步：比较 1 vs 1，取 list1 的 1（相等时取任一）
 *     dummy → 1,  tail 指向 1
 *     list1 前进到 2
 *
 *   第 2 步：比较 2 vs 1，取 list2 的 1
 *     dummy → 1 → 1,  tail 指向第二个 1
 *     list2 前进到 3
 *
 *   第 3 步：比较 2 vs 3，取 list1 的 2
 *     dummy → 1 → 1 → 2,  tail 指向 2
 *     list1 前进到 4
 *
 *   第 4 步：比较 4 vs 3，取 list2 的 3
 *     dummy → 1 → 1 → 2 → 3,  tail 指向 3
 *     list2 前进到 4
 *
 *   第 5 步：比较 4 vs 4，取 list1 的 4
 *     dummy → 1 → 1 → 2 → 3 → 4,  tail 指向 4
 *     list1 前进到 null
 *
 *   list1 为空，把 list2 剩余部分接上：
 *     dummy → 1 → 1 → 2 → 3 → 4 → 4 → null
 *
 *   返回 dummy->next ✅
 *
 * 四、处理剩余部分
 *
 *   循环结束时，list1 或 list2 至少有一个为 null。
 *   另一个链表的剩余部分本身就是有序的，直接接到 tail 后面即可。
 *   不需要逐个节点处理！
 *
 *     if list1 还有剩余：tail->next = list1
 *     if list2 还有剩余：tail->next = list2
 *
 *   可以合并为一行：tail->next = (list1 != nullptr) ? list1 : list2
 *
 * 时间复杂度：O(m + n)  —— m、n 分别为两条链表的长度
 * 空间复杂度：O(1)      —— 只用几个指针，原地拼接
 */
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        // 虚拟头节点，简化边界处理
        ListNode dummy;
        ListNode *tail = &dummy;

        while (list1 != nullptr && list2 != nullptr)
        {
            if (list1->val <= list2->val)
            {
                tail->next = list1;
                list1 = list1->next;
            }
            else
            {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }

        // 将剩余部分直接接上
        tail->next = (list1 != nullptr) ? list1 : list2;

        return dummy.next;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：递归
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   合并两个有序链表可以递归定义：
 *     - 比较两个链表头节点的值
 *     - 较小的那个作为合并后的头节点
 *     - 它的 next 指向"剩余部分的合并结果"（递归）
 *
 *   递归公式：
 *     merge(l1, l2) =
 *       if l1.val <= l2.val: l1.next = merge(l1.next, l2), return l1
 *       else:                l2.next = merge(l1, l2.next), return l2
 *
 * 二、递归过程图解
 *
 *   list1: 1 → 2 → 4
 *   list2: 1 → 3 → 4
 *
 *   merge(1→2→4, 1→3→4)
 *     1 <= 1，取 list1 的 1，递归 merge(2→4, 1→3→4)
 *       2 > 1，取 list2 的 1，递归 merge(2→4, 3→4)
 *         2 <= 3，取 list1 的 2，递归 merge(4, 3→4)
 *           4 > 3，取 list2 的 3，递归 merge(4, 4)
 *             4 <= 4，取 list1 的 4，递归 merge(null, 4)
 *               list1 为 null → 返回 list2 的 4
 *             4->next = 4，返回 4
 *           3->next = 4→4，返回 3
 *         2->next = 3→4→4，返回 2
 *       1->next = 2→3→4→4，返回 1
 *     1->next = 1→2→3→4→4，返回 1
 *
 *   结果：1 → 1 → 2 → 3 → 4 → 4 ✅
 *
 * 三、递归终止条件
 *
 *   - list1 为 null → 返回 list2（剩余部分本身有序）
 *   - list2 为 null → 返回 list1
 *
 * 时间复杂度：O(m + n)  —— 每次递归处理一个节点
 * 空间复杂度：O(m + n)  —— 递归调用栈深度最多 m + n
 */
class Solution2 {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        // 递归终止条件
        if (list1 == nullptr) return list2;
        if (list2 == nullptr) return list1;

        if (list1->val <= list2->val)
        {
            // list1 头节点较小，接上后续合并结果
            list1->next = mergeTwoLists(list1->next, list2);
            return list1;
        }
        else
        {
            // list2 头节点较小，接上后续合并结果
            list2->next = mergeTwoLists(list1, list2->next);
            return list2;
        }
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法      时间复杂度    空间复杂度    特点
 *   ────      ────────      ────────      ────
 *   迭代      O(m+n)        O(1)          最优解，用 dummy 简化边界
 *   递归      O(m+n)        O(m+n)        代码简洁，但有栈溢出风险
 *
 *   面试建议：
 *   - 优先写迭代解法（空间最优），展示 dummy 节点技巧
 *   - 递归解法代码更简洁，可作为补充
 *   - 延伸：23 题（合并 K 个有序链表）用分治 + 本题作为子过程
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   23. Merge k Sorted Lists   —— 分治法，两两合并
 *   88. Merge Sorted Array     —— 数组版本的合并
 *   148. Sort List             —— 链表排序（归并排序用到本题）
 */
