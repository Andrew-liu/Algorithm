/*
 * 15. 3Sum
 * https://leetcode.com/problems/3sum/
 *
 * 给定整数数组 nums，判断是否存在三元组 [nums[i], nums[j], nums[k]]
 * 满足 i != j、i != k 且 j != k，同时 nums[i] + nums[j] + nums[k] == 0。
 * 返回所有和为 0 且不重复的三元组。
 *
 * ──────────────────────────────────────────────
 * 解法一：暴力枚举（Brute Force）
 * ──────────────────────────────────────────────
 * 思路：
 *   三重循环枚举所有 (i, j, k) 三元组，判断 nums[i]+nums[j]+nums[k]==0。
 *   为了去重，先对数组排序，将每个满足条件的三元组以有序形式存入 set。
 *
 * 示例：nums = [-1, 0, 1, 2, -1, -4]
 *   排序后：[-4, -1, -1, 0, 1, 2]
 *   i=0, j=1, k=5: -4+(-1)+2 = -3  ✗
 *   ...
 *   i=1, j=2, k=4: -1+(-1)+2 =  0  ✅ → [-1,-1,2]
 *   i=1, j=3, k=4: -1+0+1    =  0  ✅ → [-1,0,1]
 *   ...
 *   用 set 自动去除重复三元组。
 *
 * 时间复杂度：O(n^3)  —— 三重循环
 * 空间复杂度：O(n)    —— set 存储去重结果
 */
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int n = (int)nums.size();
        sort(nums.begin(), nums.end());
        set<vector<int>> resSet;

        for (int i = 0; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                for (int k = j + 1; k < n; ++k)
                {
                    if (nums[i] + nums[j] + nums[k] == 0)
                    {
                        resSet.insert({nums[i], nums[j], nums[k]});
                    }
                }
            }
        }

        return vector<vector<int>>(resSet.begin(), resSet.end());
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：排序 + 双指针（Sort + Two Pointers）
 * ──────────────────────────────────────────────
 * 思路：
 *   将三数之和问题转化为：固定一个数 nums[i]，在其右侧区间用双指针找两数之和 = -nums[i]。
 *
 *   步骤：
 *   1. 先对数组排序（升序）
 *   2. 遍历 i 从 0 到 n-3：
 *      - 若 nums[i] > 0，三数之和不可能为 0，直接 break
 *      - 跳过重复的 nums[i]（去重第一个数）
 *      - 令 left = i+1, right = n-1，在 [left, right] 区间内双指针查找：
 *        · sum = nums[i] + nums[left] + nums[right]
 *        · sum < 0 → left++（需要更大的数）
 *        · sum > 0 → right--（需要更小的数）
 *        · sum == 0 → 记录结果，同时跳过 left/right 的重复值（去重第二、三个数）
 *
 *   去重原理（排序后相同值相邻）：
 *     - 第一层：nums[i] == nums[i-1] 时跳过，避免固定相同的第一个数
 *     - 第二层：找到解后，left 右移跳过相同值，right 左移跳过相同值
 *
 * 图解：nums = [-1, 0, 1, 2, -1, -4]
 *   排序后：[-4, -1, -1, 0, 1, 2]
 *            0    1    2   3  4  5
 *
 *   i=0, nums[i]=-4, 需要 nums[left]+nums[right] = -(-4) = 4:
 *     left=1, right=5: sum=-4+(-1)+2=-3 < 0  → left++
 *     left=2, right=5: sum=-4+(-1)+2=-3 < 0  → left++
 *     left=3, right=5: sum=-4+0+2  =-2 < 0   → left++
 *     left=4, right=5: sum=-4+1+2  =-1 < 0   → left++
 *     left >= right → 下一个 i
 *
 *   i=1, nums[i]=-1, 需要 nums[left]+nums[right] = -(-1) = 1:
 *     left=2, right=5: sum=-1+(-1)+2=0 == 0 ✅ → 记录 [-1,-1,2], left++, right--
 *     left=3, right=4: sum=-1+0+1  =0 == 0 ✅ → 记录 [-1,0,1],  left++, right--
 *     left >= right → 下一个 i
 *
 *   i=2, nums[i]=-1, 与 nums[1] 相同 → 跳过（去重）
 *
 *   i=3, nums[i]=0, 需要 nums[left]+nums[right] = 0:
 *     left=4, right=5: sum=0+1+2=3 > 0  → right--
 *     left >= right → 下一个 i
 *
 *   i=4, 剩余不足 3 个数 → 结束
 *
 *   结果：[[-1,-1,2], [-1,0,1]]  ✅
 *
 * 时间复杂度：O(n^2)  —— 排序 O(nlogn) + 外层循环 O(n) × 双指针 O(n)
 * 空间复杂度：O(1)    —— 排序的栈空间外，只用常数变量（不计输出）
 */
class Solution2 {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int n = (int)nums.size();
        sort(nums.begin(), nums.end());
        vector<vector<int>> res;

        for (int i = 0; i < n - 2; ++i)
        {
            // 最小值已 > 0，后面不可能凑出和为 0
            if (nums[i] > 0)
            {
                break;
            }

            // 去重：跳过相同的第一个数
            if (i > 0 && nums[i] == nums[i - 1])
            {
                continue;
            }

            int left = i + 1;
            int right = n - 1;

            while (left < right)
            {
                int sum = nums[i] + nums[left] + nums[right];

                if (sum < 0)
                {
                    left++;
                }
                else if (sum > 0)
                {
                    right--;
                }
                else
                {
                    // 找到一组解
                    res.push_back({nums[i], nums[left], nums[right]});

                    // 去重：跳过相同的第二个数
                    while (left < right && nums[left] == nums[left + 1])
                    {
                        left++;
                    }
                    // 去重：跳过相同的第三个数
                    while (left < right && nums[right] == nums[right - 1])
                    {
                        right--;
                    }

                    // 双指针同时收缩
                    left++;
                    right--;
                }
            }
        }

        return res;
    }
};
