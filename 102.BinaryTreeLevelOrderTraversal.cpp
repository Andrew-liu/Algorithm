/*
 * 102. Binary Tree Level Order Traversal
 * https://leetcode.com/problems/binary-tree-level-order-traversal/
 *
 * 给你二叉树的根节点 root，返回其节点值的层序遍历。
 * （即逐层地，从左到右访问所有节点）。
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
 *   示例 1：
 *         3
 *        / \
 *       9   20
 *          / \
 *         15  7
 *
 *   输出：[[3], [9, 20], [15, 7]]
 *
 *   示例 2：
 *       1
 *   输出：[[1]]
 *
 *   示例 3：
 *       空树
 *   输出：[]
 *
 * ──────────────────────────────────────────────
 * 解法一：BFS（队列，逐层处理）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   层序遍历是 BFS 的经典应用。
 *   关键技巧：用 levelSize 区分"哪些节点属于当前层"。
 *
 *   算法流程：
 *     1. 根节点入队
 *     2. 每轮循环处理一层：
 *        - 记录当前队列大小 levelSize（即当前层的节点数）
 *        - 依次出队 levelSize 个节点，收集它们的值
 *        - 将它们的子节点入队（这些子节点属于下一层）
 *     3. 队列为空时结束
 *
 * 二、图解
 *
 *         3
 *        / \
 *       9   20
 *          / \
 *         15  7
 *
 *   初始：queue = [3]
 *
 *   ─── 第 1 层 ───
 *   levelSize = 1（队列中有 1 个节点属于本层）
 *   出队 3：子节点 9, 20 入队
 *   本层结果：[3]
 *   queue = [9, 20]
 *
 *   ─── 第 2 层 ───
 *   levelSize = 2（队列中有 2 个节点属于本层）
 *   出队 9：无子节点
 *   出队 20：子节点 15, 7 入队
 *   本层结果：[9, 20]
 *   queue = [15, 7]
 *
 *   ─── 第 3 层 ───
 *   levelSize = 2
 *   出队 15：无子节点
 *   出队 7：无子节点
 *   本层结果：[15, 7]
 *   queue = []
 *
 *   队列为空，结束。
 *   最终结果：[[3], [9, 20], [15, 7]] ✅
 *
 * 三、为什么 levelSize 能正确分层？
 *
 *   关键不变量：每轮循环开始时，队列中恰好包含"当前层的所有节点"。
 *
 *   证明：
 *   - 初始：队列只有根节点 = 第 1 层的所有节点 ✓
 *   - 归纳：假设本轮开始时队列是第 k 层的所有节点（共 levelSize 个）
 *     处理完这 levelSize 个节点后，它们的子节点全部入队
 *     这些子节点恰好是第 k+1 层的所有节点 ✓
 *
 *   所以 levelSize = queue.size() 就是当前层的节点数。
 *
 * 时间复杂度：O(n)  —— 每个节点入队出队各一次
 * 空间复杂度：O(n)  —— 队列最多存储一层的节点数（最坏 n/2，完全二叉树最后一层）
 */
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (root == nullptr) return result;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty())
        {
            int levelSize = q.size(); // 当前层的节点数
            vector<int> level;        // 存储当前层的值

            for (int i = 0; i < levelSize; ++i)
            {
                TreeNode *node = q.front();
                q.pop();

                level.push_back(node->val);

                if (node->left != nullptr) q.push(node->left);
                if (node->right != nullptr) q.push(node->right);
            }

            result.push_back(level);
        }

        return result;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：DFS 递归（前序遍历 + 层号）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   虽然层序遍历的自然实现是 BFS，但也可以用 DFS 实现：
 *   递归时传递当前节点的层号，将节点值放入对应层的数组中。
 *
 *   只要保证同一层中"左边的节点先于右边的节点被访问"，
 *   结果就与 BFS 层序遍历完全一致。前序遍历（根→左→右）满足这个条件。
 *
 * 二、图解
 *
 *         3           level 0
 *        / \
 *       9   20        level 1
 *          / \
 *         15  7       level 2
 *
 *   DFS 前序遍历顺序：3 → 9 → 20 → 15 → 7
 *
 *   dfs(3, level=0)：result[0] = [3]
 *     dfs(9, level=1)：result[1] = [9]
 *     dfs(20, level=1)：result[1] = [9, 20]
 *       dfs(15, level=2)：result[2] = [15]
 *       dfs(7, level=2)：result[2] = [15, 7]
 *
 *   结果：[[3], [9, 20], [15, 7]] ✅
 *
 * 三、如何确保 result 有足够的空间？
 *
 *   当 level == result.size() 时，说明到达了新的一层，需要添加一个空数组。
 *   由于前序遍历保证第一次到达某层时 level 是递增的，所以只需 push_back 即可。
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(h)  —— 递归栈深度；最坏 O(n)，平衡时 O(log n)
 *                      （不算结果数组；结果数组本身 O(n)）
 */
