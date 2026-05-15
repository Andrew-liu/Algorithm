/*
 * 437. Path Sum III
 * https://leetcode.com/problems/path-sum-iii/
 *
 * 给定一个二叉树的根节点 root，和一个整数 targetSum，
 * 求该二叉树里节点值之和等于 targetSum 的路径的数目。
 *
 * 路径不需要从根节点开始，也不需要在叶子节点结束，
 * 但是路径方向必须是向下的（只能从父节点到子节点）。
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
 *              10
 *             /  \
 *            5   -3
 *           / \    \
 *          3   2    11
 *         / \   \
 *        3  -2   1
 *
 *   targetSum = 8
 *   输出：3
 *   解释：和等于 8 的路径有 3 条：
 *     1. 5 → 3          （5+3 = 8）
 *     2. 5 → 2 → 1     （5+2+1 = 8）
 *     3. -3 → 11        （-3+11 = 8）
 *
 * ──────────────────────────────────────────────
 * 本题与 560 题的关系
 * ──────────────────────────────────────────────
 *
 *   560 题：在数组中找和为 k 的子数组个数 → 前缀和 + HashMap
 *   437 题：在二叉树中找和为 targetSum 的路径个数
 *
 *   二叉树中"从某祖先到当前节点的路径" 就相当于 数组中的"子数组"：
 *     - 数组的前缀和：从下标 0 累加到当前位置
 *     - 树的前缀和：从根节点累加到当前节点
 *     - 数组的子数组和：prefix[j] - prefix[i]
 *     - 树的路径和：curSum - 某祖先的前缀和
 *
 *   区别：数组是线性的，树是分叉的。
 *   树的 DFS 路径是一条从根到叶子的"链"，在这条链上就是线性问题！
 *   但进入不同分支时需要"回溯"（移除当前路径的前缀和）。
 *
 * ──────────────────────────────────────────────
 * 解法一：前缀和 + 回溯（最优解）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   DFS 遍历树，维护"从根到当前节点的前缀和" curSum。
 *   用 HashMap 记录"当前路径上每个前缀和出现的次数"。
 *
 *   对于当前节点，路径和 == targetSum 的条件：
 *     curSum - 某祖先的前缀和 == targetSum
 *     即：某祖先的前缀和 == curSum - targetSum
 *
 *   所以：count += map[curSum - targetSum]
 *
 *   与 560 题完全相同的逻辑！只是：
 *     - 560 题遍历数组，向右走
 *     - 437 题遍历树，DFS 向下走，回溯时撤销
 *
 * 二、回溯的关键
 *
 *   进入左/右子树前：map[curSum]++
 *   离开左/右子树后：map[curSum]--（撤销）
 *
 *   为什么要撤销？
 *   因为不同分支的路径不能混用。节点 A 的前缀和只对 A 的子孙有意义，
 *   当 DFS 回到 A 的父节点并进入另一个分支时，A 的前缀和不应该存在于 map 中。
 *
 * 三、图解
 *
 *              10
 *             /  \
 *            5   -3
 *           / \    \
 *          3   2    11
 *         / \   \
 *        3  -2   1
 *
 *   targetSum = 8，初始 map = {0: 1}
 *
 *   dfs(10)：curSum = 10
 *     查 map[10-8] = map[2] = 0，count += 0
 *     map = {0:1, 10:1}
 *
 *     dfs(5)：curSum = 15
 *       查 map[15-8] = map[7] = 0，count += 0
 *       map = {0:1, 10:1, 15:1}
 *
 *       dfs(3)：curSum = 18
 *         查 map[18-8] = map[10] = 1，count += 1  ← 路径 5→3（sum=18-10=8）
 *         map = {0:1, 10:1, 15:1, 18:1}
 *
 *         dfs(3)：curSum = 21
 *           查 map[21-8] = map[13] = 0
 *           map = {0:1, 10:1, 15:1, 18:1, 21:1}
 *           回溯：map[21]-- → map = {0:1, 10:1, 15:1, 18:1}
 *
 *         dfs(-2)：curSum = 16
 *           查 map[16-8] = map[8] = 0
 *           回溯：map[16]--
 *
 *         回溯：map[18]-- → map = {0:1, 10:1, 15:1}
 *
 *       dfs(2)：curSum = 17
 *         查 map[17-8] = map[9] = 0
 *         map = {0:1, 10:1, 15:1, 17:1}
 *
 *         dfs(1)：curSum = 18
 *           查 map[18-8] = map[10] = 1，count += 1  ← 路径 5→2→1（sum=18-10=8）
 *           回溯：map[18]--
 *
 *         回溯：map[17]--
 *
 *       回溯：map[15]-- → map = {0:1, 10:1}
 *
 *     dfs(-3)：curSum = 7
 *       查 map[7-8] = map[-1] = 0
 *       map = {0:1, 10:1, 7:1}
 *
 *       dfs(11)：curSum = 18
 *         查 map[18-8] = map[10] = 1，count += 1  ← 路径 -3→11（sum=18-10=8）
 *         回溯：map[18]--
 *
 *       回溯：map[7]--
 *
 *     回溯：map[10]--
 *
 *   最终 count = 3 ✅
 *
 * 四、为什么 map 初始化 {0: 1}？
 *
 *   与 560 题相同：前缀和为 0 代表"从根节点开始的路径"。
 *   如果 curSum == targetSum，那么从根到当前节点本身就是一条合法路径，
 *   需要 map[curSum - targetSum] = map[0] = 1 来统计到它。
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次，HashMap 操作 O(1)
 * 空间复杂度：O(n)  —— HashMap 最多存储 O(n) 个前缀和 + 递归栈 O(h)
 */
