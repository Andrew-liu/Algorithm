/*
 * 25. Reverse Nodes in k-Group
 * https://leetcode.com/problems/reverse-nodes-in-k-group/
 *
 * 给你链表的头节点 head，每 k 个节点一组进行翻转，请你返回修改后的链表。
 * k 是一个正整数，它的值小于或等于链表的长度。
 * 如果节点总数不是 k 的整数倍，那么请将最后剩余的节点保持原有顺序。
 * 你不能只是单纯的改变节点内部的值，而是需要实际进行节点交换。
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
 * 解法一：迭代（分组翻转）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   将链表按 k 个节点分组，对每一组进行翻转，然后把各组首尾相连。
 *   分为三步：
 *     1. 检查剩余节点是否够 k 个（不够就保持原样）
 *     2. 翻转当前这 k 个节点
 *     3. 连接前一组的尾部和当前组翻转后的头部
 *
 * 二、单组翻转的过程（复用 206 题的思路）
 *
 *   翻转 [start, end) 区间内的 k 个节点：
 *
 *     翻转前：prev → [a → b → c] → next_group
 *                     ↑ start       ↑ end
 *
 *     翻转后：prev → [c → b → a] → next_group
 *                     ↑ 新头        ↑ 新尾（原来的 start）
 *
 * 三、完整图解
 *
 *   链表：1 → 2 → 3 → 4 → 5 → null,  k = 3
 *
 *   加上 dummy：dummy → 1 → 2 → 3 → 4 → 5 → null
 *
 *   ─── 第 1 组 ───
 *
 *   检查：从 dummy->next 开始数 k=3 个，1,2,3 够了 ✓
 *
 *   翻转 [1,2,3]：
 *     prev=dummy, 翻转区间：1 → 2 → 3
 *
 *     翻转过程（标准三指针翻转）：
 *       null ← 1    2 → 3     (p=null, c=1, n=2)
 *       null ← 1 ← 2    3    (p=1, c=2, n=3)
 *       null ← 1 ← 2 ← 3    (p=2, c=3, n=null...不对，n=4)
 *
 *     实际上翻转 k 个节点后：
 *       翻转部分：3 → 2 → 1
 *       节点 1 的 next 应接上下一组的头（节点 4）
 *
 *   连接：
 *     dummy->next = 3（翻转后的新头）
 *     1->next = 4（下一组的头）
 *
 *     结果：dummy → 3 → 2 → 1 → 4 → 5 → null
 *
 *   更新 prev = 节点 1（当前组翻转后的尾）
 *
 *   ─── 第 2 组 ───
 *
 *   检查：从节点 1->next=4 开始数 k=3 个，4,5... 只有 2 个，不够 ✗
 *   保持原样，结束
 *
 *   最终结果：3 → 2 → 1 → 4 → 5 → null ✅
 *
 * 四、k = 2 的情况（对比 24 题）
 *
 *   链表：1 → 2 → 3 → 4 → null,  k = 2
 *
 *   第 1 组：翻转 [1,2] → 2 → 1，prev = 1
 *   第 2 组：翻转 [3,4] → 4 → 3，prev = 3
 *
 *   结果：2 → 1 → 4 → 3 → null ✅（与 24 题结果一致）
 *
 * 五、辅助函数：翻转 k 个节点
 *
 *   reverse(head, k) 翻转从 head 开始的 k 个节点
 *   返回翻转后的新头（原来的第 k 个节点）
 *   翻转后 head 变为尾节点（head->next 指向第 k+1 个节点之后的内容）
 *
 * 时间复杂度：O(n)  —— 每个节点最多被访问 2 次（检查 + 翻转）
 * 空间复杂度：O(1)  —— 只用几个指针
 */
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0, head);
        ListNode *prev = &dummy;

        while (true)
        {
            // 步骤 1：检查剩余节点是否够 k 个
            ListNode *check = prev;
            for (int i = 0; i < k; ++i)
            {
                check = check->next;
                if (check == nullptr)
                {
                    return dummy.next; // 不够 k 个，直接返回
                }
            }

            // 步骤 2：翻转从 prev->next 开始的 k 个节点
            ListNode *groupHead = prev->next;  // 当前组的第一个节点（翻转后变尾）
            ListNode *curr = groupHead;
            ListNode *p = nullptr;

            for (int i = 0; i < k; ++i)
            {
                ListNode *next = curr->next;
                curr->next = p;
                p = curr;
                curr = next;
            }
            // 此时 p = 翻转后的新头，curr = 下一组的第一个节点

            // 步骤 3：连接
            prev->next = p;             // 前一组的尾 → 当前组翻转后的头
            groupHead->next = curr;     // 当前组翻转后的尾 → 下一组的头

            // 更新 prev 为当前组翻转后的尾（即原来的 groupHead）
            prev = groupHead;
        }

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
 *   递归定义：
 *     1. 检查当前位置开始是否有 k 个节点
 *     2. 如果有，翻转这 k 个节点
 *     3. 将翻转后的尾节点的 next 指向递归处理后续部分的结果
 *     4. 返回翻转后的头节点
 *
 *   递归公式：
 *     reverseKGroup(head, k) =
 *       if 不足 k 个: return head（保持原样）
 *       翻转前 k 个节点，得到 newHead
 *       head->next = reverseKGroup(第 k+1 个节点, k)
 *       return newHead
 *
 * 二、递归过程图解
 *
 *   链表：1 → 2 → 3 → 4 → 5 → null,  k = 3
 *
 *   reverseKGroup(1→2→3→4→5, 3)
 *     检查：1,2,3 够 3 个 ✓
 *     翻转 [1,2,3]：得到 3→2→1，下一组从 4 开始
 *     递归：1->next = reverseKGroup(4→5, 3)
 *       检查：4,5 只有 2 个，不够 3 个 ✗
 *       返回 4→5（保持原样）
 *     1->next = 4→5
 *     返回 3→2→1→4→5
 *
 *   结果：3 → 2 → 1 → 4 → 5 → null ✅
 *
 * 三、递归图解（k=2, 完全整除的情况）
 *
 *   链表：1 → 2 → 3 → 4 → null,  k = 2
 *
 *   reverseKGroup(1→2→3→4, 2)
 *     翻转 [1,2]：得到 2→1
 *     1->next = reverseKGroup(3→4, 2)
 *       翻转 [3,4]：得到 4→3
 *       3->next = reverseKGroup(null, 2)
 *         head == null，不足 k 个，返回 null
 *       3->next = null
 *       返回 4→3→null
 *     1->next = 4→3→null
 *     返回 2→1→4→3→null ✅
 *
 * 时间复杂度：O(n)  —— 每个节点处理一次
 * 空间复杂度：O(n/k)  —— 递归调用栈深度为 n/k（共 n/k 组）
 */
