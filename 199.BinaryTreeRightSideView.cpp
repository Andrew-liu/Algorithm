/*
 * 199. Binary Tree Right Side View
 * https://leetcode.com/problems/binary-tree-right-side-view/
 *
 * 给定一个二叉树的根节点 root，想象自己站在它的右侧，
 * 按照从顶部到底部的顺序，返回从右侧所能看到的节点值。
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
 *         1        ←  看到 1
 *        / \
 *       2   3      ←  看到 3
 *        \   \
 *         5   4    ←  看到 4
 *
 *   输出：[1, 3, 4]
 *
 *   示例 2：
 *         1        ←  看到 1
 *          \
 *           3      ←  看到 3
 *
 *   输出：[1, 3]
 *
 *   示例 3：
 *         1        ←  看到 1
 *        /
 *       2          ←  看到 2（右边没有节点，左边的反而可见）
 *      /
 *     3            ←  看到 3
 *
 *   输出：[1, 2, 3]
 *
 * ──────────────────────────────────────────────
 * 核心洞察
 * ──────────────────────────────────────────────
 *
 *   "右侧视图" = 每一层的最右边节点。
 *
 *   注意：不是只走右子树！如果某层右子树为空，左子树的节点也会被看到。
 *   例如示例 3 中，每层只有一个左子节点，但它们都是该层最右的节点。
 *
 * ──────────────────────────────────────────────
 * 解法一：BFS（层序遍历，取每层最后一个）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   层序遍历（102 题模板），每层处理完后取最后一个节点的值。
 *   "每层最后一个" = 从右侧看到的节点。
 *
 * 二、图解
 *
 *         1
 *        / \
 *       2   3
 *        \   \
 *         5   4
 *
 *   ─── 第 1 层 ───
 *   queue = [1]，levelSize = 1
 *   处理 1：子节点 2, 3 入队
 *   本层最后一个 = 1 → result = [1]
 *
 *   ─── 第 2 层 ───
 *   queue = [2, 3]，levelSize = 2
 *   处理 2：子节点 5 入队
 *   处理 3：子节点 4 入队
 *   本层最后一个 = 3 → result = [1, 3]
 *
 *   ─── 第 3 层 ───
 *   queue = [5, 4]，levelSize = 2
 *   处理 5：无子节点
 *   处理 4：无子节点
 *   本层最后一个 = 4 → result = [1, 3, 4]
 *
 *   队列为空，结束。结果：[1, 3, 4] ✅
 *
 * 三、如何判断"最后一个"？
 *
 *   在 for 循环中，当 i == levelSize - 1 时，就是本层最后一个节点。
 *   或者：每次都更新 lastVal，循环结束后 lastVal 就是最后一个的值。
 *
 * 时间复杂度：O(n)  —— 每个节点入队出队各一次
 * 空间复杂度：O(n)  —— 队列最多存储一层节点数（最坏 n/2）
 */
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        if (root == nullptr) return result;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty())
        {
            int levelSize = q.size();

            for (int i = 0; i < levelSize; ++i)
            {
                TreeNode *node = q.front();
                q.pop();

                // 本层最后一个节点 → 右侧可见
                if (i == levelSize - 1)
                {
                    result.push_back(node->val);
                }

                if (node->left != nullptr) q.push(node->left);
                if (node->right != nullptr) q.push(node->right);
            }
        }

        return result;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：DFS 递归（先右后左，每层第一个到达的）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   DFS 遍历时，如果先访问右子树再访问左子树（根→右→左），
 *   那么每层第一个被访问到的节点就是该层最右边的节点。
 *
 *   如何判断"第一个到达新的一层"？
 *   当 depth == result.size() 时，说明这是该层第一次被访问。
 *
 * 二、图解
 *
 *         1
 *        / \
 *       2   3
 *        \   \
 *         5   4
 *
 *   DFS（根→右→左）遍历顺序：1 → 3 → 4 → 2 → 5
 *
 *   dfs(1, depth=0)：depth==result.size()==0 → result=[1]
 *     dfs(3, depth=1)：depth==result.size()==1 → result=[1,3]
 *       dfs(4, depth=2)：depth==result.size()==2 → result=[1,3,4]
 *     dfs(2, depth=1)：depth=1 < result.size()=3 → 该层已有值，跳过
 *       dfs(5, depth=2)：depth=2 < result.size()=3 → 跳过
 *
 *   结果：[1, 3, 4] ✅
 *
 * 三、为什么"先右后左"有效？
 *
 *   先右后左保证：同一层中，最右边的节点总是第一个被 DFS 到。
 *   一旦某层已经被记录（result.size() > depth），后续到达该层的节点都被跳过。
 *
 * 四、左侧视图怎么做？
 *
 *   改为"先左后右"即可（根→左→右），每层第一个到达的就是最左边的。
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(h)  —— 递归栈深度；最坏 O(n)，平衡时 O(log n)
 */
class Solution2 {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        dfs(root, 0, result);
        return result;
    }

