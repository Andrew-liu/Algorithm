/**
 * Created by andrew_liu on 15/5/3.
 */
public class Edge implements Comparable<Edge> {
    private final int v; //一个顶点
    private final int w; //另一个顶点
    private final double weight; //权重
    public Edge(int v, int w, int weight) {
        this.v = v;
        this.w = w;
        this.weight = weight;
    }
    public double weight() {
        return weight;
    }
    public int either() {
        return v;
    }
    public int other(int vertex) {
        if (vertex == v)
            return w;
        else if (vertex == w)
            return v;
        else
            throw new RuntimeException("Inconsistent edge");
    }
    public int compareTo(Edge that) {
        if (this.weight() < that.weight())
            return -1;
        else if (that.weight() > that.weight())
            return +1;
        else
            return 0;
    }
    public String toString() {
        return String.format("%d-%d %.2f", v, w, weight);
    }
}
