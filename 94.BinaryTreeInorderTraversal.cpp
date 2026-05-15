/*
 * 94. Binary Tree Inorder Traversal
 * https://leetcode.com/problems/binary-tree-inorder-traversal/
 *
 * 给定一个二叉树的根节点 root，返回它的中序遍历。
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
 * ──────────────────────────────────────────────
 * 背景知识：三种遍历顺序
 * ──────────────────────────────────────────────
 *
 *   前序（Pre-order）：  根 → 左 → 右
 *   中序（In-order）：   左 → 根 → 右    ← 本题
 *   后序（Post-order）： 左 → 右 → 根
 *
 *   对于 BST（二叉搜索树），中序遍历的结果是升序序列。
 *
 *   示例：
 *         1
 *          \
 *           2
 *          /
 *         3
 *
 *   中序遍历：[1, 3, 2]
 *
 *         4
 *        / \
 *       2   6
 *      / \ / \
 *     1  3 5  7
 *
 *   中序遍历：[1, 2, 3, 4, 5, 6, 7]（BST → 升序）
 *
 * ──────────────────────────────────────────────
 * 解法一：递归
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   中序遍历的定义本身就是递归的：
 *     1. 递归遍历左子树
 *     2. 访问当前节点
 *     3. 递归遍历右子树
 *
 * 二、图解
 *
 *         4
 *        / \
 *       2   6
 *      / \
 *     1   3
 *
 *   inorder(4)
 *     inorder(2)              ← 先遍历左子树
 *       inorder(1)
 *         inorder(null) → 返回
 *         访问 1 → result: [1]
 *         inorder(null) → 返回
 *       访问 2 → result: [1, 2]
 *       inorder(3)
 *         inorder(null) → 返回
 *         访问 3 → result: [1, 2, 3]
 *         inorder(null) → 返回
 *     访问 4 → result: [1, 2, 3, 4]    ← 再访问根
 *     inorder(6)              ← 最后遍历右子树
 *       inorder(null) → 返回
 *       访问 6 → result: [1, 2, 3, 4, 6]
 *       inorder(null) → 返回
 *
 *   结果：[1, 2, 3, 4, 6] ✅
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(n)  —— 递归栈深度最坏 O(n)（退化为链表时）
 */
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        inorder(root, result);
        return result;
    }

