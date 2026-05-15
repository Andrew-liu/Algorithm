/*
 * 160. Intersection of Two Linked Lists
 * https://leetcode.com/problems/intersection-of-two-linked-lists/
 *
 * 给你两个单链表的头节点 headA 和 headB，请你找出并返回两个单链表相交的起始节点。
 * 如果两个链表不存在相交节点，返回 null。
 *
 * 注意：相交是指节点引用相同（不是值相同），相交后两条链表共享后续所有节点。
 *
 *   链表结构：
 *     struct ListNode {
 *         int val;
 *         ListNode *next;
 *         ListNode(int x) : val(x), next(NULL) {}
 *     };
 *
 * ──────────────────────────────────────────────
 * 解法一：哈希集合（Hash Set）
 * ──────────────────────────────────────────────
 * 思路：
 *   先遍历链表 A，把所有节点地址存入 HashSet。
 *   再遍历链表 B，逐个检查当前节点是否在 Set 中。
 *   第一个命中的节点就是相交起始节点；遍历完都没命中则不相交。
 *
 * 图解：
 *     A: a1 → a2 ↘
 *                   c1 → c2 → c3
 *     B: b1 → b2 → b3 ↗
 *
 *   遍历 A：set = {a1, a2, c1, c2, c3}
 *   遍历 B：b1 ∉ set, b2 ∉ set, b3 ∉ set, c1 ∈ set → 找到！ ✅
 *
 * 时间复杂度：O(m + n)  —— m、n 分别为两条链表的长度
 * 空间复杂度：O(m)      —— 存储链表 A 的所有节点
 */
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        unordered_set<ListNode*> visited;

        // 将链表 A 的所有节点存入集合
        ListNode *cur = headA;
        while (cur != nullptr)
        {
            visited.insert(cur);
            cur = cur->next;
        }

        // 遍历链表 B，查找第一个在集合中的节点
        cur = headB;
        while (cur != nullptr)
        {
            if (visited.count(cur))
            {
                return cur;
            }
            cur = cur->next;
        }

        return nullptr;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：双指针（Two Pointers）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   两个指针 pA、pB 分别从 headA、headB 出发，每次各走一步。
 *   当 pA 走到链表 A 末尾时，跳转到 headB 继续走。
 *   当 pB 走到链表 B 末尾时，跳转到 headA 继续走。
 *
 *   这样做的效果：两个指针最终走过的总长度相等，会在相交节点处相遇。
 *
 * 二、为什么一定会相遇？
 *
 *   设链表 A 独有部分长度 = a，链表 B 独有部分长度 = b，公共部分长度 = c。
 *
 *     A: [── a ──][── c ──]
 *     B: [── b ──][── c ──]
 *
 *   pA 的路径：a + c + b = a + b + c
 *   pB 的路径：b + c + a = a + b + c
 *
 *   两条路径长度相同！走完后两个指针恰好都站在公共部分的起点。
 *
 *   如果不相交（c = 0）：
 *   pA 的路径：a + 0 + b = a + b
 *   pB 的路径：b + 0 + a = a + b
 *   两个指针同时走到 nullptr，循环结束，返回 nullptr。
 *
 * 三、图解
 *
 *   A: a1 → a2 ↘
 *                 c1 → c2 → c3       a = 2, b = 3, c = 3
 *   B: b1 → b2 → b3 ↗
 *
 *   pA 路径：a1 → a2 → c1 → c2 → c3 → (跳到B) b1 → b2 → b3 → [c1]
 *   pB 路径：b1 → b2 → b3 → c1 → c2 → c3 → (跳到A) a1 → a2 → [c1]
 *                                                               ↑
 *                                                         同时到达 c1！ ✅
 *
 *   逐步模拟（一共走 a + b + c = 2 + 3 + 3 = 8 步）：
 *
 *     步骤  pA 所在节点     pB 所在节点
 *     ────  ────────────    ────────────
 *      0    a1              b1
 *      1    a2              b2
 *      2    c1              b3
 *      3    c2              c1
 *      4    c3              c2
 *      5    null→跳到B→b1   c3
 *      6    b2              null→跳到A→a1
 *      7    b3              a2
 *      8    c1              c1     ← pA == pB，相遇！ ✅
 *
 *   不相交的情况：
 *
 *     A: a1 → a2 → a3             a = 3, b = 2, c = 0
 *     B: b1 → b2
 *
 *     pA 路径：a1 → a2 → a3 → (跳到B) b1 → b2 → [null]
 *     pB 路径：b1 → b2 → (跳到A) a1 → a2 → a3 → [null]
 *                                                  ↑
 *                                            同时到达 null，返回 nullptr ✅
 *
 * 四、对比
 *
 *   - 解法一 O(m+n) 时间但需要 O(m) 额外空间（Hash Set）
 *   - 解法二 O(m+n) 时间 + O(1) 空间，是最优解
 *
 * 时间复杂度：O(m + n)  —— 每个指针最多走 m + n 步
 * 空间复杂度：O(1)      —— 只用两个指针
 */
