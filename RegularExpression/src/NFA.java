/**
 * Created by andrew_liu on 15/5/10.
 * ((A*B| AC))D)
 */
public class NFA {
    private char[] re;  //匹配转换
    private Digraph G;  //epsilon转换
    private int M;
    public NFA(String regexp) {
        //根据自顶正则表达式构造NFA
        Stack<Integer> ops = new Stack<>();
        re = regexp.toCharArray();
        M = re.length;
        G = new Digraph(M + 1);
        for (int i = 0; i < M; i++) {
            int lp = i;
            if (re[i] == '(' || re[i] == '|')
                ops.push(i);
            else if (re[i] == ')') {  //小括号里带或操作
                int or = ops.pop();
                if (re[or] == '|') { //或操作
                    lp = ops.pop();
                    G.addEdge(lp, or + 1);
                    G.addEdge(or, i);
                }
                else lp = or;
            }
            if (i < M - 1 && re[i + 1] == '*') {//查看下一个字符, 闭包操作
                G.addEdge(lp, i + 1);
                G.addEdge(i + 1, lp);
            }
            if (re[i] == '(' || re[i] == '*' || re[i] == ')')
                G.addEdge(i, i + 1);
        }
    }
    public boolean recognizes(String txt) {
        //NFA识别文本txt
        Bag<Integer> pc = new Bag<>();
        DirectedDFS dfs = new DirectedDFS(G, 0);
        for (int v = 0; v < G.V(); v++) {
            if (dfs.marked(v))
                pc.add(v);
        }
        for (int i = 0; i < txt.length(); i++) {
            Bag<Integer> match = new Bag<>();
            for (int v : pc)
                if (v < M)
                    if (re[v] == txt.charAt(i) || re[v] == '.')
                        match.add(v + 1);
            pc = new Bag<>();
            dfs = new DirectedDFS(G, match);
            for (int v = 0; v < G.V(); v++)
                if (dfs.marked(v))
                    pc.add(v);
        }
        for (int v : pc)
            if (v == M)
                return true;
        return false;
    }
}
