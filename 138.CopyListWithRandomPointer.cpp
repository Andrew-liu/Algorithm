/*
 * 138. Copy List with Random Pointer
 * https://leetcode.com/problems/copy-list-with-random-pointer/
 *
 * 给你一个长度为 n 的链表，每个节点包含一个额外增加的随机指针 random，
 * 该指针可以指向链表中的任何节点或空节点。构造这个链表的深拷贝。
 *
 *   节点结构：
 *     class Node {
 *     public:
 *         int val;
 *         Node* next;
 *         Node* random;
 *         Node(int _val) : val(_val), next(NULL), random(NULL) {}
 *     };
 *
 * ──────────────────────────────────────────────
 * 难点分析
 * ──────────────────────────────────────────────
 *
 *   普通链表的深拷贝很简单：遍历一遍，逐个创建新节点并连接 next。
 *
 *   但本题多了 random 指针，难点在于：
 *     当我们拷贝节点 A 时，A.random 指向的节点 B 可能还没被创建。
 *     我们需要一种方式，将"原节点"映射到"新节点"，以便设置 random 指针。
 *
 *   核心问题：如何快速找到原节点对应的新节点？
 *
 * ──────────────────────────────────────────────
 * 解法一：哈希表（两次遍历）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   用哈希表建立"原节点 → 新节点"的映射关系。
 *   第一次遍历：创建所有新节点，存入哈希表。
 *   第二次遍历：根据映射设置每个新节点的 next 和 random。
 *
 * 二、图解
 *
 *   原链表：
 *     A(7) → B(13) → C(11) → D(10) → E(1) → null
 *     random 指向：
 *       A.random = null
 *       B.random = A
 *       C.random = E
 *       D.random = C
 *       E.random = A
 *
 *   第一次遍历（创建节点 + 建立映射）：
 *
 *     map = {
 *       A → A'(7),
 *       B → B'(13),
 *       C → C'(11),
 *       D → D'(10),
 *       E → E'(1)
 *     }
 *
 *   第二次遍历（设置 next 和 random）：
 *
 *     A'->next = map[A->next] = map[B] = B'
 *     A'->random = map[A->random] = map[null] = null
 *
 *     B'->next = map[B->next] = map[C] = C'
 *     B'->random = map[B->random] = map[A] = A'
 *
 *     C'->next = map[C->next] = map[D] = D'
 *     C'->random = map[C->random] = map[E] = E'
 *
 *     D'->next = map[D->next] = map[E] = E'
 *     D'->random = map[D->random] = map[C] = C'
 *
 *     E'->next = map[E->next] = map[null] = null
 *     E'->random = map[E->random] = map[A] = A'
 *
 *   结果：A'(7) → B'(13) → C'(11) → D'(10) → E'(1) → null
 *         所有 random 指针正确指向新链表中对应的节点 ✅
 *
 * 时间复杂度：O(n)  —— 两次遍历
 * 空间复杂度：O(n)  —— 哈希表存储 n 个映射
 */
class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (head == nullptr) return nullptr;

        unordered_map<Node*, Node*> map; // 原节点 → 新节点

        // 第一次遍历：创建所有新节点
        Node *cur = head;
        while (cur != nullptr)
        {
            map[cur] = new Node(cur->val);
            cur = cur->next;
        }

        // 第二次遍历：设置 next 和 random
        cur = head;
        while (cur != nullptr)
        {
            map[cur]->next = map[cur->next];     // next 映射
            map[cur]->random = map[cur->random]; // random 映射
            cur = cur->next;
        }

        return map[head];
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：哈希表（一次遍历）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   与解法一类似用哈希表，但只遍历一次。
 *   遇到一个节点时，如果它的新节点还没创建就"按需创建"。
 *   处理 next 和 random 时，目标节点如果不存在就先创建。
 *
 * 二、图解（以节点 B 为例）
 *
 *   遍历到 B(13)：
 *     - B' 不存在？创建 B'(13)，map[B] = B'
 *     - B->next = C，C' 不存在？创建 C'(11)，map[C] = C'
 *       设置 B'->next = C'
 *     - B->random = A，A' 已存在（之前创建过）
 *       设置 B'->random = A'
 *
 *   这种"懒创建"方式保证每个节点只被创建一次。
 *
 * 时间复杂度：O(n)  —— 一次遍历
 * 空间复杂度：O(n)  —— 哈希表
 */
