/*
 * 49. Group Anagrams
 * https://leetcode.com/problems/group-anagrams/
 *
 * 给定字符串数组 strs，将所有字母异位词（Anagram）分组后返回。
 * 字母异位词：由相同字母重新排列组成的字符串，如 "eat"/"tea"/"ate"。
 *
 * ──────────────────────────────────────────────
 * 解法一：排序作 Key（Sort Key）
 * ──────────────────────────────────────────────
 * 思路：
 *   同一组字母异位词排序后得到的字符串完全相同，可用排序后的字符串作为
 *   哈希表的 key，将原字符串归入对应分组。
 *
 * 示例：strs = ["eat","tea","tan","ate","nat","bat"]
 *   "eat" → sort → "aet"
 *   "tea" → sort → "aet"  ← 与 "eat" 同组
 *   "tan" → sort → "ant"
 *   "ate" → sort → "aet"  ← 同组
 *   "nat" → sort → "ant"  ← 与 "tan" 同组
 *   "bat" → sort → "abt"
 *
 *   map: {"aet":["eat","tea","ate"], "ant":["tan","nat"], "abt":["bat"]}
 *
 * 时间复杂度：O(n * k*logk)  n=字符串个数, k=字符串最大长度（排序耗时）
 * 空间复杂度：O(n * k)
 */
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> mp;
        for (const string& s : strs)
        {
            string key = s;
            sort(key.begin(), key.end());
            mp[key].push_back(s);
        }

        vector<vector<string>> res;
        for (auto& [key, group] : mp)
        {
            res.push_back(move(group));
        }

        return res;
    }
};

/*
 * ──────────────────────────────────────────────
 * 解法二：字母频次作 Key（Counting Key）
 * ──────────────────────────────────────────────
 * 思路：
 *   同一组字母异位词各字母出现的次数完全相同。
 *   统计每个字符串中 26 个字母各自的频次，将频次数组编码成字符串（用 '#'
 *   作分隔符防止歧义）作为哈希表的 key，省去排序步骤。
 *
 * 示例："eat" → count: a=1,e=1,t=1,其余=0
 *   key = "1#0#0#0#1#0#0#0#0#0#0#0#0#0#0#0#0#0#0#1#0#0#0#0#0#0#"
 *   "tea" → 相同 key → 与 "eat" 归入同组 ✅
 *
 * 时间复杂度：O(n * k)   —— 只需遍历字符，不再排序
 * 空间复杂度：O(n * k)
 */
class Solution2 {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> mp;
        for (const string& s : strs)
        {
            int count[26] = {0};
            for (char c : s)
            {
                count[c - 'a']++;
            }
            // 将频次数组编码为 "1#0#0#...#2#" 形式，'#' 防止 "1,12" 与 "11,2" 混淆
            string key;
            for (int i = 0; i < 26; ++i)
            {
                key += to_string(count[i]) + '#';
            }
            mp[key].push_back(s);
        }

        vector<vector<string>> res;
        for (auto& [key, group] : mp)
        {
            res.push_back(move(group));
        }

        return res;
    }
};
