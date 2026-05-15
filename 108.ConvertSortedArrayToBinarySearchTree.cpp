/*
 * 108. Convert Sorted Array to Binary Search Tree
 * https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/
 *
 * 给你一个整数数组 nums，其中元素已经按升序排列，请你将其转换为一棵平衡二叉搜索树。
 * 平衡二叉搜索树：每个节点的左右子树高度差不超过 1。
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
 *     输入：nums = [-10, -3, 0, 5, 9]
 *     输出：[0, -3, 9, -10, null, 5]（答案不唯一）
 *
 *           0
 *          / \
 *        -3   9
 *        /   /
 *      -10  5
 *
 *   示例 2：
 *     输入：nums = [1, 3]
 *     输出：[3, 1] 或 [1, null, 3]
 *
 * ──────────────────────────────────────────────
 * 解法一：递归（分治，取中间元素为根）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   BST 的性质：中序遍历是升序序列。
 *   所以有序数组就是这棵 BST 的中序遍历结果。
 *
 *   要构建平衡 BST，关键是：每次选数组的中间元素作为根。
 *   这样左半部分构建左子树，右半部分构建右子树，
 *   左右元素个数最多相差 1，保证了高度平衡。
 *
 *   递归定义：
 *     - 输入：数组区间 [left, right]
 *     - base case：left > right → 返回 null（空区间，无节点）
 *     - 递归步骤：
 *       1. 取中间位置 mid = left + (right - left) / 2
 *       2. nums[mid] 作为当前子树的根
 *       3. [left, mid-1] 递归构建左子树
 *       4. [mid+1, right] 递归构建右子树
 *
 * 二、为什么取中间元素能保证平衡？
 *
 *   取中间元素后：
 *     - 左半部分有 mid - left 个元素 → 构建左子树
 *     - 右半部分有 right - mid 个元素 → 构建右子树
 *     - 两边元素个数最多相差 1
 *
 *   元素个数相差 ≤ 1 → 子树高度相差 ≤ 1 → 平衡 ✓
 *
 *   这本质上是二分法：每次把问题平均分成两半。
 *
 * 三、图解
 *
 *   nums = [-10, -3, 0, 5, 9]
 *            0    1  2  3  4
 *
 *   build([0, 4])：mid = 2，根 = nums[2] = 0
 *     build([0, 1])：mid = 0，根 = nums[0] = -10
 *       build([0, -1])：left > right → null
 *       build([1, 1])：mid = 1，根 = nums[1] = -3
 *         build([1, 0])：null
 *         build([2, 1])：null
 *         返回 节点(-3)
 *       返回 节点(-10, left=null, right=节点(-3))
 *     build([3, 4])：mid = 3，根 = nums[3] = 5
 *       build([3, 2])：null
 *       build([4, 4])：mid = 4，根 = nums[4] = 9
 *         build([4, 3])：null
 *         build([5, 4])：null
 *         返回 节点(9)
 *       返回 节点(5, left=null, right=节点(9))
 *     返回 节点(0, left=节点(-10), right=节点(5))
 *
 *   构建的树：
 *           0
 *          / \
 *       -10   5
 *          \   \
 *          -3   9
 *
 * 四、递归树（标注区间和选取的根）
 *
 *              [0,4] → 根=0
 *              /         \
 *        [0,1]→根=-10   [3,4]→根=5
 *         /  \           /  \
 *     [0,-1] [1,1]→-3  [3,2] [4,4]→9
 *      null   / \       null   / \
 *          null null         null null
 *
 * 五、答案为什么不唯一？
 *
 *   当数组长度为偶数时，"中间"有两个候选位置：
 *     - mid = left + (right - left) / 2        ← 偏左
 *     - mid = left + (right - left + 1) / 2    ← 偏右
 *
 *   选左或选右都能构建合法的平衡 BST，只是形状不同。
 *   例如 [1, 3]：选 1 为根得到 [1, null, 3]，选 3 为根得到 [3, 1]。
 *
 * 时间复杂度：O(n)  —— 每个元素恰好被访问一次，创建一个节点
 * 空间复杂度：O(log n)  —— 递归栈深度（树是平衡的，高度为 log n）
 */
class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return build(nums, 0, nums.size() - 1);
    }

