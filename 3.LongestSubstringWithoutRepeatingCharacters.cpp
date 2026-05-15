/*
 * 3. Longest Substring Without Repeating Characters
 * https://leetcode.com/problems/longest-substring-without-repeating-characters/
 *
 * 给定一个字符串 s，请你找出其中不含有重复字符的最长子串的长度。
 *
 * ──────────────────────────────────────────────
 * 解法一：暴力枚举（Brute Force）
 * ──────────────────────────────────────────────
 * 思路：
 *   双重循环枚举所有子串的起点 i 和终点 j，
 *   用 set 检测子串 [i, j] 内是否有重复字符。
 *   遇到重复则 break，否则更新最大长度。
 *
 * 示例：s = "abcabcbb"
 *   i=0: "a"→1  "ab"→2  "abc"→3  "abca"→重复 break  → 本轮最长 3
 *   i=1: "b"→1  "bc"→2  "bca"→3  "bcab"→重复 break  → 本轮最长 3
 *   i=2: "c"→1  "ca"→2  "cab"→3  "cabc"→重复 break  → 本轮最长 3
 *   ...
 *   最终结果：3（"abc"）
 *
 * 时间复杂度：O(n^2)  —— 双重循环，set 操作 O(1)
 * 空间复杂度：O(n)    —— set 最多存 n 个字符
 */
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int n = (int)s.size();
        int maxLen = 0;

        for (int i = 0; i < n; ++i)
        {
            unordered_set<char> charSet;
            for (int j = i; j < n; ++j)
            {
                // 遇到重复字符，当前起点的最长无重复子串已确定
                if (charSet.count(s[j]))
                {
                    break;
                }
                charSet.insert(s[j]);
                maxLen = max(maxLen, j - i + 1);
            }
        }

        return maxLen;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：滑动窗口（Sliding Window）
 * ──────────────────────────────────────────────
 * 思路：
 *   维护一个窗口 [left, right]，保证窗口内没有重复字符。
 *   用 unordered_set 记录窗口内的字符集合。
 *
 *   步骤：
 *   1. right 不断右移，尝试扩大窗口
 *   2. 若 s[right] 已在 set 中（出现重复）：
 *      - 不断移动 left 并从 set 中移除 s[left]，直到窗口内不再包含 s[right]
 *   3. 将 s[right] 加入 set，更新最大长度
 *
 *   为什么 left 只需要往右移？
 *     窗口 [left, right-1] 无重复，现在 s[right] 与窗口内某个 s[k] 重复。
 *     要消除重复，必须把 s[k] 移出窗口，即 left 至少移到 k+1。
 *     left 往右移的过程中，窗口只会缩小，不会引入新的重复。
 *     因此 left 单调递增，整个过程 left 和 right 各最多走 n 步。
 *
 * 图解：s = "abcabcbb"
 *
 *   right=0: 'a' 不在 set → set={a},           窗口="a",      len=1
 *   right=1: 'b' 不在 set → set={a,b},         窗口="ab",     len=2
 *   right=2: 'c' 不在 set → set={a,b,c},       窗口="abc",    len=3 ← 最大
 *   right=3: 'a' 在 set 中 → 移除 s[0]='a', left=1
 *                           → set={b,c,a},      窗口="bca",    len=3
 *   right=4: 'b' 在 set 中 → 移除 s[1]='b', left=2
 *                           → set={c,a,b},      窗口="cab",    len=3
 *   right=5: 'c' 在 set 中 → 移除 s[2]='c', left=3
 *                           → set={a,b,c},      窗口="abc",    len=3
 *   right=6: 'b' 在 set 中 → 移除 s[3]='a', left=4
 *            'b' 仍在 set  → 移除 s[4]='b', left=5
 *                           → set={c,b},        窗口="cb",     len=2
 *   right=7: 'b' 在 set 中 → 移除 s[5]='c', left=6
 *            'b' 仍在 set  → 移除 s[6]='b', left=7
 *                           → set={b},          窗口="b",      len=1
 *
 *   最大长度 = 3（"abc"）✅
 *
 * 时间复杂度：O(n)  —— left 和 right 各最多遍历一次数组
 * 空间复杂度：O(n)  —— set 最多存 min(n, 字符集大小) 个字符
 */
class Solution2 {
public:
    int lengthOfLongestSubstring(string s) {
        int n = (int)s.size();
        int left = 0;
        int maxLen = 0;
        unordered_set<char> charSet;

        for (int right = 0; right < n; ++right)
        {
            // 窗口内已有 s[right]，不断收缩左边界直到消除重复
            while (charSet.count(s[right]))
            {
                charSet.erase(s[left]);
                left++;
            }

            charSet.insert(s[right]);
            maxLen = max(maxLen, right - left + 1);
        }

        return maxLen;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：滑动窗口 + HashMap 优化（Sliding Window + HashMap）
 * ──────────────────────────────────────────────
 * 思路：
 *   解法二中遇到重复时，left 需要逐步右移（一个一个删），效率不够高。
 *   优化：用 HashMap 记录每个字符最后出现的下标，
 *   遇到重复时 left 直接跳到 "重复字符上次出现位置 + 1"，一步到位。
 *
 *   步骤：
 *   1. right 不断右移
 *   2. 若 s[right] 在 map 中，且上次出现位置 >= left（在当前窗口内）：
 *      - left 直接跳到 map[s[right]] + 1
 *   3. 更新 map[s[right]] = right，更新最大长度
 *
 * 图解：s = "abcabcbb"
 *
 *   right=0: 'a' 不在窗口内 → map={a:0},         窗口="a",   len=1
 *   right=1: 'b' 不在窗口内 → map={a:0,b:1},     窗口="ab",  len=2
 *   right=2: 'c' 不在窗口内 → map={a:0,b:1,c:2}, 窗口="abc", len=3 ← 最大
 *   right=3: 'a' 在窗口内(pos=0) → left 跳到 0+1=1
 *                             map={a:3,b:1,c:2},  窗口="bca", len=3
 *   right=4: 'b' 在窗口内(pos=1) → left 跳到 1+1=2
 *                             map={a:3,b:4,c:2},  窗口="cab", len=3
 *   right=5: 'c' 在窗口内(pos=2) → left 跳到 2+1=3
 *                             map={a:3,b:4,c:5},  窗口="abc", len=3
 *   right=6: 'b' 在窗口内(pos=4) → left 跳到 4+1=5
 *                             map={a:3,b:6,c:5},  窗口="cb",  len=2
 *   right=7: 'b' 在窗口内(pos=6) → left 跳到 6+1=7
 *                             map={a:3,b:7,c:5},  窗口="b",   len=1
 *
 *   最大长度 = 3（"abc"）✅
 *
 * 时间复杂度：O(n)  —— right 遍历一次，left 每次最多跳一次（不需要逐步移动）
 * 空间复杂度：O(n)  —— HashMap 最多存 min(n, 字符集大小) 个字符
 */
class Solution3 {
public:
    int lengthOfLongestSubstring(string s) {
        int n = (int)s.size();
        int left = 0;
        int maxLen = 0;
        unordered_map<char, int> charIndex; // {字符 → 最后出现的下标}

        for (int right = 0; right < n; ++right)
        {
            // 若 s[right] 在当前窗口内出现过，left 直接跳到重复位置的下一位
            if (charIndex.count(s[right]) && charIndex[s[right]] >= left)
            {
                left = charIndex[s[right]] + 1;
            }

            charIndex[s[right]] = right;
            maxLen = max(maxLen, right - left + 1);
        }

        return maxLen;
    }
};
