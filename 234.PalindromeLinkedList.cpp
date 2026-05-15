/*
 * 234. Palindrome Linked List
 * https://leetcode.com/problems/palindrome-linked-list/
 *
 * 给你一个单链表的头节点 head，请你判断该链表是否为回文链表。
 * 如果是，返回 true；否则，返回 false。
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
 * 解法一：复制到数组 + 双指针
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   链表不支持随机访问，无法直接用下标比较首尾元素。
 *   最直观的方法：把链表的值复制到数组中，然后用双指针从两端向中间比较。
 *
 * 二、图解
 *
 *   链表：1 → 2 → 2 → 1 → null
 *
 *   复制到数组：[1, 2, 2, 1]
 *
 *   双指针比较：
 *     left=0, right=3：arr[0]=1 == arr[3]=1 ✓
 *     left=1, right=2：arr[1]=2 == arr[2]=2 ✓
 *     left >= right，结束 → 是回文 ✅
 *
 *   非回文示例：1 → 2 → 3 → 1 → null → [1, 2, 3, 1]
 *     left=0, right=3：arr[0]=1 == arr[3]=1 ✓
 *     left=1, right=2：arr[1]=2 != arr[2]=3 ✗ → 不是回文 ❌
 *
 * 时间复杂度：O(n)  —— 遍历链表 + 双指针各 O(n)
 * 空间复杂度：O(n)  —— 需要额外数组存储所有值
 */
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        // 将链表值复制到数组
        vector<int> vals;
        ListNode *cur = head;
        while (cur != nullptr)
        {
            vals.push_back(cur->val);
            cur = cur->next;
        }

        // 双指针从两端向中间比较
        int left = 0, right = vals.size() - 1;
        while (left < right)
        {
            if (vals[left] != vals[right])
            {
                return false;
            }
            left++;
            right--;
        }

        return true;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：快慢指针找中点 + 反转后半部分 + 逐一比较
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   要做到 O(1) 空间，核心思路分三步：
 *     1. 用快慢指针找到链表中点
 *     2. 反转后半部分链表
 *     3. 前半部分和反转后的后半部分逐一比较
 *
 *   （可选）第 4 步：恢复链表原始结构（面试加分项）
 *
 * 二、图解（奇数长度）
 *
 *   原始链表：1 → 2 → 3 → 2 → 1 → null
 *
 *   步骤 1：快慢指针找中点
 *     slow 每次走 1 步，fast 每次走 2 步
 *
 *     初始：  slow=1, fast=1
 *     第1步： slow=2, fast=3
 *     第2步： slow=3, fast=5
 *     第3步： slow 不动（fast->next == null，停止）
 *                          ↑ slow 停在节点 3（中点）
 *
 *     实际上 fast->next 为 null 时停止，slow 指向中间节点。
 *     后半部分起点 = slow->next = 节点 2（第二个）
 *
 *     前半部分：1 → 2 → 3
 *     后半部分：2 → 1
 *
 *   步骤 2：反转后半部分
 *     2 → 1 → null  变成  1 → 2 → null
 *
 *   步骤 3：逐一比较
 *     前半：1 → 2 → 3...
 *     后半：1 → 2 → null
 *
 *     比较：1==1 ✓, 2==2 ✓, 后半到 null 结束 → 是回文 ✅
 *
 * 三、图解（偶数长度）
 *
 *   原始链表：1 → 2 → 2 → 1 → null
 *
 *   步骤 1：快慢指针找中点
 *     初始：  slow=1, fast=1
 *     第1步： slow=2, fast=2（第二个）
 *     第2步： fast->next->next == null，停止
 *                    ↑ slow 停在节点 2（前半部分的尾）
 *
 *     前半部分：1 → 2
 *     后半部分：2 → 1
 *
 *   步骤 2：反转后半部分
 *     2 → 1 → null  变成  1 → 2 → null
 *
 *   步骤 3：逐一比较
 *     前半：1 → 2
 *     后半：1 → 2 → null
 *
 *     比较：1==1 ✓, 2==2 ✓ → 是回文 ✅
 *
 * 四、快慢指针停止条件详解
 *
 *   循环条件：while (fast->next != nullptr && fast->next->next != nullptr)
 *
 *   奇数节点 (1→2→3→2→1)：
 *     fast 最终停在最后一个节点，slow 停在正中间
 *     slow 指向中点，后半部分 = slow->next
 *
 *   偶数节点 (1→2→2→1)：
 *     fast 最终停在倒数第二个节点，slow 停在前半部分的最后
 *     后半部分 = slow->next
 *
 *   两种情况下 slow->next 都是后半部分的起点！
 *
 * 时间复杂度：O(n)  —— 找中点 O(n) + 反转 O(n/2) + 比较 O(n/2)
 * 空间复杂度：O(1)  —— 只用几个指针，原地操作
 */
