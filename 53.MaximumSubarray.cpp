/*
 * 53. Maximum Subarray
 * https://leetcode.com/problems/maximum-subarray/
 *
 * 给你一个整数数组 nums，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），
 * 返回其最大和。
 *
 * ──────────────────────────────────────────────
 * 解法一：暴力枚举（Brute Force）
 * ──────────────────────────────────────────────
 * 思路：
 *   双重循环枚举所有子数组的起点 i 和终点 j，
 *   内层循环累加求和，维护全局最大值。
 *
 * 示例：nums = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
 *   i=0: -2, -1, -4, 0, -1, 1, 2, -3, 1   → 本轮最大 2
 *   i=1:  1, -2,  2,  1, 3, 4, -1,  3      → 本轮最大 4
 *   i=2: -3,  1,  0,  2, 3, -2,  2         → 本轮最大 3
 *   i=3:  4,  3,  5,  6,  1,  5            → 本轮最大 6 ← 全局最大
 *   ...
 *   结果：6（子数组 [4, -1, 2, 1]）
 *
 * 时间复杂度：O(n^2)  —— 双重循环
 * 空间复杂度：O(1)    —— 只用常数变量
 */
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int n = (int)nums.size();
        int maxSum = INT_MIN;

        for (int i = 0; i < n; ++i)
        {
            int sum = 0;
            for (int j = i; j < n; ++j)
            {
                sum += nums[j];
                maxSum = max(maxSum, sum);
            }
        }

        return maxSum;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：动态规划 / Kadane 算法（Dynamic Programming）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   对于每个位置 j，我们只需要做一个决策：
 *     "以 nums[j] 结尾的最大子数组，是把 nums[j] 接在前面的子数组后面，
 *      还是从 nums[j] 自己重新开始？"
 *
 *   定义 dp[j] = 以 nums[j] 结尾的最大子数组和
 *
 *   转移方程：
 *     dp[j] = max(dp[j-1] + nums[j],   ← 接在前面（前面的子数组和 + 当前值）
 *                 nums[j])              ← 重新开始（前面的子数组和为负数，还不如丢掉）
 *
 *   等价于：
 *     dp[j] = max(dp[j-1], 0) + nums[j]
 *     如果 dp[j-1] > 0：前面有正收益，接上
 *     如果 dp[j-1] <= 0：前面是累赘，丢掉，从自己开始
 *
 *   答案 = max(dp[0], dp[1], ..., dp[n-1])
 *
 * 二、为什么只需要看 dp[j-1]，不需要看更前面？
 *
 *   因为 dp[j-1] 已经是 "以 nums[j-1] 结尾的最优解"，
 *   它已经包含了所有以 nums[j-1] 结尾的可能性中最好的。
 *   如果连最好的前缀都是负数，那更前面的所有可能性一定也不值得接。
 *
 * 三、空间优化
 *
 *   dp[j] 只依赖于 dp[j-1]，不需要数组，一个变量 curSum 即可。
 *   curSum = max(curSum, 0) + nums[j]
 *     等价于：if (curSum < 0) curSum = 0;  curSum += nums[j];
 *
 * 图解：nums = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
 *
 *   j=0: nums[j]=-2
 *        curSum = max(0, 0) + (-2) = -2    （无前缀，只能选自己）
 *        maxSum = max(INT_MIN, -2) = -2
 *
 *   j=1: nums[j]=1
 *        curSum=-2 < 0，前面是累赘，丢掉
 *        curSum = max(-2, 0) + 1 = 1       （从 1 重新开始）
 *        maxSum = max(-2, 1) = 1
 *
 *   j=2: nums[j]=-3
 *        curSum=1 > 0，前面有正收益，接上
 *        curSum = max(1, 0) + (-3) = -2    （[1, -3]，虽然变负了但先记着）
 *        maxSum = max(1, -2) = 1
 *
 *   j=3: nums[j]=4
 *        curSum=-2 < 0，前面是累赘，丢掉
 *        curSum = max(-2, 0) + 4 = 4       （从 4 重新开始）★
 *        maxSum = max(1, 4) = 4
 *
 *   j=4: nums[j]=-1
 *        curSum=4 > 0，接上
 *        curSum = max(4, 0) + (-1) = 3     （[4, -1]）
 *        maxSum = max(4, 3) = 4
 *
 *   j=5: nums[j]=2
 *        curSum=3 > 0，接上
 *        curSum = max(3, 0) + 2 = 5        （[4, -1, 2]）
 *        maxSum = max(4, 5) = 5
 *
 *   j=6: nums[j]=1
 *        curSum=5 > 0，接上
 *        curSum = max(5, 0) + 1 = 6        （[4, -1, 2, 1]）★ 最大子数组
 *        maxSum = max(5, 6) = 6             ← 全局最大
 *
 *   j=7: nums[j]=-5
 *        curSum=6 > 0，接上
 *        curSum = max(6, 0) + (-5) = 1     （[4, -1, 2, 1, -5]）
 *        maxSum = max(6, 1) = 6
 *
 *   j=8: nums[j]=4
 *        curSum=1 > 0，接上
 *        curSum = max(1, 0) + 4 = 5        （[4, -1, 2, 1, -5, 4]）
 *        maxSum = max(6, 5) = 6
 *
 *   结果：maxSum = 6 ✅（子数组 [4, -1, 2, 1]，从 j=3 到 j=6）
 *
 * 时间复杂度：O(n)  —— 遍历一次数组
 * 空间复杂度：O(1)  —— 只用两个变量 curSum 和 maxSum
 */
