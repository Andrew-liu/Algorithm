/*
 * 19. Remove Nth Node From End of List
 * https://leetcode.com/problems/remove-nth-node-from-end-of-list/
 *
 * 给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。
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
 * 解法一：两次遍历（计算长度）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   第一次遍历：计算链表总长度 len。
 *   第二次遍历：倒数第 n 个 = 正数第 (len - n + 1) 个，
 *              走到它的前一个节点（第 len-n 个），执行删除。
 *
 *   使用 dummy 虚拟头节点，统一处理删除头节点的边界情况。
 *
 * 二、图解
 *
 *   链表：1 → 2 → 3 → 4 → 5 → null,  n = 2
 *
 *   第一次遍历：len = 5
 *
 *   倒数第 2 个 = 正数第 4 个（节点 4）
 *   需要走到它前面的节点 = 正数第 3 个（节点 3）
 *
 *   从 dummy 出发走 len - n = 5 - 2 = 3 步：
 *     dummy → 1 → 2 → 3
 *                       ↑ cur 停在这里
 *
 *   删除操作：cur->next = cur->next->next
 *     即 3->next = 5（跳过节点 4）
 *
 *   结果：1 → 2 → 3 → 5 → null ✅
 *
 * 三、删除头节点的情况
 *
 *   链表：1 → 2 → null,  n = 2（删除倒数第 2 个 = 头节点）
 *
 *   len = 2，从 dummy 走 len - n = 0 步 → cur 就是 dummy
 *   dummy->next = dummy->next->next = 节点 2
 *
 *   结果：2 → null ✅（dummy 节点统一了边界处理）
 *
 * 时间复杂度：O(n)  —— 两次遍历，总共约 2n 步
 * 空间复杂度：O(1)  —— 只用几个指针
 */
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode *cur = &dummy;

        // 第一次遍历：计算链表长度
        int len = 0;
        ListNode *p = head;
        while (p != nullptr)
        {
            len++;
            p = p->next;
        }

        // 第二次遍历：走到待删除节点的前一个
        for (int i = 0; i < len - n; ++i)
        {
            cur = cur->next;
        }

        // 删除 cur 的下一个节点
        cur->next = cur->next->next;

        return dummy.next;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：快慢指针（一次遍历）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   让 fast 指针先走 n 步，然后 fast 和 slow 同时前进。
 *   当 fast 到达链表末尾（null）时，slow 恰好停在待删除节点的前一个。
 *
 *   原理：fast 和 slow 之间始终保持 n 个节点的间距。
 *         fast 到末尾时距离末尾 0 步，slow 距离末尾 n 步，
 *         即 slow 的下一个就是倒数第 n 个节点。
 *
 * 二、图解
 *
 *   链表：1 → 2 → 3 → 4 → 5 → null,  n = 2
 *
 *   使用 dummy 节点：dummy → 1 → 2 → 3 → 4 → 5 → null
 *
 *   初始：slow = dummy, fast = dummy
 *
 *   步骤 1：fast 先走 n = 2 步
 *     fast: dummy → 1 → 2
 *                        ↑ fast
 *     slow 不动：
 *     ↑ slow（在 dummy）
 *
 *   步骤 2：slow 和 fast 同时前进，直到 fast->next == null
 *
 *     同步第1步：slow=1,  fast=3
 *     同步第2步：slow=2,  fast=4
 *     同步第3步：slow=3,  fast=5
 *     fast->next == null，停止！
 *
 *     此时：
 *       dummy → 1 → 2 → 3 → 4 → 5 → null
 *                         ↑              ↑
 *                        slow           fast
 *
 *     slow->next = 节点 4 = 倒数第 2 个 ← 就是要删除的！
 *
 *   步骤 3：删除 slow->next
 *     slow->next = slow->next->next
 *     即 3->next = 5
 *
 *   结果：1 → 2 → 3 → 5 → null ✅
 *
 * 三、为什么 fast 先走 n 步后再同步？
 *
 *   设链表长度为 L，我们要找到第 (L-n) 个节点（待删除节点的前一个）。
 *
 *   fast 先走 n 步后，fast 在位置 n，slow 在位置 0。
 *   之后同时走，fast 到位置 L（null）时，slow 到位置 L - n。
 *   slow 在第 L-n 个位置 = 倒数第 n 个节点的前一个节点 ✓
 *
 *   更直观：fast 和 slow 间距始终为 n
 *
 *     [slow] ← n 个节点 → [fast]
 *                              ↓ (到达末尾)
 *     [slow] ← n 个节点 → [末尾]
 *        ↑
 *     倒数第 n+1 个（正好是待删除节点的前驱）
 *
 * 四、删除头节点的情况
 *
 *   链表：1 → null,  n = 1
 *
 *   dummy → 1 → null
 *   fast 先走 1 步：fast = 节点 1
 *   fast->next == null，slow 不用动，slow = dummy
 *   删除 slow->next：dummy->next = null
 *
 *   返回 dummy->next = null ✅（空链表）
 *
 * 时间复杂度：O(L)  —— 只遍历一次链表
 * 空间复杂度：O(1)  —— 只用两个指针
 */
