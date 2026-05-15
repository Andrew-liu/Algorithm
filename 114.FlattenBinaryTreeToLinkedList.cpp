/*
 * 114. Flatten Binary Tree to Linked List
 * https://leetcode.com/problems/flatten-binary-tree-to-linked-list/
 *
 * 给你二叉树的根结点 root，请你将它展开为一个单链表：
 *   - 展开后的单链表应该同样使用 TreeNode，其中 right 子指针指向链表中下一个结点，
 *     而左子指针始终为 null。
 *   - 展开后的单链表应该与二叉树先序遍历顺序相同。
 *
 *   二叉树结构：
 *     struct TreeNode {
 *         int val;
 *         TreeNode *left;
 *         TreeNode *right;
 *         TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *         TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *         TreeNode(int x, TreeNode *left, TreeNode *right)
 *             : val(x), left(left), right(right) {}
 *     };
 *
 *   示例：
 *         1                1
 *        / \                \
 *       2   5       →        2
 *      / \   \                \
 *     3   4   6                3
 *                               \
 *                                4
 *                                 \
 *                                  5
 *                                   \
 *                                    6
 *
 *   先序遍历：1 → 2 → 3 → 4 → 5 → 6
 *   展开后链表：1 → 2 → 3 → 4 → 5 → 6（通过 right 指针连接）
 *
 * ──────────────────────────────────────────────
 * 解法一：递归（后序处理，右→左→根的逆先序）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   先序遍历顺序是：根 → 左 → 右
 *   如果我们按照逆序处理：右 → 左 → 根，
 *   那么每次处理一个节点时，它在链表中的"下一个节点"已经被处理好了。
 *
 *   维护一个 prev 指针，指向"上一个被处理的节点"（即当前节点在链表中的后继）。
 *   对每个节点：
 *     node->right = prev
 *     node->left = null
 *     prev = node
 *
 * 二、图解
 *
 *         1
 *        / \
 *       2   5
 *      / \   \
 *     3   4   6
 *
 *   按"右→左→根"顺序处理：6 → 5 → 4 → 3 → 2 → 1
 *
 *   处理 6：prev=null，   6->right=null, 6->left=null, prev=6
 *   处理 5：prev=6，      5->right=6, 5->left=null, prev=5
 *   处理 4：prev=5，      4->right=5, 4->left=null, prev=4
 *   处理 3：prev=4，      3->right=4, 3->left=null, prev=3
 *   处理 2：prev=3，      2->right=3, 2->left=null, prev=2
 *   处理 1：prev=2，      1->right=2, 1->left=null, prev=1
 *
 *   最终：1→2→3→4→5→6 ✅
 *
 * 三、为什么是"右→左→根"而不是直接先序？
 *
 *   如果用先序（根→左→右），处理根节点时会修改 root->right，
 *   导致右子树丢失（还没遍历就被覆盖了）。
 *
 *   逆先序（右→左→根）的巧妙之处：
 *   处理一个节点时，它的左右子树都已经处理完了，
 *   所以可以放心地修改 right 和 left 指针。
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(h)  —— 递归栈深度；最坏 O(n)，平衡时 O(log n)
 */
class Solution {
public:
    void flatten(TreeNode* root) {
        prev_ = nullptr;
        helper(root);
    }

private:
    TreeNode *prev_;