class Solution2 {
public:
    Node* copyRandomList(Node* head) {
        if (head == nullptr) return nullptr;

        unordered_map<Node*, Node*> map;

        Node *cur = head;
        while (cur != nullptr)
        {
            // 按需创建当前节点的副本
            if (!map.count(cur))
            {
                map[cur] = new Node(cur->val);
            }

            // 按需创建 next 对应的副本
            if (cur->next != nullptr)
            {
                if (!map.count(cur->next))
                {
                    map[cur->next] = new Node(cur->next->val);
                }
                map[cur]->next = map[cur->next];
            }

            // 按需创建 random 对应的副本
            if (cur->random != nullptr)
            {
                if (!map.count(cur->random))
                {
                    map[cur->random] = new Node(cur->random->val);
                }
                map[cur]->random = map[cur->random];
            }

            cur = cur->next;
        }

        return map[head];
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：原地交织（节点拼接 + 拆分）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   不用哈希表，而是把新节点直接插入到原节点后面，形成交织链表。
 *   这样"原节点的 next"就是它对应的新节点，天然建立了映射关系。
 *
 *   分三步：
 *     1. 在每个原节点后面插入其副本
 *     2. 利用交织结构设置新节点的 random
 *     3. 将交织链表拆分为原链表和新链表
 *
 * 二、步骤 1：插入副本节点
 *
 *   原链表：A → B → C → null
 *
 *   插入后：A → A' → B → B' → C → C' → null
 *
 *   每个原节点的 next 就是它的副本！
 *   即 map[orig] = orig->next（无需哈希表）
 *
 * 三、步骤 2：设置 random 指针
 *
 *   对于原节点 cur，它的副本是 cur->next。
 *   cur->random 指向某个原节点 X，X 的副本是 X->next。
 *
 *   所以：cur->next->random = cur->random->next
 *   即：  副本的 random    = 原节点 random 的副本
 *
 *   图解：
 *     原链表中 B.random = A
 *     交织链表：A → A' → B → B' → C → C' → null
 *
 *     设置 B' 的 random：
 *       B'->random = B->random->next = A->next = A' ✅
 *
 * 四、步骤 3：拆分链表
 *
 *   交织链表：A → A' → B → B' → C → C' → null
 *
 *   拆分为：
 *     原链表：A → B → C → null（恢复原样）
 *     新链表：A' → B' → C' → null
 *
 *   操作：
 *     A->next = A'->next = B
 *     A'->next = B->next = B'
 *     B->next = B'->next = C
 *     B'->next = C->next = C'
 *     C->next = C'->next = null
 *     C'->next = null
 *
 * 五、完整图解
 *
 *   原链表：A(7) → B(13) → C(11) → null
 *   random：A→null, B→A, C→A
 *
 *   步骤 1 后：
 *     A(7) → A'(7) → B(13) → B'(13) → C(11) → C'(11) → null
 *
 *   步骤 2（设置 random）：
 *     A'->random = A->random == null ? null : A->random->next = null
 *     B'->random = B->random->next = A->next = A'
 *     C'->random = C->random->next = A->next = A'
 *
 *   步骤 3（拆分）：
 *     原链表：A(7) → B(13) → C(11) → null ✅（恢复）
 *     新链表：A'(7) → B'(13) → C'(11) → null ✅
 *     A'->random = null, B'->random = A', C'->random = A' ✅
 *
 * 时间复杂度：O(n)  —— 三次遍历，每次 O(n)
 * 空间复杂度：O(1)  —— 不使用哈希表（新链表本身是输出，不算额外空间）
 */
class Solution3 {
public:
    Node* copyRandomList(Node* head) {
        if (head == nullptr) return nullptr;

        // 步骤 1：在每个原节点后面插入副本
        Node *cur = head;
        while (cur != nullptr)
        {
            Node *copy = new Node(cur->val);
            copy->next = cur->next;
            cur->next = copy;
            cur = copy->next; // 跳到下一个原节点
        }

        // 步骤 2：设置副本节点的 random
        cur = head;
        while (cur != nullptr)
        {
            Node *copy = cur->next;
            copy->random = (cur->random != nullptr) ? cur->random->next : nullptr;
            cur = copy->next; // 跳到下一个原节点
        }

        // 步骤 3：拆分交织链表
        Node *newHead = head->next;
        cur = head;
        while (cur != nullptr)
        {
            Node *copy = cur->next;
            cur->next = copy->next;                                      // 恢复原链表
            copy->next = (copy->next != nullptr) ? copy->next->next : nullptr; // 连接新链表
            cur = cur->next;                                             // 下一个原节点
        }

        return newHead;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法            时间复杂度    空间复杂度    特点
 *   ────            ────────      ────────      ────
 *   哈希表两次遍历  O(n)          O(n)          最直观，逻辑清晰
 *   哈希表一次遍历  O(n)          O(n)          按需创建，一次完成
 *   原地交织        O(n)          O(1)          最优空间，但修改了原链表（最后恢复）
 *
 *   面试建议：
 *   - 解法一最容易写对，优先保证正确性
 *   - 解法三是经典的面试亮点解法，体现对链表操作的深度掌握
 *   - 关键理解：难点不是拷贝节点，而是"如何找到新节点间的对应关系"
 *     - 哈希表方案：用 map 存映射
 *     - 交织方案：用"插入在后面"天然建立映射
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   133. Clone Graph           —— 图的深拷贝（DFS/BFS + 哈希表）
 *   1485. Clone Binary Tree    —— 带随机指针的二叉树深拷贝
 */
