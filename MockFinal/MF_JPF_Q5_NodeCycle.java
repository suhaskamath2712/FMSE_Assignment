package MockFinal;

public class MF_JPF_Q5_NodeCycle {
    MF_JPF_Q5_NodeCycle next;

    // Intentionally buggy cycle check: fast pointer advances only 1 step.
    public static boolean hasCycleBoundedBug(MF_JPF_Q5_NodeCycle head, int maxSteps) {
        MF_JPF_Q5_NodeCycle slow = head;
        MF_JPF_Q5_NodeCycle fast = head;
        int steps = 0;

        while (fast != null && fast.next != null && steps < maxSteps) {
            slow = slow.next;
            fast = fast.next; // BUG: should be fast.next.next
            if (slow == fast)
                return true;
            steps++;
        }

        return false;
    }

    // Bounded spec: returns true iff a node reference repeats within maxSteps+1 traversals.
    public static boolean repeatsWithin(MF_JPF_Q5_NodeCycle head, int maxSteps) {
        MF_JPF_Q5_NodeCycle[] seen = new MF_JPF_Q5_NodeCycle[maxSteps + 1];
        MF_JPF_Q5_NodeCycle cur = head;
        int i = 0;

        while (cur != null && i < seen.length) {
            for (int j = 0; j < i; j++) {
                if (seen[j] == cur)
                    return true;
            }
            seen[i] = cur;
            cur = cur.next;
            i++;
        }

        return false;
    }

    public static void checkSoundness(MF_JPF_Q5_NodeCycle head, int maxSteps) {
        boolean alg = hasCycleBoundedBug(head, maxSteps);
        boolean spec = repeatsWithin(head, maxSteps);

        if (alg && !spec) {
            // TODO: Enforce soundness, e.g. by triggering an assertion failure.
        }
    }

    public static void main(String[] args) {
        checkSoundness(new MF_JPF_Q5_NodeCycle(), 3);
    }
}
