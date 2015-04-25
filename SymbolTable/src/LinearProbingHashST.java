/**
 * Created by andrew_liu on 15/4/25.
 */
public class LinearProbingHashST<Key, Value> {
    private int N;
    private int M = 16;
    private Key[] keys;
    private Value[] vals;
    public LinearProbingHashST() {
        keys = (Key[]) new Object[M];
        vals = (Value[]) new Object[M];
    }
    public LinearProbingHashST(int cap) {
        keys = (Key[]) new Object[cap];
        vals = (Value[]) new Object[cap];
    }
    private int hash(Key key) {
        return (key.hashCode() & 0x7fffffff) % M; //正确的自定义inthash写法
    }
    private void resize(int cap) {
        //修改数组大小, 原来的二倍
        LinearProbingHashST<Key, Value> t;
        t = new LinearProbingHashST<Key, Value>(cap);
        for (int i = 0; i < M; i++) {
            if (keys[i] != null)
                t.put(keys[i], vals[i]);
        }
        keys = t.keys;
        vals = t.vals;
        M = t.M;
    }
    public void put(Key key, Value val) {
        if (N >= M / 2)
            resize(2 * M);
        int i;
        for (i = hash(key); keys[i] != null; i = (i + 1) % M)
            if (keys[i].equals(key)) { //找到相同key, 则修改value
                vals[i] = val;
                return;
            }
        //冲突解决
        keys[i] = key;
        vals[i] = val;
        N++;
    }
    public Value get(Key key) {
        for (int i = hash(key); keys[i] != null; i = (i + 1) % M)
            if (keys[i].equals(key))
                return vals[i];
        return null;
    }
}
