/*
 * 11. Container With Most Water
 * https://leetcode.com/problems/container-with-most-water/
 *
 * 给定长度为 n 的整数数组 height，有 n 条垂线，第 i 条线的端点是 (i,0) 和 (i,height[i])。
 * 找出其中两条线，使它们与 x 轴构成的容器可以容纳最多的水，返回最大水量。
 *
 * ──────────────────────────────────────────────
 * 解法一：暴力枚举（Brute Force）
 * ──────────────────────────────────────────────
 * 思路：
 *   双重循环枚举所有 (i, j) 对，计算每对围成的面积，取最大值。
 *   面积公式：min(height[i], height[j]) * (j - i)
 *
 * 示例：height = [1,8,6,2,5,4,8,3,7]
 *   i=1, j=8: min(8,7)*(8-1) = 49 ← 最大
 *
 * 时间复杂度：O(n^2)
 * 空间复杂度：O(1)
 */
class Solution {
public:
    int maxArea(vector<int>& height) {
        int maxWater = 0;

        for (int i = 0; i < (int)height.size(); ++i)
        {
            for (int j = i + 1; j < (int)height.size(); ++j)
            {
                int w = j - i;
                int h = min(height[i], height[j]);
                maxWater = max(maxWater, w * h);
            }
        }

        return maxWater;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：双指针（Two Pointers）
 * ──────────────────────────────────────────────
 * 思路：
 *   左右指针分别从数组两端出发，每次移动较短的那一侧。
 *
 *   为什么移动短板？
 *     面积 = min(height[left], height[right]) * (right - left)
 *     向内收缩时宽度必然减 1；
 *     - 若移动较长的线：高度仍受限于短板，面积必定变小 → 不可能更优
 *     - 若移动较短的线：高度有可能增大，面积有可能变大 → 可能更优
 *     因此保留短板的所有组合都不可能超过当前面积，可以安全淘汰。
 *
 * 图解：height = [1, 8, 6, 2, 5, 4, 8, 3, 7]
 *                 0  1  2  3  4  5  6  7  8
 *
 *        8   8
 *        |   |   7
 *        | 6 |   |
 *        | | | 5 | |
 *        | | | | 4 | |
 *        | | | | | | | 3 |
 *        | | 2 | | | | | |
 *     1  | | | | | | | | |
 *     ─────────────────────
 *     0  1  2  3  4  5  6  7  8
 *
 *   left=0, right=8: min(1,7)*8 =  8  → 移动left  (1<7)
 *   left=1, right=8: min(8,7)*7 = 49  → 移动right (8>7)
 *   left=1, right=7: min(8,3)*6 = 18  → 移动right (8>3)
 *   left=1, right=6: min(8,8)*5 = 40  → 移动right (相等移任意一侧)
 *   left=1, right=5: min(8,4)*4 = 16  → 移动right (8>4)
 *   left=1, right=4: min(8,5)*3 = 15  → 移动right (8>5)
 *   left=1, right=3: min(8,2)*2 =  4  → 移动right (8>2)
 *   left=1, right=2: min(8,6)*1 =  6  → 移动right (8>6)
 *   left >= right → 退出循环
 *
 *   最大水量 = 49 ✅
 *
 * 时间复杂度：O(n)   —— 左右指针各走一遍
 * 空间复杂度：O(1)   —— 只用常数变量
 */
class Solution2 {
public:
    int maxArea(vector<int>& height) {
        int left = 0;
        int right = (int)height.size() - 1;
        int maxWater = 0;

        while (left < right)
        {
            int w = right - left;
            int h = min(height[left], height[right]);
            maxWater = max(maxWater, w * h);

            // 移动较短的那一侧
            if (height[left] < height[right])
            {
                left++;
            }
            else
            {
                right--;
            }
        }

        return maxWater;
    }
};
