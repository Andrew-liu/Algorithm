/*
 * 543. Diameter of Binary Tree
 * https://leetcode.com/problems/diameter-of-binary-tree/
 *
 * 给你一棵二叉树的根节点，返回该树的直径。
 * 二叉树的直径是指树中任意两个节点之间最长路径的长度。
 * 这条路径可能经过也可能不经过根节点 root。
 * 两节点之间路径的长度由它们之间边数表示。
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
 *           1
 *          / \
 *         2   3
 *        / \
 *       4   5
 *
 *   输出：3（最长路径为 4→2→1→3 或 5→2→1→3，长度为 3 条边）
 *
 *   示例 2：
 *       1
 *      /
 *     2
 *
 *   输出：1（路径 2→1，长度为 1 条边）
 *
 * ──────────────────────────────────────────────
 * 解法一：递归（DFS，后序遍历 + 全局最大值）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   关键洞察：任意一条路径，都有一个"最高节点"（拐弯点）。
 *   经过某节点的最长路径 = 该节点的左子树深度 + 右子树深度。
 *
 *   所以：直径 = max{ 每个节点的 (左深度 + 右深度) }
 *
 *   我们可以在计算深度的递归过程中，顺便更新全局最大直径。
 *   这样只需一次 DFS 即可同时完成两件事：
 *     1. 计算每个节点的深度（返回给父节点用）
 *     2. 更新经过当前节点的路径长度（左深度 + 右深度）
 *
 * 二、为什么"路径经过也可能不经过根节点"？
 *
 *   考虑这棵树：
 *           1
 *          /
 *         2
 *        / \
 *       3   4
 *      /     \
 *     5       6
 *
 *   经过根节点 1 的最长路径 = 左深度 3 + 右深度 0 = 3
 *   但经过节点 2 的最长路径 = 左深度 2 + 右深度 2 = 4 ← 更长！
 *
 *   所以我们需要检查每个节点，而不仅仅是根节点。
 *
 * 三、图解（示例 1）
 *
 *           1
 *          / \
 *         2   3
 *        / \
 *       4   5
 *
 *   DFS 过程（后序遍历：先处理子树，再处理当前节点）：
 *
 *   depth(4)：左=0, 右=0 → 经过4的路径=0+0=0 → 返回深度 max(0,0)+1 = 1
 *   depth(5)：左=0, 右=0 → 经过5的路径=0+0=0 → 返回深度 max(0,0)+1 = 1
 *   depth(2)：左=1, 右=1 → 经过2的路径=1+1=2 → 返回深度 max(1,1)+1 = 2
 *   depth(3)：左=0, 右=0 → 经过3的路径=0+0=0 → 返回深度 max(0,0)+1 = 1
 *   depth(1)：左=2, 右=1 → 经过1的路径=2+1=3 → 返回深度 max(2,1)+1 = 3
 *
 *   全局最大路径 = max(0, 0, 2, 0, 3) = 3 ✅
 *
 * 四、深度 vs 路径长度的区别
 *
 *   注意：这里的"深度"指边的数量（不是节点数量）。
 *   空节点返回 0，叶子节点返回的深度 = max(0,0)+1 = 1（到叶子有 1 条边）。
 *
 *   等价地，也可以理解为"节点数量"的深度：
 *   空节点返回 0，叶子返回 1，但路径长度 = 左深度 + 右深度（不再需要 -2）。
 *   两种理解方式计算结果一致。
 *
 * 五、递归树（标注返回的深度值）
 *
 *           1  → 深度 3
 *          / \
 *     深度2← 2   3 →深度 1
 *        / \
 *   深度1← 4   5 →深度 1
 *
 *   经过每个节点的路径长度：
 *     节点 4：0 + 0 = 0
 *     节点 5：0 + 0 = 0
 *     节点 2：1 + 1 = 2
 *     节点 3：0 + 0 = 0
 *     节点 1：2 + 1 = 3  ← 最大值
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(h)  —— h 为树高，递归栈深度；最坏 O(n)，平衡时 O(log n)
 */
class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int maxDiameter = 0;
        depth(root, maxDiameter);
        return maxDiameter;
    }

