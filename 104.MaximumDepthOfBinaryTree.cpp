/*
 * 104. Maximum Depth of Binary Tree
 * https://leetcode.com/problems/maximum-depth-of-binary-tree/
 *
 * 给定一个二叉树 root，返回其最大深度。
 * 二叉树的最大深度是指从根节点到最远叶子节点的最长路径上的节点数。
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
 *         3
 *        / \
 *       9   20
 *          / \
 *         15  7
 *
 *   最大深度 = 3（路径：3 → 20 → 15 或 3 → 20 → 7）
 *
 * ──────────────────────────────────────────────
 * 解法一：递归（DFS，自底向上）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   一棵树的最大深度 = max(左子树最大深度, 右子树最大深度) + 1
 *
 *   这是一个天然的递归定义：
 *     - base case：空节点深度为 0
 *     - 递归：当前节点深度 = max(左, 右) + 1
 *
 * 二、图解
 *
 *         3
 *        / \
 *       9   20
 *          / \
 *         15  7
 *
 *   maxDepth(3)
 *     maxDepth(9)
 *       maxDepth(null) = 0
 *       maxDepth(null) = 0
 *       返回 max(0, 0) + 1 = 1
 *     maxDepth(20)
 *       maxDepth(15)
 *         maxDepth(null) = 0
 *         maxDepth(null) = 0
 *         返回 max(0, 0) + 1 = 1
 *       maxDepth(7)
 *         maxDepth(null) = 0
 *         maxDepth(null) = 0
 *         返回 max(0, 0) + 1 = 1
 *       返回 max(1, 1) + 1 = 2
 *     返回 max(1, 2) + 1 = 3 ✅
 *
 * 三、递归树（标注返回值）
 *
 *         3  → 返回 3
 *        / \
 *    1← 9   20 → 返回 2
 *          / \
 *      1← 15  7 →1
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(h)  —— h 为树高，递归栈深度；最坏 O(n)，平衡时 O(log n)
 */
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (root == nullptr)
        {
            return 0;
        }

        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);

        return max(leftDepth, rightDepth) + 1;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：BFS（层序遍历）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   层序遍历（BFS）逐层处理，遍历完所有层后，层数就是最大深度。
 *   用队列实现：每处理完一层，深度 +1。
 *
 * 二、图解
 *
 *         3          ← 第 1 层
 *        / \
 *       9   20       ← 第 2 层
 *          / \
 *         15  7      ← 第 3 层
 *
 *   初始：queue = [3], depth = 0
 *
 *   第 1 层：queue = [3], 本层大小 = 1
 *     处理 3：子节点 9, 20 入队
 *     queue = [9, 20], depth = 1
 *
 *   第 2 层：queue = [9, 20], 本层大小 = 2
 *     处理 9：无子节点
 *     处理 20：子节点 15, 7 入队
 *     queue = [15, 7], depth = 2
 *
 *   第 3 层：queue = [15, 7], 本层大小 = 2
 *     处理 15：无子节点
 *     处理 7：无子节点
 *     queue = [], depth = 3
 *
 *   队列为空，结束。最大深度 = 3 ✅
 *
 * 三、层序遍历的模板
 *
 *     queue = [root]
 *     depth = 0
 *     while queue 不为空:
 *         levelSize = queue.size()    // 当前层的节点数
 *         for i in 0..levelSize-1:
 *             node = queue.front(); queue.pop()
 *             把 node 的子节点加入队列
 *         depth++
 *     return depth
 *
 *   关键：用 levelSize 区分"哪些节点属于当前层"。
 *
 * 时间复杂度：O(n)  —— 每个节点入队出队各一次
 * 空间复杂度：O(n)  —— 队列最多存储一层的节点数（最坏 n/2）
 */
class Solution2 {
public:
    int maxDepth(TreeNode* root) {
        if (root == nullptr) return 0;

        queue<TreeNode*> q;
        q.push(root);
        int depth = 0;

        while (!q.empty())
        {
            int levelSize = q.size(); // 当前层的节点数

            // 处理当前层的所有节点
            for (int i = 0; i < levelSize; ++i)
            {
                TreeNode *node = q.front();
                q.pop();

                if (node->left != nullptr) q.push(node->left);
                if (node->right != nullptr) q.push(node->right);
            }

            depth++; // 一层处理完，深度 +1
        }

        return depth;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：DFS 迭代（显式栈）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   用栈模拟 DFS，栈中存储 {节点, 当前深度} 的配对。
 *   遍历过程中维护最大深度。
 *
 * 二、图解
 *
 *         3
 *        / \
 *       9   20
 *          / \
 *         15  7
 *
 *   栈：[(3, 1)]，maxD = 0
 *
 *   弹出 (3,1)：maxD = max(0,1) = 1
 *     压入 (20,2), (9,2)
 *
 *   弹出 (9,2)：maxD = max(1,2) = 2
 *     无子节点
 *
 *   弹出 (20,2)：maxD = max(2,2) = 2
 *     压入 (7,3), (15,3)
 *
 *   弹出 (15,3)：maxD = max(2,3) = 3
 *   弹出 (7,3)：maxD = max(3,3) = 3
 *
 *   栈为空，返回 3 ✅
 *
 * 时间复杂度：O(n)  —— 每个节点入栈出栈各一次
 * 空间复杂度：O(n)  —— 栈的最大深度
 */
class Solution3 {
public:
    int maxDepth(TreeNode* root) {
        if (root == nullptr) return 0;

        stack<pair<TreeNode*, int>> stk;
        stk.push({root, 1});
        int maxD = 0;

        while (!stk.empty())
        {
            auto [node, depth] = stk.top();
            stk.pop();

            maxD = max(maxD, depth);

            if (node->left != nullptr) stk.push({node->left, depth + 1});
            if (node->right != nullptr) stk.push({node->right, depth + 1});
        }

        return maxD;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法          时间复杂度    空间复杂度    特点
 *   ────          ────────      ────────      ────
 *   递归 DFS      O(n)          O(h)          最简洁，2-3 行核心代码
 *   BFS 层序      O(n)          O(n)          直观：层数 = 深度
 *   迭代 DFS      O(n)          O(n)          显式栈，避免递归栈溢出
 *
 *   h = 树高，平衡树 h = O(log n)，最坏 h = O(n)
 *
 *   面试建议：
 *   - 递归解法必须秒写（最基础的树递归题）
 *   - BFS 解法展示层序遍历模板，后续很多题都会复用
 *   - 本题是"树的递归思维"入门题：
 *     大问题（整棵树的深度）= 小问题（子树的深度）+ 1
 *
 * ──────────────────────────────────────────────
 * 递归思维模式
 * ──────────────────────────────────────────────
 *
 *   很多树的题都遵循这个模式：
 *
 *   f(root) = combine(f(root->left), f(root->right), root->val)
 *
 *   本题：maxDepth(root) = max(maxDepth(left), maxDepth(right)) + 1
 *   226 题：翻转树 = 交换左右 + 递归翻转左右子树
 *   543 题：直径 = 左深度 + 右深度（顺便更新全局最大值）
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   111. Minimum Depth of Binary Tree   —— 最小深度（注意叶子节点的定义）
 *   110. Balanced Binary Tree           —— 判断是否平衡（深度差 ≤ 1）
 *   543. Diameter of Binary Tree        —— 直径 = 左深度 + 右深度
 *   102. Binary Tree Level Order        —— 层序遍历（BFS 模板）
 */