class Solution {
public:
    int pathSum(TreeNode* root, int targetSum) {
        unordered_map<long long, int> prefixCount; // {前缀和 → 出现次数}
        prefixCount[0] = 1; // 空路径，前缀和为 0
        int count = 0;
        dfs(root, 0, targetSum, prefixCount, count);
        return count;
    }

private:
    void dfs(TreeNode* node, long long curSum, int targetSum,
             unordered_map<long long, int>& prefixCount, int& count) {
        if (node == nullptr) return;

        // 更新当前前缀和
        curSum += node->val;

        // 查询：有多少个祖先的前缀和 == curSum - targetSum
        if (prefixCount.count(curSum - targetSum))
        {
            count += prefixCount[curSum - targetSum];
        }

        // 存入当前前缀和（供子孙节点查询）
        prefixCount[curSum]++;

        // 递归左右子树
        dfs(node->left, curSum, targetSum, prefixCount, count);
        dfs(node->right, curSum, targetSum, prefixCount, count);

        // 回溯：离开当前节点，撤销前缀和
        prefixCount[curSum]--;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：暴力 DFS（双重递归）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   对每个节点，以它为起点向下搜索所有路径，统计和为 targetSum 的路径数。
 *
 *   外层递归：遍历每个节点（作为路径起点）
 *   内层递归：从该起点出发，向下搜索所有路径
 *
 *   结果 = 以 root 为起点的路径数 + 以 root->left 为根的子树中的路径数
 *                                   + 以 root->right 为根的子树中的路径数
 *
 * 二、图解
 *
 *              10
 *             /  \
 *            5   -3
 *           / \    \
 *          3   2    11
 *
 *   targetSum = 8
 *
 *   对节点 10 为起点：向下搜索
 *     10 → sum=10 ≠ 8
 *     10→5 → sum=15 ≠ 8
 *     10→5→3 → sum=18 ≠ 8
 *     10→5→2 → sum=17 ≠ 8
 *     10→-3 → sum=7 ≠ 8
 *     10→-3→11 → sum=18 ≠ 8
 *     以 10 为起点：0 条
 *
 *   对节点 5 为起点：
 *     5 → sum=5 ≠ 8
 *     5→3 → sum=8 == 8 ✅
 *     5→2 → sum=7 ≠ 8
 *     5→2→1 → sum=8 == 8 ✅
 *     以 5 为起点：2 条
 *
 *   对节点 -3 为起点：
 *     -3 → sum=-3 ≠ 8
 *     -3→11 → sum=8 == 8 ✅
 *     以 -3 为起点：1 条
 *
 *   总计：0 + 2 + 1 = 3 ✅
 *
 * 三、两个递归函数的关系
 *
 *   pathSum(root)：遍历所有节点作为起点（外层）
 *   countFrom(node, remain)：从 node 出发向下找 remain 和的路径数（内层）
 *
 * 时间复杂度：O(n²)  —— 每个节点作为起点，向下最坏遍历 O(n)
 *             平衡树约 O(n log n)（每个节点向下搜索深度 O(log n)）
 * 空间复杂度：O(h)   —— 递归栈深度
 */
class Solution2 {
public:
    int pathSum(TreeNode* root, int targetSum) {
        if (root == nullptr) return 0;

        // 以当前节点为起点的路径数 + 左子树中的路径数 + 右子树中的路径数
        return countFrom(root, targetSum)
             + pathSum(root->left, targetSum)
             + pathSum(root->right, targetSum);
    }

private:
    // 从 node 出发向下，找路径和 == remain 的路径数
    int countFrom(TreeNode* node, long long remain) {
        if (node == nullptr) return 0;

        int count = 0;

        // 当前节点值恰好等于剩余目标值 → 找到一条路径
        if (node->val == remain)
        {
            count++;
        }

        // 继续向下搜索（即使已经找到，后面可能有负数使得再次满足）
        count += countFrom(node->left, remain - node->val);
        count += countFrom(node->right, remain - node->val);

        return count;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法              时间复杂度        空间复杂度    特点
 *   ────              ────────          ────────      ────
 *   前缀和+回溯        O(n)              O(n)          最优解，树上的 560 题
 *   暴力双重递归        O(n²) / O(nlogn)  O(h)          直观但慢
 *
 *   h = 树高，平衡树 h = O(log n)，最坏 h = O(n)
 *
 *   面试建议：
 *   - 解法一（前缀和+回溯）是最优解，展示对 560 题的深入理解和迁移能力
 *   - 解法二适合快速给出正确答案，但面试官可能追问优化
 *   - 关键理解：树上的路径问题 → 根到当前节点的链 → 线性前缀和问题
 *   - 注意用 long long 避免整数溢出（节点值可能很大）
 *
 * ──────────────────────────────────────────────
 * 前缀和 + 回溯的模板
 * ──────────────────────────────────────────────
 *
 *   void dfs(node, curSum, target, map, count) {
 *       if (!node) return;
 *
 *       curSum += node->val;
 *       count += map[curSum - target];    // 查询
 *       map[curSum]++;                     // 存入
 *
 *       dfs(left, ...);                   // 递归
 *       dfs(right, ...);
 *
 *       map[curSum]--;                     // 回溯（关键！）
 *   }
 *
 *   回溯的本质：HashMap 只保存"当前路径"上的前缀和，
 *   而不是"所有访问过的节点"的前缀和。
 *   这确保查询时只匹配祖先节点，不会错误匹配其他分支的节点。
 *
 * ──────────────────────────────────────────────
 * 为什么暴力解中找到 remain==0 后还要继续搜索？
 * ──────────────────────────────────────────────
 *
 *   因为节点值可能为负数！
 *
 *   例如：路径 5 → 3 → -3 → 3，targetSum = 8
 *     5+3 = 8 ✅（已找到一条）
 *     但 5+3+(-3)+3 = 8 ✅（又一条更长的路径）
 *
 *   所以找到和为 target 后不能停止，必须继续向下搜索。
 *
 * ──────────────────────────────────────────────
 * 递归思维
 * ──────────────────────────────────────────────
 *
 *   本题结合了两种递归模式：
 *
 *   解法一：DFS + 回溯（维护路径状态）
 *     进入节点：更新状态（map[curSum]++）
 *     离开节点：撤销状态（map[curSum]--）
 *     经典的回溯模板，类似于排列/组合/子集问题。
 *
 *   解法二：双重递归
 *     外层：遍历所有节点（O(n) 个起点）
 *     内层：从每个起点向下搜索（单源 DFS）
 *     类似于"对每个元素做一次完整搜索"的暴力模式。
 *
 *   对比 560 题（数组版本）：
 *     560：线性遍历 + 前缀和 + HashMap
 *     437：树的 DFS + 前缀和 + HashMap + 回溯
 *     唯一的新增就是"回溯"——因为树有分支，离开分支时需撤销状态。
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   560. Subarray Sum Equals K            —— 数组版前缀和（本题的线性版本）
 *   112. Path Sum                         —— 判断是否存在根到叶子路径和为 target
 *   113. Path Sum II                      —— 找出所有根到叶子路径和为 target 的路径
 *   124. Binary Tree Maximum Path Sum     —— 最大路径和（任意节点到任意节点）
 *   543. Diameter of Binary Tree          —— 类似的"路径经过某节点"的思维
 */
