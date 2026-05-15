/*
 * 238. Product of Array Except Self
 * https://leetcode.com/problems/product-of-array-except-self/
 *
 * 给你一个整数数组 nums，返回数组 answer，其中 answer[i] 等于 nums 中除 nums[i]
 * 之外其余各元素的乘积。不能使用除法，且要求 O(n) 时间复杂度。
 *
 * ──────────────────────────────────────────────
 * 解法一：暴力枚举（Brute Force）
 * ──────────────────────────────────────────────
 * 思路：
 *   对于每个位置 i，遍历数组中所有其他位置 j (j != i)，
 *   把它们的值相乘，即为 answer[i]。
 *
 * 图解：nums = [1, 2, 3, 4]
 *
 *   i=0: 跳过 nums[0]=1, 乘积 = 2 * 3 * 4 = 24
 *   i=1: 跳过 nums[1]=2, 乘积 = 1 * 3 * 4 = 12
 *   i=2: 跳过 nums[2]=3, 乘积 = 1 * 2 * 4 =  8
 *   i=3: 跳过 nums[3]=4, 乘积 = 1 * 2 * 3 =  6
 *
 *   answer = [24, 12, 8, 6] ✅
 *
 * 时间复杂度：O(n^2)  —— 双重循环
 * 空间复杂度：O(1)    —— 不计输出数组
 */
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = (int)nums.size();
        vector<int> answer(n, 1);

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (j != i)
                {
                    answer[i] *= nums[j];
                }
            }
        }

        return answer;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：前缀积 + 后缀积（Prefix & Suffix Products）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   answer[i] = "i 左边所有元素的乘积" × "i 右边所有元素的乘积"
 *
 *   定义：
 *     prefix[i] = nums[0] * nums[1] * ... * nums[i-1]   （i 左边所有元素的乘积）
 *     suffix[i] = nums[i+1] * nums[i+2] * ... * nums[n-1] （i 右边所有元素的乘积）
 *
 *   则：answer[i] = prefix[i] * suffix[i]
 *
 *   用两个数组分别预计算前缀积和后缀积，最后逐位相乘。
 *
 * 二、图解
 *
 *   nums = [1, 2, 3, 4]
 *
 *   前缀积（从左到右累乘，prefix[0] = 1 表示左边没有元素）：
 *     prefix[0] = 1                    （左边无元素）
 *     prefix[1] = 1                    （= nums[0]）
 *     prefix[2] = 1 * 2 = 2           （= nums[0] * nums[1]）
 *     prefix[3] = 1 * 2 * 3 = 6       （= nums[0] * nums[1] * nums[2]）
 *     prefix = [1, 1, 2, 6]
 *
 *   后缀积（从右到左累乘，suffix[n-1] = 1 表示右边没有元素）：
 *     suffix[3] = 1                    （右边无元素）
 *     suffix[2] = 4                    （= nums[3]）
 *     suffix[1] = 4 * 3 = 12          （= nums[3] * nums[2]）
 *     suffix[0] = 4 * 3 * 2 = 24      （= nums[3] * nums[2] * nums[1]）
 *     suffix = [24, 12, 4, 1]
 *
 *   answer[i] = prefix[i] * suffix[i]：
 *     answer[0] = 1  * 24 = 24
 *     answer[1] = 1  * 12 = 12
 *     answer[2] = 2  *  4 =  8
 *     answer[3] = 6  *  1 =  6
 *
 *   answer = [24, 12, 8, 6] ✅
 *
 * 时间复杂度：O(n)  —— 三次遍历（前缀积 + 后缀积 + 合并）
 * 空间复杂度：O(n)  —— 需要 prefix 和 suffix 两个额外数组
 */
class Solution2 {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = (int)nums.size();

        // 构建前缀积：prefix[i] = nums[0] * ... * nums[i-1]
        vector<int> prefix(n, 1);
        for (int i = 1; i < n; ++i)
        {
            prefix[i] = prefix[i - 1] * nums[i - 1];
        }

        // 构建后缀积：suffix[i] = nums[i+1] * ... * nums[n-1]
        vector<int> suffix(n, 1);
        for (int i = n - 2; i >= 0; --i)
        {
            suffix[i] = suffix[i + 1] * nums[i + 1];
        }

        // 合并：answer[i] = prefix[i] * suffix[i]
        vector<int> answer(n);
        for (int i = 0; i < n; ++i)
        {
            answer[i] = prefix[i] * suffix[i];
        }

        return answer;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：空间优化 —— 单数组 + 滚动后缀（Optimized）
 * ──────────────────────────────────────────────
 *
 * 一、核心思想
 *
 *   解法二需要 prefix 和 suffix 两个额外数组。
 *   观察到 answer 数组本身可以先存前缀积，再用一个变量滚动地乘上后缀积，
 *   从而省掉额外数组，空间降到 O(1)（不计输出）。
 *
 *   第一遍（从左到右）：answer[i] 存储前缀积 prefix[i]
 *   第二遍（从右到左）：用变量 suffixProd 滚动累乘后缀，
 *                       answer[i] *= suffixProd 即完成合并
 *
 * 二、图解
 *
 *   nums = [1, 2, 3, 4]
 *
 *   第一遍：从左到右，answer 存前缀积
 *     answer[0] = 1                        （左边无元素）
 *     answer[1] = answer[0] * nums[0] = 1  （= 1）
 *     answer[2] = answer[1] * nums[1] = 2  （= 1*2）
 *     answer[3] = answer[2] * nums[2] = 6  （= 1*2*3）
 *     answer = [1, 1, 2, 6]   ← 此时等于 prefix 数组
 *
 *   第二遍：从右到左，suffixProd 滚动累乘后缀
 *     初始 suffixProd = 1
 *
 *     i=3: answer[3] = 6 * 1 = 6,   suffixProd = 1 * nums[3] = 4
 *     i=2: answer[2] = 2 * 4 = 8,   suffixProd = 4 * nums[2] = 12
 *     i=1: answer[1] = 1 * 12 = 12, suffixProd = 12 * nums[1] = 24
 *     i=0: answer[0] = 1 * 24 = 24, suffixProd = 24 * nums[0] = 24
 *
 *   answer = [24, 12, 8, 6] ✅
 *
 * 三、对比
 *
 *   - 解法一 O(n^2) 暴力，太慢
 *   - 解法二 O(n) 时间但需要 O(n) 额外空间（两个辅助数组）
 *   - 解法三 O(n) 时间 + O(1) 额外空间（不计输出数组），是最优解
 *
 * 时间复杂度：O(n)  —— 两次遍历，各 O(n)
 * 空间复杂度：O(1)  —— 不计输出数组，只用一个变量 suffixProd
 */
class Solution3 {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = (int)nums.size();
        vector<int> answer(n, 1);

        // 第一遍：从左到右，answer[i] 存储前缀积
        for (int i = 1; i < n; ++i)
        {
            answer[i] = answer[i - 1] * nums[i - 1];
        }

        // 第二遍：从右到左，用 suffixProd 滚动累乘后缀积
        int suffixProd = 1;
        for (int i = n - 1; i >= 0; --i)
        {
            answer[i] *= suffixProd;       // 前缀积 × 后缀积
            suffixProd *= nums[i];         // 更新后缀积供下一轮使用
        }

        return answer;
    }
};
