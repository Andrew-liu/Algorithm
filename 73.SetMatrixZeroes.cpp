/*
 * 73. Set Matrix Zeroes
 * https://leetcode.com/problems/set-matrix-zeroes/
 *
 * 给定一个 m x n 的矩阵，如果一个元素为 0，则将其所在行和列的所有元素都设为 0。
 * 请使用原地算法。
 *
 * ──────────────────────────────────────────────
 * 解法一：暴力标记（Brute Force）
 * ──────────────────────────────────────────────
 * 思路：
 *   用两个集合分别记录所有需要置零的行号和列号。
 *   第一遍遍历矩阵，找到所有 0 的位置，把其行号和列号存入集合。
 *   第二遍遍历矩阵，如果当前元素的行号或列号在集合中，就置为 0。
 *
 * 图解：matrix =
 *     [1, 1, 1]
 *     [1, 0, 1]
 *     [1, 1, 1]
 *
 *   第一遍：matrix[1][1] = 0
 *           → zeroRows = {1}, zeroCols = {1}
 *
 *   第二遍：行 1 或列 1 的元素全部置 0
 *     [1, 0, 1]
 *     [0, 0, 0]
 *     [1, 0, 1]  ✅
 *
 * 时间复杂度：O(m * n)  —— 两遍遍历矩阵
 * 空间复杂度：O(m + n)  —— 两个集合存储行号和列号
 */
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = (int)matrix.size();
        int n = (int)matrix[0].size();

        unordered_set<int> zeroRows; // 需要置零的行
        unordered_set<int> zeroCols; // 需要置零的列

        // 第一遍：收集所有 0 所在的行号和列号
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (matrix[i][j] == 0)
                {
                    zeroRows.insert(i);
                    zeroCols.insert(j);
                }
            }
        }

        // 第二遍：行号或列号命中，则置零
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (zeroRows.count(i) || zeroCols.count(j))
                {
                    matrix[i][j] = 0;
                }
            }
        }
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：原地标记 —— 用首行首列做标记（In-place Markers）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   解法一需要 O(m + n) 额外空间来记录 "哪些行、哪些列要置零"。
 *   观察：矩阵自身的第一行和第一列可以充当这个标记数组！
 *
 *   如果 matrix[i][j] == 0，就把对应的 "行首" matrix[i][0] 和
 *   "列首" matrix[0][j] 标记为 0，表示"这一整行 / 整列都要清零"。
 *
 *   但第一行和第一列本身也可能需要清零，它们的标记位互相重叠在
 *   matrix[0][0] 上，所以需要一个额外变量 firstColZero 来单独记录
 *   "第一列是否需要清零"，而 matrix[0][0] 专门标记"第一行是否需要清零"。
 *
 * 二、算法步骤
 *
 *   1. 扫描第一列，判断是否包含 0 → firstColZero
 *   2. 从左上到右下扫描整个矩阵（含第一行），遇到 0 就在行首和列首打标记
 *   3. 从内部（第 1 行、第 1 列起）向外清零，避免标记被提前破坏
 *   4. 根据 matrix[0][0] 决定第一行是否清零
 *   5. 根据 firstColZero 决定第一列是否清零
 *
 *   为什么清零要从内部向外？
 *     如果先清第一行/列，标记信息会丢失，内部就不知道该不该清了。
 *     所以必须先用标记清内部，最后再清第一行/列。
 *
 * 三、图解
 *
 *   matrix =
 *     [0, 1, 2, 0]        m = 3, n = 4
 *     [3, 4, 5, 2]
 *     [1, 3, 1, 5]
 *
 *   Step 1: 扫描第一列 → matrix[0][0]=0 → firstColZero = true
 *
 *   Step 2: 扫描整个矩阵，遇到 0 在行首/列首打标记
 *
 *     matrix[0][0]=0 → matrix[0][0]=0（行首）, matrix[0][0]=0（列首）—— 已是 0
 *     matrix[0][3]=0 → matrix[0][0]=0（行首）, matrix[0][3]=0（列首）
 *
 *     标记后（ * 表示被标记为 0）：
 *       行首           列首
 *       ↓               ↓
 *     [ 0*, 1, 2, 0*]   ← matrix[0][0]=0 标记第 0 行，matrix[0][3]=0 标记第 3 列
 *     [ 3,  4, 5, 2 ]   ← matrix[1][0]=3 不变
 *     [ 1,  3, 1, 5 ]   ← matrix[2][0]=1 不变
 *
 *   Step 3: 从内部（i>=1, j>=1）根据标记清零
 *
 *     matrix[i][j] → 检查 matrix[i][0] 和 matrix[0][j]
 *     第 1 行：matrix[1][0]=3≠0, 无需整行清零
 *     第 2 行：matrix[2][0]=1≠0, 无需整行清零
 *     第 3 列：matrix[0][3]=0 → matrix[1][3]=0, matrix[2][3]=0
 *
 *     内部清零后：
 *     [ 0, 1, 2, 0]
 *     [ 3, 4, 5, 0]   ← 第 3 列被清零
 *     [ 1, 3, 1, 0]   ← 第 3 列被清零
 *
 *   Step 4: matrix[0][0]=0 → 第一行全部清零
 *     [ 0, 0, 0, 0]
 *     [ 3, 4, 5, 0]
 *     [ 1, 3, 1, 0]
 *
 *   Step 5: firstColZero=true → 第一列全部清零
 *     [ 0, 0, 0, 0]
 *     [ 0, 4, 5, 0]
 *     [ 0, 3, 1, 0]  ✅
 *
 * 四、对比
 *
 *   - 解法一 O(m+n) 额外空间，简单直观
 *   - 解法二 O(1) 额外空间（只用一个 bool 变量），利用矩阵自身做标记
 *
 * 时间复杂度：O(m * n)  —— 常数遍矩阵遍历
 * 空间复杂度：O(1)      —— 只用一个额外布尔变量
 */
class Solution2 {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = (int)matrix.size();
        int n = (int)matrix[0].size();
        bool firstColZero = false; // 单独记录第一列是否需要清零

        // Step 1 & 2 合并：扫描整个矩阵
        for (int i = 0; i < m; ++i)
        {
            // 第一列是否有 0
            if (matrix[i][0] == 0)
            {
                firstColZero = true;
            }

            // 从第 1 列开始扫描（第 0 列已单独处理）
            for (int j = 1; j < n; ++j)
            {
                if (matrix[i][j] == 0)
                {
                    matrix[i][0] = 0; // 行首标记
                    matrix[0][j] = 0; // 列首标记
                }
            }
        }

        // Step 3: 根据标记清零内部（从内部向外，避免破坏标记）
        for (int i = 1; i < m; ++i)
        {
            for (int j = 1; j < n; ++j)
            {
                if (matrix[i][0] == 0 || matrix[0][j] == 0)
                {
                    matrix[i][j] = 0;
                }
            }
        }

        // Step 4: 根据 matrix[0][0] 决定第一行是否清零
        if (matrix[0][0] == 0)
        {
            for (int j = 0; j < n; ++j)
            {
                matrix[0][j] = 0;
            }
        }

        // Step 5: 根据 firstColZero 决定第一列是否清零
        if (firstColZero)
        {
            for (int i = 0; i < m; ++i)
            {
                matrix[i][0] = 0;
            }
        }
    }
};
