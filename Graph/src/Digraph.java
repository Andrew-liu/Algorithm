/**
 * Created by andrew_liu on 15/4/28.
 *
 * 有向图
 * 1. 最短路径
 * 2. 强连通
 * 3. 拓扑排序
 *
 * 邻接表/邻接矩阵表示
 */
public class Digraph {
    private final int V;  //顶点个数
    private int E;  //边的个数
    private Bag<Integer>[] adj;  //相邻节点

    public Digraph(int V) {
        this.V = V;
        this.E = 0;
        adj = (Bag<Integer>[]) new Bag[V];
        for (int v = 0; v < V; v++) {
            adj[v] = new Bag<Integer>();
        }
    }

    public int V() {
        return V;
    }
    public int E() {
        return E;
    }
    public void addEdge(int v, int w) {
        adj[v].add(w);
        E++;
    }

    public Iterable<Integer> adj(int v) {
        return adj[v];
    }

    public Digraph reverse() {
        Digraph R = new Digraph(V);
        for (int v = 0; v < V; v++) {
            for (int w : adj(v)) {
                R.addEdge(w, v);
            }
        }
        return R;
    }

}