private:
    void dfs(TreeNode* node, int depth, vector<int>& result) {
        if (node == nullptr) return;

        // 该层第一次被访问 → 这是最右边的节点
        if (depth == result.size())
        {
            result.push_back(node->val);
        }

        // 先右后左：保证每层最右节点先被访问
        dfs(node->right, depth + 1, result);
        dfs(node->left, depth + 1, result);
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：DFS 递归（先左后右，每层覆盖更新）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   另一种 DFS 思路：先左后右遍历（普通前序），
 *   每次到达某层时都更新该层的值（覆盖之前的）。
 *   遍历结束后，每层保留的就是最后一个被访问的节点 = 最右节点。
 *
 * 二、图解
 *
 *         1
 *        / \
 *       2   3
 *        \   \
 *         5   4
 *
 *   DFS（根→左→右）遍历顺序：1 → 2 → 5 → 3 → 4
 *
 *   dfs(1, depth=0)：result[0] = 1     → result = [1]
 *     dfs(2, depth=1)：result[1] = 2   → result = [1, 2]
 *       dfs(5, depth=2)：result[2] = 5 → result = [1, 2, 5]
 *     dfs(3, depth=1)：result[1] = 3   → result = [1, 3, 5]  ← 覆盖了 2
 *       dfs(4, depth=2)：result[2] = 4 → result = [1, 3, 4]  ← 覆盖了 5
 *
 *   结果：[1, 3, 4] ✅
 *
 * 三、与解法二的对比
 *
 *   解法二（先右后左）：每层只记录第一次到达的（最右节点），后续跳过
 *   解法三（先左后右）：每层不断覆盖，最后一次覆盖的就是最右节点
 *
 *   两者结果相同，解法二略高效（少做一些赋值操作）。
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(h)  —— 递归栈深度
 */
class Solution3 {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        dfs(root, 0, result);
        return result;
    }

private:
    void dfs(TreeNode* node, int depth, vector<int>& result) {
        if (node == nullptr) return;

        // 扩展 result 以容纳新的层
        if (depth == result.size())
        {
            result.push_back(node->val);
        }
        else
        {
            // 覆盖：后来的（更右的）节点覆盖之前的
            result[depth] = node->val;
        }

        // 先左后右：保证最右节点最后覆盖
        dfs(node->left, depth + 1, result);
        dfs(node->right, depth + 1, result);
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法              时间复杂度    空间复杂度    特点
 *   ────              ────────      ────────      ────
 *   BFS(层序遍历)      O(n)          O(n)          最直观，取每层最后一个
 *   DFS(先右后左)      O(n)          O(h)          每层第一次到达 = 最右节点
 *   DFS(先左后右)      O(n)          O(h)          覆盖更新，最终值 = 最右节点
 *
 *   h = 树高，平衡树 h = O(log n)，最坏 h = O(n)
 *
 *   面试建议：
 *   - BFS 解法最直观，是 102 层序遍历模板的直接变体
 *   - DFS 解法二（先右后左）最巧妙，空间可能更优
 *   - 关键理解："右侧视图" = "每层最右节点"，不是"只走右子树"
 *
 * ──────────────────────────────────────────────
 * 与层序遍历模板的关系
 * ──────────────────────────────────────────────
 *
 *   本题是 102 题（层序遍历）的变体。
 *   102 模板中"处理 node"的逻辑改为"只取每层最后一个"即可。
 *
 *   类似的变体：
 *     102：收集每层所有值       → level.push_back(val)
 *     199：取每层最后一个（本题）→ if (i == levelSize-1) result.push_back(val)
 *     515：取每层最大值         → levelMax = max(levelMax, val)
 *     637：取每层平均值         → levelSum / levelSize
 *     103：锯齿形（奇数层反转） → 奇数层 reverse(level)
 *
 *   掌握 102 模板后，这些变体都是一行改动。
 *
 * ──────────────────────────────────────────────
 * DFS 思维拓展
 * ──────────────────────────────────────────────
 *
 *   解法二用到的"先右后左 + depth == result.size()"技巧：
 *
 *   本质：DFS 中通过遍历顺序保证特定节点先被访问。
 *
 *   类似技巧：
 *     - 右侧视图：先右后左，每层第一个 = 最右
 *     - 左侧视图：先左后右，每层第一个 = 最左
 *     - 102 DFS 解法：先左后右 + 层号，所有节点按层收集
 *
 *   这体现了 DFS 和 BFS 的等价性：
 *   很多"看起来需要 BFS"的问题，用 DFS + 层号也能解决。
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   102. Binary Tree Level Order Traversal —— 层序遍历模板（本题基础）
 *   103. Binary Tree Zigzag Level Order    —— 锯齿形层序
 *   104. Maximum Depth of Binary Tree      —— 最大深度（层数）
 *   515. Find Largest Value in Each Row    —— 每层最大值
 *   637. Average of Levels in Binary Tree  —— 每层平均值
 *   116. Populating Next Right Pointers    —— 连接每层兄弟节点
 */
