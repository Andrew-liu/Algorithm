/*
 * 438. Find All Anagrams in a String
 * https://leetcode.com/problems/find-all-anagrams-in-a-string/
 *
 * 给定两个字符串 s 和 p，找到 s 中所有 p 的异位词的子串，返回这些子串的起始索引。
 * 异位词：由相同字母重新排列而成的字符串（包括相同的字符串），字母个数完全一致。
 *
 * ──────────────────────────────────────────────
 * 解法一：暴力枚举 + 排序比较（Brute Force）
 * ──────────────────────────────────────────────
 * 思路：
 *   枚举 s 中所有长度为 p.size() 的子串，
 *   对每个子串排序后与排序后的 p 比较，相等则为异位词。
 *
 * 示例：s = "cbaebabacd", p = "abc"
 *   sorted_p = "abc"
 *   i=0: "cba" → 排序 "abc" == "abc" ✅ → 记录 0
 *   i=1: "bae" → 排序 "abe" != "abc" ✗
 *   i=2: "aeb" → 排序 "abe" != "abc" ✗
 *   ...
 *   i=7: "acd" → 排序 "acd" != "abc" ✗
 *   结果不完整——实际还有 i=6 命中，但思路正确，只是慢。
 *
 * 时间复杂度：O((n-m) * m*log(m))  —— n=|s|, m=|p|, 每个子串排序 O(m*log(m))
 * 空间复杂度：O(m)                 —— 排序用的临时字符串
 */
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        int n = (int)s.size();
        int m = (int)p.size();
        vector<int> res;

        if (n < m)
        {
            return res;
        }

        string sortedP = p;
        sort(sortedP.begin(), sortedP.end());

        for (int i = 0; i <= n - m; ++i)
        {
            string sub = s.substr(i, m);
            sort(sub.begin(), sub.end());
            if (sub == sortedP)
            {
                res.push_back(i);
            }
        }

        return res;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：定长滑动窗口 + 字符频次数组（Sliding Window）
 * ──────────────────────────────────────────────
 * 思路：
 *   异位词的本质：两个字符串的字符频次完全相同。
 *   因此维护一个长度恰好为 m（= p.size()）的滑动窗口，
 *   用长度为 26 的数组统计窗口内的字符频次，与 p 的频次数组比较。
 *
 *   步骤：
 *   1. 统计 p 的字符频次 → pCount[26]
 *   2. right 从 0 开始右移，将 s[right] 加入窗口频次 sCount[26]
 *   3. 当窗口大小 > m 时，移除最左边的字符 s[left]，left++
 *   4. 当窗口大小 == m 时，比较 sCount 与 pCount，相等则记录 left
 *
 *   为什么用长度 26 的数组而不用 HashMap？
 *     题目限定小写字母，数组比较是 O(26) = O(1)，比 HashMap 更快更简洁。
 *
 * 图解：s = "cbaebabacd", p = "abc"
 *   pCount: a=1, b=1, c=1
 *
 *   right=0: 加入 'c' → sCount: c=1           窗口="c"     大小=1
 *   right=1: 加入 'b' → sCount: b=1,c=1       窗口="cb"    大小=2
 *   right=2: 加入 'a' → sCount: a=1,b=1,c=1   窗口="cba"   大小=3
 *            sCount == pCount ✅ → 记录 left=0
 *   right=3: 加入 'e', 移除 s[0]='c'
 *            → sCount: a=1,b=1,e=1             窗口="bae"   大小=3
 *            sCount != pCount ✗
 *   right=4: 加入 'b', 移除 s[1]='b'
 *            → sCount: a=1,b=1,e=1             窗口="aeb"   大小=3
 *            sCount != pCount ✗
 *   right=5: 加入 'a', 移除 s[2]='a'
 *            → sCount: a=1,b=1,e=1             窗口="eba"   大小=3
 *            sCount != pCount ✗
 *   right=6: 加入 'b', 移除 s[3]='e'
 *            → sCount: a=1,b=2                 窗口="bab"   大小=3
 *            sCount != pCount ✗
 *   right=7: 加入 'a', 移除 s[4]='b'
 *            → sCount: a=2,b=1                 窗口="aba"   大小=3
 *            sCount != pCount ✗
 *   right=8: 加入 'c', 移除 s[5]='a'
 *            → sCount: a=1,b=1,c=1             窗口="bac"   大小=3
 *            sCount == pCount ✅ → 记录 left=6
 *   right=9: 加入 'd', 移除 s[6]='b'
 *            → sCount: a=1,c=1,d=1             窗口="acd"   大小=3
 *            sCount != pCount ✗
 *
 *   结果：[0, 6]  ✅
 *
 * 时间复杂度：O(n * 26) = O(n)  —— 滑动窗口遍历一次，每次比较 26 个字符频次
 * 空间复杂度：O(1)              —— 两个长度 26 的数组，为常数空间
 */
