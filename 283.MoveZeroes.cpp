/*
 * 283. Move Zeroes
 * https://leetcode.com/problems/move-zeroes/
 *
 * 将数组中所有 0 移到末尾，非零元素保持相对顺序，原地操作不能复制数组。
 *
 * ──────────────────────────────────────────────
 * 解法一：双指针 · 覆盖 + 补零
 * ──────────────────────────────────────────────
 * 思路：
 *   slow 指针记录"下一个非零元素应该放置的位置"。
 *   fast 指针从左向右扫描全数组：
 *     - 遇到非零元素 → 写到 slow 位置，slow++
 *     - 遇到 0      → 跳过
 *   fast 扫完后，slow 左侧已全是非零元素（顺序不变），
 *   将 slow ~ n-1 全部填 0 即完成。
 *
 * 图解：nums = [0, 1, 0, 3, 12]
 *
 *   slow=0
 *   fast=0: nums[0]=0  → 跳过
 *   fast=1: nums[1]=1  → nums[slow=0]=1, slow=1   → [1, 1, 0, 3, 12]
 *   fast=2: nums[2]=0  → 跳过
 *   fast=3: nums[3]=3  → nums[slow=1]=3, slow=2   → [1, 3, 0, 3, 12]
 *   fast=4: nums[4]=12 → nums[slow=2]=12, slow=3  → [1, 3, 12, 3, 12]
 *
 *   补零：nums[3]=0, nums[4]=0                    → [1, 3, 12, 0, 0] ✅
 *
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int slow = 0;

        // 第一步：把所有非零元素依次移到前面
        for (int fast = 0; fast < (int)nums.size(); ++fast)
        {
            if (nums[fast] != 0)
            {
                nums[slow++] = nums[fast];
            }
        }

        // 第二步：剩余位置全部填 0
        for (int i = slow; i < (int)nums.size(); ++i)
        {
            nums[i] = 0;
        }
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：双指针 · 原地交换（swap）
 * ──────────────────────────────────────────────
 * 思路：
 *   slow 始终指向当前最左边的 0 的位置（下一个非零元素的目标位置）。
 *   fast 向右扫描，遇到非零元素就与 slow 交换，slow 右移。
 *
 *   交换的含义：把非零元素"冒泡"到前面，0 自然沉到后面。
 *   slow 左侧（不含）始终全是非零元素，slow ~ fast 之间全是 0。
 *
 * 图解：nums = [0, 1, 0, 3, 12]
 *
 *   slow=0
 *   fast=0: nums[0]=0  → 非零? ✗ 不交换
 *   fast=1: nums[1]=1  → swap(nums[0], nums[1]) → [1, 0, 0, 3, 12], slow=1
 *   fast=2: nums[2]=0  → 非零? ✗ 不交换
 *   fast=3: nums[3]=3  → swap(nums[1], nums[3]) → [1, 3, 0, 0, 12], slow=2
 *   fast=4: nums[4]=12 → swap(nums[2], nums[4]) → [1, 3, 12, 0, 0], slow=3  ✅
 *
 * 对比解法一：
 *   - 解法一会覆盖写，最后再补 0，写操作略多（非零元素个数 + 零的个数）
 *   - 解法二每次交换只发生在"fast 遇到非零"时，写操作 = 2 × 非零元素个数
 *   - 当数组中 0 很少时，解法二更优；0 很多时两者差距不大
 *
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
class Solution2 {
public:
    void moveZeroes(vector<int>& nums) {
        int slow = 0; // 指向下一个 0 应该被换走的位置

        for (int fast = 0; fast < (int)nums.size(); ++fast)
        {
            if (nums[fast] != 0)
            {
                swap(nums[slow], nums[fast]);
                slow++;
            }
        }
    }
};
