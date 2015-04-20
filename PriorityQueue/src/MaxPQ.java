/**
 * Created by andrew_liu on 15/4/19.
 */
public class MaxPQ<Key extends Comparable <Key>>{
    private Key[] pq;  //使其中元素有序排列
    private int N;     //记录元素个数

    //实现CompareTo方法
    public MaxPQ(int capacity) {
        N = 0;
    }
    public boolean isEmpty() {
        return N == 0;
    }
    public void insert(Key x) {
        pq[N++] = x;
    }
    public Key delMax() {
        int max = 0;
        for (int i = 1; i < N; i++) {
            if (less(max, i))
                max = i;  //找到最大元素下标
        }
        exch(max, N - 1);
        return pq[--N];
    }
    private boolean less(int i, int j) {
        return pq[i].compareTo(pq[j]) < 0 ? true : false;
    }
    private void exch(int i, int j) {
        Key temp = pq[i];
        pq[i] = pq[j];
        pq[j] = temp;
    }
}
