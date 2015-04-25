/**
 * Created by andrew_liu on 15/4/25.
 * 链式哈希解决冲突
 */
public class SeparateChainingHashST<Key, Value> {
    private int N;
    private int M;
    private SequentialSearch<Key, Value>[] st;
    public SeparateChainingHashST(int M) {
        this.M = M;
        st = (SequentialSearch<Key, Value>[]) new SequentialSearch[M];
        for (int i = 0; i < M; i++) {
            st[i] = new SequentialSearch<Key, Value>();
        }
    }
    private int hash(Key key) {
        return (key.hashCode() & 0x7fffffff) % M; //正确的自定义inthash写法
    }
    public Value get(Key key) {
        return (Value) st[hash(key)].get(key);
    }
    public void put(Key key, Value val) {
        st[hash(key)].put(key, val);
    }
//    public Iterable<Key> keys()
}
