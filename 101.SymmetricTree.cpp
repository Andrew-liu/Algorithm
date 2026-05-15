/*
 * 101. Symmetric Tree
 * https://leetcode.com/problems/symmetric-tree/
 *
 * 给你一个二叉树的根节点 root，检查它是否轴对称。
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
 *   示例 1（对称）：
 *           1
 *          / \
 *         2   2
 *        / \ / \
 *       3  4 4  3
 *   输出：true
 *
 *   示例 2（不对称）：
 *           1
 *          / \
 *         2   2
 *          \   \
 *          3    3
 *   输出：false
 *
 * ──────────────────────────────────────────────
 * 解法一：递归
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   一棵树是否对称，等价于：它的左子树和右子树是否互为镜像。
 *
 *   两棵树互为镜像的条件：
 *     1. 两棵树的根节点值相同
 *     2. 树 A 的左子树 与 树 B 的右子树 互为镜像
 *     3. 树 A 的右子树 与 树 B 的左子树 互为镜像
 *
 *   递归定义：
 *     - base case：两个节点都为空 → 对称（true）
 *     - base case：只有一个为空 → 不对称（false）
 *     - 递归：值相同 且 (左.left 镜像 右.right) 且 (左.right 镜像 右.left)
 *
 * 二、图解
 *
 *           1
 *          / \
 *         2   2
 *        / \ / \
 *       3  4 4  3
 *
 *   isMirror(左子树2, 右子树2)：
 *     值相同（2 == 2）✓
 *     检查 isMirror(左2.left=3, 右2.right=3)：
 *       值相同（3 == 3）✓
 *       isMirror(null, null) = true ✓
 *       isMirror(null, null) = true ✓
 *       返回 true ✓
 *     检查 isMirror(左2.right=4, 右2.left=4)：
 *       值相同（4 == 4）✓
 *       isMirror(null, null) = true ✓
 *       isMirror(null, null) = true ✓
 *       返回 true ✓
 *     返回 true ✅
 *
 * 三、镜像对比的配对关系
 *
 *           1
 *          / \
 *        [2] [2]       ← 配对比较
 *        / \ / \
 *      [3] 4 4 [3]    ← 外侧配对（左.left vs 右.right）
 *        [4] [4]       ← 内侧配对（左.right vs 右.left）
 *
 *   对称 = 外侧节点对称 + 内侧节点对称
 *
 * 时间复杂度：O(n)  —— 每个节点最多访问一次
 * 空间复杂度：O(h)  —— h 为树高，递归栈深度；最坏 O(n)，平衡时 O(log n)
 */
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (root == nullptr) return true;
        return isMirror(root->left, root->right);
    }