class Solution2 {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode *slow = &dummy;
        ListNode *fast = &dummy;

        // fast 先走 n 步
        for (int i = 0; i < n; ++i)
        {
            fast = fast->next;
        }

        // slow 和 fast 同时前进，直到 fast 到达最后一个节点
        while (fast->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next;
        }

        // 删除 slow 的下一个节点（即倒数第 n 个）
        slow->next = slow->next->next;

        return dummy.next;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：栈
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   将所有节点压入栈，然后弹出 n 个节点，栈顶就是待删除节点的前驱。
 *   利用栈的 LIFO 特性天然实现"倒数第 n 个"的定位。
 *
 * 二、图解
 *
 *   链表：1 → 2 → 3 → 4 → 5 → null,  n = 2
 *
 *   全部入栈（含 dummy）：
 *     栈底 [dummy, 1, 2, 3, 4, 5] 栈顶
 *
 *   弹出 n = 2 个：弹出 5, 4
 *     栈顶 = 3 ← 这就是待删除节点(4)的前驱
 *
 *   删除：3->next = 3->next->next = 5
 *
 *   结果：1 → 2 → 3 → 5 → null ✅
 *
 * 时间复杂度：O(L)  —— 遍历一次入栈
 * 空间复杂度：O(L)  —— 栈存储所有节点
 */
class Solution3 {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        stack<ListNode*> stk;

        // 所有节点入栈（含 dummy）
        ListNode *cur = &dummy;
        while (cur != nullptr)
        {
            stk.push(cur);
            cur = cur->next;
        }

        // 弹出 n 个节点，栈顶即为待删除节点的前驱
        for (int i = 0; i < n; ++i)
        {
            stk.pop();
        }

        // 删除栈顶节点的下一个
        ListNode *prev = stk.top();
        prev->next = prev->next->next;

        return dummy.next;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法          时间复杂度    空间复杂度    遍历次数    特点
 *   ────          ────────      ────────      ────────    ────
 *   计算长度      O(L)          O(1)          2 次        最直观
 *   快慢指针      O(L)          O(1)          1 次        最优解，一次遍历
 *   栈            O(L)          O(L)          1 次        利用 LIFO 定位
 *
 *   面试建议：
 *   - 重点写快慢指针解法，展示"一次遍历"的思维
 *   - dummy 节点是关键，统一处理删除头节点的边界
 *   - 快慢指针的间距思想在很多链表题中通用（找中点、找倒数第 k 个等）
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   61. Rotate List             —— 也需要找倒数第 k 个节点
 *   141. Linked List Cycle      —— 快慢指针的另一个经典应用
 *   876. Middle of Linked List  —— 快慢指针找中点
 */
