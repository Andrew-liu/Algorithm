/*
 * 98. Validate Binary Search Tree
 * https://leetcode.com/problems/validate-binary-search-tree/
 *
 * 给你一个二叉树的根节点 root，判断其是否是一个有效的二叉搜索树。
 *
 * 有效 BST 定义：
 *   - 节点的左子树只包含 小于 当前节点的数
 *   - 节点的右子树只包含 大于 当前节点的数
 *   - 所有左子树和右子树自身也必须是二叉搜索树
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
 *   示例 1（有效 BST）：
 *         2
 *        / \
 *       1   3
 *   输出：true
 *
 *   示例 2（无效 BST）：
 *         5
 *        / \
 *       1   4
 *          / \
 *         3   6
 *   输出：false（节点 3 在 5 的右子树中，但 3 < 5，违规）
 *
 * ──────────────────────────────────────────────
 * 常见错误思路
 * ──────────────────────────────────────────────
 *
 *   错误做法：只检查每个节点是否满足 left->val < node->val < right->val
 *
 *         5
 *        / \
 *       1   6
 *          / \
 *         3   7
 *
 *   节点 6：左子节点 3 < 6，右子节点 7 > 6 → 局部满足
 *   但节点 3 在 5 的右子树中，而 3 < 5 → 不是有效 BST！
 *
 *   正确理解：BST 要求的是"子树中所有节点"满足约束，而不仅仅是直接子节点。
 *   换句话说，每个节点有一个合法的取值范围 (lower, upper)。
 *
 * ──────────────────────────────────────────────
 * 解法一：递归（传递上下界）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   每个节点的值必须在一个合法范围 (lower, upper) 内：
 *     - 根节点：范围是 (-∞, +∞)
 *     - 走向左子树时：上界收紧为当前节点值 → (lower, node->val)
 *     - 走向右子树时：下界收紧为当前节点值 → (node->val, upper)
 *
 *   递归定义：
 *     - base case：空节点 → true
 *     - 当前节点值不在 (lower, upper) 范围内 → false
 *     - 递归验证左子树（上界收紧）和右子树（下界收紧）
 *
 * 二、图解
 *
 *         5           范围 (-∞, +∞)
 *        / \
 *       1   4         1 的范围 (-∞, 5)，4 的范围 (5, +∞)
 *          / \
 *         3   6       3 的范围 (5, 4)  ← 矛盾！lower > upper
 *
 *   验证过程：
 *   validate(5, -∞, +∞)：5 在范围内 ✓
 *     validate(1, -∞, 5)：1 在范围内 ✓
 *       validate(null) → true
 *       validate(null) → true
 *     validate(4, 5, +∞)：4 不在 (5, +∞) 范围内 ✗ → return false
 *
 *   结果：false ✅
 *
 * 三、上下界传递的直觉
 *
 *         10          范围 (-∞, +∞)
 *        /  \
 *       5    15       5: (-∞, 10)    15: (10, +∞)
 *      / \   / \
 *     3   7 12  20   3: (-∞, 5)     7: (5, 10)    ← 注意 7 的上界是 10 不是 ∞
 *                     12: (10, 15)   20: (15, +∞)
 *
 *   每往左走一次，上界收紧；每往右走一次，下界收紧。
 *   节点的范围 = 从根到该节点路径上所有"左转"和"右转"决定的。
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(h)  —— 递归栈深度；最坏 O(n)，平衡时 O(log n)
 */
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return validate(root, LONG_MIN, LONG_MAX);
    }

