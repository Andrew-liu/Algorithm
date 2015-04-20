/**
 * Created by andrew_liu on 15/4/20.
 */
public class HeapSort {
    public static void sort(Comparable[] a) {
        int N = a.length - 1;
        for (int k = N/2; k >= 1; k--)  //初始建堆
            sink(a, k, N);
        while(N > 1) {
            exch(a, 1, N); //拿出根节点 最后一个总是排好序的
            sink(a, 1, --N);
        }
    }
    private static void sink(Comparable[] a, int k, int N) {  //移除最大元素后, 将最后一个元素放在根节点, 然后调整整棵树的顺序
        while (2*k < N) {
            int j = 2 * k; //子节点的序号
            if (j < N && less(a, j, j + 1))
                j++;
            if (!less(a, k, j))  //父节点比子节点大
                break;
            exch(a, k, j);
            k = j;  //向子节点迭代
        }
    }
    private static boolean less(Comparable[] a, int i, int j) {
        return a[i].compareTo(a[j]) < 0;
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
            if (less(a, i, i - 1))
                return false;
        return true;
    }
    public static void main(String[] args) {
        StdOut.print("Please input some number to InsertionSort:\n");
        String[] a = {"", "B", "A", "C", "F"};
        sort(a);
        assert isSorted(a);
        show(a);
    }



}
