/**
 * Created by andrew_liu on 15/4/27.
 * connected components, 查看两个顶点是否连通
 */
public class CC {
    private boolean[] marked;
    private int[] id;
    private int count;

    public CC(Graph G) {
        marked = new boolean[G.V()];
        id = new int[G.V()];
        for (int s = 0; s < G.V(); s++) { //核心遍历
            if (!marked[s])
                dfs(G, s);
                count++;  //是否连通, 比较count是否相等
        }
    }
    private void dfs(Graph G, int v) {
        marked[v] = true;
        id[v] = count;
        for (int w : G.adj(v))
            if (!marked[w])
                dfs(G, w);
    }
    public boolean connected(int v, int w) {
        return id[v] == id[w];
    }
    public int id(int v) {
        return id[v];
    }
    public int count() {
        return count;
    }
}
