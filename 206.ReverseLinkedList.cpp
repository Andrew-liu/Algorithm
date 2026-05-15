/*
 * 206. Reverse Linked List
 * https://leetcode.com/problems/reverse-linked-list/
 *
 * 给你单链表的头节点 head，请你反转链表，并返回反转后的链表。
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
 * 解法一：迭代（三指针）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   用三个指针 prev、curr、next 逐个翻转每条边的方向。
 *   遍历链表，每一步把 curr->next 指向 prev（反转），然后三个指针整体右移。
 *
 * 二、图解
 *
 *   原始链表：1 → 2 → 3 → 4 → 5 → null
 *
 *   初始状态：
 *     prev = null,  curr = 1
 *
 *   第 1 步：保存 next = 2，把 1 → null，移动指针
 *     null ← 1    2 → 3 → 4 → 5 → null
 *            ↑    ↑
 *           prev curr
 *
 *   第 2 步：保存 next = 3，把 2 → 1，移动指针
 *     null ← 1 ← 2    3 → 4 → 5 → null
 *                 ↑    ↑
 *                prev curr
 *
 *   第 3 步：保存 next = 4，把 3 → 2，移动指针
 *     null ← 1 ← 2 ← 3    4 → 5 → null
 *                      ↑    ↑
 *                     prev curr
 *
 *   第 4 步：保存 next = 5，把 4 → 3，移动指针
 *     null ← 1 ← 2 ← 3 ← 4    5 → null
 *                           ↑    ↑
 *                          prev curr
 *
 *   第 5 步：保存 next = null，把 5 → 4，移动指针
 *     null ← 1 ← 2 ← 3 ← 4 ← 5    null
 *                                ↑    ↑
 *                               prev curr
 *
 *   curr == null，循环结束，prev 就是新的头节点 ✅
 *
 *   结果：5 → 4 → 3 → 2 → 1 → null
 *
 * 三、每一步的操作（伪代码）
 *
 *     next = curr->next     // 1. 暂存下一个节点（否则断链后找不到）
 *     curr->next = prev     // 2. 反转当前节点的指向
 *     prev = curr           // 3. prev 右移
 *     curr = next           // 4. curr 右移
 *
 * 时间复杂度：O(n)  —— 遍历链表一次
 * 空间复杂度：O(1)  —— 只用三个指针
 */
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode *prev = nullptr;
        ListNode *curr = head;

        while (curr != nullptr)
        {
            ListNode *next = curr->next; // 暂存下一个节点
            curr->next = prev;           // 反转指向
            prev = curr;                 // prev 右移
            curr = next;                 // curr 右移
        }

        return prev; // prev 即为反转后的头节点
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：递归
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   假设链表的后半部分已经被递归地反转了，我们只需要处理当前节点。
 *   递归到链表末尾，回溯时逐层反转指针方向。
 *
 * 二、递归过程图解
 *
 *   原始链表：1 → 2 → 3 → 4 → 5 → null
 *
 *   递归深入阶段（找到最后一个节点作为新头）：
 *     reverseList(1) → reverseList(2) → reverseList(3) → reverseList(4) → reverseList(5)
 *                                                                          ↑
 *                                                                    base case: 返回 5
 *
 *   回溯阶段（逐层反转）：
 *
 *     回溯到 node=4：
 *       4->next 是 5，让 5->next = 4，然后 4->next = null
 *       null ← 4 ← 5
 *
 *     回溯到 node=3：
 *       3->next 是 4，让 4->next = 3，然后 3->next = null
 *       null ← 3 ← 4 ← 5
 *
 *     回溯到 node=2：
 *       2->next 是 3，让 3->next = 2，然后 2->next = null
 *       null ← 2 ← 3 ← 4 ← 5
 *
 *     回溯到 node=1：
 *       1->next 是 2，让 2->next = 1，然后 1->next = null
 *       null ← 1 ← 2 ← 3 ← 4 ← 5
 *
 *   整个过程中 newHead 始终是 5（原链表的尾节点） ✅
 *
 * 三、关键操作
 *
 *     head->next->next = head;  // 让下一个节点反指向自己
 *     head->next = nullptr;     // 断开自己指向下一个节点的边（防止环）
 *
 *   以 node=3 为例：
 *     操作前：... → 3 → 4 ← 5    （4 和 3 之间还有正向边）
 *     head->next->next = head：  3 ← 4（4 反指 3）
 *     head->next = nullptr：     null ← 3 ← 4 ← 5（断开 3→4 的正向边）
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(n)  —— 递归调用栈深度为 n
 */
class Solution2 {
public:
    ListNode* reverseList(ListNode* head) {
        // base case：空链表 或 只有一个节点
        if (head == nullptr || head->next == nullptr)
        {
            return head;
        }

        // 递归反转后面的部分，newHead 是反转后的头节点
        ListNode *newHead = reverseList(head->next);

        // 让下一个节点指向自己（反转）
        head->next->next = head;
        // 断开自己到下一个节点的正向边
        head->next = nullptr;

        return newHead; // 返回新的头节点（始终是原链表的尾节点）
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法        时间复杂度    空间复杂度    特点
 *   ────        ────────      ────────      ────
 *   迭代        O(n)          O(1)          最优解，无额外空间
 *   递归        O(n)          O(n)          代码简洁，但有栈溢出风险（链表过长时）
 *
 *   面试建议：优先写迭代解法，再补充递归解法展示递归思维。
 */
