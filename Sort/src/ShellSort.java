import java.util.Objects;

/**
 * Created by andrew_liu on 15/4/15.
 */
public class ShellSort {
    public static void sort(Comparable[] a) {
        int N = a.length;
        int h = 1;
        while (h < N / 3) {
            h = 3 * h + 1;
        } // h尽量为N的1/3  假设N=10则h=4
        while (h >= 1) { //h-sort
            for (int i = h; i < N; i++) { //从4->9 找打这些元素前面h个位置的元素做比较
                for (int j = i; j >= h && less(a[j], a[j - h]); j -= h) {
                    exch(a, j, j - h);
                }
            }
            h = h / 3; //下一轮则h=1
        }
    }
    /*
    public static void shuffle(Objects[] a) {
        int N = a.length;
        for (int i = 0; i < N; i++) {
            int r = StdRandom.uniform(i + 1); //between 0 到 1
            exch(a, i, r);
        }
    }
    */
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
