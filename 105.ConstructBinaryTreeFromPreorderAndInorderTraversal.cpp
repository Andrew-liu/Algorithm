/*
 * 105. Construct Binary Tree from Preorder and Inorder Traversal
 * https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/
 *
 * 给定两个整数数组 preorder 和 inorder，其中 preorder 是二叉树的先序遍历，
 * inorder 是同一棵树的中序遍历，请构造二叉树并返回其根节点。
 *
 * 约束：节点值互不相同。
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
 *     preorder = [3, 9, 20, 15, 7]
 *     inorder  = [9, 3, 15, 20, 7]
 *
 *     输出：
 *           3
 *          / \
 *         9   20
 *            / \
 *           15  7
 *
 * ──────────────────────────────────────────────
 * 核心洞察
 * ──────────────────────────────────────────────
 *
 *   先序遍历的性质：第一个元素一定是根节点。
 *   中序遍历的性质：根节点将序列分为左子树部分和右子树部分。
 *
 *   结合两者：
 *     1. 从先序中确定根节点（第一个元素）
 *     2. 在中序中找到根节点的位置，从而确定左右子树的范围
 *     3. 递归构建左子树和右子树
 *
 *   两个序列的结构：
 *
 *     preorder: [ 根 | ←左子树的先序→ | ←右子树的先序→ ]
 *     inorder:  [ ←左子树的中序→ | 根 | ←右子树的中序→ ]
 *
 *   一旦知道左子树的节点数 leftCount（从中序中算出），
 *   就能在先序中划分出左子树和右子树的部分。
 *
 * ──────────────────────────────────────────────
 * 解法一：递归（哈希表加速查找）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   递归构建：
 *     - 先序的第一个元素 = 当前子树的根
 *     - 在中序中找到根的位置 → 确定左子树大小 leftCount
 *     - 先序中 [preLeft+1, preLeft+leftCount] = 左子树的先序
 *     - 先序中 [preLeft+leftCount+1, preRight] = 右子树的先序
 *     - 中序中 [inLeft, rootIdx-1] = 左子树的中序
 *     - 中序中 [rootIdx+1, inRight] = 右子树的中序
 *
 *   用哈希表预存 inorder 中每个值的索引，将查找从 O(n) 降到 O(1)。
 *
 * 二、图解
 *
 *     preorder = [3, 9, 20, 15, 7]
 *     inorder  = [9, 3, 15, 20, 7]
 *
 *   第一步：根 = preorder[0] = 3
 *   在 inorder 中找到 3 的位置：index = 1
 *   左子树节点数 leftCount = 1 - 0 = 1
 *
 *     preorder: [3 | 9 | 20, 15, 7]
 *                根  左    右子树
 *     inorder:  [9 | 3 | 15, 20, 7]
 *               左   根    右子树
 *
 *   递归构建左子树：preorder=[9], inorder=[9]
 *     根 = 9，无左右子树 → 叶子节点 9
 *
 *   递归构建右子树：preorder=[20, 15, 7], inorder=[15, 20, 7]
 *     根 = 20
 *     在 inorder 中找到 20 的位置：左边是 [15]，右边是 [7]
 *     leftCount = 1
 *
 *     preorder: [20 | 15 | 7]
 *     inorder:  [15 | 20 | 7]
 *
 *     左子树：preorder=[15], inorder=[15] → 叶子节点 15
 *     右子树：preorder=[7], inorder=[7]   → 叶子节点 7
 *
 *   最终结果：
 *           3
 *          / \
 *         9   20
 *            / \
 *           15  7  ✅
 *
 * 三、区间索引推导（关键！）
 *
 *   设当前递归处理的范围：
 *     先序：[preLeft, preRight]
 *     中序：[inLeft, inRight]
 *
 *   根 = preorder[preLeft]
 *   根在中序中的位置 = rootIdx
 *   左子树节点数 = leftCount = rootIdx - inLeft
 *
 *   ┌──────────────┬──────────────────────────────────────┐
 *   │              │  左子树                右子树          │
 *   ├──────────────┼──────────────────────────────────────┤
 *   │ 先序(pre)    │  [preLeft+1,           [preLeft+     │
 *   │              │   preLeft+leftCount]    leftCount+1,  │
 *   │              │                         preRight]     │
 *   ├──────────────┼──────────────────────────────────────┤
 *   │ 中序(in)     │  [inLeft,              [rootIdx+1,   │
 *   │              │   rootIdx-1]            inRight]      │
 *   └──────────────┴──────────────────────────────────────┘
 *
 * 时间复杂度：O(n)  —— 每个节点构建一次，哈希查找 O(1)
 * 空间复杂度：O(n)  —— 哈希表 O(n) + 递归栈 O(h)
 */
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        // 预处理：建立中序值 → 索引的映射
        for (int i = 0; i < inorder.size(); ++i)
        {
            inorderMap_[inorder[i]] = i;
        }

        return build(preorder, 0, preorder.size() - 1,
                     inorder, 0, inorder.size() - 1);
    }