private:
    bool isMirror(TreeNode* left, TreeNode* right) {
        // 两个都为空，对称
        if (left == nullptr && right == nullptr)
        {
            return true;
        }

        // 只有一个为空，不对称
        if (left == nullptr || right == nullptr)
        {
            return false;
        }

        // 值不同，不对称
        if (left->val != right->val)
        {
            return false;
        }

        // 递归比较：外侧 vs 外侧，内侧 vs 内侧
        return isMirror(left->left, right->right)
            && isMirror(left->right, right->left);
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：迭代（队列）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   用队列模拟递归的镜像比较过程。
 *   每次从队列中取出两个节点（一对镜像位置的节点），比较它们：
 *     - 都为空：继续
 *     - 只有一个为空 或 值不同：不对称
 *     - 值相同：把它们的子节点按镜像顺序成对入队
 *
 * 二、图解
 *
 *           1
 *          / \
 *         2   2
 *        / \ / \
 *       3  4 4  3
 *
 *   初始：queue = [2, 2]（左子树根和右子树根配对）
 *
 *   取出 (2, 2)：值相同 ✓
 *     按镜像顺序入队：
 *       (2.left=3, 2.right=3)    ← 外侧配对
 *       (2.right=4, 2.left=4)   ← 内侧配对
 *     queue = [3, 3, 4, 4]
 *
 *   取出 (3, 3)：值相同 ✓
 *     子节点都是 null，无需入队
 *     queue = [4, 4]
 *
 *   取出 (4, 4)：值相同 ✓
 *     子节点都是 null，无需入队
 *     queue = []
 *
 *   队列为空，所有配对都通过 → true ✅
 *
 * 三、入队顺序的关键
 *
 *   每次取出一对 (nodeA, nodeB)，入队顺序：
 *     (nodeA->left,  nodeB->right)   ← 外侧
 *     (nodeA->right, nodeB->left)    ← 内侧
 *
 *   这保证了镜像位置的节点总是被配对比较。
 *
 * 时间复杂度：O(n)  —— 每个节点入队出队各一次
 * 空间复杂度：O(n)  —— 队列最多存储一层的节点数
 */
class Solution2 {
public:
    bool isSymmetric(TreeNode* root) {
        if (root == nullptr) return true;

        queue<TreeNode*> q;
        q.push(root->left);
        q.push(root->right);

        while (!q.empty())
        {
            TreeNode *nodeA = q.front(); q.pop();
            TreeNode *nodeB = q.front(); q.pop();

            // 都为空，这一对通过，继续
            if (nodeA == nullptr && nodeB == nullptr)
            {
                continue;
            }

            // 只有一个为空，或值不同 → 不对称
            if (nodeA == nullptr || nodeB == nullptr)
            {
                return false;
            }
            if (nodeA->val != nodeB->val)
            {
                return false;
            }

            // 按镜像顺序入队
            q.push(nodeA->left);    // 外侧配对
            q.push(nodeB->right);
            q.push(nodeA->right);   // 内侧配对
            q.push(nodeB->left);
        }

        return true;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：迭代（栈）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   与队列解法完全相同的逻辑，只是用栈代替队列。
 *   因为我们只关心"每对节点是否匹配"，遍历顺序（BFS vs DFS）不影响结果。
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(n)  —— 栈的最大深度
 */
class Solution3 {
public:
    bool isSymmetric(TreeNode* root) {
        if (root == nullptr) return true;

        stack<TreeNode*> stk;
        stk.push(root->left);
        stk.push(root->right);

        while (!stk.empty())
        {
            TreeNode *nodeB = stk.top(); stk.pop();
            TreeNode *nodeA = stk.top(); stk.pop();

            if (nodeA == nullptr && nodeB == nullptr)
            {
                continue;
            }
            if (nodeA == nullptr || nodeB == nullptr)
            {
                return false;
            }
            if (nodeA->val != nodeB->val)
            {
                return false;
            }

            // 按镜像顺序入栈
            stk.push(nodeA->left);
            stk.push(nodeB->right);
            stk.push(nodeA->right);
            stk.push(nodeB->left);
        }

        return true;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法          时间复杂度    空间复杂度    特点
 *   ────          ────────      ────────      ────
 *   递归          O(n)          O(h)          最简洁，递归定义自然
 *   迭代(队列)    O(n)          O(n)          BFS 配对比较
 *   迭代(栈)      O(n)          O(n)          DFS 配对比较
 *
 *   h = 树高，平衡树 h = O(log n)，最坏 h = O(n)
 *
 *   面试建议：
 *   - 递归解法最推荐，思路清晰且代码简洁
 *   - 关键洞察："对称" = "左右子树互为镜像"
 *   - 镜像的递归结构：外侧对外侧，内侧对内侧
 *   - 迭代解法展示了"用队列/栈模拟递归"的通用技巧
 *
 * ──────────────────────────────────────────────
 * 递归思维
 * ──────────────────────────────────────────────
 *
 *   本题的递归模式与 100. Same Tree 类似：
 *
 *   100 题（相同的树）：
 *     isSame(A, B) = (A.val == B.val)
 *                    && isSame(A.left, B.left)
 *                    && isSame(A.right, B.right)
 *
 *   101 题（对称树）：
 *     isMirror(A, B) = (A.val == B.val)
 *                      && isMirror(A.left, B.right)   ← 注意：交叉比较
 *                      && isMirror(A.right, B.left)
 *
 *   唯一的区别：同侧比较 vs 交叉比较。
 *   对称树 = 左子树翻转后与右子树相同（联系 226. 翻转二叉树）。
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   100. Same Tree              —— 判断两棵树是否相同（同侧比较）
 *   226. Invert Binary Tree     —— 翻转二叉树（对称树 = 翻转后与自身相同）
 *   104. Maximum Depth          —— 类似递归模式
 *   572. Subtree of Another Tree —— 子树判断（类似递归结构）
 */
