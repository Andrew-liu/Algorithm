/*
 * 2. Add Two Numbers
 * https://leetcode.com/problems/add-two-numbers/
 *
 * 给你两个非空的链表，表示两个非负的整数。它们每位数字都是按照逆序的方式存储的，
 * 并且每个节点只能存储一位数字。请你将两个数相加，并以相同形式返回一个表示和的链表。
 * 你可以假设除了数字 0 之外，这两个数都不会以 0 开头。
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
 * 解法一：模拟加法（虚拟头节点）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   链表已经是逆序存储（个位在头部），天然适合从低位到高位做加法。
 *   模拟竖式加法：逐位相加，处理进位，直到两条链表都遍历完且无进位。
 *
 * 二、竖式加法回顾
 *
 *   例：342 + 465 = 807
 *
 *   链表存储（逆序）：
 *     l1: 2 → 4 → 3        （表示 342）
 *     l2: 5 → 6 → 4        （表示 465）
 *
 *   竖式计算（从个位开始）：
 *       2 + 5 = 7，进位 0
 *       4 + 6 = 10，写 0 进位 1
 *       3 + 4 + 1(进位) = 8，进位 0
 *
 *   结果链表：7 → 0 → 8     （表示 807）✅
 *
 * 三、图解详细过程
 *
 *   l1: 2 → 4 → 3 → null
 *   l2: 5 → 6 → 4 → null
 *
 *   初始：dummy → null,  carry = 0
 *
 *   第 1 步：sum = 2 + 5 + 0 = 7
 *     digit = 7 % 10 = 7,  carry = 7 / 10 = 0
 *     dummy → 7
 *     l1→4, l2→6
 *
 *   第 2 步：sum = 4 + 6 + 0 = 10
 *     digit = 10 % 10 = 0,  carry = 10 / 10 = 1
 *     dummy → 7 → 0
 *     l1→3, l2→4
 *
 *   第 3 步：sum = 3 + 4 + 1 = 8
 *     digit = 8 % 10 = 8,  carry = 8 / 10 = 0
 *     dummy → 7 → 0 → 8
 *     l1→null, l2→null
 *
 *   l1、l2 都为 null 且 carry == 0，结束
 *   返回 dummy->next: 7 → 0 → 8 ✅
 *
 * 四、不等长链表 & 最后有进位的情况
 *
 *   l1: 9 → 9 → 9 → 9 → null       （表示 9999）
 *   l2: 9 → 9 → null                （表示 99）
 *
 *   第 1 步：9 + 9 + 0 = 18 → digit=8, carry=1
 *   第 2 步：9 + 9 + 1 = 19 → digit=9, carry=1
 *   第 3 步：9 + 0 + 1 = 10 → digit=0, carry=1  （l2 已空，视为 0）
 *   第 4 步：9 + 0 + 1 = 10 → digit=0, carry=1  （l2 仍空）
 *   第 5 步：0 + 0 + 1 = 1  → digit=1, carry=0  （l1 也空了，只有进位）
 *
 *   结果：8 → 9 → 0 → 0 → 1         （表示 10098）
 *   验证：9999 + 99 = 10098 ✅
 *
 * 五、循环统一处理的技巧
 *
 *   循环条件：while (l1 || l2 || carry)
 *
 *   这样写的好处：
 *   - l1 为空时自动补 0
 *   - l2 为空时自动补 0
 *   - 最后如果还有进位（如 5+5=10），会自动多生成一个节点
 *   - 不需要单独处理这三种边界情况！
 *
 * 时间复杂度：O(max(m, n))  —— m、n 分别为两条链表的长度
 * 空间复杂度：O(max(m, n))  —— 结果链表的长度最多为 max(m,n) + 1
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy;
        ListNode *tail = &dummy;
        int carry = 0;

        // 只要还有节点没处理完 或 还有进位，就继续
        while (l1 != nullptr || l2 != nullptr || carry != 0)
        {
            int sum = carry;

            if (l1 != nullptr)
            {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2 != nullptr)
            {
                sum += l2->val;
                l2 = l2->next;
            }

            carry = sum / 10;          // 进位
            tail->next = new ListNode(sum % 10);  // 当前位
            tail = tail->next;
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
 *   递归定义：当前位的结果 = (l1.val + l2.val + carry) % 10
 *   当前节点的 next = 递归处理下一位（传入新的进位）
 *
 *   递归终止：l1、l2 都为 null 且 carry == 0
 *
 * 二、图解
 *
 *   addTwo(2→4→3, 5→6→4, carry=0)
 *     sum = 2+5+0 = 7, node(7)
 *     node(7)->next = addTwo(4→3, 6→4, carry=0)
 *       sum = 4+6+0 = 10, node(0)
 *       node(0)->next = addTwo(3, 4, carry=1)
 *         sum = 3+4+1 = 8, node(8)
 *         node(8)->next = addTwo(null, null, carry=0)
 *           全为空且无进位 → 返回 null
 *         返回 node(8)
 *       返回 node(0) → 0 → 8
 *     返回 node(7) → 7 → 0 → 8
 *
 *   结果：7 → 0 → 8 ✅
 *
 * 时间复杂度：O(max(m, n))  —— 每次递归处理一位
 * 空间复杂度：O(max(m, n))  —— 递归调用栈深度 + 结果链表
 */
class Solution2 {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        return helper(l1, l2, 0);
    }

private:
    ListNode* helper(ListNode* l1, ListNode* l2, int carry) {
        // 递归终止：两链表都空且无进位
        if (l1 == nullptr && l2 == nullptr && carry == 0)
        {
            return nullptr;
        }

        int sum = carry;
        if (l1 != nullptr) sum += l1->val;
        if (l2 != nullptr) sum += l2->val;

        // 创建当前位的节点
        ListNode *node = new ListNode(sum % 10);

        // 递归处理下一位
        node->next = helper(
            l1 ? l1->next : nullptr,
            l2 ? l2->next : nullptr,
            sum / 10
        );

        return node;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法      时间复杂度      空间复杂度          特点
 *   ────      ────────        ────────            ────
 *   迭代      O(max(m,n))     O(max(m,n))         直观，用 dummy 简化
 *   递归      O(max(m,n))     O(max(m,n))         代码简洁，额外栈空间
 *
 *   注：两种解法都需要 O(max(m,n)) 空间来存储结果链表，
 *       这是题目要求返回新链表所必需的，不算"额外"空间。
 *       递归解法的额外开销在于调用栈。
 *
 *   面试建议：
 *   - 迭代解法是标准写法，重点在于循环条件 (l1 || l2 || carry) 的统一处理
 *   - 注意边界：两链表长度不同、最高位有进位（如 5+5=10）
 *   - 延伸：445 题（链表数字正序存储）需要先反转或用栈
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   445. Add Two Numbers II  —— 数字正序存储，需要栈或反转
 *   67. Add Binary           —— 二进制字符串相加（同样模拟进位）
 *   415. Add Strings         —— 字符串数字相加（同样模拟进位）
 */