class Solution2 {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        // 步骤 1：检查剩余节点是否够 k 个
        ListNode *check = head;
        for (int i = 0; i < k; ++i)
        {
            if (check == nullptr)
            {
                return head; // 不足 k 个，保持原样
            }
            check = check->next;
        }

        // 步骤 2：翻转前 k 个节点
        ListNode *curr = head;
        ListNode *prev = nullptr;
        for (int i = 0; i < k; ++i)
        {
            ListNode *next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        // prev = 翻转后的新头，curr = 第 k+1 个节点
        // head 变为翻转后的尾

        // 步骤 3：递归处理后续部分，接到当前组的尾部
        head->next = reverseKGroup(curr, k);

        return prev; // 返回翻转后的头
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法      时间复杂度    空间复杂度    特点
 *   ────      ────────      ────────      ────
 *   迭代      O(n)          O(1)          最优解，完全原地操作
 *   递归      O(n)          O(n/k)        代码清晰，递归语义明确
 *
 *   面试建议：
 *   - 这是链表题中的 Hard 题，核心难点在于指针操作不出错
 *   - 分解子问题：先会翻转 k 个节点（206 题），再处理分组逻辑
 *   - 画图是关键！每一步操作前画出当前指针状态
 *   - dummy 节点统一处理第一组翻转后的头连接问题
 *   - 注意"检查不足 k 个则保持原样"这个约束
 *
 * ──────────────────────────────────────────────
 * 思路拆解（面试时的表达）
 * ──────────────────────────────────────────────
 *
 *   1. "这道题可以分解为：分组 + 组内翻转 + 组间连接"
 *   2. "组内翻转就是 206 题的标准翻转，只不过限定翻转 k 个"
 *   3. "分组时需要先检查够不够 k 个，不够就直接返回"
 *   4. "连接时需要：前一组的尾 → 当前组的新头，当前组的新尾 → 下一组的头"
 *   5. "用 dummy 节点统一第一组的处理"
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   24. Swap Nodes in Pairs    —— 本题 k=2 的特例
 *   206. Reverse Linked List   —— 翻转整个链表（本题的子过程）
 *   92. Reverse Linked List II —— 翻转链表指定区间
 */
