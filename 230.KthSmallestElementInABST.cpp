/*
 * 230. Kth Smallest Element in a BST
 * https://leetcode.com/problems/kth-smallest-element-in-a-bst/
 *
 * 给定一个二叉搜索树的根节点 root，和一个整数 k，
 * 请你设计一个算法查找其中第 k 小的元素（k 从 1 开始计数）。
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
 *       1   4
 *        \
 *         2
 *   k = 1，输出：1（最小的元素）
 *
 *   示例 2：
 *           5
 *          / \
 *         3   6
 *        / \
 *       2   4
 *      /
 *     1
 *   k = 3，输出：3（中序遍历：1,2,3,4,5,6，第 3 个是 3）
 *
 * ──────────────────────────────────────────────
 * 核心洞察
 * ──────────────────────────────────────────────
 *
 *   BST 的中序遍历是严格递增序列。
 *   所以"第 k 小" = 中序遍历的第 k 个节点。
 *
 *   不需要遍历所有节点：中序遍历到第 k 个就可以停止。
 *
 * ──────────────────────────────────────────────
 * 解法一：中序遍历（迭代，提前终止）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   使用迭代中序遍历模板（94 题），维护计数器 count。
 *   每访问一个节点 count++，当 count == k 时返回当前节点值。
 *
 *   迭代实现的优势：可以在找到答案时立即返回，无需遍历整棵树。
 *
 * 二、图解
 *
 *           5
 *          / \
 *         3   6
 *        / \
 *       2   4
 *      /
 *     1
 *
 *   k = 3，找第 3 小的元素
 *
 *   迭代中序遍历过程：
 *
 *   一路向左：压栈 [5, 3, 2, 1]，curr = null
 *   弹出 1：count=1（k=3，继续），curr=1->right=null
 *   弹出 2：count=2（k=3，继续），curr=2->right=null
 *   弹出 3：count=3 == k → 返回 3 ✅（不再继续遍历）
 *
 * 三、中序遍历迭代模板回顾
 *
 *   一路向左压栈 → 弹出访问 → 转向右子树
 *   （详见 94 题的解法二）
 *
 * 时间复杂度：O(H + k)  —— H 为树高（向左压栈），k 为访问的节点数
 *             最坏 O(n)（退化链表 + k=n）
 *             平衡树平均 O(log n + k)
 * 空间复杂度：O(H)  —— 栈深度
 */
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        stack<TreeNode*> stk;
        TreeNode *curr = root;
        int count = 0;

        while (curr != nullptr || !stk.empty())
        {
            // 一路向左压栈
            while (curr != nullptr)
            {
                stk.push(curr);
                curr = curr->left;
            }

            // 弹出栈顶，中序访问
            curr = stk.top();
            stk.pop();

            // 第 count 个节点
            if (++count == k)
            {
                return curr->val;
            }

            // 转向右子树
            curr = curr->right;
        }

        return -1; // 不会到达这里（题目保证 k 合法）
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：中序遍历（递归）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   递归中序遍历，用成员变量记录计数器和结果。
 *   每次"访问"节点时 count--，当 count == 0 时记录答案。
 *
 * 二、图解
 *
 *           5
 *          / \
 *         3   6
 *        / \
 *       2   4
 *      /
 *     1
 *
 *   k = 3
 *
 *   inorder(5)
 *     inorder(3)
 *       inorder(2)
 *         inorder(1)
 *           inorder(null) → 返回
 *           访问 1：count=2（还没到 0）
 *           inorder(null) → 返回
 *         访问 2：count=1
 *         inorder(null) → 返回
 *       访问 3：count=0 → result_=3 ✅
 *       （后续递归中 count <= 0 直接返回，起到"提前终止"效果）
 *
 * 时间复杂度：O(H + k)  —— 与解法一相同
 *             （虽然递归不能真正"break"，但通过 count 判断可以跳过后续操作）
 * 空间复杂度：O(H)  —— 递归栈深度
 */
class Solution2 {
public:
    int kthSmallest(TreeNode* root, int k) {
        count_ = k;
        result_ = 0;
        inorder(root);
        return result_;
    }

private:
    int count_;
    int result_;