private:
    unordered_map<int, int> inorderMap_;

    TreeNode* build(vector<int>& preorder, int preLeft, int preRight,
                    vector<int>& inorder, int inLeft, int inRight) {
        if (preLeft > preRight)
        {
            return nullptr;
        }

        // 先序的第一个元素是根
        int rootVal = preorder[preLeft];
        TreeNode *root = new TreeNode(rootVal);

        // 在中序中定位根，确定左子树大小
        int rootIdx = inorderMap_[rootVal];
        int leftCount = rootIdx - inLeft;

        // 递归构建左右子树
        root->left = build(preorder, preLeft + 1, preLeft + leftCount,
                           inorder, inLeft, rootIdx - 1);
        root->right = build(preorder, preLeft + leftCount + 1, preRight,
                            inorder, rootIdx + 1, inRight);

        return root;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：递归（不用哈希表，线性查找）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   与解法一逻辑完全相同，但不预建哈希表。
 *   每次在中序数组中线性查找根的位置。
 *
 *   适用场景：节点数较少，或者不想额外空间。
 *
 * 时间复杂度：O(n²)  —— 每次查找 O(n)，共 n 层
 *             （最坏情况：退化为链表时每层查找 O(n)）
 * 空间复杂度：O(h)   —— 只有递归栈，无额外数据结构
 */
class Solution2 {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        return build(preorder, 0, preorder.size() - 1,
                     inorder, 0, inorder.size() - 1);
    }

private:
    TreeNode* build(vector<int>& preorder, int preLeft, int preRight,
                    vector<int>& inorder, int inLeft, int inRight) {
        if (preLeft > preRight)
        {
            return nullptr;
        }

        int rootVal = preorder[preLeft];
        TreeNode *root = new TreeNode(rootVal);

        // 线性查找根在中序中的位置
        int rootIdx = inLeft;
        while (inorder[rootIdx] != rootVal)
        {
            ++rootIdx;
        }

        int leftCount = rootIdx - inLeft;

        root->left = build(preorder, preLeft + 1, preLeft + leftCount,
                           inorder, inLeft, rootIdx - 1);
        root->right = build(preorder, preLeft + leftCount + 1, preRight,
                            inorder, rootIdx + 1, inRight);

        return root;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：迭代（栈模拟）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   观察先序和中序的关系：
 *   - 先序中连续出现的节点，如果一直是"往左走"，则中序中它们的顺序是反的
 *   - 当先序中下一个节点不再是当前节点的左子节点时，它应该是某个祖先的右子节点
 *
 *   用栈维护"从根到当前节点的路径"：
 *     - 遍历先序数组，对每个节点判断它是左子节点还是右子节点
 *     - 利用中序数组确定何时该"回退"（栈顶==中序当前元素时弹栈）
 *
 * 二、图解
 *
 *     preorder = [3, 9, 20, 15, 7]
 *     inorder  = [9, 3, 15, 20, 7]
 *
 *   创建根 3，stack = [3]，inIdx = 0
 *
 *   preorder[1] = 9：
 *     stack.top()=3 ≠ inorder[0]=9 → 9 是 3 的左子节点
 *     3->left = 9，stack = [3, 9]
 *
 *   preorder[2] = 20：
 *     stack.top()=9 == inorder[0]=9 → 开始回退
 *       弹出 9，inIdx=1，stack = [3]
 *     stack.top()=3 == inorder[1]=3 → 继续回退
 *       弹出 3，inIdx=2，stack = []
 *     栈空停止，20 是最后弹出节点 3 的右子节点
 *     3->right = 20，stack = [20]
 *
 *   preorder[3] = 15：
 *     stack.top()=20 ≠ inorder[2]=15 → 15 是 20 的左子节点
 *     20->left = 15，stack = [20, 15]
 *
 *   preorder[4] = 7：
 *     stack.top()=15 == inorder[2]=15 → 回退
 *       弹出 15，inIdx=3，stack = [20]
 *     stack.top()=20 == inorder[3]=20 → 继续回退
 *       弹出 20，inIdx=4，stack = []
 *     栈空停止，7 是最后弹出节点 20 的右子节点
 *     20->right = 7，stack = [7]
 *
 *   结果：
 *           3
 *          / \
 *         9   20
 *            / \
 *           15  7  ✅
 *
 * 三、判断左子节点 vs 右子节点的规则
 *
 *   对于先序的下一个节点 preorder[i]：
 *   - 如果 stack.top() ≠ inorder[inIdx]：
 *     说明当前还在"往左走"，preorder[i] 是 stack.top() 的左子节点
 *   - 如果 stack.top() == inorder[inIdx]：
 *     说明到达了左子树的底部，需要回退找到正确的父节点
 *     不断弹栈直到 stack.top() ≠ inorder[inIdx]
 *     preorder[i] 是最后弹出节点的右子节点
 *
 * 时间复杂度：O(n)  —— 每个节点入栈出栈各一次
 * 空间复杂度：O(n)  —— 栈的最大深度
 */
class Solution3 {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if (preorder.empty()) return nullptr;

        TreeNode *root = new TreeNode(preorder[0]);
        stack<TreeNode*> stk;
        stk.push(root);
        int inIdx = 0;

        for (int i = 1; i < preorder.size(); ++i)
        {
            TreeNode *node = new TreeNode(preorder[i]);

            if (stk.top()->val != inorder[inIdx])
            {
                // 还在往左走：当前节点是栈顶的左子节点
                stk.top()->left = node;
            }
            else
            {
                // 需要回退：找到正确的父节点
                TreeNode *parent = nullptr;
                while (!stk.empty() && stk.top()->val == inorder[inIdx])
                {
                    parent = stk.top();
                    stk.pop();
                    ++inIdx;
                }
                // 当前节点是最后弹出节点的右子节点
                parent->right = node;
            }

            stk.push(node);
        }

        return root;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法              时间复杂度    空间复杂度    特点
 *   ────              ────────      ────────      ────
 *   递归+哈希表        O(n)          O(n)          最推荐，清晰高效
 *   递归+线性查找      O(n²)         O(h)          简单但慢
 *   迭代(栈)           O(n)          O(n)          巧妙但理解难度大
 *
 *   面试建议：
 *   - 解法一（递归+哈希表）是标准答案，思路清晰且效率最优
 *   - 关键要能推导出区间索引的对应关系
 *   - 必须理解：为什么先序+中序能唯一确定一棵二叉树
 *
 * ──────────────────────────────────────────────
 * 为什么先序+中序能唯一确定二叉树？
 * ──────────────────────────────────────────────
 *
 *   先序提供：根是谁（每棵子树的第一个元素）
 *   中序提供：左右子树的划分（根把序列分成左右两部分）
 *
 *   两者结合：
 *     1. 先序告诉你根 → 2. 中序告诉你左右各有几个 → 3. 递归确定子树
 *
 *   不能唯一确定的情况：
 *     - 只有先序：无法区分左右子树的边界
 *     - 只有中序：无法确定根节点
 *     - 先序+后序：当某节点只有一个子节点时无法确定是左是右
 *
 * ──────────────────────────────────────────────
 * 递归思维
 * ──────────────────────────────────────────────
 *
 *   本题的递归模式：分治构建
 *
 *     build(preorder区间, inorder区间) {
 *         根 = preorder 的第一个
 *         在 inorder 中定位根 → 确定左右子树大小
 *         root->left = build(左子树的 preorder, 左子树的 inorder)
 *         root->right = build(右子树的 preorder, 右子树的 inorder)
 *         return root
 *     }
 *
 *   对比 108 题（有序数组→BST）：
 *     108：取中间为根，天然的左右划分
 *     105：从先序取根，用中序确定左右划分
 *
 *   两者都是"分治构建树"的模式：
 *     选根 → 确定左右范围 → 递归构建左右子树
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   106. Construct BT from Inorder and Postorder  —— 中序+后序构建（后序最后一个=根）
 *   889. Construct BT from Preorder and Postorder —— 先序+后序构建（不唯一）
 *   108. Convert Sorted Array to BST              —— 有序数组构建 BST（类似分治）
 *   297. Serialize and Deserialize Binary Tree    —— 序列化/反序列化
 *   94. Binary Tree Inorder Traversal             —— 中序遍历
 *   144. Binary Tree Preorder Traversal           —— 先序遍历
 */