class Solution2 {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        dfs(root, 0, result);
        return result;
    }

private:
    void dfs(TreeNode* node, int level, vector<vector<int>>& result) {
        if (node == nullptr) return;

        // 到达新的一层，扩展 result
        if (level == result.size())
        {
            result.push_back({});
        }

        // 将当前节点的值加入对应层
        result[level].push_back(node->val);

        // 先左后右，保证同层从左到右的顺序
        dfs(node->left, level + 1, result);
        dfs(node->right, level + 1, result);
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法          时间复杂度    空间复杂度    特点
 *   ────          ────────      ────────      ────
 *   BFS（队列）    O(n)          O(n)          最直观，标准层序遍历模板
 *   DFS（递归）    O(n)          O(h)          巧妙利用层号，空间可能更优
 *
 *   面试建议：
 *   - BFS 解法是必须掌握的标准模板，大量题目会复用
 *   - DFS 解法作为思维拓展，展示"BFS 问题也能用 DFS 解决"
 *   - BFS 解法的关键：levelSize 分层技巧
 *
 * ──────────────────────────────────────────────
 * BFS 层序遍历模板（通用）
 * ──────────────────────────────────────────────
 *
 *   queue = [root]
 *   while queue 不为空:
 *       levelSize = queue.size()
 *       for i in 0..levelSize-1:
 *           node = queue.front(); queue.pop()
 *           处理 node
 *           把 node 的子节点加入 queue
 *
 *   这个模板可以直接套用到以下题目：
 *   - 需要分层的：用 levelSize 循环
 *   - 不需要分层的：省略 levelSize，直接逐个出队处理
 *
 * ──────────────────────────────────────────────
 * 层序遍历的变体题目
 * ──────────────────────────────────────────────
 *
 *   本题（102）是层序遍历的"基础版"，很多题是它的变体：
 *
 *   107. Level Order Traversal II    —— 自底向上层序（result 反转即可）
 *   103. Zigzag Level Order          —— 锯齿形层序（奇数层反转）
 *   199. Binary Tree Right Side View —— 每层最右节点（取 level 最后一个）
 *   637. Average of Levels           —— 每层平均值
 *   515. Find Largest Value          —— 每层最大值
 *   116. Populating Next Right Pointers —— 连接每层的兄弟节点
 *   104. Maximum Depth               —— 层数 = 最大深度
 *
 *   掌握本题模板后，以上变体只需微调"处理 node"的逻辑。
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   104. Maximum Depth of Binary Tree —— BFS 层数 = 最大深度
 *   107. Level Order Traversal II     —— 自底向上的层序遍历
 *   103. Binary Tree Zigzag Level Order —— 锯齿形层序遍历
 *   199. Binary Tree Right Side View  —— 右视图（每层最右）
 *   429. N-ary Tree Level Order       —— N 叉树的层序遍历
 */
