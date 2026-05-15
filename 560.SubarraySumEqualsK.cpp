/*
 * 560. Subarray Sum Equals K
 * https://leetcode.com/problems/subarray-sum-equals-k/
 *
 * 给你一个整数数组 nums 和一个整数 k，
 * 请你统计并返回该数组中和为 k 的子数组的个数。
 * 子数组是数组中元素的连续非空序列。
 *
 * 注意：数组元素可能为负数，因此不能用滑动窗口（窗口和不具有单调性）。
 *       若题目保证元素全为正数，则可以用滑动窗口（见解法三）。
 *
 * ──────────────────────────────────────────────
 * 解法一：暴力枚举（Brute Force）
 * ──────────────────────────────────────────────
 * 思路：
 *   双重循环枚举所有子数组的起点 i 和终点 j，
 *   内层循环累加求和，判断是否等于 k。
 *
 * 示例：nums = [1, 1, 1], k = 2
 *   i=0: sum=1         (j=0, sum!=2)
 *        sum=1+1=2     (j=1, sum==2 ✅) count++
 *        sum=2+1=3     (j=2, sum!=2)
 *   i=1: sum=1         (j=1, sum!=2)
 *        sum=1+1=2     (j=2, sum==2 ✅) count++
 *   i=2: sum=1         (j=2, sum!=2)
 *   结果：count = 2 ✅
 *
 * 时间复杂度：O(n^2)  —— 双重循环
 * 空间复杂度：O(1)    —— 只用常数变量
 */
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = (int)nums.size();
        int count = 0;

        for (int i = 0; i < n; ++i)
        {
            int sum = 0;
            for (int j = i; j < n; ++j)
            {
                sum += nums[j];
                if (sum == k)
                {
                    count++;
                }
            }
        }

        return count;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：前缀和 + HashMap（Prefix Sum + HashMap）
 * ──────────────────────────────────────────────
 *
 * 一、什么是前缀和？
 *
 *   前缀和就是 "从头累加到第 i 个元素的总和"：
 *     prefix[0] = 0                           （空前缀，一个数都不加）
 *     prefix[1] = nums[0]
 *     prefix[2] = nums[0] + nums[1]
 *     prefix[3] = nums[0] + nums[1] + nums[2]
 *     ...
 *
 *   示例：nums = [1, 2, 1, -1, 1, 2]
 *
 *     下标 i:    0   1   2    3   4   5
 *     nums:     1   2   1   -1   1   2
 *     prefix:  0  1   3   4    3   4   6
 *              ↑  ↑
 *              |  prefix[1] = nums[0] = 1
 *              prefix[0] = 0（空前缀）
 *
 * 二、前缀和有什么用？—— 快速求任意子数组的和
 *
 *   子数组 [i, j] 的和 = prefix[j+1] - prefix[i]
 *
 *   原理：prefix[j+1] 是前 j+1 个数的和，prefix[i] 是前 i 个数的和，
 *         两者相减，就把前 i 个数消掉了，剩下的刚好是 nums[i] 到 nums[j]。
 *
 *   图示：
 *     prefix[j+1] = nums[0] + nums[1] + ... + nums[i-1] + nums[i] + ... + nums[j]
 *     prefix[i]   = nums[0] + nums[1] + ... + nums[i-1]
 *     ────────────────────────────────────────────────────────────────────────────
 *     相减         =                                        nums[i] + ... + nums[j]
 *                                                           └──── 子数组 [i,j] ────┘
 *
 *   举例：求子数组 [1, 2] 的和（即 nums[1]+nums[2] = 2+1 = 3）
 *     prefix[3] - prefix[1] = 4 - 1 = 3  ✅
 *
 * 三、怎么用前缀和找 "和为 k 的子数组"？
 *
 *   我们要找：prefix[j+1] - prefix[i] == k
 *   移项得到：prefix[i] == prefix[j+1] - k
 *
 *   翻译成大白话：
 *     当我走到位置 j，算出当前前缀和 curSum = prefix[j+1]，
 *     我想知道：之前有没有某个位置 i，使得 prefix[i] == curSum - k？
 *     如果有，那子数组 [i, j] 的和就是 curSum - prefix[i] = k。
 *     如果这样的 i 有多个，那就有多个满足条件的子数组。
 *
 *   所以用 HashMap 记录 "每个前缀和出现了几次"，边走边查：
 *     - 查：map[curSum - k] 就是满足条件的子数组个数
 *     - 存：map[curSum]++，供后续位置查询
 *
 * 四、为什么初始化 map = {0: 1}？
 *
 *   因为 prefix[0] = 0（空前缀）。
 *   如果某个位置 j 的前缀和 curSum 恰好等于 k，
 *   那么 curSum - k = 0，需要查到 map[0] = 1，
 *   表示 "从数组开头到 j 的整段子数组和为 k"。
 *
 *   举例：nums = [3, 4, 7], k = 7
 *     i=2: curSum=14, curSum-k=7, 查 map[7] → 找子数组 [2,2]
 *     但还有一个答案：nums[0]+nums[1]+nums[2] = 3+4+7 = 14... 不对
 *     实际是 nums[2]=7 本身就是答案。
 *     更好的例子：nums = [3, 4], k = 7
 *     i=1: curSum=7, curSum-k=0, 查 map[0]=1 ✅ → 找到 [0,1]: 3+4=7
 *     如果没有 map={0:1}，这个从头开始的子数组就会被漏掉！
 *
 * 五、为什么先查询再存入？
 *
 *   保证只统计当前位置之前的前缀和，不会把自己也算进去。
 *   如果先存后查，curSum - k == curSum（即 k == 0）时会把自己匹配到自己。
 *
 * 图解：nums = [1, 2, 1, -1, 1, 2], k = 3
 *
 *   前缀和数组（方便理解，实际代码中不需要存这个数组）：
 *     prefix:  0   1   3   4   3   4   6
 *              ↑   ↑   ↑   ↑   ↑   ↑   ↑
 *              p0  p1  p2  p3  p4  p5  p6
 *
 *   下标:           0    1    2    3    4    5
 *   nums:          1    2    1   -1    1    2
 *   curSum:        1    3    4    3    4    6
 *   curSum - k:   -2    0    1    0    1    3
 *
 *                                  查询                   更新 map
 *   初始          map = {0:1}
 *   i=0: curSum=1  map[-2]=0, count=0          map={0:1, 1:1}
 *        （找不到前缀和为 -2 的位置，无解）
 *
 *   i=1: curSum=3  map[0] =1, count=0+1=1     map={0:1, 1:1, 3:1}
 *        （前缀和 0 出现在 p0，所以子数组 [0,1] 的和 = p2 - p0 = 3-0 = 3 ✅）
 *
 *   i=2: curSum=4  map[1] =1, count=1+1=2     map={0:1, 1:1, 3:1, 4:1}
 *        （前缀和 1 出现在 p1，所以子数组 [1,2] 的和 = p3 - p1 = 4-1 = 3 ✅）
 *
 *   i=3: curSum=3  map[0] =1, count=2+1=3     map={0:1, 1:1, 3:2, 4:1}
 *        （前缀和 0 出现在 p0，所以子数组 [0,3] 的和 = p4 - p0 = 3-0 = 3 ✅）
 *
 *   i=4: curSum=4  map[1] =1, count=3+1=4     map={0:1, 1:1, 3:2, 4:2}
 *        （前缀和 1 出现在 p1，所以子数组 [1,4] 的和 = p5 - p1 = 4-1 = 3 ✅）
 *
 *   i=5: curSum=6  map[3] =2, count=4+2=6     map={0:1, 1:1, 3:2, 4:2, 6:1}
 *        （前缀和 3 出现在 p2 和 p4 两个位置，对应两个子数组：
 *          子数组 [2,5] 的和 = p6 - p2 = 6-4 = ... 等等，让我们严格算：
 *          p2=3 → 子数组起点 i=2: 和 = p6 - p2 = 6-3 = 3 ✅ 即 [1,-1,1,2]
 *          p4=3 → 子数组起点 i=4: 和 = p6 - p4 = 6-3 = 3 ✅ 即 [1,2]）
 *
 *   结果：count = 6 ✅
 *
 *   为什么不能用滑动窗口？
 *     nums = [1, -1, 1], k = 1
 *     满足条件的子数组：[1]、[1,-1,1]、[1]（下标 0、0~2、2），共 3 个。
 *     由于元素有负数，窗口和的增减没有单调性：
 *     右移 right 和可能减小，右移 left 和可能增大，无法判断该移动哪边。
 *
 * 时间复杂度：O(n)  —— 遍历一次数组，HashMap 操作 O(1)
 * 空间复杂度：O(n)  —— HashMap 最多存 n+1 个前缀和
 */
