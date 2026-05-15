/*
 * 1. Two Sum
 * https://leetcode.com/problems/two-sum/
 *
 * 给定整数数组 nums 和目标值 target，返回两个数之和等于 target 的下标。
 * 每种输入只对应一个答案，且同一元素不能使用两次。
 *
 * ──────────────────────────────────────────────
 * 解法一：暴力枚举（Brute Force）
 * ──────────────────────────────────────────────
 * 思路：
 *   双重循环遍历所有下标对 (i, j)，判断 nums[i] + nums[j] == target。
 *
 * 示例：nums = [2,7,11,15], target = 9
 *   i=0, j=1: 2+7=9 ✅ → 返回 [0,1]
 *
 * 时间复杂度：O(n²)
 * 空间复杂度：O(1)
 */
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> res;
        for (int32_t i = 0; i < nums.size(); ++i)
        {
            for (int32_t j = i + 1; j < nums.size(); ++j)
            {
                if (nums[i] + nums[j] == target)
                {
                    res.push_back(i);
                    res.push_back(j);
                }
            }
        }

        return res;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：HashMap（哈希表）一次遍历
 * ──────────────────────────────────────────────
 * 思路：
 *   用 unordered_map 记录已遍历元素的 {值 → 下标}。
 *   遍历每个元素 nums[i] 时，计算其"补数" complement = target - nums[i]，
 *   若 complement 已在哈希表中，则直接返回两个下标；
 *   否则将 nums[i] 及其下标存入哈希表，继续遍历。
 *
 * 示例：nums = [2,7,11,15], target = 9
 *   i=0: complement=7, map 中没有 → 存入 {2:0}
 *   i=1: complement=2, map 中有 2 → 返回 [0,1] ✅
 *
 *   map 变化：
 *   {}  →  {2:0}  →  找到答案，返回
 *
 * 时间复杂度：O(n)   —— 只需遍历一次
 * 空间复杂度：O(n)   —— 哈希表最多存 n 个元素
 */
class Solution2 {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> mp; // {值 → 下标}

        for (int i = 0; i < (int)nums.size(); ++i)
        {
            int complement = target - nums[i];
            if (mp.count(complement))
            {
                return {mp[complement], i};
            }
            mp[nums[i]] = i;
        }

        return {}; // 题目保证有解，不会到达这里
    }
};
