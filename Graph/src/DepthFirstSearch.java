/**
 * Created by andrew_liu on 15/4/27.
 * 深度优先遍历适用于无向图和有向图
 */
public class DepthFirstSearch {
    private boolean[] marked;
    private int count;

    public DepthFirstSearch(Graph G, int s) {
        marked = new boolean[G.V()];
        dfs(G, s);
    }

    private void dfs(Graph G, int v) {
        marked[v] = true;
        count++;
        for (int w : G.adj(v)) {
            if (!marked(w))
                dfs(G, w);
        }
    }
    public boolean marked(int w) {
        return marked[w];
    }
    public int count() {
        return count;
    }
}
