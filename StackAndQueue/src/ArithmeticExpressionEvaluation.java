import java.awt.print.Printable;


public class ArithmeticExpressionEvaluation {
	public static void main(String[] args) {
		Stack<Double> values = new Stack<Double>();
		Stack<String> ops = new Stack<String>();
		StdOut.print("Please input expresss:\n");
		while (!StdIn.isEmpty()) {
			String s = StdIn.readString();
			if (s.equals("(")) {
				//ignore
			} else if (s.equals("+") || s.equals("") || s.equals("-") || s.equals("*") || s.equals("/")) {
				ops.push(s);
			} else if (s.equals(")")) {
				switch (ops.pop()) {
				case "+":
					values.push(values.pop() + values.pop());
					break;
				case "-":
					values.push(values.pop() - values.pop());
					break;
				case "*":
					values.push(values.pop() * values.pop());
					break;
				case "/":
					values.push(values.pop() / values.pop());
					break;
				default:
					break;
				}
			} else {
				values.push(Double.parseDouble(s));
			}
		}
		StdOut.print(values.pop());
	}
}
