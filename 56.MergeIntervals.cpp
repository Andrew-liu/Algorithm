/*
 * 56. Merge Intervals
 * https://leetcode.com/problems/merge-intervals/
 *
 * 以数组 intervals 表示若干个区间的集合，其中单个区间为 intervals[i] = [starti, endi]。
 * 请你合并所有重叠的区间，并返回一个不重叠的区间数组，该数组需恰好覆盖输入中的所有区间。
 *
 * ──────────────────────────────────────────────
 * 解法一：暴力合并（Brute Force）
 * ──────────────────────────────────────────────
 * 思路：
 *   不断扫描区间列表，每次找到两个重叠的区间就合并成一个，
 *   直到一轮扫描下来没有任何合并发生为止。
 *
 *   两个区间 A=[a1,a2]、B=[b1,b2] 重叠的条件：a1 <= b2 && b1 <= a2
 *   合并结果：[min(a1,b1), max(a2,b2)]
 *
 * 示例：intervals = [[1,3],[2,6],[8,10],[15,18]]
 *   第 1 轮：[1,3] 与 [2,6] 重叠 → 合并为 [1,6]，列表变为 [[1,6],[8,10],[15,18]]
 *   第 2 轮：没有重叠 → 结束
 *   结果：[[1,6],[8,10],[15,18]] ✅
 *
 * 时间复杂度：O(n^2)  —— 最坏情况每轮只合并一对，共 n-1 轮，每轮扫描 O(n)
 * 空间复杂度：O(n)    —— 存储结果
 */
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        vector<vector<int>> res(intervals.begin(), intervals.end());

        bool merged = true;
        while (merged)
        {
            merged = false;
            for (int i = 0; i < (int)res.size(); ++i)
            {
                for (int j = i + 1; j < (int)res.size(); ++j)
                {
                    // 判断是否重叠
                    if (res[i][0] <= res[j][1] && res[j][0] <= res[i][1])
                    {
                        // 合并到 res[i]
                        res[i][0] = min(res[i][0], res[j][0]);
                        res[i][1] = max(res[i][1], res[j][1]);
                        // 删除 res[j]
                        res.erase(res.begin() + j);
                        merged = true;
                        j--; // 删除后下标回退
                    }
                }
            }
        }

        return res;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：排序 + 一次遍历（Sort + Linear Scan）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   先按区间左端点排序，排序后重叠的区间一定相邻。
 *   这样只需一次遍历，逐个判断当前区间能否与结果数组的最后一个区间合并。
 *
 *   为什么排序后重叠的区间一定相邻？
 *     假设排序后 A 在 B 前面，B 在 C 前面（A.start <= B.start <= C.start）。
 *     如果 A 与 C 重叠（A.end >= C.start），
 *     那么 A.end >= C.start >= B.start，所以 A 与 B 也重叠。
 *     → 不可能出现 "A 跳过 B 直接和 C 重叠" 的情况。
 *
 * 二、合并判断
 *
 *   排序后，对于相邻两个区间 prev 和 cur：
 *     - cur.start <= prev.end → 重叠，合并：prev.end = max(prev.end, cur.end)
 *     - cur.start >  prev.end → 不重叠，prev 定型，cur 成为新的待合并区间
 *
 *   为什么只需要比较 cur.start 和 prev.end？
 *     因为排序后 cur.start >= prev.start 恒成立，
 *     所以重叠条件 prev.start <= cur.end && cur.start <= prev.end
 *     简化为 cur.start <= prev.end（另一半自动满足）。
 *
 * 图解：intervals = [[1,3],[2,6],[8,10],[15,18]]
 *
 *   排序后（本例已有序）：[[1,3],[2,6],[8,10],[15,18]]
 *
 *   数轴展示：
 *     1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
 *     [──1,3──]
 *        [─────2,6─────]
 *                          [──8,10──]
 *                                               [──15,18──]
 *
 *   合并后：
 *     [────────1,6─────]   [──8,10──]           [──15,18──]
 *
 *   逐步模拟：
 *   初始：res = [[1,3]]
 *
 *   cur=[2,6]:  cur.start=2 <= res末尾.end=3 → 重叠
 *               合并：res末尾.end = max(3, 6) = 6
 *               res = [[1,6]]
 *
 *   cur=[8,10]: cur.start=8 > res末尾.end=6 → 不重叠
 *               直接加入
 *               res = [[1,6],[8,10]]
 *
 *   cur=[15,18]: cur.start=15 > res末尾.end=10 → 不重叠
 *                直接加入
 *                res = [[1,6],[8,10],[15,18]]
 *
 *   结果：[[1,6],[8,10],[15,18]] ✅
 *
 * 补充示例：intervals = [[1,4],[0,4]]（需要排序的情况）
 *
 *   排序后：[[0,4],[1,4]]
 *
 *     0  1  2  3  4
 *     [─────0,4────]
 *        [──1,4────]
 *
 *   cur=[1,4]: cur.start=1 <= res末尾.end=4 → 重叠
 *              合并：res末尾.end = max(4, 4) = 4
 *              res = [[0,4]]
 *
 *   结果：[[0,4]] ✅
 *
 * 时间复杂度：O(n*logn)  —— 排序 O(n*logn) + 遍历 O(n)
 * 空间复杂度：O(1)       —— 排序的栈空间外，只用常数变量（不计输出）
 */
class Solution2 {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        // 按左端点排序
        sort(intervals.begin(), intervals.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 return a[0] < b[0];
             });

        vector<vector<int>> res;

        for (const auto& cur : intervals)
        {
            // 结果为空，或当前区间与 res 末尾不重叠 → 直接加入
            if (res.empty() || cur[0] > res.back()[1])
            {
                res.push_back(cur);
            }
            else
            {
                // 重叠 → 合并：扩展 res 末尾区间的右端点
                res.back()[1] = max(res.back()[1], cur[1]);
            }
        }

        return res;
    }
};
