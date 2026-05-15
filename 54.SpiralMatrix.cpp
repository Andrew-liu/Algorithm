/*
 * 54. Spiral Matrix
 * https://leetcode.com/problems/spiral-matrix/
 *
 * 给你一个 m 行 n 列的矩阵 matrix，请按照顺时针螺旋顺序，返回矩阵中的所有元素。
 *
 * ──────────────────────────────────────────────
 * 解法一：逐层模拟（Layer-by-Layer Simulation）
 * ──────────────────────────────────────────────
 * 思路：
 *   把矩阵看成若干层"圈"，从最外圈向内圈逐层剥离。
 *   每一圈按 "右 → 下 → 左 → 上" 四个方向依次收集元素。
 *
 *   用四个边界变量控制当前圈的范围：
 *     top    —— 当前圈的上边行号
 *     bottom —— 当前圈的下边行号
 *     left   —— 当前圈的左边列号
 *     right  —— 当前圈的右边列号
 *
 *   每收集完一条边，就把对应的边界向内收缩一步，
 *   直到 top > bottom 或 left > right 表示所有圈都处理完毕。
 *
 * 图解：matrix =
 *     [ 1,  2,  3,  4]
 *     [ 5,  6,  7,  8]
 *     [ 9, 10, 11, 12]
 *
 *     m = 3, n = 4
 *     初始：top=0, bottom=2, left=0, right=3
 *
 *   ─── 第 1 圈 ───
 *
 *     ① 向右（top 行，left → right）：收集 1, 2, 3, 4      top++  → top=1
 *
 *        → → → →
 *       [1, 2, 3, 4]
 *       [5, 6, 7, 8]
 *       [9,10,11,12]
 *
 *     ② 向下（right 列，top → bottom）：收集 8, 12           right-- → right=2
 *
 *       [ ·, ·, ·, 4]
 *       [ 5, 6, 7, 8↓]
 *       [ 9,10,11,12↓]
 *
 *     ③ 向左（bottom 行，right → left）：收集 11, 10, 9      bottom-- → bottom=1
 *
 *       [ ·, ·, ·, ·]
 *       [ 5, 6, 7, ·]
 *       [ 9←10←11, ·]
 *
 *     ④ 向上（left 列，bottom → top）：收集 5                 left++ → left=1
 *
 *       [ ·, ·, ·, ·]
 *       [5↑, 6, 7, ·]
 *       [ ·, ·, ·, ·]
 *
 *   ─── 第 2 圈 ───
 *
 *     此时：top=1, bottom=1, left=1, right=2
 *
 *     ① 向右（top 行，left → right）：收集 6, 7              top++ → top=2
 *
 *       [ ·, ·, ·, ·]
 *       [ ·, 6→, 7→,·]
 *       [ ·, ·, ·, ·]
 *
 *     top=2 > bottom=1 → 结束
 *
 *   结果：[1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7] ✅
 *
 *   补充：为什么向左和向上需要额外判断？
 *
 *     考虑只有一行的情况：matrix = [[1, 2, 3]]
 *       top=0, bottom=0, left=0, right=2
 *       ① 向右：收集 1, 2, 3，top++ → top=1
 *       ③ 向左：此时 top=1 > bottom=0，如果不判断会重复收集！
 *
 *     考虑只有一列的情况：matrix = [[1], [2], [3]]
 *       top=0, bottom=2, left=0, right=0
 *       ① 向右：收集 1，top++ → top=1
 *       ② 向下：收集 2, 3，right-- → right=-1
 *       ④ 向上：此时 left=0 > right=-1，如果不判断会重复收集！
 *
 *     所以向左必须检查 top <= bottom，向上必须检查 left <= right。
 *
 * 时间复杂度：O(m * n)  —— 每个元素恰好访问一次
 * 空间复杂度：O(1)      —— 不计输出数组，只用常数变量
 */
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int m = (int)matrix.size();
        int n = (int)matrix[0].size();
        vector<int> result;
        result.reserve(m * n);

        int top = 0, bottom = m - 1;
        int left = 0, right = n - 1;

        while (top <= bottom && left <= right)
        {
            // ① 向右：沿 top 行，从 left 到 right
            for (int j = left; j <= right; ++j)
            {
                result.push_back(matrix[top][j]);
            }
            top++;

            // ② 向下：沿 right 列，从 top 到 bottom
            for (int i = top; i <= bottom; ++i)
            {
                result.push_back(matrix[i][right]);
            }
            right--;

            // ③ 向左：沿 bottom 行，从 right 到 left（需确认还有行未处理）
            if (top <= bottom)
            {
                for (int j = right; j >= left; --j)
                {
                    result.push_back(matrix[bottom][j]);
                }
                bottom--;
            }

            // ④ 向上：沿 left 列，从 bottom 到 top（需确认还有列未处理）
            if (left <= right)
            {
                for (int i = bottom; i >= top; --i)
                {
                    result.push_back(matrix[i][left]);
                }
                left++;
            }
        }

        return result;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：方向数组模拟（Direction Simulation）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   模拟一个"指针"在矩阵上走路：
 *     - 按 右 → 下 → 左 → 上 的顺序循环前进
 *     - 碰到边界或已访问的格子就顺时针转向
 *     - 用 visited 矩阵标记已走过的格子
 *     - 走满 m*n 步即结束
 *
 *   方向数组：
 *     dirs = {{0,1}, {1,0}, {0,-1}, {-1,0}}
 *              右      下      左       上
 *     di = 0（初始向右），转向时 di = (di + 1) % 4
 *
 * 二、图解
 *
 *   matrix =
 *     [1, 2, 3]
 *     [4, 5, 6]
 *     [7, 8, 9]
 *
 *   指针从 (0,0) 出发，方向 = 右：
 *
 *     → → →
 *     (0,0)=1, (0,1)=2, (0,2)=3
 *     下一步 (0,3) 越界 → 转向"下"
 *           ↓
 *     (1,2)=6, (2,2)=9
 *     下一步 (3,2) 越界 → 转向"左"
 *         ← ←
 *     (2,1)=8, (2,0)=7
 *     下一步 (2,-1) 越界 → 转向"上"
 *     ↑
 *     (1,0)=4
 *     下一步 (0,0) 已访问 → 转向"右"
 *       →
 *     (1,1)=5
 *     共 9 步，结束
 *
 *   结果：[1, 2, 3, 6, 9, 8, 7, 4, 5] ✅
 *
 * 三、对比
 *
 *   - 解法一（逐层剥离）：逻辑清晰，四个边界变量控制，不需要额外标记数组
 *   - 解法二（方向模拟）：更通用的模拟思路，好理解但需要 O(m*n) 的 visited 数组
 *
 * 时间复杂度：O(m * n)  —— 每个元素恰好访问一次
 * 空间复杂度：O(m * n)  —— visited 标记数组
 */
class Solution2 {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int m = (int)matrix.size();
        int n = (int)matrix[0].size();
        int total = m * n;

        vector<int> result;
        result.reserve(total);

        // visited 标记矩阵
        vector<vector<bool>> visited(m, vector<bool>(n, false));

        // 方向数组：右、下、左、上
        int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        int di = 0; // 当前方向索引，0=右

        int row = 0, col = 0;

        for (int step = 0; step < total; ++step)
        {
            result.push_back(matrix[row][col]);
            visited[row][col] = true;

            // 试探下一步
            int nextRow = row + dirs[di][0];
            int nextCol = col + dirs[di][1];

            // 越界或已访问 → 顺时针转向
            if (nextRow < 0 || nextRow >= m ||
                nextCol < 0 || nextCol >= n ||
                visited[nextRow][nextCol])
            {
                di = (di + 1) % 4;
                nextRow = row + dirs[di][0];
                nextCol = col + dirs[di][1];
            }

            row = nextRow;
            col = nextCol;
        }

        return result;
    }
};