class Solution2 {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = (int)nums.size();
        int count = 0;
        int curSum = 0;
        unordered_map<int, int> prefixCount; // {前缀和 → 出现次数}

        // 空前缀：前缀和为 0 出现 1 次
        prefixCount[0] = 1;

        for (int i = 0; i < n; ++i)
        {
            curSum += nums[i];

            // 查询：之前有多少个前缀和 == curSum - k
            if (prefixCount.count(curSum - k))
            {
                count += prefixCount[curSum - k];
            }

            // 存入当前前缀和
            prefixCount[curSum]++;
        }

        return count;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：滑动窗口（Sliding Window）—— 仅适用于元素全为正数
 * ──────────────────────────────────────────────
 * 思路：
 *   当所有元素 > 0 时，窗口和具有单调性：
 *     - right 右移 → 窗口和必定增大
 *     - left  右移 → 窗口和必定减小
 *   因此可以根据窗口和与 k 的关系，明确判断该移动哪边。
 *
 *   步骤：
 *   1. left = 0, sum = 0
 *   2. right 不断右移，将 nums[right] 加入 sum
 *   3. 若 sum > k：不断右移 left 并减去 nums[left]，直到 sum <= k
 *   4. 若 sum == k：找到一个子数组，count++
 *
 *   为什么原题不能用这个方法？
 *     原题元素可能为负数或零，单调性不成立：
 *     - right 右移，加入负数 → 窗口和可能减小
 *     - left  右移，减去负数 → 窗口和可能增大
 *     无法判断该移动哪边，会遗漏答案。
 *
 * 图解：nums = [1, 2, 3, 2, 1, 1], k = 5（全为正数）
 *
 *   right=0: sum+=1 → sum=1              窗口=[1]           sum<k
 *   right=1: sum+=2 → sum=3              窗口=[1,2]         sum<k
 *   right=2: sum+=3 → sum=6              窗口=[1,2,3]       sum>k
 *            移除 left=0: sum-=1 → sum=5 窗口=[2,3]         sum==k ✅ count=1
 *   right=3: sum+=2 → sum=7              窗口=[2,3,2]       sum>k
 *            移除 left=1: sum-=2 → sum=5 窗口=[3,2]         sum==k ✅ count=2
 *   right=4: sum+=1 → sum=6              窗口=[3,2,1]       sum>k
 *            移除 left=2: sum-=3 → sum=3 窗口=[2,1]         sum<k
 *   right=5: sum+=1 → sum=4              窗口=[2,1,1]       sum<k
 *
 *   结果：count = 2 ✅
 *   对应子数组：[2,3] 和 [3,2]
 *
 *   对比：如果 nums = [1, -1, 1], k = 1（含负数）
 *     滑动窗口只能找到 [1]（下标 0）和 [1]（下标 2），共 2 个。
 *     但实际还有 [1,-1,1]（下标 0~2），共 3 个。
 *     因为 right=1 时加入 -1，sum 从 1 变为 0，窗口和变小了，
 *     滑动窗口认为应该继续扩大，从而跳过了 sum==1 再次出现的时机。
 *     → 含负数时滑动窗口会漏解，必须用前缀和 + HashMap（解法二）。
 *
 * 时间复杂度：O(n)  —— left 和 right 各最多遍历一次数组
 * 空间复杂度：O(1)  —— 只用常数变量
 */
class Solution3 {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = (int)nums.size();
        int count = 0;
        int sum = 0;
        int left = 0;

        for (int right = 0; right < n; ++right)
        {
            sum += nums[right];

            // 窗口和过大，收缩左边界
            while (sum > k && left <= right)
            {
                sum -= nums[left];
                left++;
            }

            // 窗口和恰好等于 k
            if (sum == k)
            {
                count++;
            }
        }

        return count;
    }
};
