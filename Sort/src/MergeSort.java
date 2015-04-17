/**
 * Created by andrew_liu on 15/4/17.
 * NlogN 空间复杂度N
 */
public class MergeSort {
    public static void sort(Comparable[] a) {
        Comparable[] aux = new Comparable[a.length];
        sort(a, aux, 0, a.length -1);
    }
    //归并排序的非递归实现
    /*
    private static void sort(Comparable[] a) {
        int N = a.length;
        Comparable[] aux = new Comparable[N];
        for (int sz = 1; sz < N; sz = sz + sz) {  //1 2 4 8
            for (int lo = 0; lo < N - sz; lo += sz + sz)
                merge(a, lo, lo + sz - 1, Math.min(lo + sz + sz - 1, N - 1));
        }
    }
    */
    public static void sort(Comparable[] a, Comparable[] aux, int lo, int hi) {
        if (hi <= lo)
            return;
        /**
         * if (hi <= lo + CUTOFF -1) {
         *     InsertionSort.sort(a, lo, hi);
         *     return;
         * }  //短排序用插入排序优化
         */
        int mid = lo + (lo + hi) / 2;
        sort(a, aux, lo, mid);
        sort(a, aux, mid + 1, hi);
//        if (!less(a[mid + 1], a[mid]))  //优化
//            return;
        merge(a, aux, lo, mid, hi);
    }
    private static void merge(Comparable[] a, Comparable[] aux, int lo, int mid, int hi) {
        //被排序的首 中间 尾
        assert isSorted(a, lo, mid);  //java -ea myprogram enable assert
        assert isSorted(a, mid + 1, hi); // java -da disable assert

        for (int k = lo; k <= hi; k++) {
            aux[k] = a[k];  // copy原来的数据到新的数组中
        }

        int  i = lo;
        int j = mid + 1;
        for (int k = lo; k <= hi; k++) {
            if (i > mid)   //i部分已经完成排序
                a[k] = aux[j++];
            else if (j > hi)   //j部分已经完成排序
                a[k] = aux[i++];
            else if (less(aux[j], aux[i]))
                a[k] = aux[j++];
            else
                a[k] = aux[i++];
        }
        assert isSorted(a, lo, hi);
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
    public static boolean isSorted(Comparable[] a, int lo, int hi) {
        for (int i = lo + 1; i <= hi; i++)
            if (less(a[i], a[i - 1]))
                return false;
        return true;
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
        StdOut.print(a.length);
        sort(a);
        assert isSorted(a);
        show(a);
    }
}
