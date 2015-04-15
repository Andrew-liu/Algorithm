/**
 * Created by andrew_liu on 15/4/15.
 */
public class InsertionSort {
    public static void sort(Comparable[] a) {
        int N = a.length;
        for (int i = 0; i < N - 1; i++) { //只需要进行N-1次排序
            for(int j = i + 1; j > 0 && less(a[j], a[j - 1]); j--){  //j从1开始,
                    exch(a, j, j - 1);
            }
        }
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