private:
    TreeNode* build(vector<int>& nums, int left, int right) {
        if (left > right)
        {
            return nullptr;
        }

        // 取中间元素作为根（偏左）
        int mid = left + (right - left) / 2;

        TreeNode *root = new TreeNode(nums[mid]);
        root->left = build(nums, left, mid - 1);
        root->right = build(nums, mid + 1, right);

        return root;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：迭代（BFS 模拟，用队列管理待构建的区间）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   用队列模拟递归过程。队列中存储三元组：(节点指针, 区间左端, 区间右端)。
 *   每次取出一个三元组，计算 mid，设置节点值，然后为左右子节点创建占位，
 *   将对应的区间信息入队。
 *
 * 二、图解
 *
 *   nums = [-10, -3, 0, 5, 9]
 *
 *   初始：创建根节点 root，queue = [(root, 0, 4)]
 *
 *   取出 (root, 0, 4)：mid=2, root->val=0
 *     左区间 [0,1] 非空：创建左子节点，入队 (root->left, 0, 1)
 *     右区间 [3,4] 非空：创建右子节点，入队 (root->right, 3, 4)
 *
 *   取出 (node, 0, 1)：mid=0, node->val=-10
 *     左区间 [0,-1] 为空：不创建
 *     右区间 [1,1] 非空：创建右子节点，入队 (node->right, 1, 1)
 *
 *   取出 (node, 3, 4)：mid=3, node->val=5
 *     左区间 [3,2] 为空：不创建
 *     右区间 [4,4] 非空：创建右子节点，入队 (node->right, 4, 4)
 *
 *   取出 (node, 1, 1)：mid=1, node->val=-3
 *     左右区间都为空：不创建
 *
 *   取出 (node, 4, 4)：mid=4, node->val=9
 *     左右区间都为空：不创建
 *
 *   队列为空，结束 ✅
 *
 * 时间复杂度：O(n)  —— 每个元素处理一次
 * 空间复杂度：O(n)  —— 队列最多存储 O(n) 个待处理项
 */
class Solution2 {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        if (nums.empty()) return nullptr;

        TreeNode *root = new TreeNode(0); // 占位，稍后设置值

        // 队列存储：{节点指针, 左端点, 右端点}
        queue<tuple<TreeNode*, int, int>> q;
        q.push({root, 0, (int)nums.size() - 1});

        while (!q.empty())
        {
            auto [node, left, right] = q.front();
            q.pop();

            int mid = left + (right - left) / 2;
            node->val = nums[mid];

            // 左区间非空，创建左子节点
            if (left <= mid - 1)
            {
                node->left = new TreeNode(0);
                q.push({node->left, left, mid - 1});
            }

            // 右区间非空，创建右子节点
            if (mid + 1 <= right)
            {
                node->right = new TreeNode(0);
                q.push({node->right, mid + 1, right});
            }
        }

        return root;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：迭代（栈模拟 DFS）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   与解法二逻辑完全相同，只是用栈代替队列。
 *   栈模拟的是 DFS 顺序（先处理左子树），队列模拟的是 BFS 顺序（逐层处理）。
 *   最终构建的树结构完全一致。
 *
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */
class Solution3 {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        if (nums.empty()) return nullptr;

        TreeNode *root = new TreeNode(0);

        stack<tuple<TreeNode*, int, int>> stk;
        stk.push({root, 0, (int)nums.size() - 1});

        while (!stk.empty())
        {
            auto [node, left, right] = stk.top();
            stk.pop();

            int mid = left + (right - left) / 2;
            node->val = nums[mid];

            if (left <= mid - 1)
            {
                node->left = new TreeNode(0);
                stk.push({node->left, left, mid - 1});
            }

            if (mid + 1 <= right)
            {
                node->right = new TreeNode(0);
                stk.push({node->right, mid + 1, right});
            }
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
 *   递归(分治)     O(n)          O(log n)     最简洁，天然的分治结构
 *   迭代(队列)     O(n)          O(n)          BFS 模拟，避免递归
 *   迭代(栈)       O(n)          O(n)          DFS 模拟，避免递归
 *
 *   面试建议：
 *   - 递归解法是标准答案，5 行核心代码
 *   - 关键洞察：有序数组 = BST 的中序遍历，取中间构建根保证平衡
 *   - 本题是"分治法"的经典体现：
 *     大问题（构建整棵树）= 选根 + 构建左子树 + 构建右子树
 *
 * ──────────────────────────────────────────────
 * 递归思维
 * ──────────────────────────────────────────────
 *
 *   本题的递归模式："选取 + 分治"
 *
 *     build(区间) {
 *         选取区间中间元素为根
 *         root->left = build(左半区间)
 *         root->right = build(右半区间)
 *         return root
 *     }
 *
 *   对比其他树的递归题：
 *     104（最大深度）：自底向上聚合信息
 *     226（翻转二叉树）：自顶向下修改结构
 *     543（直径）：递归中维护全局信息
 *     108（本题）：分治构建，自顶向下创建节点
 *
 *   本题与归并排序的关系：
 *     归并排序：分成两半 → 递归排序 → 合并
 *     本题：取中间为根 → 递归构建左右子树
 *     都是经典的"分而治之"思想。
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   109. Convert Sorted List to BST  —— 有序链表转 BST（无法随机访问中点）
 *   110. Balanced Binary Tree        —— 判断是否平衡
 *   1382. Balance a BST              —— 将任意 BST 变平衡（中序遍历 + 本题）
 *   105. Construct BT from Preorder and Inorder —— 从遍历序列构建树
 *   106. Construct BT from Inorder and Postorder —— 从遍历序列构建树
 */
