/**
 * binary heap
 * Created by andrew_liu on 15/4/20.
 * Heap,  insert logN , del logN,
 */
public class HeapMaxPQ<Key extends Comparable<Key>> {
    private Key[] pq;
    private int N;
    //对特殊的Key特殊定义CompareTo的方法
    public HeapMaxPQ(int capacity) {
        pq = (Key[]) new Comparable[capacity + 1];
        N = 0;
    }

    public boolean isEmpty() {
        return N == 0;
    }

    public void insert(Key key) {
        pq[++N] = key;  //从1开始, 方便计算父子节点序号
        swim(N);
    }
    public Key delMax() {
        Key max = pq[1]; //获取根节点, 也是最大值
        exch(1, N--);   //注意N--, 相当于不在计算那个节点了
        sink(1);
        pq[N + 1] = null;//释放节点
        return max;
    }

    public void swim(int k) { //大顶堆, 如果新插入的元素比父节点大, 则交换
        while(k > 1 && less(k/2, k)) {
            exch(k, k/2);
            k = k/2;
        }
    }

    public void sink(int k) {  //移除最大元素后, 将最后一个元素放在根节点, 然后调整整棵树的顺序
        while (2*k < N) {
            int j = 2 * k; //子节点的序号
            if (j < N && less(j, j + 1))
                j++;
            if (!less(k, j))  //父节点比子节点大
                break;
            exch(k, j);
            k = j;  //向子节点迭代
        }
    }
    private boolean less(int i, int j) {
        return pq[i].compareTo(pq[j]) < 0;
    }

    private void exch(int i, int j) {
        Key temp = pq[i];
        pq[i] = pq[j];
        pq[j] = temp;
    }
}
