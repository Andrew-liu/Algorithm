/**
 * Created by andrew_liu on 15/5/5.
 */
public class LSD {
    public static void sort(String[] a, int W) {
        //通过后W个字符将a[]排序, 从低位开始
        int N = a.length; //字符串数组长度
        int R = 256;  //radix; 最多表示256的字符extern-ASCII
        String[] aux = new String[N];  //存储排序后的字符串数组
        for (int d = W - 1; d >= 0; d--) {  //低位排序从这里体现
            int[] count = new int[R + 1];
            //计算出现频率
            for (int i = 0; i < N; i++) {
                count[a[i].charAt(d) + 1]++;  //count每一位统计对应的字符的出现次数
            }
            //将频率转换成索引, 从0开始, 256个字符从第二个字符串开始, 所在的索引应该是第一个字符的频率, 第三个字符串索引应该是第一个字符加上第二个字符
            for (int r = 0; r < R; r++) {
                count[r + 1] += count[r];  //第一个字符换索引为0
            }
            for (int i = 0; i < N; i++) {
                aux[count[a[i].charAt(d)]++] = a[i];  //将字符串应对的d位的数, 放在当前索引下, 然后索引加1
            }
            //写会a[], 重复W词, 因为按照W个字符排序
            for (int i = 0; i < N; i++) {
                a[i] = aux[i];
            }
        }
    }
}
