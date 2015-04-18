import java.util.Comparator;
import java.util.Arrays;  //         Arrays.sort();
/**
 * Created by andrew_liu on 15/4/18.
 */
public class QuickSort {
    public static void sort(Comparable[] a) {
        StdRandom.shuffle(a);  //使之随机分布
        sort(a, 0, a.length - 1);
    }
    public static void threeWayQuickSort(Comparable[] a, int lo, int hi) {
        /**
         * Dijkstra 3-way partitioning
         * 用于序列中有大量相等的元素
         * a[i] < v => 交换a[lt]和a[i], lt++ i ++
         * a[i] > v => 交换a[gt]和a[i], gt--
         * a[i] = v => i++
         */
        if (hi <= lo)
            return;
        int lt = lo, gt = hi;
        Comparable v = a[lo];
        int i = lo;
        while (i <= gt) {
            int cmp = a[i].compareTo(v);
            if (cmp < 0)
                exch(a, lt++, i++);
            else if (cmp > 0)
                exch(a, i, gt--);
            else
                i++;
        }
        threeWayQuickSort(a, lo, lt - 1);
        threeWayQuickSort(a, gt + 1, hi);
    }
    public static void sort(Comparable[] a, int lo, int hi) {
        if (hi <= lo)
            return;
        /**
         * 选择povit在中间的部分, 使序列被平均分
         * if (hi <= lo + CUTOFF -1) {  约等于10
         *     InsertionSort.sort(a, lo, hi);
         *     return;
         * }  //短排序用插入排序优化
         */
        int j = partition(a, lo, hi);
        sort(a, lo, j - 1);
        sort(a, j + 1, hi);
    }

    //找到Top k个数字
    public static Comparable select(Comparable[] a, int k) {
        StdRandom.shuffle(a);
        int lo = 0, hi = a.length - 1;
        while (lo < hi) {
            int j = partition(a, lo, hi);
            if (j < k)
                lo = j + 1;
            else if (j > k)
                hi = j - 1;
            else
                return a[k];  //povit = j = k的位置返回
        }
        return a[k];
    }
    private static int partition(Comparable[] a, int lo, int hi) {
        int i = lo, j = hi + 1;
        while(true) {
            while (less(a[++i], a[lo]))
                if (i == hi)  //移动到了最右侧
                    break;
            while (less(a[lo], a[--j]))
                if (j == lo) //移动到了最左侧,
                    break;
            if (i >= j)
                break;
            exch(a, i, j);  //找到一个比锚点小的在右侧, 一个比锚点大的在左侧, 进行交换
        }
        exch(a, lo, j);  //将lo上的元素放在正确的位置, 使前面的都比它小, 后面都比它大
        return j;
    }
    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }
    private static void exch(Comparable[] a, int i, int j) {
        Comparable temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
    private static void show(Comparable[] a) {
        for (Comparable anA : a) {
            StdOut.print(anA + " ");
        }
        StdOut.println();
    }
    public static boolean isSorted(Comparable[] a) {
        for (int i = 1; i < a.length; i++)
            if (less(a[i], a[i - 1]))
                return false;
        return true;
    }
    public static void main(String[] args) {
        StdOut.print("Please input some number to InsertionSort:\n");
        String[] a = StdIn.readStrings();
        sort(a);
        assert isSorted(a);
        show(a);
    }
}