    void helper(TreeNode* node) {
        if (node == nullptr) return;

        // 逆先序：右 → 左 → 根
        helper(node->right);
        helper(node->left);

        // 处理当前节点：接到 prev 前面
        node->right = prev_;
        node->left = nullptr;
        prev_ = node;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：迭代（先序遍历 + 栈）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   用栈进行先序遍历，遍历过程中依次将节点串成链表。
 *   每弹出一个节点，将其 right 和 left 入栈（先右后左，保证左先处理），
 *   然后将当前节点的 right 指向栈顶（即先序的下一个节点），left 置空。
 *
 * 二、图解
 *
 *         1
 *        / \
 *       2   5
 *      / \   \
 *     3   4   6
 *
 *   初始：stack = [1]
 *
 *   弹出 1：右子5入栈，左子2入栈 → stack = [5, 2]
 *     1->right = 栈顶 2，1->left = null
 *
 *   弹出 2：右子4入栈，左子3入栈 → stack = [5, 4, 3]
 *     2->right = 栈顶 3，2->left = null
 *
 *   弹出 3：无子节点 → stack = [5, 4]
 *     3->right = 栈顶 4，3->left = null
 *
 *   弹出 4：无子节点 → stack = [5]
 *     4->right = 栈顶 5，4->left = null
 *
 *   弹出 5：右子6入栈 → stack = [6]
 *     5->right = 栈顶 6，5->left = null
 *
 *   弹出 6：无子节点 → stack = []
 *     6->right = null，6->left = null
 *
 *   结果：1→2→3→4→5→6 ✅
 *
 * 时间复杂度：O(n)  —— 每个节点入栈出栈各一次
 * 空间复杂度：O(n)  —— 栈的最大深度（最坏 O(n)）
 */
class Solution2 {
public:
    void flatten(TreeNode* root) {
        if (root == nullptr) return;

        stack<TreeNode*> stk;
        stk.push(root);

        while (!stk.empty())
        {
            TreeNode *node = stk.top();
            stk.pop();

            // 先右后左入栈（保证左子树先被处理）
            if (node->right != nullptr) stk.push(node->right);
            if (node->left != nullptr) stk.push(node->left);

            // 当前节点的 right 指向先序的下一个节点（栈顶）
            node->right = stk.empty() ? nullptr : stk.top();
            node->left = nullptr;
        }
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：O(1) 空间（寻找前驱节点）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   对于每个节点，如果它有左子树，需要把左子树"插入"到当前节点和右子树之间。
 *
 *   具体操作：
 *     1. 找到左子树的最右节点（即左子树中先序遍历的最后一个节点）
 *     2. 将右子树接到这个最右节点的 right 上
 *     3. 将左子树移到 right 位置，left 置空
 *     4. 移动到下一个节点（node = node->right）
 *
 *   这样每步都是 O(1) 的指针操作（找前驱需要遍历，但总共 O(n)）。
 *
 * 二、图解
 *
 *         1
 *        / \
 *       2   5
 *      / \   \
 *     3   4   6
 *
 *   ─── 处理节点 1 ───
 *   左子树存在，找左子树的最右节点：2→4（4 是最右）
 *   将右子树 5 接到 4->right：
 *         1
 *        /
 *       2
 *      / \
 *     3   4
 *          \
 *           5
 *            \
 *             6
 *   将左子树移到 right，left 置空：
 *     1
 *      \
 *       2
 *      / \
 *     3   4
 *          \
 *           5
 *            \
 *             6
 *   移动到 node = 1->right = 2
 *
 *   ─── 处理节点 2 ───
 *   左子树存在，找左子树的最右节点：3（3 无右子节点，自身就是最右）
 *   将右子树 4 接到 3->right：
 *     1
 *      \
 *       2
 *      /
 *     3
 *      \
 *       4
 *        \
 *         5
 *          \
 *           6
 *   将左子树移到 right，left 置空：
 *     1
 *      \
 *       2
 *        \
 *         3
 *          \
 *           4
 *            \
 *             5
 *              \
 *               6
 *   移动到 node = 2->right = 3
 *
 *   ─── 处理节点 3 ───
 *   无左子树，直接移动到 node = 3->right = 4
 *
 *   ─── 处理节点 4, 5, 6 ───
 *   都无左子树，直接移动
 *
 *   最终：1→2→3→4→5→6 ✅
 *
 * 三、为什么总时间是 O(n)？
 *
 *   虽然每个节点可能需要"找最右节点"，看似每次都是 O(h) 的操作。
 *   但每条边最多被访问 2 次（一次找前驱，一次沿链表移动），
 *   总边数 = n-1，所以总时间 O(n)。
 *
 *   这与 Morris 遍历（94 题解法三）的时间分析类似。
 *
 * 四、与 Morris 遍历的关系
 *
 *   本解法本质上就是 Morris 前序遍历的变体：
 *   - Morris 遍历：建立线索（临时指针）用于回溯，遍历后恢复
 *   - 本题：建立永久连接（题目要求改变树的结构），无需恢复
 *
 * 时间复杂度：O(n)  —— 每条边最多被访问 2 次
 * 空间复杂度：O(1)  —— 只用常数个指针变量
 */
class Solution3 {
public:
    void flatten(TreeNode* root) {
        TreeNode *curr = root;

        while (curr != nullptr)
        {
            if (curr->left != nullptr)
            {
                // 找左子树的最右节点（前驱）
                TreeNode *predecessor = curr->left;
                while (predecessor->right != nullptr)
                {
                    predecessor = predecessor->right;
                }

                // 右子树接到前驱的 right
                predecessor->right = curr->right;

                // 左子树移到 right，left 置空
                curr->right = curr->left;
                curr->left = nullptr;
            }

            // 移动到下一个节点
            curr = curr->right;
        }
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法              时间复杂度    空间复杂度    特点
 *   ────              ────────      ────────      ────
 *   递归(逆先序)       O(n)          O(h)          最巧妙，反向构建
 *   迭代(栈)           O(n)          O(n)          直观的先序遍历
 *   前驱节点(O(1)空间) O(n)          O(1)          最优空间，Morris 变体
 *
 *   h = 树高，平衡树 h = O(log n)，最坏 h = O(n)
 *
 *   面试建议：
 *   - 解法三（O(1) 空间）是最优解，展示对 Morris 遍历的理解
 *   - 解法一（逆先序）最巧妙，展示逆向思维
 *   - 解法二（栈）最直观，不容易出错
 *   - 关键理解：展开后的顺序 = 先序遍历顺序
 *
 * ──────────────────────────────────────────────
 * 三种解法的核心思维对比
 * ──────────────────────────────────────────────
 *
 *   解法一（逆先序）：
 *     "我从后往前构建链表，处理每个节点时它的后续已经就绪"
 *     关键：逆序处理避免了覆盖未遍历节点的问题
 *
 *   解法二（栈）：
 *     "我按先序顺序遍历，每个节点的下一个就是栈顶"
 *     关键：栈保存了右子树，不怕被覆盖
 *
 *   解法三（前驱节点）：
 *     "我把右子树挂到左子树的最右边，然后左变右"
 *     关键：找前驱 + 指针重连，原地操作
 *
 * ──────────────────────────────────────────────
 * 递归思维
 * ──────────────────────────────────────────────
 *
 *   本题展示了"逆向递归"的技巧：
 *
 *   正向（先序）处理的困难：
 *     处理根时修改 right → 右子树丢失 → 需要额外保存
 *
 *   逆向（右→左→根）处理的优势：
 *     处理一个节点时，它后面的链表已经构建好了
 *     直接 node->right = prev 即可
 *
 *   类似的"逆向处理"技巧：
 *     206. Reverse Linked List    —— 从后往前想更容易理解递归
 *     94. Morris Inorder          —— 利用线索避免栈空间
 *
 *   对比 226（翻转二叉树）的递归模式：
 *     226：操作顺序无所谓（前序/后序都行）
 *     114：操作顺序很关键（必须避免覆盖未处理的子树）
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   94. Binary Tree Inorder Traversal   —— Morris 遍历（类似的 O(1) 空间技巧）
 *   144. Binary Tree Preorder Traversal —— 先序遍历（本题的遍历顺序）
 *   206. Reverse Linked List            —— 链表反转（类似逆向思维）
 *   226. Invert Binary Tree             —— 修改树结构（对比递归模式）
 *   430. Flatten a Multilevel DLL       —— 展开多级双向链表（类似思路）
 *   897. Increasing Order Search Tree   —— BST 展开为链表（中序版本）
 */