class Solution2 {
public:
    int maxSubArray(vector<int>& nums) {
        int n = (int)nums.size();
        int curSum = 0;
        int maxSum = INT_MIN;

        for (int j = 0; j < n; ++j)
        {
            // 前面的累计和为负数，丢掉，从当前元素重新开始
            // 前面的累计和为正数，接上，继续累加
            curSum = max(curSum, 0) + nums[j];

            maxSum = max(maxSum, curSum);
        }

        return maxSum;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：前缀和思路（Prefix Sum）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   子数组 [i, j] 的和 = prefix[j+1] - prefix[i]
 *   要让这个值最大，prefix[j+1] 固定时，prefix[i] 越小越好。
 *
 *   所以：遍历过程中维护 "之前见过的最小前缀和"（minPrefix），
 *   当前前缀和 curSum - minPrefix 就是以当前位置结尾的最大子数组和。
 *
 *   与 Kadane 本质相同，只是换了个视角：
 *     Kadane："前面的子数组和为负就丢掉"
 *     前缀和："从前缀和最低点到当前位置，就是最大子数组"
 *
 * 图解：nums = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
 *
 *   prefix:  0  -2  -1  -4  0  -1  1  2  -3  1
 *            ↑                              ↑
 *            p0                             p9
 *
 *   画成折线图：
 *
 *       2 ·                          ·(p7)
 *       1 ·               ·(p6)           ·(p9)
 *       0 ·(p0)          ·(p4)
 *      -1 ·   ·(p2)          ·(p5)
 *      -2 · ·(p1)                          ·(p8)
 *      -3 ·
 *      -4 ·      ·(p3)
 *          ────────────────────────────────────
 *          p0  p1  p2  p3  p4  p5  p6  p7  p8  p9
 *
 *   最大子数组和 = 折线图上 "最低点到之后最高点" 的落差
 *   最低点 p3 = -4，之后最高点 p7 = 2，落差 = 2 - (-4) = 6 ✅
 *   对应子数组 [3, 6]：nums[3]+nums[4]+nums[5]+nums[6] = 4+(-1)+2+1 = 6
 *
 *   逐步模拟：
 *   j=0: curSum=-2, minPrefix=0,  -2-0  =-2, maxSum=-2
 *   j=1: curSum=-1, minPrefix=-2, -1-(-2)= 1, maxSum=1
 *   j=2: curSum=-4, minPrefix=-2, -4-(-2)=-2, maxSum=1
 *   j=3: curSum= 0, minPrefix=-4,  0-(-4)= 4, maxSum=4
 *   j=4: curSum=-1, minPrefix=-4, -1-(-4)= 3, maxSum=4
 *   j=5: curSum= 1, minPrefix=-4,  1-(-4)= 5, maxSum=5
 *   j=6: curSum= 2, minPrefix=-4,  2-(-4)= 6, maxSum=6  ← 最大
 *   j=7: curSum=-3, minPrefix=-4, -3-(-4)= 1, maxSum=6
 *   j=8: curSum= 1, minPrefix=-4,  1-(-4)= 5, maxSum=6
 *
 *   结果：maxSum = 6 ✅
 *
 * 时间复杂度：O(n)  —— 遍历一次数组
 * 空间复杂度：O(1)  —— 只用常数变量
 */
class Solution3 {
public:
    int maxSubArray(vector<int>& nums) {
        int n = (int)nums.size();
        int curSum = 0;       // 当前前缀和 prefix[j+1]
        int minPrefix = 0;    // 之前见过的最小前缀和 min(prefix[0..j])
        int maxSum = INT_MIN;

        for (int j = 0; j < n; ++j)
        {
            curSum += nums[j];

            // 当前前缀和 - 历史最小前缀和 = 以当前位置结尾的最大子数组和
            maxSum = max(maxSum, curSum - minPrefix);

            // 更新历史最小前缀和（供后续位置使用，所以放在 maxSum 更新之后）
            minPrefix = min(minPrefix, curSum);
        }

        return maxSum;
    }
};
