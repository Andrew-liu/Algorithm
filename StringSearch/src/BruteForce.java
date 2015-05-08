/**
 * Created by andrew_liu on 15/5/8.
 * 子字符串暴力搜索
 */
public class BruteForce {
    public static int search(String pat, String txt) {
        int M = pat.length(); //子串的长度
        int N = txt.length();  //检索的文本长度
        for (int i = 0; i <= N - M; i++) {
            int j;
            for (j = 0; j < M; j++) {
                if (txt.charAt(i + j) != pat.charAt(j))
                    break;
            if (j == M)
                return i;  //找到匹配初始的位置
            }
        }
        return N; //未找到匹配
    }
    public static int searchBackUp(String pat, String txt) {
        int j, M = pat.length();
        int i, N = txt.length();
        for (i = 0, j = 0; i < N && j < M; i++) {
            if (txt.charAt(i) == pat.charAt(j))
                j++;
            else {  //不匹配时两位置回退, i回退到比较的与子字符串比较的开头, j = 0
                i -= j;
                j = 0;
            }
        }
        if (j == M)
            return i - M; //找到匹配
        else
            return N;  //未找到匹配
    }
}