    void inorder(TreeNode* node) {
        if (node == nullptr || count_ <= 0) return;

        // 1. 递归左子树
        inorder(node->left);

        // 2. 访问当前节点
        if (--count_ == 0)
        {
            result_ = node->val;
            return;
        }

        // 3. 递归右子树
        inorder(node->right);
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：递归（计算左子树大小定位）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   利用 BST 的结构性质：如果知道左子树的节点数 leftCount，
 *   就可以判断第 k 小在哪个子树中：
 *
 *     - leftCount == k-1：根节点就是第 k 小
 *     - leftCount >= k：第 k 小在左子树中
 *     - leftCount < k-1：第 k 小在右子树中，且是右子树的第 (k-leftCount-1) 小
 *
 *   类似二分查找的思想：每次排除一半。
 *
 * 二、图解
 *
 *           5              leftCount = 4
 *          / \
 *         3   6            k=3, leftCount=4 ≥ k → 在左子树中找第 3 小
 *        / \
 *       2   4              leftCount = 1
 *      /
 *     1                    k=3, leftCount=1 < k-1 → 在右子树找第 (3-1-1)=1 小
 *
 *   在以 4 为根的子树中找第 1 小：
 *     leftCount = 0 == k-1 → 根节点 4？
 *     等等，让我重新走一遍...
 *
 *   实际上走到节点 3（左子树大小=1）：
 *     k=3, leftCount=1, 1 < 3-1=2 → 不在左子树，不是根 → 去右子树找第 3-1-1=1 小
 *     右子树是节点 4（左子树大小=0）：
 *       k=1, leftCount=0, 0 == k-1=0 → 根节点 4 就是答案？
 *
 *   不对，让我重新分析示例 2（k=3 答案是 3）...
 *
 *   从根节点 5 开始：
 *     左子树（以3为根）的节点数 = 4
 *     leftCount=4 >= k=3 → 第 3 小在左子树中
 *
 *   在节点 3 中找第 3 小：
 *     左子树（以2为根）的节点数 = 2
 *     leftCount=2 == k-1=2 → 根节点 3 就是第 3 小 ✅
 *
 * 三、countNodes 的代价
 *
 *   每次计算 leftCount 需要遍历左子树，整体时间复杂度最坏 O(n²)。
 *   但如果树是平衡的，类似于每层做一次 O(n) 的 count → O(n log n)。
 *
 *   优化：如果节点中存储子树大小（augmented BST），则可以 O(H) 完成。
 *
 * 时间复杂度：O(n)~O(n²)  —— 取决于树的平衡程度
 *             （平衡树约 O(n)，因为每层 count 的总和 ≈ n）
 * 空间复杂度：O(H)  —— 递归栈
 */
class Solution3 {
public:
    int kthSmallest(TreeNode* root, int k) {
        int leftCount = countNodes(root->left);

        if (leftCount == k - 1)
        {
            // 根就是第 k 小
            return root->val;
        }
        else if (leftCount >= k)
        {
            // 第 k 小在左子树中
            return kthSmallest(root->left, k);
        }
        else
        {
            // 第 k 小在右子树中，排除左子树和根
            return kthSmallest(root->right, k - leftCount - 1);
        }
    }

private:
    int countNodes(TreeNode* node) {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法              时间复杂度      空间复杂度    特点
 *   ────              ────────        ────────      ────
 *   中序遍历(迭代)     O(H + k)        O(H)          最推荐，可提前终止
 *   中序遍历(递归)     O(H + k)        O(H)          代码简洁
 *   左子树计数         O(n)~O(n²)      O(H)          思路清晰但效率不如中序
 *
 *   H = 树高，平衡树 H = O(log n)，最坏 H = O(n)
 *
 *   面试建议：
 *   - 中序遍历迭代解法是最佳答案：时间最优 + 可提前终止
 *   - 关键洞察一句话："BST 中序 = 有序，第 k 小 = 中序第 k 个"
 *   - 进阶问题：如果频繁查询不同 k 值怎么办？
 *     → 在每个节点存储子树大小（Augmented BST），每次查询 O(H)
 *
 * ──────────────────────────────────────────────
 * 进阶：频繁查询的优化
 * ──────────────────────────────────────────────
 *
 *   如果这棵 BST 会被频繁修改（插入/删除），并且频繁查询第 k 小：
 *
 *   方案一：Augmented BST
 *     每个节点额外存储一个字段 size = 左子树大小。
 *     查询时：
 *       if (k == left.size + 1) return root
 *       if (k <= left.size) 去左子树
 *       else 去右子树（k -= left.size + 1）
 *     查询 O(H)，修改时维护 size 也是 O(H)。
 *
 *   方案二：平衡 BST（AVL / 红黑树）+ size 字段
 *     保证 H = O(log n)，查询和修改都是 O(log n)。
 *
 * ──────────────────────────────────────────────
 * 递归思维
 * ──────────────────────────────────────────────
 *
 *   本题利用 BST 的核心性质：中序遍历 = 有序序列。
 *
 *   这个性质在多道题中反复使用：
 *     94. 中序遍历           —— 模板
 *     98. 验证 BST           —— 中序递增 ⟺ 有效 BST
 *     230. 第 k 小（本题）    —— 中序第 k 个
 *     501. BST 众数          —— 中序遍历中统计频率
 *     538. BST 转累加树      —— 反向中序（右→根→左）累加
 *
 *   掌握中序遍历的递归和迭代模板后，这类 BST 题目都能迎刃而解。
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   94. Binary Tree Inorder Traversal   —— 中序遍历模板（本题基础）
 *   98. Validate BST                    —— 利用中序递增验证 BST
 *   108. Convert Sorted Array to BST    —— 有序数组 → BST（中序逆过程）
 *   378. Kth Smallest Element in Matrix —— 矩阵中第 k 小（不同数据结构）
 *   671. Second Minimum Node in BT      —— 第 2 小的节点
 */
