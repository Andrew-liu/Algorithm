/**
 * Created by andrew_liu on 15/4/27.
 * 广度优先遍历使用队里存储连通轨迹
 *
 * 广度优先遍历实现爬虫程序, 初始站点入队, 正则匹配所有初始站点中的page, 然后所有page入队, 继续广度优先遍历, 并且用set保证网站不重复遍历
 */
public class BreadthFirstPaths {
    private boolean[] marked;
    private int[] edgeTo;
    private final int s;  //source
    public BreadthFirstPaths(Graph G, int s) {
        marked = new boolean[G.V()];
        edgeTo = new int[G.V()];
        this.s = s;
        bfs(G, s);
    }
    private void bfs(Graph G, int s) {
        Queue<Integer> queue = new Queue<>();
        marked[s] = true;  //标记为已访问
        queue.enqueue(s);
        while (!queue.isEmpty()) {
            int v = queue.dequeue();
            for (int w : G.adj(v)) {
                if (!marked[w])
                    edgeTo[w] = v;  //有v到w的路径那个,v和w是连通的
                    marked[w] = true; //w已访问
                    queue.enqueue(w);
            }
        }
    }

    public boolean hasPathTo(int v) {
        return marked[v];
    }
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
