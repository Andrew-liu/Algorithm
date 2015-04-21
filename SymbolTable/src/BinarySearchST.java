import java.util.Iterator;
import java.util.Queue;

/**
 * Created by andrew_liu on 15/4/21.
 *
 * 可用于计算词频
 */
public class BinarySearchST<Key extends Comparable<Key>, Value> {
    private Key[] keys;
    private Value[] vals;
    private int N;
    public BinarySearchST(int capacity) {
        keys = (Key[]) new Comparable[capacity];
        vals = (Value[]) new Object[capacity];
    }
    public int size() {
        return N;
    }
    public boolean isEmpty() {
        return N == 0;
    }
    public Value get(Key key) {
        if (isEmpty())
            return null;
        int i = rank(key);  //查找对应key的序号
        if (i < N && keys[i].compareTo(key) == 0)
            return vals[i];
        else
            return null;
    }
//    二分查找
    private int rank(Key key) {
        int lo = 0, hi = N - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int cmp = key.compareTo(keys[mid]);
            if (cmp < 0)
                hi = mid - 1;
            else if (cmp > 0)
                lo = mid + 1;
            else
                return mid;
        }
        return lo;
    }
    public void put(Key key, Value val) {
        int i = rank(key); //已有覆盖
        if (i < N && keys[i].compareTo(key) == 0) {
            vals[i] = val;
            return;
        }
        //没有添加
        for (int j = N; j > i; j--) {
            keys[j] = keys[j - 1];
            vals[j] = vals[j - 1];
        }
        keys[i] = key;
        vals[i] = val;
        N++;
    }
//    public Iterable<Key> keys() {
//        Queue<Key> q = new Queue<Key>();
//        for (int i = 0; i < N; i++) {
//            q.enqueue(keys[i]);
//        }
//        return q;
//    }
    public static void main(String[] args) {
        BinarySearchST<String, Integer> st = new BinarySearchST<String, Integer>(10);
        for (int i = 0; !StdIn.isEmpty(); i++) {
            String key = StdIn.readString();
            st.put(key, i);
        }
//        for (String s : st.keys()) {
//            StdOut.println(s + " " + st.get(s));
//        }
    }
}