private:
    bool validate(TreeNode* node, long lower, long upper) {
        if (node == nullptr)
        {
            return true;
        }

        // 当前节点值必须严格在 (lower, upper) 范围内
        if (node->val <= lower || node->val >= upper)
        {
            return false;
        }

        // 左子树上界收紧为 node->val，右子树下界收紧为 node->val
        return validate(node->left, lower, node->val)
            && validate(node->right, node->val, upper);
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：中序遍历（递归）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   BST 的核心性质：中序遍历的结果是严格递增序列。
 *
 *   所以只需中序遍历这棵树，检查遍历序列是否严格递增即可。
 *   实现时不需要真的存储序列，只需维护"上一个访问的值"（prev），
 *   每次访问新节点时检查 node->val > prev。
 *
 * 二、图解
 *
 *   有效 BST：
 *         5
 *        / \
 *       3   7
 *      / \
 *     2   4
 *
 *   中序遍历：2 → 3 → 4 → 5 → 7（严格递增 ✓）
 *
 *   无效 BST：
 *         5
 *        / \
 *       1   4
 *          / \
 *         3   6
 *
 *   中序遍历：1 → 5 → 3 → ...（5 > 3，非递增 ✗）
 *
 * 三、为什么中序遍历能判断 BST？
 *
 *   BST 的定义等价于：中序遍历严格递增。
 *
 *   正向证明：BST → 中序递增
 *     中序先访问左子树（都 < 根），再访问根，再访问右子树（都 > 根）
 *     递归地，每棵子树内部也是递增的，所以整体递增。
 *
 *   反向证明：中序递增 → BST
 *     如果中序递增，那么根左边的值都 < 根，根右边的值都 > 根 → 满足 BST 定义。
 *
 * 时间复杂度：O(n)  —— 每个节点访问一次
 * 空间复杂度：O(h)  —— 递归栈深度
 */
class Solution2 {
public:
    bool isValidBST(TreeNode* root) {
        prev_ = LONG_MIN;
        return inorder(root);
    }

private:
    long prev_; // 记录中序遍历中上一个节点的值

    bool inorder(TreeNode* node) {
        if (node == nullptr) return true;

        // 1. 递归验证左子树
        if (!inorder(node->left))
        {
            return false;
        }

        // 2. 访问当前节点：检查是否严格大于前一个值
        if (node->val <= prev_)
        {
            return false;
        }
        prev_ = node->val;

        // 3. 递归验证右子树
        return inorder(node->right);
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：中序遍历（迭代，显式栈）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   与解法二的逻辑完全相同（中序遍历 + 检查严格递增），
 *   但用迭代方式实现中序遍历（即 94 题的迭代模板）。
 *
 * 二、图解
 *
 *         5
 *        / \
 *       3   7
 *      / \
 *     2   4
 *
 *   迭代中序遍历过程：
 *
 *   一路向左：压栈 [5, 3, 2]，curr = null
 *   弹出 2：prev=LONG_MIN, 2 > prev ✓，prev=2，curr=2->right=null
 *   弹出 3：3 > 2 ✓，prev=3，curr=3->right=4
 *   压栈 [5, 4]，curr = null
 *   弹出 4：4 > 3 ✓，prev=4，curr=4->right=null
 *   弹出 5：5 > 4 ✓，prev=5，curr=5->right=7
 *   压栈 [7]，curr = null
 *   弹出 7：7 > 5 ✓，prev=7，curr=7->right=null
 *
 *   全部通过 → true ✅
 *
 * 三、中序遍历迭代模板回顾（详见 94 题）
 *
 *   一路向左压栈 → 弹出访问 → 转向右子树
 *
 * 时间复杂度：O(n)  —— 每个节点入栈出栈各一次
 * 空间复杂度：O(h)  —— 栈深度，最坏 O(n)
 */
class Solution3 {
public:
    bool isValidBST(TreeNode* root) {
        stack<TreeNode*> stk;
        TreeNode *curr = root;
        long prev = LONG_MIN;

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

            // 检查是否严格递增
            if (curr->val <= prev)
            {
                return false;
            }
            prev = curr->val;

            // 转向右子树
            curr = curr->right;
        }

        return true;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法对比
 * ──────────────────────────────────────────────
 *
 *   解法              时间复杂度    空间复杂度    特点
 *   ────              ────────      ────────      ────
 *   递归(上下界)       O(n)          O(h)          直接验证 BST 定义
 *   中序遍历(递归)     O(n)          O(h)          利用"中序递增"性质
 *   中序遍历(迭代)     O(n)          O(h)          迭代实现，可提前终止
 *
 *   h = 树高，平衡树 h = O(log n)，最坏 h = O(n)
 *
 *   面试建议：
 *   - 两种思路都要会：上下界法直接体现 BST 定义；中序遍历法利用等价性质
 *   - 上下界法注意：用 long 避免 INT_MIN/INT_MAX 边界问题
 *   - 中序遍历法注意：是"严格递增"，不允许相等
 *   - 常见陷阱：只比较父子关系而忽略祖先约束
 *
 * ──────────────────────────────────────────────
 * 两种思路的本质联系
 * ──────────────────────────────────────────────
 *
 *   上下界法：自顶向下传递约束
 *     每个节点继承祖先传下来的合法范围，验证自身是否满足。
 *
 *   中序遍历法：自底向上/顺序验证
 *     BST ⟺ 中序严格递增，直接验证这个等价条件。
 *
 *   两者殊途同归：
 *     上下界法中，节点的 lower 就是它中序遍历的前一个节点的值；
 *     upper 就是它中序遍历的下一个节点的值。
 *
 *         10          中序：3, 5, 7, 10, 12, 15, 20
 *        /  \
 *       5    15       节点 7 的范围是 (5, 10)
 *      / \   / \      即中序中 7 的前驱=5，后继=10
 *     3   7 12  20
 *
 * ──────────────────────────────────────────────
 * 递归思维
 * ──────────────────────────────────────────────
 *
 *   本题展示了两种递归模式：
 *
 *   模式一：自顶向下传递信息（上下界）
 *     validate(node, lower, upper)
 *     父节点把约束"传给"子节点，子节点验证自身并继续传递。
 *     类似题：104 最大深度的迭代版本（传递深度值）
 *
 *   模式二：利用遍历序列性质
 *     中序遍历本身就是递归的（左→根→右），
 *     在遍历过程中维护 prev，检查序列性质。
 *     类似题：94 中序遍历，230 第 k 小的元素
 *
 * ──────────────────────────────────────────────
 * 相关题目
 * ──────────────────────────────────────────────
 *
 *   94. Binary Tree Inorder Traversal  —— 中序遍历模板
 *   230. Kth Smallest Element in BST   —— 利用中序遍历找第 k 小
 *   501. Find Mode in BST              —— 利用中序遍历找众数
 *   700. Search in a BST               —— BST 查找
 *   108. Convert Sorted Array to BST   —— 有序数组构建 BST（中序逆过程）
 *   99. Recover BST                    —— 恢复 BST（中序找两个错位元素）
 */
