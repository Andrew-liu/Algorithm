/*
 * 226. Invert Binary Tree
 * https://leetcode.com/problems/invert-binary-tree/
 *
 * 给你一棵二叉树的根节点 root，翻转这棵二叉树，并返回其根节点。
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
 *         4                 4
 *        / \               / \
 *       2   7     →       7   2
 *      / \ / \           / \ / \
 *     1  3 6  9         9  6 3  1
 *
 * ──────────────────────────────────────────────
 * 解法一：递归（DFS）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   翻转一棵树 = 交换当前节点的左右子树 + 递归翻转左右子树
 *
 *   递归定义：
 *     - base case：空节点，直接返回 null
 *     - 递归步骤：交换左右子节点，然后递归翻转左子树和右子树
 *
 *   交换和递归的顺序无所谓（先交换再递归 or 先递归再交换 都可以）。
 *
 * 二、图解（先交换再递归）
 *
 *         4
 *        / \
 *       2   7
 *      / \ / \
 *     1  3 6  9
 *
 *   invert(4)：交换 4 的左右子节点
 *         4
 *        / \
 *       7   2          ← 左右交换了
 *      / \ / \
 *     6  9 1  3
 *
 *   invert(7)：交换 7 的左右子节点
 *       7
 *      / \
 *     9   6            ← 左右交换了
 *
 *   invert(2)：交换 2 的左右子节点
 *       2
 *      / \
 *     3   1            ← 左右交换了
 *
 *   最终结果：
 *         4
 *        / \
 *       7   2
 *      / \ / \
 *     9  6 3  1  ✅
 *
 * 三、图解（先递归再交换）
 *
 *   invert(4)：
 *     先递归 invert(2) → 得到翻转后的左子树
 *       invert(1) → 返回 1（叶子节点）
 *       invert(3) → 返回 3（叶子节点）
 *       交换 2 的左右：2 的子节点变为 [3, 1]
 *     先递归 invert(7) → 得到翻转后的右子树
 *       invert(6) → 返回 6
 *       invert(9) → 返回 9
 *       交换 7 的左右：7 的子节点变为 [9, 6]
 *     交换 4 的左右子树：4 的子节点变为 [翻转后的7, 翻转后的2]
 *
 *   两种顺序结果相同，因为每个节点最终都会被交换一次。
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(h)  —— h 为树高，递归栈深度；最坏 O(n)，平衡时 O(log n)
 */
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (root == nullptr)
        {
            return nullptr;
        }

        // 交换左右子节点
        swap(root->left, root->right);

        // 递归翻转左右子树
        invertTree(root->left);
        invertTree(root->right);

        return root;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：BFS（层序遍历）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   层序遍历每个节点，对每个节点交换它的左右子节点。
 *   不需要关心遍历顺序（前序、中序、后序、层序都行），
 *   只要保证每个节点都被交换一次。
 *
 * 二、图解
 *
 *         4
 *        / \
 *       2   7
 *      / \ / \
 *     1  3 6  9
 *
 *   队列：[4]
 *
 *   处理 4：交换左右 → 左=7, 右=2，子节点入队
 *         4
 *        / \
 *       7   2
 *      / \ / \
 *     6  9 1  3
 *   队列：[7, 2]
 *
 *   处理 7：交换左右 → 左=9, 右=6
 *       7
 *      / \
 *     9   6
 *   队列：[2, 9, 6]
 *
 *   处理 2：交换左右 → 左=3, 右=1
 *       2
 *      / \
 *     3   1
 *   队列：[9, 6, 3, 1]
 *
 *   处理 9, 6, 3, 1：都是叶子节点，无子节点可交换
 *
 *   最终结果：
 *         4
 *        / \
 *       7   2
 *      / \ / \
 *     9  6 3  1  ✅
 *
 * 时间复杂度：O(n)  —— 每个节点入队出队各一次
 * 空间复杂度：O(n)  —— 队列最多存储一层的节点数（最坏 n/2）
 */
class Solution2 {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (root == nullptr) return nullptr;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty())
        {
            TreeNode *node = q.front();
            q.pop();

            // 交换当前节点的左右子节点
            swap(node->left, node->right);

            // 子节点入队
            if (node->left != nullptr) q.push(node->left);
            if (node->right != nullptr) q.push(node->right);
        }

        return root;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：DFS 迭代（显式栈）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   与 BFS 思路相同：遍历每个节点，交换其左右子节点。
 *   只是用栈代替队列，遍历顺序变为 DFS。
 *
 * 二、本质理解
 *
 *   翻转二叉树的本质：对每个节点，交换它的左右子节点。
 *   至于用什么顺序遍历（DFS/BFS/前序/后序），都无所谓，
 *   只要每个节点恰好被交换一次即可。
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(n)  —— 栈的最大深度
 */
class Solution3 {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (root == nullptr) return nullptr;

        stack<TreeNode*> stk;
        stk.push(root);

        while (!stk.empty())
        {
            TreeNode *node = stk.top();
            stk.pop();

            // 交换当前节点的左右子节点
            swap(node->left, node->right);

            // 子节点入栈
            if (node->left != nullptr) stk.push(node->left);
            if (node->right != nullptr) stk.push(node->right);
        }

        return root;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法          时间复杂度    空间复杂度    特点
 *   ────          ────────      ────────      ────
 *   递归 DFS      O(n)          O(h)          最简洁，3 行核心代码
 *   BFS           O(n)          O(n)          层序遍历，直观
 *   迭代 DFS      O(n)          O(n)          栈模拟，避免递归
 *
 *   面试建议：
 *   - 递归解法必须秒写（经典的递归入门题）
 *   - 本题的本质理解：翻转 = 对每个节点交换左右子节点
 *     遍历顺序无关紧要，任何遍历方式都能实现
 *   - 这道题是 Homebrew 作者 Max Howell 的经典面试轶事：
 *     "Google 面试没通过因为不会翻转二叉树"
 *
 * ──────────────────────────────────────────────
 * 递归思维
 * ──────────────────────────────────────────────
 *
 *   翻转整棵树 = 翻转子问题：
 *     1. 交换当前节点的左右子树
 *     2. 递归翻转新的左子树
 *     3. 递归翻转新的右子树
 *
 *   对比 104 题（最大深度）的递归模式：
 *     104：结果 = combine(递归左, 递归右)
 *     226：操作 = 交换 + 递归左 + 递归右
 *
 *   两者都是"把大问题分解为子问题"的思维。
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   100. Same Tree                    —— 判断两棵树是否相同
 *   101. Symmetric Tree               —— 判断是否对称（翻转后与自身相同）
 *   104. Maximum Depth of Binary Tree —— 最大深度（类似递归模式）
 *   951. Flip Equivalent Binary Trees —— 翻转等价判断
 */