class Solution2 {
public:
    vector<int> findAnagrams(string s, string p) {
        int n = (int)s.size();
        int m = (int)p.size();
        vector<int> res;

        if (n < m)
        {
            return res;
        }

        vector<int> pCount(26, 0);
        vector<int> sCount(26, 0);

        // 统计 p 的字符频次
        for (char c : p)
        {
            pCount[c - 'a']++;
        }

        int left = 0;
        for (int right = 0; right < n; ++right)
        {
            // 右边界字符加入窗口
            sCount[s[right] - 'a']++;

            // 窗口大小超过 m，左边界字符移出
            if (right - left + 1 > m)
            {
                sCount[s[left] - 'a']--;
                left++;
            }

            // 窗口大小恰好为 m，比较频次
            if (right - left + 1 == m && sCount == pCount)
            {
                res.push_back(left);
            }
        }

        return res;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法三：滑动窗口 + diff 计数优化（Sliding Window + Diff Count）
 * ──────────────────────────────────────────────
 * 思路：
 *   解法二每次都要比较两个长度 26 的数组（虽然是 O(1)，但常数为 26）。
 *   优化：用一个 diff 变量记录 "有多少种字符的频次还不匹配"。
 *   当 diff == 0 时，说明所有字符频次完全一致，即找到了一个异位词。
 *
 *   核心思想：
 *     只维护一个频次差数组 count[26]，含义：count[c] = pCount[c] - sCount[c]
 *     - count[c] > 0：窗口中字符 c 还少了 count[c] 个
 *     - count[c] < 0：窗口中字符 c 多了 |count[c]| 个
 *     - count[c] == 0：字符 c 的数量刚好匹配
 *     diff = count[c] != 0 的字符种类数
 *
 *   滑动窗口更新 diff：
 *     加入字符 c（right 右移）：
 *       count[c]-- （窗口多了一个 c，差值减 1）
 *       - 若 count[c] 从 1 变成 0 → 新匹配了一种字符，diff--
 *       - 若 count[c] 从 0 变成 -1 → 原本匹配的字符失配了，diff++
 *     移除字符 c（left 右移）：
 *       count[c]++ （窗口少了一个 c，差值加 1）
 *       - 若 count[c] 从 -1 变成 0 → 新匹配了一种字符，diff--
 *       - 若 count[c] 从 0 变成 1 → 原本匹配的字符失配了，diff++
 *
 * 图解：s = "cbaebabacd", p = "abc"
 *   初始 count: a=1, b=1, c=1 （p 中各字符需要的数量）
 *   初始 diff = 3（a, b, c 三种字符都不匹配）
 *
 *   right=0: 加入 'c' → count[c]: 1→0, diff: 3→2    窗口="c"
 *   right=1: 加入 'b' → count[b]: 1→0, diff: 2→1    窗口="cb"
 *   right=2: 加入 'a' → count[a]: 1→0, diff: 1→0    窗口="cba"
 *            diff==0 ✅ → 记录 left=0
 *   right=3: 移除 'c' → count[c]: 0→1, diff: 0→1
 *            加入 'e' → count[e]: 0→-1, diff: 1→2    窗口="bae"
 *   right=4: 移除 'b' → count[b]: 0→1, diff: 2→3
 *            加入 'b' → count[b]: 1→0, diff: 3→2     窗口="aeb"
 *   right=5: 移除 'a' → count[a]: 0→1, diff: 2→3
 *            加入 'a' → count[a]: 1→0, diff: 3→2     窗口="eba"
 *   right=6: 移除 'e' → count[e]: -1→0, diff: 2→1
 *            加入 'b' → count[b]: 0→-1, diff: 1→2    窗口="bab"
 *   right=7: 移除 'b' → count[b]: -1→0, diff: 2→1
 *            加入 'a' → count[a]: 0→-1, diff: 1→2    窗口="aba"
 *   right=8: 移除 'a' → count[a]: -1→0, diff: 2→1
 *            加入 'c' → count[c]: 1→0, diff: 1→0     窗口="bac"
 *            diff==0 ✅ → 记录 left=6
 *   right=9: 移除 'b' → count[b]: 0→1, diff: 0→1
 *            加入 'd' → count[d]: 0→-1, diff: 1→2    窗口="acd"
 *
 *   结果：[0, 6]  ✅
 *
 * 时间复杂度：O(n)  —— 滑动窗口遍历一次，每次更新 diff 为 O(1)
 * 空间复杂度：O(1)  —— 长度 26 的数组，为常数空间
 */
class Solution3 {
public:
    vector<int> findAnagrams(string s, string p) {
        int n = (int)s.size();
        int m = (int)p.size();
        vector<int> res;

        if (n < m)
        {
            return res;
        }

        vector<int> count(26, 0); // count[c] = p 中还需要的字符 c 的数量
        int diff = 0;             // 频次不匹配的字符种类数

        // 统计 p 的字符频次
        for (char c : p)
        {
            if (count[c - 'a'] == 0)
            {
                diff++;
            }
            count[c - 'a']++;
        }

        int left = 0;
        for (int right = 0; right < n; ++right)
        {
            // 右边界字符加入窗口：count[c]--
            int rc = s[right] - 'a';
            count[rc]--;
            if (count[rc] == 0)
            {
                diff--; // 从不匹配变为匹配
            }
            else if (count[rc] == -1)
            {
                diff++; // 从匹配变为不匹配
            }

            // 窗口大小超过 m，左边界字符移出：count[c]++
            if (right - left + 1 > m)
            {
                int lc = s[left] - 'a';
                count[lc]++;
                if (count[lc] == 0)
                {
                    diff--; // 从不匹配变为匹配
                }
                else if (count[lc] == 1)
                {
                    diff++; // 从匹配变为不匹配
                }
                left++;
            }

            // diff == 0 意味着所有字符频次完全一致
            if (diff == 0)
            {
                res.push_back(left);
            }
        }

        return res;
    }
};
