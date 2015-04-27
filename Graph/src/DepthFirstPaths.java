/**
 * Created by andrew_liu on 15/4/27.
 * 深度优先遍历使用占存储连通路径
 */
public class DepthFirstPaths {
    private boolean[] marked;
    private int[] edgeTo;
    private final int s;  //source
    public DepthFirstPaths(Graph G, int s) {
        marked = new boolean[G.V()];
        edgeTo = new int[G.V()];
        this.s = s;
        dfs(G, s);
    }

    private void dfs(Graph G, int v) {
        marked[v] = true;
        for (int w : G.adj(v)) {
            if (!marked[w])
                edgeTo[w] = v;
                dfs(G, w);
        }
    }

    public boolean hasPathTo(int v) {
        return marked[v];
    }
    //查看v是否和s有路径(连通)
    public Iterable<Integer> pathTo(int v) {
        if (!hasPathTo(v))
            return null;
        Stack<Integer> path = new Stack<Integer>();
        for (int x = v; x != s; x = edgeTo[x])
            path.push(x);
        path.push(s); //到源点了
        return path;
    }
}
