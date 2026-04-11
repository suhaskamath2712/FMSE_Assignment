public class NodeSimple {
    NodeSimple next;

    public static boolean hasCycleBounded(NodeSimple head, int maxSteps) {
        NodeSimple slow = head;
        NodeSimple fast = head;
        int steps = 0;

        while (fast != null && fast.next != null && steps < maxSteps) {
            slow = slow.next;
            NodeSimple t = fast.next;
            fast = t.next;
            if (slow == fast)
                return true;
            steps++;
        }

        return false;
    }

    public static void main(String[] args) {
        hasCycleBounded(new NodeSimple(), 1);
    }
}