class Solution2 {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode *pA = headA;
        ListNode *pB = headB;

        // pA 和 pB 要么在相交点相遇，要么同时到达 nullptr
        while (pA != pB)
        {
            // pA 走到 A 末尾就跳到 B 头部，否则继续前进
            pA = (pA != nullptr) ? pA->next : headB;
            // pB 走到 B 末尾就跳到 A 头部，否则继续前进
            pB = (pB != nullptr) ? pB->next : headA;
        }

        return pA; // 相交点 或 nullptr
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：对齐长度 + 同步前进（Align + Walk Together）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   两条链表可能长度不同，但相交后的尾部一定等长。
 *   所以：先计算两条链表的长度差 d，让较长的链表先走 d 步，
 *   然后两个指针同步前进，第一个相同节点就是相交点。
 *
 *   本质上和解法二等价——都是让两个指针"对齐"公共部分的起点。
 *   解法二用跳转巧妙地实现了对齐，解法三用显式的长度计算来对齐。
 *
 * 二、图解
 *
 *   A: a1 → a2 ↘
 *                 c1 → c2 → c3       lenA = 5, lenB = 6
 *   B: b1 → b2 → b3 ↗
 *
 *   lenA = 5（a1, a2, c1, c2, c3）
 *   lenB = 6（b1, b2, b3, c1, c2, c3）
 *   diff = 6 - 5 = 1 → B 先走 1 步
 *
 *   B 先走 1 步：pB 从 b1 → b2
 *   现在 pA = a1, pB = b2，同步前进：
 *
 *     pA = a1,  pB = b2   → 不同
 *     pA = a2,  pB = b3   → 不同
 *     pA = c1,  pB = c1   → 相同！找到！ ✅
 *
 * 时间复杂度：O(m + n)  —— 计算长度 O(m+n) + 同步前进 O(min(m,n))
 * 空间复杂度：O(1)      —— 只用常数变量
 */
class Solution3 {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // 计算两条链表的长度
        int lenA = 0, lenB = 0;
        ListNode *cur = headA;
        while (cur != nullptr)
        {
            lenA++;
            cur = cur->next;
        }
        cur = headB;
        while (cur != nullptr)
        {
            lenB++;
            cur = cur->next;
        }

        // 让较长的链表先走 |lenA - lenB| 步
        ListNode *pA = headA;
        ListNode *pB = headB;

        if (lenA > lenB)
        {
            for (int i = 0; i < lenA - lenB; ++i)
            {
                pA = pA->next;
            }
        }
        else
        {
            for (int i = 0; i < lenB - lenA; ++i)
            {
                pB = pB->next;
            }
        }

        // 同步前进，第一个相同节点就是相交点
        while (pA != pB)
        {
            pA = pA->next;
            pB = pB->next;
        }

        return pA; // 相交点 或 nullptr
    }
};
