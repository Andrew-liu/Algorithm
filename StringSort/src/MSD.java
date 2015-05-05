/**
 * Created by andrew_liu on 15/5/5.
 */
public class MSD {
    private static int R = 256; //字符数
    private static final int M = 15;  //小数组的切换阈值
    private static String[] aux;  //辅助数组, 存储中间排序结果

    private static int charAt(String s, int d) {
        if (d < s.length())
            return s.charAt(d);
        else
            return -1;
    }
    public static void sort(String[] a) { //排序的外部接口
        int N = a.length;
        aux = new String[N];
        sort(a, 0, N - 1, 0);
    }
    //排序的内部实现, 以第d个字符为键将a[lo]至a[hi]排序, 递归过程
    private static void sort(String[] a, int lo, int hi, int d) {
        if (hi <= lo + M) {
            Insertion.sort(a, lo, hi, d);  //大于阈值则使用插入排序
            return;
        }
        int[] count = new int [R + 2]; //计算频率
        for (int i = lo; i <= hi; i++) {
            count[charAt(a[i], d) + 2]++; //由左到右同此每个字符的频率
        }
        for (int r = 0; r < R + 1; r++) {
            count[r + 1] += count[r]; //频率转换为索引
        }
        //数据分类
        for (int i = lo; i <= hi; i++) {
            aux[count[charAt(a[i], d) + 1]++] = a[i];
        }
        //回写
        for (int r = 0; r < R; r++) {
            sort(a, lo + count[r], lo + count[r + 1] - 1, d + 1); //对每一段相同字符的字符串数据再进行排序, 比如所有以a开头的字符串数据, 对他们第2未再排序
        }
    }
    public static class Insertion {
        public static void sort(String[] a, int lo, int hi, int d) {
            for (int i = lo; i <= hi; i++) {
                for (int j = i; j > lo && less(a[j], a[j - 1], d); j--)
                    exch(a, j, j - 1);
            }
        }
        private static boolean less(String v, String w, int d) {
            return v.substring(d).compareTo(w.substring(d)) < 0;
        }
        private static void exch(String[] a, int i, int j) {
            String temp = a[i];
            a[i] = a[j];;
            a[j] = temp;
        }
    }
}
