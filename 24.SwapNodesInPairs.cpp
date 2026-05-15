/*
 * 24. Swap Nodes in Pairs
 * https://leetcode.com/problems/swap-nodes-in-pairs/
 *
 * 给你一个链表，两两交换其中相邻的节点，并返回交换后链表的头节点。
 * 你必须在不修改节点内部的值的情况下完成本题（即，只能进行节点交换）。
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
 *   每次处理一对相邻节点，交换它们的位置。
 *   需要三个关键指针：prev（前驱）、first（第一个）、second（第二个）。
 *   交换后更新 prev，继续处理下一对。
 *
 * 二、单次交换的指针操作
 *
 *   交换前：prev → first → second → next_pair
 *   交换后：prev → second → first → next_pair
 *
 *   三步操作：
 *     1. prev->next = second        // prev 指向 second
 *     2. first->next = second->next  // first 指向下一对的头
 *     3. second->next = first        // second 指向 first
 *
 *   图示：
 *     交换前：
 *       prev → [first] → [second] → next_pair
 *
 *     第1步 prev->next = second：
 *       prev → [second] → next_pair
 *              [first] → [second]（first 还指着 second，下一步修正）
 *
 *     第2步 first->next = second->next：
 *       prev → [second] → next_pair
 *              [first] → next_pair
 *
 *     第3步 second->next = first：
 *       prev → [second] → [first] → next_pair ✅
 *
 * 三、完整图解
 *
 *   链表：1 → 2 → 3 → 4 → null
 *
 *   加上 dummy：dummy → 1 → 2 → 3 → 4 → null
 *
 *   第 1 轮：prev=dummy, first=1, second=2
 *
 *     交换前：dummy → 1 → 2 → 3 → 4
 *     执行三步操作：
 *       dummy->next = 2
 *       1->next = 2->next = 3
 *       2->next = 1
 *     交换后：dummy → 2 → 1 → 3 → 4
 *
 *     更新 prev = first = 节点 1
 *
 *   第 2 轮：prev=1, first=3, second=4
 *
 *     交换前：... → 1 → 3 → 4 → null
 *     执行三步操作：
 *       1->next = 4
 *       3->next = 4->next = null
 *       4->next = 3
 *     交换后：... → 1 → 4 → 3 → null
 *
 *     更新 prev = first = 节点 3
 *
 *   prev->next == null，不足两个节点，结束
 *
 *   结果：dummy → 2 → 1 → 4 → 3 → null
 *   返回 dummy->next: 2 → 1 → 4 → 3 ✅
 *
 * 四、奇数节点情况
 *
 *   链表：1 → 2 → 3 → null
 *
 *   第 1 轮：交换 1 和 2 → dummy → 2 → 1 → 3
 *   第 2 轮：prev=1, first=3, second=null（不足两个，跳过）
 *
 *   结果：2 → 1 → 3 → null ✅（最后一个节点保持原位）
 *
 * 五、循环条件
 *
 *   while (prev->next != nullptr && prev->next->next != nullptr)
 *
 *   含义：prev 后面至少还有 2 个节点才需要交换。
 *   - prev->next == null：没有节点了
 *   - prev->next->next == null：只剩 1 个节点，无需交换
 *
 * 时间复杂度：O(n)  —— 遍历链表一次，每对处理 O(1)
 * 空间复杂度：O(1)  —— 只用几个指针
 */
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode dummy(0, head);
        ListNode *prev = &dummy;

        // 至少还有两个节点才交换
        while (prev->next != nullptr && prev->next->next != nullptr)
        {
            ListNode *first = prev->next;
            ListNode *second = prev->next->next;

            // 三步交换
            prev->next = second;              // 1. prev 指向 second
            first->next = second->next;       // 2. first 指向下一对
            second->next = first;             // 3. second 指向 first

            // prev 移动到下一对的前驱（即交换后的 first）
            prev = first;
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
 *     - 取出当前的一对节点（head 和 head->next）
 *     - 递归处理剩余链表，得到后续部分交换完的结果
 *     - 交换当前这一对，并把后续结果接上
 *
 *   递归公式：
 *     swap(head) =
 *       second = head->next
 *       head->next = swap(second->next)  // 递归处理后续
 *       second->next = head               // 交换当前对
 *       return second                     // second 成为新头
 *
 * 二、递归过程图解
 *
 *   链表：1 → 2 → 3 → 4 → null
 *
 *   swapPairs(1→2→3→4)
 *     first=1, second=2
 *     递归处理 3→4：swapPairs(3→4)
 *       first=3, second=4
 *       递归处理 null：swapPairs(null)
 *         base case → 返回 null
 *       3->next = null
 *       4->next = 3
 *       返回 4 → 3 → null
 *     1->next = 4→3→null（递归返回的结果）
 *     2->next = 1
 *     返回 2 → 1 → 4 → 3 → null
 *
 *   结果：2 → 1 → 4 → 3 → null ✅
 *
 * 三、递归终止条件
 *
 *   - head == null：空链表，无需交换
 *   - head->next == null：只有一个节点，无法成对，直接返回
 *
 * 四、图解递归栈展开
 *
 *     swapPairs(1) → 需要 swapPairs(3) 的结果
 *       swapPairs(3) → 需要 swapPairs(null) 的结果
 *         swapPairs(null) → 返回 null
 *       3->next = null, 4->next = 3, 返回 4→3
 *     1->next = 4→3, 2->next = 1, 返回 2→1→4→3
 *
 * 时间复杂度：O(n)  —— 每对节点处理一次
 * 空间复杂度：O(n)  —— 递归调用栈深度为 n/2
 */
class Solution2 {
public:
    ListNode* swapPairs(ListNode* head) {
        // base case：空链表 或 只剩一个节点
        if (head == nullptr || head->next == nullptr)
        {
            return head;
        }

        ListNode *first = head;
        ListNode *second = head->next;

        // 递归处理后续部分
        first->next = swapPairs(second->next);
        // 交换当前对
        second->next = first;

        // second 成为这一对的新头
        return second;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法      时间复杂度    空间复杂度    特点
 *   ────      ────────      ────────      ────
 *   迭代      O(n)          O(1)          最优解，三步交换法
 *   递归      O(n)          O(n/2)        代码简洁，递归思维清晰
 *
 *   面试建议：
 *   - 迭代解法重点在于"三步交换"的指针操作顺序，画图辅助理解
 *   - 递归解法更简洁，但需要清楚解释递归语义
 *   - dummy 节点再次统一了边界处理（头节点参与交换时无需特判）
 *   - 延伸：25 题（K 个一组翻转链表）是本题的通用版（K=2 即本题）
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   25. Reverse Nodes in k-Group  —— K 个一组翻转（本题是 K=2 的特例）
 *   206. Reverse Linked List      —— 反转整个链表
 *   92. Reverse Linked List II    —— 反转链表的指定区间
 */