class Solution2 {
public:
    bool isPalindrome(ListNode* head) {
        if (head == nullptr || head->next == nullptr)
        {
            return true;
        }

        // 步骤 1：快慢指针找中点
        ListNode *slow = head;
        ListNode *fast = head;
        while (fast->next != nullptr && fast->next->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        // 步骤 2：反转后半部分链表
        ListNode *secondHalf = reverseList(slow->next);

        // 步骤 3：逐一比较前半部分和反转后的后半部分
        ListNode *p1 = head;
        ListNode *p2 = secondHalf;
        bool result = true;
        while (p2 != nullptr)
        {
            if (p1->val != p2->val)
            {
                result = false;
                break;
            }
            p1 = p1->next;
            p2 = p2->next;
        }

        // 步骤 4（可选）：恢复链表原始结构
        slow->next = reverseList(secondHalf);

        return result;
    }

private:
    // 反转链表（迭代法）
    ListNode* reverseList(ListNode* head) {
        ListNode *prev = nullptr;
        ListNode *curr = head;
        while (curr != nullptr)
        {
            ListNode *next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：递归（利用函数调用栈模拟反向遍历）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   递归到链表尾部，回溯时相当于从后往前遍历。
 *   同时维护一个"前指针"从头往后走，这样就能同时比较首尾对应的元素。
 *
 *   递归天然提供了"逆序访问"的能力：
 *     递归深入 = 正向走到尾
 *     递归回溯 = 反向从尾回头
 *
 * 二、图解
 *
 *   链表：1 → 2 → 3 → 2 → 1 → null
 *
 *   递归深入：front 不动，curr 一路递归到末尾
 *     check(1) → check(2) → check(3) → check(2) → check(1)
 *                                                    ↑ base: curr->next==null
 *
 *   回溯比较（从内向外）：
 *     回溯 curr=1（尾）：front=1（头），1==1 ✓，front 前进到 2
 *     回溯 curr=2：      front=2，      2==2 ✓，front 前进到 3
 *     回溯 curr=3：      front=3，      3==3 ✓，front 前进到 2
 *     回溯 curr=2：      front=2，      2==2 ✓，front 前进到 1
 *     回溯 curr=1（头）：front=1，      1==1 ✓
 *     全部匹配 → 是回文 ✅
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(n)  —— 递归调用栈深度为 n
 */
class Solution3 {
public:
    bool isPalindrome(ListNode* head) {
        frontPointer = head;
        return check(head);
    }

private:
    ListNode *frontPointer; // 从头部向后移动的指针

    bool check(ListNode *curr) {
        if (curr == nullptr)
        {
            return true;
        }

        // 先递归到尾部
        if (!check(curr->next))
        {
            return false;
        }

        // 回溯时比较：curr 从后往前，frontPointer 从前往后
        if (curr->val != frontPointer->val)
        {
            return false;
        }

        // frontPointer 前进一步
        frontPointer = frontPointer->next;
        return true;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法              时间复杂度    空间复杂度    特点
 *   ────              ────────      ────────      ────
 *   复制到数组        O(n)          O(n)          最直观，易理解
 *   快慢指针+反转     O(n)          O(1)          最优解，但修改了原链表
 *   递归              O(n)          O(n)          巧妙利用调用栈，但实际空间开销大
 *
 *   面试建议：
 *   - 先说解法一展示基本思路
 *   - 重点写解法二（O(1) 空间），体现对链表操作的熟练度
 *   - 解法二中"恢复链表"是加分项，说明考虑了副作用
 */
