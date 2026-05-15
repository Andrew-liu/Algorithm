/*
 * 128. Longest Consecutive Sequence
 * https://leetcode.com/problems/longest-consecutive-sequence/
 *
 * 给定未排序的整数数组 nums，返回最长连续数字序列的长度。
 * 要求时间复杂度 O(n)。
 *
 * ──────────────────────────────────────────────
 * 解法一：HashSet + 找序列起点
 * ──────────────────────────────────────────────
 * 思路：
 *   将所有数放入 unordered_set，去重的同时支持 O(1) 查找。
 *   遍历 set，对每个数 num 判断 num-1 是否在 set 中：
 *     - 若 num-1 存在 → num 不是序列起点，跳过（避免重复计算）
 *     - 若 num-1 不存在 → num 是某段连续序列的起点，
 *       从 num 开始向右逐一查找 num+1, num+2 …，统计长度。
 *   取所有序列中的最大长度。
 *
 * 示例：nums = [100, 4, 200, 1, 3, 2]
 *   numSet = {100, 4, 200, 1, 3, 2}
 *
 *   遍历：
 *     100: 99∉set → 起点 → 101? ✗ → 长度 1
 *       4:  3∈set → 跳过
 *     200: 199∉set → 起点 → 201? ✗ → 长度 1
 *       1:  0∉set → 起点 → 2✓→3✓→4✓→5? ✗ → 长度 4 ✅
 *       3:  2∈set → 跳过
 *       2:  1∈set → 跳过
 *
 *   结果：4
 *
 * 时间复杂度：O(n)  —— 每个数最多被访问两次（作为起点 + 向右延伸）
 * 空间复杂度：O(n)  —— set 存储
 */
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> numSet(nums.begin(), nums.end());
        int longest = 0;

        for (int num : numSet)
        {
            // 只从序列起点开始计数
            if (numSet.find(num - 1) == numSet.end())
            {
                int currentNum = num;
                int currentLen = 1;

                while (numSet.find(currentNum + 1) != numSet.end())
                {
                    currentNum++;
                    currentLen++;
                }

                longest = max(longest, currentLen);
            }
        }

        return longest;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：排序 + 双指针（滑动窗口）
 * ──────────────────────────────────────────────
 * 思路：
 *   先对数组排序，连续数字必然相邻。
 *   用 fast 指针向右扫描：
 *     - 若 nums[fast] == nums[fast-1]    → 重复元素，跳过
 *     - 若 nums[fast] == nums[fast-1]+1  → 连续，currentLen++
 *     - 否则                             → 序列断开，currentLen 重置为 1
 *   每步更新最大长度。
 *
 * 示例：nums = [100, 4, 200, 1, 3, 2]
 *   排序后：[1, 2, 3, 4, 100, 200]
 *
 *   fast=1: 2=1+1 → len=2
 *   fast=2: 3=2+1 → len=3
 *   fast=3: 4=3+1 → len=4 ✅
 *   fast=4: 100≠4+1 → len 重置=1
 *   fast=5: 200≠100+1 → len 重置=1
 *
 *   结果：4
 *
 * 时间复杂度：O(n log n)  —— 排序主导
 * 空间复杂度：O(1) 或 O(n)（取决于排序实现，原地排序为 O(1)）
 *
 * 对比解法一：不满足题目 O(n) 要求，但空间占用更小，实现直观。
 */
class Solution2 {
public:
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) return 0;

        sort(nums.begin(), nums.end());

        int longest = 1;
        int currentLen = 1;

        for (int fast = 1; fast < (int)nums.size(); ++fast)
        {
            // 跳过重复元素
            if (nums[fast] == nums[fast - 1])
                continue;

            // 连续：延伸当前序列
            if (nums[fast] == nums[fast - 1] + 1)
            {
                currentLen++;
                longest = max(longest, currentLen);
            }
            else
            {
                // 序列断开，重置计数器
                currentLen = 1;
            }
        }

        return longest;
    }
};