private:
    // 返回以 node 为根的子树的深度（边数）
    // 同时更新经过 node 的最长路径
    int depth(TreeNode* node, int& maxDiameter) {
        if (node == nullptr)
        {
            return 0;
        }

        int leftDepth = depth(node->left, maxDiameter);
        int rightDepth = depth(node->right, maxDiameter);

        // 经过当前节点的路径长度 = 左深度 + 右深度
        maxDiameter = max(maxDiameter, leftDepth + rightDepth);

        // 返回当前节点的深度（供父节点使用）
        return max(leftDepth, rightDepth) + 1;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：递归（使用成员变量）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   与解法一完全相同，只是将全局最大值存储在成员变量中，
 *   避免通过引用参数传递，代码更简洁。
 *
 * 时间复杂度：O(n)
 * 空间复杂度：O(h)
 */
class Solution2 {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        maxDiameter_ = 0;
        depth(root);
        return maxDiameter_;
    }

private:
    int maxDiameter_;

    int depth(TreeNode* node) {
        if (node == nullptr)
        {
            return 0;
        }

        int leftDepth = depth(node->left);
        int rightDepth = depth(node->right);

        // 更新全局最大直径
        maxDiameter_ = max(maxDiameter_, leftDepth + rightDepth);

        return max(leftDepth, rightDepth) + 1;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：DFS 迭代（后序遍历 + 哈希表记录深度）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   用迭代的后序遍历替代递归。
 *   后序遍历保证：处理一个节点时，它的左右子树已经被处理过了。
 *   用哈希表记录每个节点的深度，供父节点查询。
 *
 * 二、图解
 *
 *           1
 *          / \
 *         2   3
 *        / \
 *       4   5
 *
 *   后序遍历顺序：4 → 5 → 2 → 3 → 1
 *
 *   处理 4：depth[4] = max(0, 0) + 1 = 1，经过4的路径 = 0
 *   处理 5：depth[5] = max(0, 0) + 1 = 1，经过5的路径 = 0
 *   处理 2：depth[2] = max(1, 1) + 1 = 2，经过2的路径 = 1+1 = 2
 *   处理 3：depth[3] = max(0, 0) + 1 = 1，经过3的路径 = 0
 *   处理 1：depth[1] = max(2, 1) + 1 = 3，经过1的路径 = 2+1 = 3
 *
 *   最大直径 = 3 ✅
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(n)  —— 栈 + 哈希表
 */
class Solution3 {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        if (root == nullptr) return 0;

        int maxDiameter = 0;
        unordered_map<TreeNode*, int> depthMap;
        depthMap[nullptr] = 0;

        stack<TreeNode*> stk;
        TreeNode *curr = root;
        TreeNode *lastVisited = nullptr;

        while (curr != nullptr || !stk.empty())
        {
            // 一路向左压栈
            while (curr != nullptr)
            {
                stk.push(curr);
                curr = curr->left;
            }

            TreeNode *node = stk.top();

            // 如果右子树存在且未被访问过，先处理右子树
            if (node->right != nullptr && node->right != lastVisited)
            {
                curr = node->right;
            }
            else
            {
                // 后序访问：左右子树都已处理完
                stk.pop();

                int leftDepth = depthMap.count(node->left) ? depthMap[node->left] : 0;
                int rightDepth = depthMap.count(node->right) ? depthMap[node->right] : 0;

                maxDiameter = max(maxDiameter, leftDepth + rightDepth);
                depthMap[node] = max(leftDepth, rightDepth) + 1;

                lastVisited = node;
            }
        }

        return maxDiameter;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法              时间复杂度    空间复杂度    特点
 *   ────              ────────      ────────      ────
 *   递归(引用参数)     O(n)          O(h)          最推荐，简洁高效
 *   递归(成员变量)     O(n)          O(h)          代码更简洁
 *   迭代(后序+哈希)    O(n)          O(n)          避免递归栈溢出
 *
 *   h = 树高，平衡树 h = O(log n)，最坏 h = O(n)
 *
 *   面试建议：
 *   - 递归解法是标准答案，必须掌握
 *   - 关键思维：在求深度的递归中"顺便"更新直径
 *     这种"在递归过程中维护全局信息"的模式非常常见
 *   - 常见陷阱：只检查根节点的左深度+右深度（忽略不经过根的路径）
 *
 * ──────────────────────────────────────────────
 * 递归思维
 * ──────────────────────────────────────────────
 *
 *   本题的递归模式：在递归函数中同时完成两件事
 *
 *     返回值：当前子树的深度（供父节点计算路径用）
 *     副作用：更新全局最大直径
 *
 *   这种模式在树的题目中非常常见：
 *     - 124. Binary Tree Maximum Path Sum：最大路径和（同样的"拐弯点"思维）
 *     - 687. Longest Univalue Path：最长同值路径
 *     - 110. Balanced Binary Tree：判断平衡（返回深度，同时判断是否平衡）
 *
 *   对比 104 题（最大深度）：
 *     104：只需要返回值（深度）
 *     543：返回值（深度） + 副作用（更新直径）
 *
 *   通用模式：
 *     f(node) {
 *         left = f(node->left)
 *         right = f(node->right)
 *         更新全局信息（副作用）
 *         return 供父节点使用的值
 *     }
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   104. Maximum Depth of Binary Tree     —— 最大深度（本题的子问题）
 *   110. Balanced Binary Tree             —— 平衡判断（类似递归模式）
 *   124. Binary Tree Maximum Path Sum     —— 最大路径和（同样的"拐弯点"思维，更难）
 *   687. Longest Univalue Path            —— 最长同值路径（同样思路）
 *   1522. Diameter of N-Ary Tree          —— N 叉树的直径（推广）
 */