private:
    void inorder(TreeNode* node, vector<int>& result) {
        if (node == nullptr) return;

        inorder(node->left, result);   // 1. 遍历左子树
        result.push_back(node->val);   // 2. 访问当前节点
        inorder(node->right, result);  // 3. 遍历右子树
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：迭代（显式栈）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   递归本质上使用系统调用栈，我们可以用显式栈来模拟。
 *
 *   中序遍历的规律：
 *     - 不断往左走，沿途节点压栈（这些节点的左子树还没处理完）
 *     - 走到底后，弹出栈顶节点（此时它的左子树已处理完），访问它
 *     - 然后转向它的右子树，重复上述过程
 *
 *   口诀：一路向左压栈 → 弹出访问 → 转向右子树
 *
 * 二、图解
 *
 *         4
 *        / \
 *       2   6
 *      / \
 *     1   3
 *
 *   初始：curr = 4, stack = []
 *
 *   ─── 一路向左压栈 ───
 *   curr=4：压栈 [4]，curr = 4->left = 2
 *   curr=2：压栈 [4, 2]，curr = 2->left = 1
 *   curr=1：压栈 [4, 2, 1]，curr = 1->left = null
 *
 *   ─── curr 为 null，开始弹出 ───
 *   弹出 1：访问 1，result=[1]，curr = 1->right = null
 *   弹出 2：访问 2，result=[1,2]，curr = 2->right = 3
 *
 *   ─── 转向右子树，继续向左 ───
 *   curr=3：压栈 [4, 3]，curr = 3->left = null
 *
 *   ─── 弹出 ───
 *   弹出 3：访问 3，result=[1,2,3]，curr = 3->right = null
 *   弹出 4：访问 4，result=[1,2,3,4]，curr = 4->right = 6
 *
 *   ─── 转向右子树 ───
 *   curr=6：压栈 [6]，curr = 6->left = null
 *
 *   ─── 弹出 ───
 *   弹出 6：访问 6，result=[1,2,3,4,6]，curr = 6->right = null
 *
 *   stack 为空且 curr 为 null，结束 ✅
 *
 * 三、循环条件
 *
 *   while (curr != nullptr || !stack.empty())
 *
 *   - curr != null：还有节点要往左探索
 *   - stack 非空：还有节点等待被访问（左子树已处理完的节点）
 *   - 两者都不满足时，遍历完成
 *
 * 时间复杂度：O(n)  —— 每个节点入栈出栈各一次
 * 空间复杂度：O(n)  —— 栈的深度最坏 O(n)
 */
class Solution2 {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> stk;
        TreeNode *curr = root;

        while (curr != nullptr || !stk.empty())
        {
            // 一路向左，沿途压栈
            while (curr != nullptr)
            {
                stk.push(curr);
                curr = curr->left;
            }

            // 弹出栈顶（左子树已处理完的节点），访问它
            curr = stk.top();
            stk.pop();
            result.push_back(curr->val);

            // 转向右子树
            curr = curr->right;
        }

        return result;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：Morris 遍历（O(1) 空间）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   Morris 遍历通过修改树的结构（建立临时线索）来实现 O(1) 空间遍历。
 *   核心思路：利用叶子节点的空 right 指针，建立指向后继节点的"线索"。
 *
 *   对于当前节点 curr：
 *     - 如果没有左子树：直接访问 curr，然后走向 right
 *     - 如果有左子树：找到左子树的最右节点（即 curr 的中序前驱）
 *       - 如果前驱的 right 为 null：建立线索（前驱->right = curr），走向左子树
 *       - 如果前驱的 right 为 curr：说明左子树已遍历完，断开线索，访问 curr，走向右子树
 *
 * 二、图解
 *
 *         4
 *        / \
 *       2   6
 *      / \
 *     1   3
 *
 *   curr=4：有左子树，找左子树的最右节点 = 3
 *     3->right == null → 建立线索 3->right = 4，curr 走向左
 *
 *         4 ←───────┐
 *        / \         |（线索）
 *       2   6        |
 *      / \          |
 *     1   3 ────────┘
 *
 *   curr=2：有左子树，找左子树的最右节点 = 1
 *     1->right == null → 建立线索 1->right = 2，curr 走向左
 *
 *   curr=1：没有左子树 → 访问 1，result=[1]，curr = 1->right = 2（通过线索回到 2）
 *
 *   curr=2：有左子树，找左子树的最右节点
 *     从 1 出发往右走，1->right == 2（== curr）→ 左子树已遍历完！
 *     断开线索（1->right = null），访问 2，result=[1,2]，curr = 2->right = 3
 *
 *   curr=3：没有左子树 → 访问 3，result=[1,2,3]，curr = 3->right = 4（通过线索回到 4）
 *
 *   curr=4：有左子树，找左子树的最右节点
 *     从 2 出发：2->right=3，3->right == 4（== curr）→ 左子树已遍历完！
 *     断开线索（3->right = null），访问 4，result=[1,2,3,4]，curr = 4->right = 6
 *
 *   curr=6：没有左子树 → 访问 6，result=[1,2,3,4,6]，curr = 6->right = null
 *
 *   curr == null，结束 ✅
 *
 * 三、为什么是 O(1) 空间？
 *
 *   不用栈、不用递归，只利用树中已有的空指针建立临时线索。
 *   遍历结束后树的结构会被恢复（断开线索）。
 *
 * 四、时间复杂度为什么仍然是 O(n)？
 *
 *   虽然"找前驱"看似额外遍历，但每条边最多被访问 2 次（建立线索 + 断开线索）。
 *   总边数 = n-1，所以总操作次数 ≤ 2(n-1) + n = O(n)。
 *
 * 时间复杂度：O(n)  —— 每条边最多访问 2 次
 * 空间复杂度：O(1)  —— 不使用栈或递归（结果数组不算）
 */
class Solution3 {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        TreeNode *curr = root;

        while (curr != nullptr)
        {
            if (curr->left == nullptr)
            {
                // 没有左子树：直接访问，转向右
                result.push_back(curr->val);
                curr = curr->right;
            }
            else
            {
                // 有左子树：找中序前驱（左子树的最右节点）
                TreeNode *predecessor = curr->left;
                while (predecessor->right != nullptr && predecessor->right != curr)
                {
                    predecessor = predecessor->right;
                }

                if (predecessor->right == nullptr)
                {
                    // 前驱的 right 为空：建立线索，走向左子树
                    predecessor->right = curr;
                    curr = curr->left;
                }
                else
                {
                    // 前驱的 right == curr：左子树已遍历完
                    // 断开线索，访问当前节点，转向右子树
                    predecessor->right = nullptr;
                    result.push_back(curr->val);
                    curr = curr->right;
                }
            }
        }

        return result;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法          时间复杂度    空间复杂度    特点
 *   ────          ────────      ────────      ────
 *   递归          O(n)          O(n)          最直观，3 行核心代码
 *   迭代（栈）    O(n)          O(n)          显式模拟递归过程
 *   Morris        O(n)          O(1)          利用线索实现 O(1) 空间
 *
 *   面试建议：
 *   - 递归解法必须秒写，展示对中序遍历定义的理解
 *   - 迭代解法是面试重点，考察对栈模拟递归的掌握
 *   - Morris 遍历是进阶，能写出来是亮点（注意恢复树结构）
 *   - 掌握中序遍历后，前序/后序的迭代写法可以类推
 *
 * ──────────────────────────────────────────────
 * 延伸：迭代法的统一模板（前/中/后序）
 * ──────────────────────────────────────────────
 *
 *   中序迭代的核心模式（本题）：
 *     向左压栈 → 弹出访问 → 转向右
 *
 *   前序迭代：
 *     访问当前 → 右子压栈 → 转向左
 *     （或：向左走时先访问再压栈）
 *
 *   后序迭代：
 *     前序（根→右→左）的结果反转 = 后序（左→右→根）
 *     或用"上一个访问节点"判断是否从右子树返回
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   144. Binary Tree Preorder Traversal   —— 前序遍历
 *   145. Binary Tree Postorder Traversal  —— 后序遍历
 *   98. Validate BST                      —— 利用中序遍历验证 BST
 *   230. Kth Smallest Element in BST      —— 中序遍历找第 k 小
 *   173. Binary Search Tree Iterator      —— 用迭代法实现 BST 迭代器
 */
